[%raw "require('isomorphic-fetch')"];

open Types;

type t = {
  poolId: string,
  clientId: string,
  endpoint: string,
  authenticationFlowType,
};

let makeConfig =
    (~poolId, ~clientId, ~region, ~authenticationFlowType=USER_SRP_AUTH, ()) => {
  clientId,
  poolId,
  authenticationFlowType,
  endpoint:
    "https://cognito-idp." ++ makeRegionString(region) ++ ".amazonaws.com/",
};

module type Client = {
  type error = [
    | `CognitoClientError(Js.Promise.error)
    | `CognitoJsonParseError(Js.Promise.error)
  ];
  type response = {
    status: int,
    json: Js.Json.t,
  };
  let request:
    (t, string, Js.Dict.t(Js.Json.t)) =>
    Future.t(Belt.Result.t(response, [< error]));
};

module Client = {
  // In the end we really prefer a generic client interface, probably more
  // generic than this one.  Why?  So we can plug out the implmentation
  // on native, mobile, or what have you.  Right now, this is JS/Node
  // targets only.
  type status =
    | Informational(int)
    | Success(int)
    | Redirect(int)
    | ClientError(int)
    | ServerError(int);

  type response = {
    status,
    json: Js.Json.t,
  };

  // Error is a polymorphic variant that composes across the entire
  // library.  Because polymorphic variants are GLOBAL, we use highly
  // descriptive names.  Because we are lazy, we are mostly passing along
  // Js.Promise.Error here, to give end-users maximum handling flexibility.
  type error = [
    | `CognitoClientError(Js.Promise.error)
    | `CognitoJsonParseError(Js.Promise.error)
    | `CognitoApiError(response)
  ];

  // Request is the core export of this library.  It's purpose is to
  // talk to the network and pass a JSON blob back to its caller if
  // there is no error in making the request.  If the request itself
  // returns an error code (4XX, 5XX), that should be handled elsewhere.
  //
  // TODO: More generic implemntation (no Js.* types)
  let request = (config, operation: operation, params: Js.Dict.t(Js.Json.t)) => {
    // Setup headers.
    let headers = Js.Dict.empty();
    let target =
      "AWSCognitoIdentityProviderService." ++ makeOperationString(operation);
    Js.Dict.set(headers, "X-Amz-Target", target);
    Js.Dict.set(headers, "Content-Type", "application/x-amz-json-1.1");
    Js.Dict.set(headers, "X-Amz-User-Agent", "reason-cognito/0.1.x js");

    // Add the configured clientId to the request.
    Js.Dict.set(params, "ClientId", Js.Json.string(config.clientId));

    Fetch.fetchWithInit(
      config.endpoint,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~mode=NoCORS,
        ~cache=NoCache,
        ~headers=Fetch.HeadersInit.makeWithDict(headers),
        ~body=
          Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(params))),
        (),
      ),
    )
    // Error = fetch did not do anything.
    ->FutureJs.fromPromise(fetchError => `CognitoClientError(fetchError))
    // OK = fetch did something. Map result to easily consumable type.
    ->Future.flatMapOk(apiResponse =>
        Fetch.Response.json(apiResponse)
        ->FutureJs.fromPromise(err => `CognitoJsonParseError(err))
        ->Future.flatMapOk(json => {
            let status =
              switch (Fetch.Response.status(apiResponse)) {
              | code when code < 200 => Informational(code)
              | code when code >= 200 && code < 300 => Success(code)
              | code when code >= 300 && code < 400 => Redirect(code)
              | code when code >= 400 && code < 500 => ClientError(code)
              | code => ServerError(code)
              };
            Future.make(resolve => resolve(Belt.Result.Ok({status, json})));
          })
      );
  };
};

let jsonMapString = arr =>
  Array.map(
    item => Js.Dict.map((. value) => Js.Json.string(value), item),
    arr,
  );

type signUpErrors = [
  | `CognitoUnknownError(string)
  | `CognitoCodeDeliveryFailure(string)
  | `CognitoInternalError(string)
  | `CognitoInvalidEmailRoleAccessPolicy(string)
  | `CognitoInvalidLambdaResponse(string)
  | `CognitoInvalidParameter(string)
  | `CognitoInvalidPassword(string)
  | `CognitoInvalidSmsRoleAccessPolicys(string)
  | `CognitoInvalidSmsRoleTrustRelationship(string)
  | `CognitoNotAuthorized(string)
  | `CognitoResourceNotFound(string)
  | `CognitoTooManyRequests(string)
  | `CognitoUnexpectedLambda(string)
  | `CognitoUserLambdaValidation(string)
  | `CognitoUsernameExists(string)
  | `CognitoConfirmationCodeValidation(string)
];

let signUp =
    (config, ~username, ~password, ~attributes=[||], ~validationData=[||], ()) => {
  // Map attrib arrays into JSON form.
  let jsonAttribs = jsonMapString(attributes);
  let jsonVData = jsonMapString(validationData);

  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "Username", Js.Json.string(username));
  Js.Dict.set(payload, "Password", Js.Json.string(password));
  Js.Dict.set(payload, "UserAttributes", Js.Json.objectArray(jsonAttribs));
  Js.Dict.set(payload, "ValidationData", Js.Json.objectArray(jsonVData));

  Client.request(config, SignUp, payload)
  ->Future.flatMapOk(res => {
      Js.log2("Raw Response", res);

      switch (res.status) {
      | Success(_) =>
        // We're _really_ hoping amazon holds to their API contract here.
        // If not, we're going to see null type errors.
        Js.log2("Raw Response", res);
        let signUpResponse = makeSignupResponse(res);
        let cddDecoder = signUpResponse->codeDeliveryDetailsDecoderGet;
        let codeDeliveryDetails = {
          attributeName: cddDecoder->attributeNameGet,
          deliveryMedium:
            cddDecoder->deliveryMediumGet === "EMAIL" ? Email : SMS,
          destination: cddDecoder->destinationGet,
        };
        Future.make(resolve =>
          resolve(
            Belt.Result.Ok({
              codeDeliveryDetails,
              userConfirmed: signUpResponse->userConfirmedGet,
              userSub: signUpResponse->userSubGet,
            }),
          )
        );
      | Informational(_)
      | Redirect(_)
      | ClientError(_)
      | ServerError(_) =>
        // We're _really_ hoping amazon holds to their API contract here too.
        // Although the unknownerror variant helps lots.
        let isErrorResponse = makeResponse(res.json);
        let err = isErrorResponse->__typeGet;
        let msg = isErrorResponse->messageGet;
        let err =
          switch (err) {
          | "InvalidParameterException" => `CognitoInvalidParameter(msg)
          | "UsernameExistsException" => `CognitoUsernameExists(msg)
          | "CodeDeliveryFailureException" => `CognitoCodeDeliveryFailure(msg)
          | "InternalErrorException" => `CognitoInternalError(msg)
          | "InvalidEmailRoleAccessPolicyException" =>
            `CognitoInvalidEmailRoleAccessPolicy(msg)
          | "InvalidLambdaResponseException" =>
            `CognitoInvalidLambdaResponse(msg)
          | "InvalidPasswordException" => `CognitoInvalidPassword(msg)
          | "InvalidSmsRoleAccessPolicysException" =>
            `CognitoInvalidSmsRoleAccessPolicys(msg)
          | "InvalidSmsRoleTrustRelationshipException" =>
            `CognitoInvalidSmsRoleTrustRelationship(msg)
          | "NotAuthorizedException" => `CognitoNotAuthorized(msg)
          | "ResourceNotFoundException" => `CognitoResourceNotFound(msg)
          | "TooManyRequestsException" => `CognitoTooManyRequests(msg)
          | "UnexpectedLambdaException" => `CognitoUnexpectedLambda(msg)
          | "UserLambdaValidationException" =>
            `CognitoUserLambdaValidation(msg)
          | _ => `CognitoUnknownError(msg)
          };
        Future.make(resolve => resolve(Belt.Result.Error(err)));
      };
    });
};
type confirmSignUpErrors = [ | `CognitoConfirmationCodeValidation(string)];
let confirmSignUp = (config, ~username, ~confirmationCode, ()) => {
  let params = Js.Dict.empty();

  Js.Dict.set(params, "Username", Js.Json.string(username));
  Js.Dict.set(params, "ConfirmationCode", Js.Json.string(confirmationCode));

  Client.request(config, ConfirmSignUp, params)
  ->Future.flatMapOk(res => {
      /*
             This is the raw response. Not sure what to do with this if anything. What is `tag:1`
             Raw Response [ [ 200, tag: 1 ], {} ]
       Raw Response [ [ 200, tag: 1 ], {} ] */
      Js.log2("Raw Response", res);

      switch (res.status) {
      | Success(_) =>
        // We're _really_ hoping amazon holds to their API contract here.
        // If not, we're going to see null type errors.
        Future.make(resolve =>
          resolve(
            Belt.Result.Ok(
              {
                res;
              },
            ),
          )
        )
      | Informational(_)
      | Redirect(_)
      | ClientError(_)
      | ServerError(_) =>
        // We're _really_ hoping amazon holds to their API contract here too.
        // Although the unknownerror variant helps lots.
        let isErrorResponse = makeResponse(res.json);
        let err = isErrorResponse->__typeGet;
        let msg = isErrorResponse->messageGet;
        let err =
          switch (err) {
          | "CodeMismatchException" => `CognitoConfirmationCodeValidation(msg)
          | _ => `CognitoUnknownError(msg)
          };
        Future.make(resolve => resolve(Belt.Result.Error(err)));
      };
    });
};

type signInExceptions = [ | `NotAuthorizedException(string)];
let signIn = (config, ~username: string, ~password: string, ()) => {
  let authParams = Js.Dict.empty();
  Js.Dict.set(authParams, "USERNAME", Js.Json.string(username));
  Js.Dict.set(authParams, "PASSWORD", Js.Json.string(password));

  let params = Js.Dict.empty();
  Js.Dict.set(params, "AuthParameters", Js.Json.object_(authParams));
  Js.Dict.set(params, "AuthFlow", Js.Json.string("USER_PASSWORD_AUTH"));
  Client.request(config, InitiateAuth, params)
  ->Future.flatMapOk(res => {
      /*
             This is the raw response. Not sure what to do with this if anything. What is `tag:1`
             Raw Response [ [ 200, tag: 1 ], {} ]
       Raw Response [ [ 200, tag: 1 ], {} ] */
      Js.log2("Raw Response", res);

      switch (res.status) {
      | Success(_) =>
        // We're _really_ hoping amazon holds to their API contract here.
        // If not, we're going to see null type errors.
        let signInResponse = makeSignInResponse(res);
        let authDecoder = signInResponse->authenticationResultDecoderGet;
        let authenticationResult = {
          accessToken: authDecoder->accessTokenGet,
          idToken: authDecoder->idTokenGet,
          refreshToken: authDecoder->refreshTokenGet,
          tokenType: authDecoder->tokenTypeGet,
          expiresIn: authDecoder->expiresInGet,
        };
        Future.make(resolve =>
          resolve(
            Belt.Result.Ok({
              authenticationResult,
              challengeParameters: signInResponse->challengeParametersGet,
            }),
          )
        );
      | Informational(_)
      | Redirect(_)
      | ClientError(_)
      | ServerError(_) =>
        // We're _really_ hoping amazon holds to their API contract here too.
        // Although the unknownerror variant helps lots.
        let isErrorResponse = makeResponse(res.json);
        let err = isErrorResponse->__typeGet;
        let msg = isErrorResponse->messageGet;
        let err =
          switch (err) {
          | "InvalidParameterException" => `CognitoInvalidParameter(msg)
          | "NotAuthorizedException" => `CognitoNotAuthorized(msg)
          | _ => `CognitoUnknownError(msg)
          };
        Future.make(resolve => resolve(Belt.Result.Error(err)));
      };
    });
};
let changePassword =
    (config, ~username, ~oldPassword, ~newPassword, ~accessToken, ()) => {
  let params = Js.Dict.empty();

  Js.Dict.set(params, "AccessToken", Js.Json.string(accessToken));
  Js.Dict.set(params, "PreviousPassword", Js.Json.string(oldPassword));
  Js.Dict.set(params, "USERNAME", Js.Json.string(username));
  Js.Dict.set(params, "ProposedPassword", Js.Json.string(newPassword));

  Client.request(config, ChangePassword, params)
  ->Future.flatMapOk(res => {
      /*
             This is the raw response. Not sure what to do with this if anything. What is `tag:1`
             Raw Response [ [ 200, tag: 1 ], {} ]
       Raw Response [ [ 200, tag: 1 ], {} ] */
      Js.log2("Raw Response", res);

      switch (res.status) {
      | Success(_) =>
        // We're _really_ hoping amazon holds to their API contract here.
        // If not, we're going to see null type errors.
        Future.make(resolve =>
          resolve(
            Belt.Result.Ok(
              {
                res;
              },
            ),
          )
        )
      | Informational(_)
      | Redirect(_)
      | ClientError(_)
      | ServerError(_) =>
        // We're _really_ hoping amazon holds to their API contract here too.
        // Although the unknownerror variant helps lots.
        let isErrorResponse = makeResponse(res.json);
        let err = isErrorResponse->__typeGet;
        let msg = isErrorResponse->messageGet;
        let err =
          switch (err) {
          | "CodeMismatchException" => `CognitoConfirmationCodeValidation(msg)
          | _ => `CognitoUnknownError(msg)
          };
        Future.make(resolve => resolve(Belt.Result.Error(err)));
      };
    });
};
let signOut =
    (config, ()) => {
  let params = Js.Dict.empty();

  Client.request(config, SignOut, params)
  ->Future.flatMapOk(res => {

      Js.log2("Raw Response", res);

      switch (res.status) {
      | Success(_) =>
        // We're _really_ hoping amazon holds to their API contract here.
        // If not, we're going to see null type errors.
        Future.make(resolve =>
          resolve(
            Belt.Result.Ok(
              {
                res;
              },
            ),
          )
        )
      | Informational(_)
      | Redirect(_)
      | ClientError(_)
      | ServerError(_) =>
        // We're _really_ hoping amazon holds to their API contract here too.
        // Although the unknownerror variant helps lots.
        let isErrorResponse = makeResponse(res.json);
        let err = isErrorResponse->__typeGet;
        let msg = isErrorResponse->messageGet;
        let err =
          switch (err) {
          | "CodeMismatchException" => `CognitoConfirmationCodeValidation(msg)
          | _ => `CognitoUnknownError(msg)
          };
        Future.make(resolve => resolve(Belt.Result.Error(err)));
      };
    });
};
let deleteUser =
    (config, ()) => {
  let params = Js.Dict.empty();

  Client.request(config, InitiateAuth, params)
  ->Future.flatMapOk(res => {

      Js.log2("Raw Response", res);

      switch (res.status) {
      | Success(_) =>
        // We're _really_ hoping amazon holds to their API contract here.
        // If not, we're going to see null type errors.
        Future.make(resolve =>
          resolve(
            Belt.Result.Ok(
              {
                res;
              },
            ),
          )
        )
      | Informational(_)
      | Redirect(_)
      | ClientError(_)
      | ServerError(_) =>
        // We're _really_ hoping amazon holds to their API contract here too.
        // Although the unknownerror variant helps lots.
        let isErrorResponse = makeResponse(res.json);
        let err = isErrorResponse->__typeGet;
        let msg = isErrorResponse->messageGet;
        let err =
          switch (err) {
          | "CodeMismatchException" => `CognitoConfirmationCodeValidation(msg)
          | _ => `CognitoUnknownError(msg)
          };
        Future.make(resolve => resolve(Belt.Result.Error(err)));
      };
    });
};
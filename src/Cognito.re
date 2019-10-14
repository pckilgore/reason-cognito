[%raw "require('isomorphic-fetch')"];

open Types;
open Serde;

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
    (t, string, Js.Dict.t(Js.Json.t), t, operation, Js.Dict.t(Js.Json.t)) =>
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

  // Request's purpose is to
  // talk to the network and pass a JSON blob back to its caller if
  // there is no error in making the request.  If the request itself
  // returns an error code (4XX, 5XX), that should be handled elsewhere.
  //
  // TODO: More generic implemntation (no Js.* types)
  let request = (config, operation, params: Js.Dict.t(Js.Json.t)) => {
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
        ->Future.mapOk(json => {
            let status =
              switch (Fetch.Response.status(apiResponse)) {
              | code when code < 200 => Informational(code)
              | code when code >= 200 && code < 300 => Success(code)
              | code when code >= 300 && code < 400 => Redirect(code)
              | code when code >= 400 && code < 500 => ClientError(code)
              | code => ServerError(code)
              };
            {status, json};
          })
      );
  };
};

let signUp =
    (
      config,
      ~username,
      ~password,
      ~attributes=[||],
      ~validationData=[||],
      ~analyticsEndpointId=?,
      ~clientMetadata=?,
      ~secretHash=?,
      (),
    ) => {
  // https://docs.aws.amazon.com/cognito-user-identity-pools/latest/APIReference/API_SignUp.html

  // {
  let params = Js.Dict.empty();
  //   "AnalyticsMetadata": {
  //     "AnalyticsEndpointId": "string",
  //   },
  switch (analyticsEndpointId) {
  | Some(id) => Js.Dict.set(params, "AnalyticsMetadata", Js.Json.object_(id))
  | None => ()
  };
  //   "ClientId": "string", <- from client config

  //   "ClientMetadata": {
  //     "string": "string",
  //   },
  switch (clientMetadata) {
  | Some(data) =>
    Js.Dict.set(params, "ClientMetadata", Js.Json.object_(data))
  | None => ()
  };

  //   "Password": "string",
  Js.Dict.set(params, "Password", Js.Json.string(password));

  //   "SecretHash": "string",
  switch (secretHash) {
  | Some(secretHash) =>
    Js.Dict.set(params, "SecretHash", Js.Json.boolean(secretHash))
  | None => ()
  };

  //   "UserAttributes": [{"Name": "string", "Value": "string"}],
  Js.Dict.set(
    params,
    "UserAttributes",
    jsonMapString(attributes)->Js.Json.objectArray,
  );

  // ==ADVANCED SECURITY UNIMPLEMENTED==
  //   "UserContextData": {
  //     "EncodedData": "string",
  //   },
  /* ^^ADVANCED SECURITY UNIMPLEMENTED^*/

  //   "Username": "string",
  Js.Dict.set(params, "Username", Js.Json.string(username));

  //   "ValidationData": [{"Name": "string", "Value": "string"}],
  Js.Dict.set(
    params,
    "ValidationData",
    jsonMapString(validationData)->Js.Json.objectArray,
  );
  // };

  Client.request(config, SignUp, params)
  ->Future.flatMapOk(res =>
      Future.value(
        switch (res.status) {
        | Success(_) =>
          switch (makeSignupResponse(res.json)) {
          | Some(result) => Belt.Result.Ok(result)
          | None => Belt.Result.Error(`ReasonCognitoSerdeError(res.json))
          }
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(res.json->makeErrKind(makeSignUpErrors))
        },
      )
    );
};

let confirmSignUp =
    (
      config,
      ~username,
      ~confirmationCode,
      ~forceAliasCreation=false,
      ~secretHash=?,
      ~clientMetadata=?,
      ~analyticsEndpointId=?,
      (),
    ) => {
  // https://docs.aws.amazon.com/cognito-user-identity-pools/latest/APIReference/API_ConfirmSignUp.html

  // {
  let params = Js.Dict.empty();
  //   "AnalyticsMetadata": {
  //     "AnalyticsEndpointId": "string",
  //   },
  switch (analyticsEndpointId) {
  | Some(id) => Js.Dict.set(params, "AnalyticsMetadata", Js.Json.object_(id))
  | None => ()
  };

  //   "ClientId": "string", <- from client config

  //   "ClientMetadata": {
  //     "string": "string",
  //   },
  switch (clientMetadata) {
  | Some(data) =>
    Js.Dict.set(params, "ClientMetadata", Js.Json.object_(data))
  | None => ()
  };

  //   "ConfirmationCode": "string",
  Js.Dict.set(params, "ConfirmationCode", Js.Json.string(confirmationCode));

  //   "ForceAliasCreation": boolean,
  Js.Dict.set(
    params,
    "ForceAliasCreation",
    Js.Json.boolean(forceAliasCreation),
  );

  //   "SecretHash": "string",
  switch (secretHash) {
  | Some(secretHash) =>
    Js.Dict.set(params, "SecretHash", Js.Json.boolean(secretHash))
  | None => ()
  };

  // ==ADVANCED SECURITY UNIMPLEMENTED==
  //   "UserContextData": {
  //     "EncodedData": "string",
  //   },
  // ^^ADVANCED SECURITY UNIMPLEMENTED^^

  //   "Username": "string",
  Js.Dict.set(params, "Username", Js.Json.string(username));
  // };

  Client.request(config, ConfirmSignUp, params)
  ->Future.flatMapOk(res =>
      Future.value(
        switch (res.status) {
        | Success(_) => Belt.Result.Ok()
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(res.json->makeErrKind(makeConfirmSignUpErrors))
        },
      )
    );
};

let initiateAuth = (config, ~username: string, ~password: string, ()) => {
  let authParams = Js.Dict.empty();
  Js.Dict.set(authParams, "USERNAME", Js.Json.string(username));
  Js.Dict.set(authParams, "PASSWORD", Js.Json.string(password));

  let params = Js.Dict.empty();
  Js.Dict.set(params, "AuthParameters", Js.Json.object_(authParams));
  Js.Dict.set(params, "AuthFlow", Js.Json.string("USER_PASSWORD_AUTH"));
  Client.request(config, InitiateAuth, params)
  ->Future.flatMapOk(res =>
      Future.value(
        switch (res.status) {
        | Success(_) =>
          // We're _really_ hoping amazon holds to their API contract here.
          // If not, we're going to see null type errors.
          Js.log(res);
          let signInResponse = makeSignInResponse(res);
          let authDecoder = signInResponse->authenticationResultDecoderGet;
          let authenticationResult = {
            accessToken: authDecoder->accessTokenGet,
            idToken: authDecoder->idTokenGet,
            refreshToken: authDecoder->refreshTokenGet,
            tokenType: authDecoder->tokenTypeGet,
            expiresIn: authDecoder->expiresInGet,
          };
          Belt.Result.Ok({
            authenticationResult,
            challengeParameters: signInResponse->challengeParametersGet,
          });
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(
            parseCognitoError(res.json)
            ->Belt.Option.mapWithDefault(
                `CognitoUnknownError("temp error"), err =>
                switch (err.__type) {
                | "InvalidParameterException" =>
                  `CognitoInvalidParameter(err.message)
                | "NotAuthorizedException" =>
                  `CognitoNotAuthorized(err.message)
                | _ => `CognitoUnknownError(err.message)
                }
              ),
          )
        },
      )
    );
};
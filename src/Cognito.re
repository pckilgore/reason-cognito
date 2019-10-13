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
  let request = (config, operation, params: Js.Dict.t(Js.Json.t)) => {
    // Setup headers.
    let headers = Js.Dict.empty();
    let target = "AWSCognitoIdentityProviderService." ++ operation;
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

let jsonMapString = arr =>
  Array.map(
    item => Js.Dict.map((. value) => Js.Json.string(value), item),
    arr,
  );

let parseCognitoError = err =>
  switch (Js.Json.decodeObject(err)) {
  | Some(envelope) =>
    switch (
      Js.Dict.get(envelope, "__type"),
      Js.Dict.get(envelope, "message"),
    ) {
    | (Some(type_), Some(msg)) =>
      switch (Js.Json.decodeString(type_), Js.Json.decodeString(msg)) {
      | (Some(__type), Some(message)) => Some({__type, message})
      | _ => None
      }
    | _ => None
    }
  | None => None
  };

let makeSignupResponse = json => {
  switch (Js.Json.decodeObject(json)) {
  | Some(env) =>
    switch (
      Js.Dict.get(env, "UserConfirmed"),
      Js.Dict.get(env, "UserSub"),
      Js.Dict.get(env, "CodeDeliveryDetails"),
    ) {
    | (Some(confirmed), Some(sub), Some(delivery)) =>
      switch (
        Js.Json.decodeBoolean(confirmed),
        Js.Json.decodeString(sub),
        Js.Json.decodeObject(delivery),
      ) {
      | (Some(userConfirmed), Some(userSub), Some(delivery)) =>
        switch (
          Js.Dict.get(delivery, "AttributeName"),
          Js.Dict.get(delivery, "DeliveryMedium"),
          Js.Dict.get(delivery, "Destination"),
        ) {
        | (Some(attrib), Some(medium), Some(dest)) =>
          switch (
            Js.Json.decodeString(attrib),
            Js.Json.decodeString(dest),
            switch (Js.Json.decodeString(medium)) {
            | Some(medium) when medium == "EMAIL" => Email
            | Some(medium) when medium == "SMS" => SMS
            | Some(_)
            | None => UnknownDeliveryMedium
            },
          ) {
          | (Some(attributeName), Some(destination), deliveryMedium) =>
            Some({
              userSub,
              userConfirmed,
              codeDeliveryDetails: {
                attributeName,
                deliveryMedium,
                destination,
              },
            })
          | _ => None
          }
        | _ => None
        }
      | _ => None
      }
    | _ => None
    }
  | None => None
  };
};

let makeSignupErrorVariant = ({__type, message: msg}) => {
  switch (__type) {
  | "InvalidParameterException" => `CognitoInvalidParameter(msg)
  | "UsernameExistsException" => `CognitoUsernameExists(msg)
  | "CodeDeliveryFailureException" => `CognitoCodeDeliveryFailure(msg)
  | "InternalErrorException" => `CognitoInternalError(msg)
  | "InvalidEmailRoleAccessPolicyException" =>
    `CognitoInvalidEmailRoleAccessPolicy(msg)
  | "InvalidLambdaResponseException" => `CognitoInvalidLambdaResponse(msg)
  | "InvalidPasswordException" => `CognitoInvalidPassword(msg)
  | "InvalidSmsRoleAccessPolicysException" =>
    `CognitoInvalidSmsRoleAccessPolicys(msg)
  | "InvalidSmsRoleTrustRelationshipException" =>
    `CognitoInvalidSmsRoleTrustRelationship(msg)
  | "NotAuthorizedException" => `CognitoNotAuthorized(msg)
  | "ResourceNotFoundException" => `CognitoResourceNotFound(msg)
  | "TooManyRequestsException" => `CognitoTooManyRequests(msg)
  | "UnexpectedLambdaException" => `CognitoUnexpectedLambda(msg)
  | "UserLambdaValidationException" => `CognitoUserLambdaValidation(msg)
  | _ => `CognitoUnknownError(msg)
  };
};

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

  Client.request(config, "SignUp", payload)
  ->Future.flatMapOk(res =>
      Future.value(
        switch (res.status) {
        | Success(_) =>
          switch (makeSignupResponse(res.json)) {
          | Some(result) => Belt.Result.Ok(result)
          | None => Belt.Result.Error(`CognitoDeserializeError(res.json))
          }
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(
            switch (parseCognitoError(res.json)) {
            | Some(err) => makeSignupErrorVariant(err)
            | None => `CognitoDeserializeError(res.json)
            },
          )
        },
      )
    );
};
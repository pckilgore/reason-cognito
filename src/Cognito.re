[%raw "require('isomorphic-fetch')"];

open Utils;

module Config = {
  module AuthenticationFlowType = {
    /* https://docs.aws.amazon.com/cognito-user-identity-pools/latest/APIReference/API_InitiateAuth.html#API_InitiateAuth_RequestSyntax */
    type t =
      | USER_PASSWORD_AUTH
      | USER_SRP_AUTH
      | CUSTOM_AUTH;

    let toString =
      fun
      | USER_PASSWORD_AUTH => "USER_PASSWORD_AUTH"
      | USER_SRP_AUTH => "USER_SRP_AUTH"
      | CUSTOM_AUTH => "CUSTOM_AUTH";

    let toJson = t => t->toString->Js.Json.string;
  };
  module Region = {
    type t =
      | UsEast1
      | UsEast2
      | UsWest2
      | ApSouth1
      | ApNortheast1
      | ApNortheast2
      | ApSoutheast1
      | ApSoutheast2
      | CaCentral1
      | EuCentral1
      | EuWest1
      | EuWest2;

    let toString =
      fun
      | UsEast1 => "us-east-1"
      | UsEast2 => "us-east-2"
      | UsWest2 => "us-west-2"
      | ApSouth1 => "ap-south-1"
      | ApNortheast1 => "ap-northeast-2"
      | ApNortheast2 => "ap-northeast-2"
      | ApSoutheast1 => "ap-southeast-1"
      | ApSoutheast2 => "ap-southeast-2"
      | CaCentral1 => "ca-central-1"
      | EuCentral1 => "eu-central-1"
      | EuWest1 => "eu-west-1"
      | EuWest2 => "eu-west-2";
  };
  type t = {
    poolId: string,
    clientId: string,
    endpoint: string,
    authenticationFlowType: AuthenticationFlowType.t,
  };

  let make =
      (
        ~poolId,
        ~clientId,
        ~region,
        ~authenticationFlowType=AuthenticationFlowType.USER_SRP_AUTH,
        (),
      ) => {
    clientId,
    poolId,
    authenticationFlowType,
    endpoint:
      "https://cognito-idp." ++ Region.toString(region) ++ ".amazonaws.com/",
  };
};

module Client = {
  // Trying to maintain a generic implementation here so that it can be plugged out
  // in different libraries.
  type status =
    | Informational(int)
    | Success(int)
    | Redirect(int)
    | ClientError(int)
    | ServerError(int);

  module Operation = {
    type t =
      | ChangePassword
      | ConfirmForgotPassword
      | ConfirmSignUp
      | ForgotPassword
      | InitiateAuth
      | ResendConfirmationCode
      | RespondToAuthChallenge
      | SignUp;

    let toString =
      fun
      | ChangePassword => "ChangePassword"
      | ConfirmForgotPassword => "ConfirmForgotPassword"
      | ConfirmSignUp => "ConfirmSignUp"
      | ForgotPassword => "ForgotPassword"
      | InitiateAuth => "InitiateAuth"
      | ResendConfirmationCode => "ResendConfirmationCode"
      | RespondToAuthChallenge => "RespondToAuthChallenge"
      | SignUp => "SignUp";
  };

  type response = {
    status,
    json: Js.Json.t,
  };

  // Error is a polymorphic variant that composes across the entire
  // library.  Because polymorphic variants are GLOBAL, we use highly
  // descriptive names.  Because we are lazy, we are mostly passing along
  // Js.Promise.Error here, to give end-users maximum handling flexibility.
  type error = [ | `ReasonCognitoClientError(Js.Promise.error)];

  // Request's purpose is to
  // talk to the network and pass a JSON blob back to its caller if
  // there is no error in making the request.  If the request itself
  // returns an error code (4XX, 5XX), that should be handled elsewhere.
  //
  // TODO: More generic implemntation (no Js.* types)
  let request = (config: Config.t, operation, params: Js.Dict.t(Js.Json.t)) => {
    // Setup headers.
    let headers = Js.Dict.empty();
    let target =
      "AWSCognitoIdentityProviderService." ++ Operation.toString(operation);
    Js.Dict.set(headers, "X-Amz-Target", target);
    Js.Dict.set(headers, "Content-Type", "application/x-amz-json-1.1");
    Js.Dict.set(headers, "X-Amz-User-Agent", "reason-cognito/0.1.x js");

    // Add the configured clientId to the request.
    Js.Dict.set(params, "ClientId", Js.Json.string(config.clientId));

    Fetch.fetchWithInit(
      config.endpoint,
      Fetch.RequestInit.make(
        ~method_=Post,
        ~cache=NoCache,
        ~headers=Fetch.HeadersInit.makeWithDict(headers),
        ~body=
          Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(params))),
        (),
      ),
    )
    // Error = fetch did not do anything.
    ->FutureJs.fromPromise(fetchError =>
        `ReasonCognitoClientError(fetchError)
      )
    // OK = fetch did something. Map result to easily consumable type.
    ->Future.flatMapOk(apiResponse =>
        Fetch.Response.json(apiResponse)
        ->FutureJs.fromPromise(err => `ReasonCognitoClientError(err))
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
          Belt.Result.Ok(CognitoJson_bs.read_signUpResponse(res.json))
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(Errors.SignUpErrors.makeFromJson(res.json))
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
  ->Future.flatMapOk(({status, json}) =>
      Future.value(
        switch (status) {
        | Success(_) => Belt.Result.Ok()
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(Errors.ConfirmSignUp.makeFromJson(json))
        },
      )
    );
};

let initiateAuth =
    (
      config,
      ~authParameters,
      ~authFlow,
      ~clientMetadata=?,
      ~analyticsEndpointId=?,
      (),
    ) => {
  // https://docs.aws.amazon.com/cognito-user-identity-pools/latest/APIReference/API_InitiateAuth.html

  open Config;

  let params = Js.Dict.empty();
  //   {
  //    "AnalyticsMetadata": {
  //       "AnalyticsEndpointId": "string"
  //    },
  switch (analyticsEndpointId) {
  | Some(id) => Js.Dict.set(params, "AnalyticsMetadata", Js.Json.object_(id))
  | None => ()
  };

  //    "AuthFlow": "string",
  Js.Dict.set(params, "AuthFlow", AuthenticationFlowType.toJson(authFlow));

  //    "AuthParameters": {
  //       "string" : "string"
  //    },
  // TODO: Make this type smarter (certain auth flows have required AuthParameters)
  Js.Dict.set(
    params,
    "AuthParameters",
    Js.Json.object_(makeJsonStringsFromDictValues(authParameters)),
  );

  //    "ClientId": "string", <- from client config
  //    "ClientMetadata": {
  //       "string" : "string"
  //    },
  switch (clientMetadata) {
  | Some(data) =>
    Js.Dict.set(params, "ClientMetadata", Js.Json.object_(data))
  | None => ()
  };

  // ==ADVANCED SECURITY UNIMPLEMENTED==
  //    "UserContextData": {
  //       "EncodedData": "string"
  //    }
  // ^^ADVANCED SECURITY UNIMPLEMENTED^^
  // }

  Client.request(config, InitiateAuth, params)
  ->Future.flatMapOk(({status, json}) =>
      Future.value(
        switch (status) {
        | Success(_) =>
          Belt.Result.Ok(CognitoJson_bs.read_authenticationResponse(json))
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(Errors.InitiateAuth.makeFromJson(json))
        },
      )
    );
};

let changePassword =
    (
      config,
      ~accessToken: string,
      ~previousPassword: string,
      ~proposedPassword: string,
      (),
    ) => {
  let params = Js.Dict.empty();
  Js.Dict.set(params, "AccessToken", Js.Json.string(accessToken));
  Js.Dict.set(params, "PreviousPassword", Js.Json.string(previousPassword));
  Js.Dict.set(params, "ProposedPassword", Js.Json.string(proposedPassword));

  Client.request(config, ChangePassword, params)
  ->Future.flatMapOk(({status, json}) =>
      Future.value(
        switch (status) {
        | Success(_) => Belt.Result.Ok()
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(Errors.ChangePassword.makeFromJson(json))
        },
      )
    );
};

let confirmForgotPassword =
    (
      config,
      ~username,
      ~password,
      ~confirmationCode,
      ~analyticsEndpointId=?,
      ~clientMetadata=?,
      (),
    ) => {
  let params = Js.Dict.empty();
  //   {
  //    "AnalyticsMetadata": {
  //       "AnalyticsEndpointId": "string"
  //    },
  switch (analyticsEndpointId) {
  | Some(id) => Js.Dict.set(params, "AnalyticsMetadata", Js.Json.object_(id))
  | None => ()
  };

  //    "ClientId": "string", <- from client config
  //    "ClientMetadata": {
  //       "string" : "string"
  //    },
  switch (clientMetadata) {
  | Some(data) =>
    Js.Dict.set(params, "ClientMetadata", Js.Json.object_(data))
  | None => ()
  };

  //    "ConfirmationCode": "string",
  Js.Dict.set(params, "ConfirmationCode", Js.Json.string(confirmationCode));

  //    "Password": "string",
  Js.Dict.set(params, "Password", Js.Json.string(password));

  // ====Unimplemented=========
  //    "SecretHash": "string",
  // ====End Unimplemented=====

  // ==ADVANCED SECURITY UNIMPLEMENTED==
  //   "UserContextData": {
  //     "EncodedData": "string",
  //   },
  // ^^ADVANCED SECURITY UNIMPLEMENTED^^

  //    "Username": "string"
  Js.Dict.set(params, "Username", Js.Json.string(username));
  // }

  Client.request(config, ConfirmForgotPassword, params)
  ->Future.flatMapOk(({status, json}) =>
      Future.value(
        switch (status) {
        | Success(_) => Belt.Result.Ok()
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(Errors.ConfirmForgotPassword.makeFromJson(json))
        },
      )
    );
};

let forgotPassword =
    (config, ~username, ~analyticsEndpointId=?, ~clientMetadata=?, ()) => {
  let params = Js.Dict.empty();
  //   {
  //    "AnalyticsMetadata": {
  //       "AnalyticsEndpointId": "string"
  //    },
  switch (analyticsEndpointId) {
  | Some(id) => Js.Dict.set(params, "AnalyticsMetadata", Js.Json.object_(id))
  | None => ()
  };

  //    "ClientId": "string", <- from config
  //    "ClientMetadata": {
  //       "string" : "string"
  //    },
  switch (clientMetadata) {
  | Some(data) =>
    Js.Dict.set(params, "ClientMetadata", Js.Json.object_(data))
  | None => ()
  };

  // ====Unimplemented=========
  //    "SecretHash": "string",
  // ====End Unimplemented=====

  // ==ADVANCED SECURITY UNIMPLEMENTED==
  //   "UserContextData": {
  //     "EncodedData": "string",
  //   },
  // ^^ADVANCED SECURITY UNIMPLEMENTED^^

  //    "Username": "string"
  Js.Dict.set(params, "Username", Js.Json.string(username));
  // }

  Client.request(config, ForgotPassword, params)
  ->Future.flatMapOk(({status, json}) =>
      Future.value(
        switch (status) {
        | Success(_) =>
          Belt.Result.Ok(CognitoJson_bs.read_codeDeliveryResponse(json))
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(Errors.ForgotPassword.makeFromJson(json))
        },
      )
    );
};

let resendConfirmationCode =
    (config, ~username, ~analyticsEndpointId=?, ~clientMetadata=?, ()) => {
  let params = Js.Dict.empty();
  //   {
  //    "AnalyticsMetadata": {
  //       "AnalyticsEndpointId": "string"
  //    },
  switch (analyticsEndpointId) {
  | Some(id) => Js.Dict.set(params, "AnalyticsMetadata", Js.Json.object_(id))
  | None => ()
  };

  //    "ClientId": "string", <- from config
  //    "ClientMetadata": {
  //       "string" : "string"
  //    },
  switch (clientMetadata) {
  | Some(data) =>
    Js.Dict.set(params, "ClientMetadata", Js.Json.object_(data))
  | None => ()
  };

  // ====Unimplemented=========
  //    "SecretHash": "string",
  // ====End Unimplemented=====

  // ==ADVANCED SECURITY UNIMPLEMENTED==
  //   "UserContextData": {
  //     "EncodedData": "string",
  //   },
  // ^^ADVANCED SECURITY UNIMPLEMENTED^^

  //    "Username": "string"
  Js.Dict.set(params, "Username", Js.Json.string(username));
  // }

  Client.request(config, ResendConfirmationCode, params)
  ->Future.flatMapOk(({status, json}) =>
      Future.value(
        switch (status) {
        | Success(_) =>
          Belt.Result.Ok(CognitoJson_bs.read_codeDeliveryResponse(json))
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(Errors.ResendConfirmationCode.makeFromJson(json))
        },
      )
    );
};

let respondToAuthChallenge =
    (
      config,
      ~challengeName: CognitoJson_bs.challengeName,
      ~challengeResponses=?,
      ~session=?,
      ~analyticsEndpointId=?,
      ~clientMetadata=?,
      (),
    ) => {
  let params = Js.Dict.empty();
  //   {
  //    "AnalyticsMetadata": {
  //       "AnalyticsEndpointId": "string"
  //    },
  switch (analyticsEndpointId) {
  | Some(id) => Js.Dict.set(params, "AnalyticsMetadata", Js.Json.object_(id))
  | None => ()
  };

  //  "ChallengeName": "string",
  Js.Dict.set(
    params,
    "ChallengeName",
    CognitoJson_bs.write_challengeName(challengeName),
  );
  // "ChallengeResponses": {
  //     "string" : "string"
  //  },
  switch (challengeResponses) {
  | Some(data) =>
    Js.Dict.set(params, "ChallengeResponses", Js.Json.object_(data))
  | None => ()
  };
  //    "ClientId": "string", <- from config
  //    "ClientMetadata": {
  //       "string" : "string"
  //    },
  switch (clientMetadata) {
  | Some(data) =>
    Js.Dict.set(params, "ClientMetadata", Js.Json.object_(data))
  | None => ()
  };

  //    "Session": "string"
  switch (session) {
  | Some(data) => Js.Dict.set(params, "Session", Js.Json.string(data))
  | None => ()
  };

  // ==ADVANCED SECURITY UNIMPLEMENTED==
  //   "UserContextData": {
  //     "EncodedData": "string",
  //   },
  // ^^ADVANCED SECURITY UNIMPLEMENTED^^
  // }

  Client.request(config, RespondToAuthChallenge, params)
  ->Future.flatMapOk(({status, json}) =>
      Future.value(
        switch (status) {
        | Success(_) =>
          Belt.Result.Ok(CognitoJson_bs.read_authenticationResponse(json))
        | Informational(_)
        | Redirect(_)
        | ClientError(_)
        | ServerError(_) =>
          Belt.Result.Error(Errors.RespondToAuthChallenge.makeFromJson(json))
        },
      )
    );
};
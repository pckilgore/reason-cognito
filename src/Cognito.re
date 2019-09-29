[@bs.deriving abstract]
type amznErrResponse = {
  __type: string,
  message: string,
};
[@bs.deriving {abstract: light}]
type authenticationParameters = {
  [@bs.as "USERNAME"]
  username: string,
  [@bs.as "PASSWORD"]
  password: string,
};

  [@bs.deriving {abstract: light}]
  type params_bsd = {
    [@bs.as "AuthFlow"]
    authFlow: string,
    [@bs.as "SRP_A"]
    srpa: Js.Json.t,
    [@bs.as "AuthParameters"]
    authParameters: authenticationParameters,
  };
[@bs.deriving abstract]
type codeDeliveryDetailsDecoder = {
  [@bs.as "AttributeName"]
  attributeName: string,
  [@bs.as "DeliveryMedium"]
  deliveryMedium: string,
  [@bs.as "Destination"]
  destination: string,
};

[@bs.deriving abstract]
type signUpResponseDecoder = {
  [@bs.as "CodeDeliveryDetails"]
  codeDeliveryDetailsDecoder,
  [@bs.as "UserConfirmed"]
  userConfirmed: bool,
  [@bs.as "UserSub"]
  userSub: string,
};

type signUpResponse = {
  codeDeliveryDetails,
  userConfirmed: bool,
  userSub: string,
}
and codeDeliveryDetails = {
  attributeName: string,
  deliveryMedium: string,
  destination: string,
};

type amazonResponse('t) =
  | Ok('t)
  | InvalidParameterException(string)
  | UsernameExistsException(string)
  | ReasonCognitoUnknownErrorException(string);

external makeResponse: Js.Json.t => amznErrResponse = "%identity";
external makeSignupResponse: 't => signUpResponseDecoder = "%identity";

let signUp =
    (~username, ~password, ~attributes=[||], ~validationData=[||], ()) => {
  let params = Js.Dict.empty();
  let jsonAttribs =
    Array.map(
      attrib => Js.Dict.map((. av) => Js.Json.string(av), attrib),
      attributes,
    );

  Js.Dict.set(params, "Username", Js.Json.string(username));
  Js.Dict.set(params, "Password", Js.Json.string(password));
  Js.Dict.set(params, "UserAttributes", Js.Json.objectArray(jsonAttribs));
  Js.Dict.set(params, "ValidationData", Js.Json.objectArray(validationData));

  Client.request(`SignUp, params)
  ->Future.mapOk(resp => {
      let isErrorResponse = makeResponse(resp);
      let err = isErrorResponse->__typeGet;
      let msg = isErrorResponse->messageGet;
      Js.log2("Raw Response", resp);

      switch (err) {
      | "InvalidParameterException" => InvalidParameterException(msg)
      | "UsernameExistsException" => UsernameExistsException(msg)
      | _ =>
        let signUpResponse = makeSignupResponse(resp);
        let cddDecoder = signUpResponse->codeDeliveryDetailsDecoderGet;
        let codeDeliveryDetails = {
          attributeName: cddDecoder->attributeNameGet,
          deliveryMedium: cddDecoder->deliveryMediumGet,
          destination: cddDecoder->destinationGet,
        };
        Ok({
          codeDeliveryDetails,
          userConfirmed: signUpResponse->userConfirmedGet,
          userSub: signUpResponse->userSubGet,
        });
      };
    });
};

let confirmSignUp = (~username, ~confirmationCode, ()) => {
  let params = Js.Dict.empty();

  Js.Dict.set(params, "Username", Js.Json.string(username));
  Js.Dict.set(params, "ConfirmationCode", Js.Json.string(confirmationCode));

  Client.request(`ConfirmSignUp, params)
  ->Future.mapOk(resp => {
      let isErrorResponse = makeResponse(resp);
      let err = isErrorResponse->__typeGet;
      let msg = isErrorResponse->messageGet;
      Js.log2("Raw Response", resp);

      switch (err) {
      | "InvalidParameterException" => InvalidParameterException(msg)
      | "UsernameExistsException" => UsernameExistsException(msg)
      | _ =>
        let signUpResponse = makeSignupResponse(resp);
        let cddDecoder = signUpResponse->codeDeliveryDetailsDecoderGet;
        let codeDeliveryDetails = {
          attributeName: cddDecoder->attributeNameGet,
          deliveryMedium: cddDecoder->deliveryMediumGet,
          destination: cddDecoder->destinationGet,
        };
        Ok({
          codeDeliveryDetails,
          userConfirmed: signUpResponse->userConfirmedGet,
          userSub: signUpResponse->userSubGet,
        });
      };
    });
};
// this doesnt work yet. I need to figure out how to get a token from the endpoint to pass to this client request
let changePassword = (~oldUserPassword: string, ~newUserPassword: string, ()) => {
  let params = Js.Dict.empty();
  Js.Dict.set(params, "PreviousPassword", Js.Json.string(oldUserPassword));
  Js.Dict.set(params, "ProposedPassword", Js.Json.string(newUserPassword));

  Client.request(`ChangePassword, params)
  ->Future.mapOk(resp => {
      let isErrorResponse = makeResponse(resp);
      let err = isErrorResponse->__typeGet;
      let msg = isErrorResponse->messageGet;
      Js.log2("Raw Response", resp);

      switch (err) {
      | "InvalidParameterException" => InvalidParameterException(msg)
      | "UsernameExistsException" => UsernameExistsException(msg)
      | _ =>
        let signUpResponse = makeSignupResponse(resp);
        let cddDecoder = signUpResponse->codeDeliveryDetailsDecoderGet;
        let codeDeliveryDetails = {
          attributeName: cddDecoder->attributeNameGet,
          deliveryMedium: cddDecoder->deliveryMediumGet,
          destination: cddDecoder->destinationGet,
        };
        Ok({
          codeDeliveryDetails,
          userConfirmed: signUpResponse->userConfirmedGet,
          userSub: signUpResponse->userSubGet,
        });
      };
    });
};
external jsDict2Str: Js.Dict.t(string) => string = "%identity";
external toJsDict: 'a => Js.Dict.t(Js.Json.t) = "%identity";
let signIn = (~username: string, ~password: string, ()) => {
  let signInParams = authenticationParameters(~username, ~password)->toJsDict;
/* create and object of this shape using our `authenticationParameters` type
   ```json
   {
     AuthParameters : {
       "USERNAME": "idkjs",
       "PASSWORD": "somepassword",
     }
   }
   ```
   At this point we know we have a valid js object which can test with `Js.Json.test(signInParams, Object)`
   Since we know its valid we will change its type to Js.Dict.t(Js.Json.t) which is expected by out params dict. We then use `Js.Dict.set(params, "AuthParameters", Js.Json.object_(signInParams));`to push it to the `AuthParameters` key expected by the cognito client.
    */
  let params = Js.Dict.empty();
    Js.Dict.set(params, "AuthParameters", Js.Json.object_(signInParams));
  Client.request(`InitiateAuth, params)
  ->Future.mapOk(resp => {
      let isErrorResponse = makeResponse(resp);
      let err = isErrorResponse->__typeGet;
      let msg = isErrorResponse->messageGet;
      Js.log2("Raw Response", resp);

      switch (err) {
      | "InvalidParameterException" => InvalidParameterException(msg)
      | "UsernameExistsException" => UsernameExistsException(msg)
      | _ =>
        let signUpResponse = makeSignupResponse(resp);
        let cddDecoder = signUpResponse->codeDeliveryDetailsDecoderGet;
        let codeDeliveryDetails = {
          attributeName: cddDecoder->attributeNameGet,
          deliveryMedium: cddDecoder->deliveryMediumGet,
          destination: cddDecoder->destinationGet,
        };
        Ok({
          codeDeliveryDetails,
          userConfirmed: signUpResponse->userConfirmedGet,
          userSub: signUpResponse->userSubGet,
        });
      };
    });
};
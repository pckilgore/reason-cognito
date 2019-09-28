[@bs.deriving abstract]
type amznErrResponse = {
  __type: string,
  message: string,
};
[@bs.deriving {abstract: light}]
type auth_params_bsd = {
  [@bs.as "USERNAME"]
  username: string,
  [@bs.as "PASSWORD"]
  password: string,
};
// [@bs.deriving {abstract: light}]
// type passWord = {
//   [@bs.as "Password"]
//   password: string,
// };
[@bs.deriving {abstract: light}]
type params_bsd = {
  [@bs.as "AuthFlow"]
  authFlow: string,
  [@bs.as "AuthParameters"]
  authParameters: auth_params_bsd,
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
let initiateAuth = (~username: string, ~password: string, ()) => {
  let auth_bsd = auth_params_bsd(~username, ~password);
  let params_bsd =
    params_bsd(~authFlow="ADMIN_NO_SRP_AUTH", ~authParameters=auth_bsd);
  let params = Js.Dict.empty();
  let userNameObj = Js.Dict.empty();
  Js.Dict.set(userNameObj, "USERNAME", username);

  Js.log2("userNameObj", userNameObj);
  let username = userNameObj->jsDict2Str;
  Js.Dict.set(params, "AuthFlow", Js.Json.string("CUSTOM_AUTH"));
  Js.Dict.set(params, "AuthParameters", Js.Json.string(username));
  let params = params_bsd->toJsDict;
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
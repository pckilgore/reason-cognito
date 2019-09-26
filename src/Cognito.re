[@bs.deriving abstract]
type amznErrResponse = {
  __type: string,
  message: string,
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

  Client.request("SignUp", params)
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
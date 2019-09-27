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
  deliveryMedium,
  destination: string,
}
and deliveryMedium =
  | Email
  | SMS;


type amazonResponse('t) =
  | Ok('t)
  | ReasonCognitoUnknownErrorException(string)
  | CodeDeliveryFailureException(string)
  | InternalErrorException(string)
  | InvalidEmailRoleAccessPolicyException(string)
  | InvalidLambdaResponseException(string)
  | InvalidParameterException(string)
  | InvalidPasswordException(string)
  | InvalidSmsRoleAccessPolicyExceptions(string)
  | InvalidSmsRoleTrustRelationshipException(string)
  | NotAuthorizedException(string)
  | ResourceNotFoundException(string)
  | TooManyRequestsException(string)
  | UnexpectedLambdaException(string)
  | UserLambdaValidationException(string)
  | UsernameExistsException(string);

external makeResponse: Js.Json.t => amznErrResponse = "%identity";
external makeSignupResponse: 't => signUpResponseDecoder = "%identity";

let jsonMapString = arr =>
  Array.map(
    item => Js.Dict.map((. value) => Js.Json.string(value), item),
    arr,
  );

let signUp =
    (~username, ~password, ~attributes=[||], ~validationData=[||], ()) => {
  let jsonAttribs = jsonMapString(attributes);
  let jsonVData = jsonMapString(validationData);

  let params = Js.Dict.empty();
  Js.Dict.set(params, "Username", Js.Json.string(username));
  Js.Dict.set(params, "Password", Js.Json.string(password));
  Js.Dict.set(params, "UserAttributes", Js.Json.objectArray(jsonAttribs));
  Js.Dict.set(params, "ValidationData", Js.Json.objectArray(jsonVData));

  Client.request("SignUp", params)
  ->Future.mapOk(resp => {
      // do this from header status code see https://docs.aws.amazon.com/en_pv/cognito-user-identity-pools/latest/APIReference/API_SignUp.html
      let isErrorResponse = makeResponse(resp);
      let err = isErrorResponse->__typeGet;
      let msg = isErrorResponse->messageGet;
      Js.log2("Raw Response", resp);

      switch (err) {
      | "InvalidParameterException" => InvalidParameterException(msg)
      | "UsernameExistsException" => UsernameExistsException(msg)
      | "UserLambdaValidationException"
      | _ =>
        let signUpResponse = makeSignupResponse(resp);
        let cddDecoder = signUpResponse->codeDeliveryDetailsDecoderGet;
        let codeDeliveryDetails = {
          attributeName: cddDecoder->attributeNameGet,
          deliveryMedium:
            cddDecoder->deliveryMediumGet === "EMAIL" ? Email : SMS,
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
type authenticationFlowType =
  | USER_PASSWORD_AUTH
  | USER_SRP_AUTH
  | CUSTOM_AUTH;

type supportedRegions =
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

let makeRegionString =
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
  | SMS
  | UnknownDeliveryMedium;

type rawCognitoError = {
  __type: string,
  message: string,
};

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
  | `CognitoDeserializeError(Js.Json.t)
];
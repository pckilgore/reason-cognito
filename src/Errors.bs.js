// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var CognitoJson_bs = require("./atd/CognitoJson_bs.bs.js");

function make(param) {
  var message = param[/* message */1];
  switch (param[/* __type */0]) {
    case "InternalErrorException" :
        return /* `CognitoInternalError */[
                636052602,
                message
              ];
    case "InvalidLambdaResponseException" :
        return /* `CognitoInvalidLambdaResponse */[
                -1072578002,
                message
              ];
    case "InvalidParameterException" :
        return /* `CognitoInvalidParameter */[
                -267133469,
                message
              ];
    case "NotAuthorizedException" :
        return /* `CognitoNotAuthorized */[
                -1019683139,
                message
              ];
    case "ResourceNotFoundException" :
        return /* `CognitoResourceNotFound */[
                281060686,
                message
              ];
    case "TooManyRequestsException" :
        return /* `CognitoTooManyRequests */[
                165203366,
                message
              ];
    case "UnexpectedLambdaException" :
        return /* `CognitoUnexpectedLambda */[
                -539579927,
                message
              ];
    case "UserLambdaValidationException" :
        return /* `CognitoUserLambdaValidation */[
                1019055420,
                message
              ];
    default:
      return ;
  }
}

function makeFromJson(json) {
  return make(Curry._1(CognitoJson_bs.read_error, json));
}

var Common = {
  make: make,
  makeFromJson: makeFromJson
};

function make$1(param) {
  var message = param[/* message */1];
  switch (param[/* __type */0]) {
    case "CodeDeliveryFailureException" :
        return /* `CognitoCodeDeliveryFailure */[
                307717752,
                message
              ];
    case "InvalidEmailRoleAccessPolicyException" :
        return /* `CognitoInvalidEmailRoleAccessPolicy */[
                -320011326,
                message
              ];
    case "InvalidPasswordException" :
        return /* `CognitoInvalidPassword */[
                -17702879,
                message
              ];
    case "InvalidSmsRoleAccessPolicysException" :
        return /* `CognitoInvalidSmsRoleAccessPolicys */[
                598714580,
                message
              ];
    case "InvalidSmsRoleTrustRelationshipException" :
        return /* `CognitoInvalidSmsRoleTrustRelationship */[
                -198400121,
                message
              ];
    case "UsernameExistsException" :
        return /* `CognitoUsernameExists */[
                -157849469,
                message
              ];
    default:
      return /* ReasonCognitoUnknownError */536808491;
  }
}

function makeFromJson$1(json) {
  var err = Curry._1(CognitoJson_bs.read_error, json);
  var match = make(err);
  if (match !== undefined) {
    return match;
  } else {
    return make$1(err);
  }
}

var ConfirmSignUp = {
  make: make$1,
  makeFromJson: makeFromJson$1
};

function make$2(param) {
  var message = param[/* message */1];
  switch (param[/* __type */0]) {
    case "CodeDeliveryFailureException" :
        return /* `CognitoCodeDeliveryFailure */[
                307717752,
                message
              ];
    case "InvalidEmailRoleAccessPolicyException" :
        return /* `CognitoInvalidEmailRoleAccessPolicy */[
                -320011326,
                message
              ];
    case "InvalidPasswordException" :
        return /* `CognitoInvalidPassword */[
                -17702879,
                message
              ];
    case "InvalidSmsRoleAccessPolicysException" :
        return /* `CognitoInvalidSmsRoleAccessPolicys */[
                598714580,
                message
              ];
    case "InvalidSmsRoleTrustRelationshipException" :
        return /* `CognitoInvalidSmsRoleTrustRelationship */[
                -198400121,
                message
              ];
    case "UsernameExistsException" :
        return /* `CognitoUsernameExists */[
                -157849469,
                message
              ];
    default:
      return /* ReasonCognitoUnknownError */536808491;
  }
}

function makeFromJson$2(json) {
  var err = Curry._1(CognitoJson_bs.read_error, json);
  var match = make(err);
  if (match !== undefined) {
    return match;
  } else {
    return make$2(err);
  }
}

var SignUpErrors = {
  make: make$2,
  makeFromJson: makeFromJson$2
};

function make$3(param) {
  var message = param[/* message */1];
  switch (param[/* __type */0]) {
    case "InvalidSmsRoleAccessPolicysException" :
        return /* `CognitoInvalidSmsRoleAccessPolicys */[
                598714580,
                message
              ];
    case "InvalidSmsRoleTrustRelationshipException" :
        return /* `CognitoInvalidSmsRoleTrustRelationship */[
                -198400121,
                message
              ];
    case "InvalidUserPoolConfigurationException" :
        return /* `CognitoInvalidUserPoolConfiguration */[
                992401065,
                message
              ];
    case "PasswordResetRequiredException" :
        return /* `CognitoPasswordResetRequired */[
                959433314,
                message
              ];
    case "UserNotConfirmedException" :
        return /* `CognitoUserNotConfirmed */[
                937418472,
                message
              ];
    case "UserNotFoundException" :
        return /* `CognitoUserNotFound */[
                376697675,
                message
              ];
    default:
      return /* ReasonCognitoUnknownError */536808491;
  }
}

function makeFromJson$3(json) {
  var err = Curry._1(CognitoJson_bs.read_error, json);
  var match = make(err);
  if (match !== undefined) {
    return match;
  } else {
    return make$3(err);
  }
}

var InitiateAuth = {
  make: make$3,
  makeFromJson: makeFromJson$3
};

function make$4(param) {
  var message = param[/* message */1];
  switch (param[/* __type */0]) {
    case "InvalidPasswordException" :
        return /* `CognitoInvalidPassword */[
                -17702879,
                message
              ];
    case "LimitExceededException" :
        return /* `CognitoLimitExceeded */[
                767083101,
                message
              ];
    case "PasswordResetRequiredException" :
        return /* `CognitoPasswordResetRequired */[
                959433314,
                message
              ];
    case "UserNotConfirmedException" :
        return /* `CognitoUserNotConfirmed */[
                937418472,
                message
              ];
    case "UserNotFoundException" :
        return /* `CognitoUserNotFound */[
                376697675,
                message
              ];
    default:
      return /* ReasonCognitoUnknownError */536808491;
  }
}

function makeFromJson$4(json) {
  var err = Curry._1(CognitoJson_bs.read_error, json);
  var match = make(err);
  if (match !== undefined) {
    return match;
  } else {
    return make$4(err);
  }
}

var ChangePassword = {
  make: make$4,
  makeFromJson: makeFromJson$4
};

function make$5(param) {
  var message = param[/* message */1];
  switch (param[/* __type */0]) {
    case "CodeMismatchException" :
        return /* `CognitoCodeMismatch */[
                -817363892,
                message
              ];
    case "ExpiredCodeException" :
        return /* `CognitoExpiredCode */[
                -64788767,
                message
              ];
    case "InvalidPasswordException" :
        return /* `CognitoInvalidPassword */[
                -17702879,
                message
              ];
    case "LimitExceededException" :
        return /* `CognitoLimitExceeded */[
                767083101,
                message
              ];
    case "TooManyFailedAttemptsException" :
        return /* `CognitoTooManyFailedAttempts */[
                261105285,
                message
              ];
    case "UserNotConfirmedException" :
        return /* `CognitoUserNotConfirmed */[
                937418472,
                message
              ];
    case "UserNotFoundException" :
        return /* `CognitoUserNotFound */[
                376697675,
                message
              ];
    default:
      return /* ReasonCognitoUnknownError */536808491;
  }
}

function makeFromJson$5(json) {
  var err = Curry._1(CognitoJson_bs.read_error, json);
  var match = make(err);
  if (match !== undefined) {
    return match;
  } else {
    return make$5(err);
  }
}

var ConfirmForgotPassword = {
  make: make$5,
  makeFromJson: makeFromJson$5
};

function make$6(param) {
  var message = param[/* message */1];
  switch (param[/* __type */0]) {
    case "CodeDeliveryFailureException" :
        return /* `CognitoCodeDeliveryFailure */[
                307717752,
                message
              ];
    case "InvalidEmailRoleAccessPolicyException" :
        return /* `CognitoInvalidEmailRoleAccessPolicy */[
                -320011326,
                message
              ];
    case "InvalidSmsRoleAccessPolicyException" :
        return /* `CognitoInvalidSmsRoleAccessPolicy */[
                782712511,
                message
              ];
    case "InvalidSmsRoleTrustRelationshipException" :
        return /* `CognitoInvalidSmsRoleTrustRelationship */[
                -198400121,
                message
              ];
    case "LimitExceededException" :
        return /* `CognitoLimitExceeded */[
                767083101,
                message
              ];
    case "UserNotConfirmedException" :
        return /* `CognitoUserNotConfirmed */[
                937418472,
                message
              ];
    case "UserNotFoundException" :
        return /* `CognitoUserNotFound */[
                376697675,
                message
              ];
    default:
      return /* ReasonCognitoUnknownError */536808491;
  }
}

function makeFromJson$6(json) {
  var err = Curry._1(CognitoJson_bs.read_error, json);
  var match = make(err);
  if (match !== undefined) {
    return match;
  } else {
    return make$6(err);
  }
}

var ForgotPassword = {
  make: make$6,
  makeFromJson: makeFromJson$6
};

function make$7(param) {
  var message = param[/* message */1];
  switch (param[/* __type */0]) {
    case "CodeDeliveryFailureException" :
        return /* `CognitoCodeDeliveryFailure */[
                307717752,
                message
              ];
    case "InvalidEmailRoleAccessPolicyException" :
        return /* `CognitoInvalidEmailRoleAccessPolicy */[
                -320011326,
                message
              ];
    case "InvalidSmsRoleAccessPolicyException" :
        return /* `CognitoInvalidSmsRoleAccessPolicy */[
                782712511,
                message
              ];
    case "InvalidSmsRoleTrustRelationshipException" :
        return /* `CognitoInvalidSmsRoleTrustRelationship */[
                -198400121,
                message
              ];
    case "LimitExceededException" :
        return /* `CognitoLimitExceeded */[
                767083101,
                message
              ];
    case "UserNotFoundException" :
        return /* `CognitoUserNotFound */[
                376697675,
                message
              ];
    default:
      return /* ReasonCognitoUnknownError */536808491;
  }
}

function makeFromJson$7(json) {
  var err = Curry._1(CognitoJson_bs.read_error, json);
  var match = make(err);
  if (match !== undefined) {
    return match;
  } else {
    return make$7(err);
  }
}

var ResendConfirmationCode = {
  make: make$7,
  makeFromJson: makeFromJson$7
};

function make$8(param) {
  var message = param[/* message */1];
  switch (param[/* __type */0]) {
    case "AliasExistsException" :
        return /* `CognitoAliasExists */[
                -498244869,
                message
              ];
    case "CodeMismatchException" :
        return /* `CognitoCodeMismatch */[
                -817363892,
                message
              ];
    case "ExpiredCodeException" :
        return /* `CognitoExpiredCode */[
                -64788767,
                message
              ];
    case "InvalidPasswordException" :
        return /* `CognitoInvalidPassword */[
                -17702879,
                message
              ];
    case "InvalidSmsRoleAccessPolicyException" :
        return /* `CognitoInvalidSmsRoleAccessPolicy */[
                782712511,
                message
              ];
    case "InvalidSmsRoleTrustRelationshipException" :
        return /* `CognitoInvalidSmsRoleTrustRelationship */[
                -198400121,
                message
              ];
    case "InvalidUserPoolConfigurationException" :
        return /* `CognitoInvalidUserPoolConfiguration */[
                992401065,
                message
              ];
    case "MFAMethodNotFoundException" :
        return /* `CognitoMFAMethodNotFound */[
                -618443289,
                message
              ];
    case "PasswordResetRequiredException" :
        return /* `CognitoPasswordResetRequired */[
                959433314,
                message
              ];
    case "SoftwareTokenMFANotFoundException" :
        return /* `CognitoSoftwareTokenMFANotFound */[
                -723187146,
                message
              ];
    case "UserNotConfirmedException" :
        return /* `CognitoUserNotConfirmed */[
                937418472,
                message
              ];
    case "UserNotFoundException" :
        return /* `CognitoUserNotFound */[
                376697675,
                message
              ];
    default:
      return /* ReasonCognitoUnknownError */536808491;
  }
}

function makeFromJson$8(json) {
  var err = Curry._1(CognitoJson_bs.read_error, json);
  var match = make(err);
  if (match !== undefined) {
    return match;
  } else {
    return make$8(err);
  }
}

var RespondToAuthChallenge = {
  make: make$8,
  makeFromJson: makeFromJson$8
};

exports.Common = Common;
exports.ConfirmSignUp = ConfirmSignUp;
exports.SignUpErrors = SignUpErrors;
exports.InitiateAuth = InitiateAuth;
exports.ChangePassword = ChangePassword;
exports.ConfirmForgotPassword = ConfirmForgotPassword;
exports.ForgotPassword = ForgotPassword;
exports.ResendConfirmationCode = ResendConfirmationCode;
exports.RespondToAuthChallenge = RespondToAuthChallenge;
/* CognitoJson_bs Not a pure module */

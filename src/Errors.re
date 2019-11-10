type apiErrorMessage = string;
type reasonCognitoErrors = [ | `ReasonCognitoUnknownError];

module Common = {
  type t = [
    | `CognitoInternalError(apiErrorMessage)
    | `CognitoInvalidLambdaResponse(apiErrorMessage)
    | `CognitoInvalidParameter(apiErrorMessage)
    | `CognitoNotAuthorized(apiErrorMessage)
    | `CognitoResourceNotFound(apiErrorMessage)
    | `CognitoTooManyRequests(apiErrorMessage)
    | `CognitoUnexpectedLambda(apiErrorMessage)
    | `CognitoUserLambdaValidation(apiErrorMessage)
  ];

  let make = ({__type, message}: CognitoJson_bs.error) =>
    switch (__type) {
    | "InternalErrorException" => Some(`CognitoInternalError(message))
    | "InvalidLambdaResponseException" =>
      Some(`CognitoInvalidLambdaResponse(message))
    | "InvalidParameterException" => Some(`CognitoInvalidParameter(message))
    | "NotAuthorizedException" => Some(`CognitoNotAuthorized(message))
    | "ResourceNotFoundException" => Some(`CognitoResourceNotFound(message))
    | "TooManyRequestsException" => Some(`CognitoTooManyRequests(message))
    | "UnexpectedLambdaException" => Some(`CognitoUnexpectedLambda(message))
    | "UserLambdaValidationException" =>
      Some(`CognitoUserLambdaValidation(message))
    | _ => None
    };

  let makeFromJson = json => json->CognitoJson_bs.read_error->make;
};

module ConfirmSignUp = {
  type t = [
    | `CognitoAliasExists(apiErrorMessage)
    | `CognitoCodeMismatch(apiErrorMessage)
    | `CognitoExpiredCode(apiErrorMessage)
    | `CognitoLimitExceeded(apiErrorMessage)
    | `CognitoTooManyFailedAttempts(apiErrorMessage)
    | `CognitoUserNotFound(apiErrorMessage)
    | `ReasonCognitoUnknownError
  ];

  let make = ({__type, message}: CognitoJson_bs.error) =>
    switch (__type) {
    | "UsernameExistsException" => `CognitoUsernameExists(message)
    | "CodeDeliveryFailureException" => `CognitoCodeDeliveryFailure(message)
    | "InvalidEmailRoleAccessPolicyException" =>
      `CognitoInvalidEmailRoleAccessPolicy(message)
    | "InvalidPasswordException" => `CognitoInvalidPassword(message)
    | "InvalidSmsRoleAccessPolicysException" =>
      `CognitoInvalidSmsRoleAccessPolicys(message)
    | "InvalidSmsRoleTrustRelationshipException" =>
      `CognitoInvalidSmsRoleTrustRelationship(message)
    | _ => `ReasonCognitoUnknownError
    };

  let makeFromJson = json => {
    let err = CognitoJson_bs.read_error(json);
    switch (Common.make(err)) {
    | Some(commonError) => commonError
    | None => make(err)
    };
  };
};

module SignUpErrors = {
  type t = [
    | `CognitoCodeDeliveryFailure(apiErrorMessage)
    | `CognitoInvalidEmailRoleAccessPolicy(apiErrorMessage)
    | `CognitoInvalidPassword(apiErrorMessage)
    | `CognitoInvalidSmsRoleAccessPolicys(apiErrorMessage)
    | `CognitoInvalidSmsRoleTrustRelationship(apiErrorMessage)
    | `CognitoUsernameExists(apiErrorMessage)
    | `ReasonCognitoUnknownError
  ];

  let make = ({__type, message}: CognitoJson_bs.error) =>
    switch (__type) {
    | "CodeDeliveryFailureException" => `CognitoCodeDeliveryFailure(message)
    | "InvalidEmailRoleAccessPolicyException" =>
      `CognitoInvalidEmailRoleAccessPolicy(message)
    | "InvalidPasswordException" => `CognitoInvalidPassword(message)
    | "InvalidSmsRoleAccessPolicysException" =>
      `CognitoInvalidSmsRoleAccessPolicys(message)
    | "InvalidSmsRoleTrustRelationshipException" =>
      `CognitoInvalidSmsRoleTrustRelationship(message)
    | "UsernameExistsException" => `CognitoUsernameExists(message)
    | _ => `ReasonCognitoUnknownError
    };

  let makeFromJson = json => {
    let err = CognitoJson_bs.read_error(json);
    switch (Common.make(err)) {
    | Some(commonError) => commonError
    | None => make(err)
    };
  };
};

module InitiateAuth = {
  type t = [
    | `CognitoInvalidSmsRoleAccessPolicy(apiErrorMessage)
    | `CognitoInvalidSmsRoleTrustRelationship(apiErrorMessage)
    | `CognitoInvalidUserPoolConfiguration(apiErrorMessage)
    | `CognitoPasswordResetRequired(apiErrorMessage)
    | `CognitoUserNotConfirmed(apiErrorMessage)
    | `CognitoUserNotFound(apiErrorMessage)
    | `ReasonCognitoUnknownError
  ];

  let make = ({__type, message}: CognitoJson_bs.error) =>
    switch (__type) {
    | "InvalidSmsRoleAccessPolicysException" =>
      `CognitoInvalidSmsRoleAccessPolicys(message)
    | "InvalidSmsRoleTrustRelationshipException" =>
      `CognitoInvalidSmsRoleTrustRelationship(message)
    | "InvalidUserPoolConfigurationException" =>
      `CognitoInvalidUserPoolConfiguration(message)
    | "PasswordResetRequiredException" =>
      `CognitoPasswordResetRequired(message)
    | "UserNotConfirmedException" => `CognitoUserNotConfirmed(message)
    | "UserNotFoundException" => `CognitoUserNotFound(message)
    | _ => `ReasonCognitoUnknownError
    };

  let makeFromJson = json => {
    let err = CognitoJson_bs.read_error(json);
    switch (Common.make(err)) {
    | Some(commonError) => commonError
    | None => make(err)
    };
  };
};

module ChangePassword = {
  type t = [
    | `CognitoInvalidPassword(apiErrorMessage)
    | `CognitoLimitExceeded(apiErrorMessage)
    | `CognitoPasswordResetRequired(apiErrorMessage)
    | `CognitoUserNotConfirmed(apiErrorMessage)
    | `CognitoUserNotFound(apiErrorMessage)
    | `ReasonCognitoUnknownError
  ];

  let make = ({__type, message}: CognitoJson_bs.error) =>
    switch (__type) {
    | "InvalidPasswordException" => `CognitoInvalidPassword(message)
    | "LimitExceededException" => `CognitoLimitExceeded(message)
    | "PasswordResetRequiredException" =>
      `CognitoPasswordResetRequired(message)
    | "UserNotConfirmedException" => `CognitoUserNotConfirmed(message)
    | "UserNotFoundException" => `CognitoUserNotFound(message)
    | _ => `ReasonCognitoUnknownError
    };

  let makeFromJson = json => {
    let err = CognitoJson_bs.read_error(json);
    switch (Common.make(err)) {
    | Some(commonError) => commonError
    | None => make(err)
    };
  };
};

module ConfirmForgotPassword = {
  type t = [
    | `CognitoCodeMismatch(apiErrorMessage)
    | `CognitoExpiredCode(apiErrorMessage)
    | `CognitoInvalidPassword(apiErrorMessage)
    | `CognitoLimitExceeded(apiErrorMessage)
    | `CognitoTooManyFailedAttempts(apiErrorMessage)
    | `CognitoUserNotConfirmed(apiErrorMessage)
    | `CognitoUserNotFound(apiErrorMessage)
    | `ReasonCognitoUnknownError
  ];

  let make = ({__type, message}: CognitoJson_bs.error) =>
    switch (__type) {
    | "CodeMismatchException" => `CognitoCodeMismatch(message)
    | "ExpiredCodeException" => `CognitoExpiredCode(message)
    | "InvalidPasswordException" => `CognitoInvalidPassword(message)
    | "LimitExceededException" => `CognitoLimitExceeded(message)
    | "TooManyFailedAttemptsException" =>
      `CognitoTooManyFailedAttempts(message)
    | "UserNotConfirmedException" => `CognitoUserNotConfirmed(message)
    | "UserNotFoundException" => `CognitoUserNotFound(message)
    | _ => `ReasonCognitoUnknownError
    };

  let makeFromJson = json => {
    let err = CognitoJson_bs.read_error(json);
    switch (Common.make(err)) {
    | Some(commonError) => commonError
    | None => make(err)
    };
  };
};

module ForgotPassword = {
  type t = [
    | `CognitoCodeDeliveryFailure(apiErrorMessage)
    | `CognitoInvalidEmailRoleAccessPolicy(apiErrorMessage)
    | `CognitoInvalidSmsRoleAccessPolicy(apiErrorMessage)
    | `CognitoInvalidSmsRoleTrustRelationship(apiErrorMessage)
    | `CognitoLimitExceeded(apiErrorMessage)
    | `CognitoUserNotConfirmed(apiErrorMessage)
    | `CognitoUserNotFound(apiErrorMessage)
    | `ReasonCognitoUnknownError
  ];

  let make = ({__type, message}: CognitoJson_bs.error) =>
    switch (__type) {
    | "CodeDeliveryFailureException" => `CognitoCodeDeliveryFailure(message)
    | "InvalidEmailRoleAccessPolicyException" =>
      `CognitoInvalidEmailRoleAccessPolicy(message)
    | "InvalidSmsRoleAccessPolicyException" =>
      `CognitoInvalidSmsRoleAccessPolicy(message)
    | "InvalidSmsRoleTrustRelationshipException" =>
      `CognitoInvalidSmsRoleTrustRelationship(message)
    | "LimitExceededException" => `CognitoLimitExceeded(message)
    | "UserNotConfirmedException" => `CognitoUserNotConfirmed(message)
    | "UserNotFoundException" => `CognitoUserNotFound(message)
    | _ => `ReasonCognitoUnknownError
    };

  let makeFromJson = json => {
    let err = CognitoJson_bs.read_error(json);
    switch (Common.make(err)) {
    | Some(commonError) => commonError
    | None => make(err)
    };
  };
};

module ResendConfirmationCode = {
  type t = [
    | `CognitoCodeDeliveryFailure(apiErrorMessage)
    | `CognitoInvalidEmailRoleAccessPolicy(apiErrorMessage)
    | `CognitoInvalidSmsRoleAccessPolicy(apiErrorMessage)
    | `CognitoInvalidSmsRoleTrustRelationship(apiErrorMessage)
    | `CognitoLimitExceeded(apiErrorMessage)
    | `CognitoUserNotFound(apiErrorMessage)
    | `ReasonCognitoUnknownError
  ];

  let make = ({__type, message}: CognitoJson_bs.error) =>
    switch (__type) {
    | "CodeDeliveryFailureException" => `CognitoCodeDeliveryFailure(message)
    | "InvalidEmailRoleAccessPolicyException" =>
      `CognitoInvalidEmailRoleAccessPolicy(message)
    | "InvalidSmsRoleAccessPolicyException" =>
      `CognitoInvalidSmsRoleAccessPolicy(message)
    | "InvalidSmsRoleTrustRelationshipException" =>
      `CognitoInvalidSmsRoleTrustRelationship(message)
    | "LimitExceededException" => `CognitoLimitExceeded(message)
    | "UserNotFoundException" => `CognitoUserNotFound(message)
    | _ => `ReasonCognitoUnknownError
    };

  let makeFromJson = json => {
    let err = CognitoJson_bs.read_error(json);
    switch (Common.make(err)) {
    | Some(commonError) => commonError
    | None => make(err)
    };
  };
};

module RespondToAuthChallenge = {
  type t = [
    | `CognitoAliasExists(apiErrorMessage)
    | `CognitoCodeMismatch(apiErrorMessage)
    | `CognitoExpiredCode(apiErrorMessage)
    | `CognitoInvalidPassword(apiErrorMessage)
    | `CognitoInvalidSmsRoleAccessPolicy(apiErrorMessage)
    | `CognitoInvalidSmsRoleTrustRelationship(apiErrorMessage)
    | `CognitoInvalidUserPoolConfiguration(apiErrorMessage)
    | `CognitoMFAMethodNotFound(apiErrorMessage)
    | `CognitoPasswordResetRequired(apiErrorMessage)
    | `CognitoSoftwareTokenMFANotFound(apiErrorMessage)
    | `CognitoUserNotConfirmed(apiErrorMessage)
    | `CognitoUserNotFound(apiErrorMessage)
    | `ReasonCognitoUnknownError
  ];

  let make = ({__type, message}: CognitoJson_bs.error) =>
    switch (__type) {
    | "AliasExistsException" => `CognitoAliasExists(message)
    | "CodeMismatchException" => `CognitoCodeMismatch(message)
    | "ExpiredCodeException" => `CognitoExpiredCode(message)
    | "InvalidPasswordException" => `CognitoInvalidPassword(message)
    | "InvalidSmsRoleAccessPolicyException" =>
      `CognitoInvalidSmsRoleAccessPolicy(message)
    | "InvalidSmsRoleTrustRelationshipException" =>
      `CognitoInvalidSmsRoleTrustRelationship(message)
    | "InvalidUserPoolConfigurationException" =>
      `CognitoInvalidUserPoolConfiguration(message)
    | "MFAMethodNotFoundException" => `CognitoMFAMethodNotFound(message)
    | "PasswordResetRequiredException" =>
      `CognitoPasswordResetRequired(message)
    | "SoftwareTokenMFANotFoundException" =>
      `CognitoSoftwareTokenMFANotFound(message)
    | "UserNotConfirmedException" => `CognitoUserNotConfirmed(message)
    | "UserNotFoundException" => `CognitoUserNotFound(message)
    | _ => `ReasonCognitoUnknownError
    };

  let makeFromJson = json => {
    let err = CognitoJson_bs.read_error(json);
    switch (Common.make(err)) {
    | Some(commonError) => commonError
    | None => make(err)
    };
  };
};
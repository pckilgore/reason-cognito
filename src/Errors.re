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
    | "InvalidParameterException" => Some(`CognitoInvalidParameter(message))
    | "InvalidLambdaResponseException" =>
      Some(`CognitoInvalidLambdaResponse(message))
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
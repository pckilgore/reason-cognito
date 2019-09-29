// all operations found in `/amazon-cognito-identity-js/src/CognitoUser.js` except `SignUp` which is in `CognitoUserPool.js`;
[@bs.deriving jsConverter]
type operation = [
  | [@bs.as "SignUp"] `SignUp
  | [@bs.as "SignIn"] `SignIn
  | [@bs.as "ConfirmSignUp"] `ConfirmSignUp
  | [@bs.as "ChangePassword"] `ChangePassword
  | [@bs.as "RespondToAuthChallenge"] `RespondToAuthChallenge
  | [@bs.as "ForgotPassword"] `ForgotPassword
  | [@bs.as "ConfirmForgotPassword"] `ConfirmForgotPassword
  | [@bs.as "InitiateAuth"] `InitiateAuth
];
/* https://docs.aws.amazon.com/cognito-user-identity-pools/latest/APIReference/API_InitiateAuth.html#API_InitiateAuth_RequestSyntax */
[@bs.deriving jsConverter]
type authenticationFlowType = [
  | [@bs.as "USER_SRP_AUTH"] `USER_SRP_AUTH
  | [@bs.as "CUSTOM_AUTH"] `CUSTOM_AUTH
  | [@bs.as "REFRESH_TOKEN_AUTH"] `REFRESH_TOKEN_AUTH
  | [@bs.as "USER_PASSWORD_AUTH"] `USER_PASSWORD_AUTH
  | [@bs.as "REFRESH_TOKEN"] `REFRESH_TOKEN
  | [@bs.as "ADMIN_NO_SRP_AUTH"] `ADMIN_NO_SRP_AUTH
  | [@bs.as "USER_PASSWORD_AUTH"] `USER_PASSWORD_AUTH
];
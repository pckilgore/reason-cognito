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
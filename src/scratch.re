[@bs.deriving jsConverter]
type response_type = [ | [@bs.as "code"] `Code | [@bs.as "token"] `Token];

type config = {
  response_type,
  client_id: string,
};
let emailAttribute = Js.Dict.empty();
Js.Dict.set(emailAttribute, "Name", "email");
Js.Dict.set(emailAttribute, "Value", "a.armand.inbox@gmail.com");

// Cognito.signUp(
//   ~username="i.dkjs",
//   ~password="123Password#",
//   ~attributes=[|emailAttribute|],
//   (),
// )
// ->Future.mapError(err => Js.log(err))
// ->Future.mapOk(response =>
//     switch (response) {
//     | Ok(resp) => Js.log2("hey it worked", resp)
//     | InvalidParameterException(msg)
//     | UsernameExistsException(msg) => Js.log2("Errmsg: ", msg)
//     | ReasonCognitoUnknownErrorException(msg) => Js.log(msg)
//     }
//   );
// Cognito.confirmSignUp(~username="i.dkjs",~confirmationCode="287973", ())
// ->Future.mapError(err => Js.log(err))
// ->Future.mapOk(response =>
//     switch (response) {
//     | Ok(resp) => Js.log2("hey it worked", resp)
//     | InvalidParameterException(msg)
//     | UsernameExistsException(msg) => Js.log2("Errmsg: ", msg)
//     | ReasonCognitoUnknownErrorException(msg) => Js.log(msg)
//     }
//   );

// Cognito.changePassword(
//   ~oldUserPassword="123Password#",
//   ~newUserPassword="new123Password#",
//   (),
// )
// ->Future.mapError(err => Js.log(err))
// ->Future.mapOk(response =>
//     switch (response) {
//     | Ok(resp) => Js.log2("hey it worked", resp)
//     | InvalidParameterException(msg)
//     | UsernameExistsException(msg) => Js.log2("Errmsg: ", msg)
//     | ReasonCognitoUnknownErrorException(msg) => Js.log(msg)
//     }
//   );
let ia = () =>
  Cognito.signIn(~username="idkjs", ~password="123Password#", ())
  ->Future.mapError(err => Js.log(err))
  ->Future.mapOk(response =>
      switch (response) {
      | Ok(resp) => Js.log2("hey it worked", resp)
      | InvalidParameterException(msg)
      | UsernameExistsException(msg) => Js.log2("Errmsg: ", msg)
      | ReasonCognitoUnknownErrorException(msg) => Js.log(msg)
      }
    );
// Cognito.confirmSignUp(~username="i.dkjs", ~confirmationCode="287973", ())
// ->Future.mapError(err => Js.log(err))
// ->Future.mapOk(response =>
//     switch (response) {
//     | Ok(resp) => Js.log2("hey it worked", resp)
//     | InvalidParameterException(msg)
//     | UsernameExistsException(msg) => Js.log2("Errmsg: ", msg)
//     | ReasonCognitoUnknownErrorException(msg) => Js.log(msg)
//     }
//   );
let authParameters = Js.Dict.empty();
Js.Dict.set(authParameters, "USERNAME", "idkjs");

let signIn = () =>
  Cognito.signIn(~username="idkjs",~password="123Password#", ())
  ->Future.mapError(err => Js.log(err))
  ->Future.mapOk(response =>
      switch (response) {
      | Ok(resp) => Js.log2("hey it worked", resp)
      | InvalidParameterException(msg)
      | UsernameExistsException(msg) => Js.log2("Errmsg: ", msg)
      | ReasonCognitoUnknownErrorException(msg) => Js.log(msg)
      }
    );

signIn();
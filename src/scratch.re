[@bs.deriving jsConverter]
type response_type = [ | [@bs.as "code"] `Code | [@bs.as "token"] `Token];

type config = {
  response_type,
  client_id: string,
};
let emailAttribute = Js.Dict.empty();
Js.Dict.set(emailAttribute, "Name", "email");
Js.Dict.set(emailAttribute, "Value", "reasoncog@pck.email");

Cognito.signUp(
  ~username="patrickasdf",
  ~password="123Password#",
  ~attributes=[|emailAttribute|],
  (),
)
->Future.mapError(err => Js.log(err))
->Future.mapOk(response =>
    switch (response) {
    | Ok(resp) => Js.log2("hey it worked", resp)
    | InvalidParameterException(msg)
    | UsernameExistsException(msg) => Js.log2("Errmsg: ", msg)
    | ReasonCognitoUnknownErrorException(msg) => Js.log(msg)
    }
  );
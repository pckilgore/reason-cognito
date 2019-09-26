[@bs.deriving jsConverter]
type response_type = [ | [@bs.as "code"] `Code | [@bs.as "token"] `Token];

type config = {
  response_type,
  client_id: string,
};

let params = Js.Dict.empty();
Js.Dict.set(
  params,
  "ClientId",
  Js.Json.string("3vjshpa4lgf92nfisjrg9os21a"),
);
Js.Dict.set(params, "Username", Js.Json.string("patrick"));
Js.Dict.set(params, "Password", Js.Json.string("Password123!"));
Js.Dict.set(params, "Password", Js.Json.string("Password123!"));
Js.Dict.set(params, "UserAttributes", Js.Json.array([||]));
Js.Dict.set(params, "ValidationData", Js.Json.array([||]));

Client.request("SignUp", params)
->Future.mapError(err => Js.log(err))
->Future.mapOk(response =>
    switch (response) {
    | Ok => Js.log("hey it worked")
    | InvalidParameterException(msg) => Js.log2("YAY", msg)
    | _ => Js.log("TODO")
    }
  );
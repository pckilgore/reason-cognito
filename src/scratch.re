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
->Future.mapError(err =>
    switch (err) {
    | NetworkError(_) => Js.log("Network err")
    | InvalidParameterException(msg) => Js.log2("Invalid param: ", msg)
    | _ => Js.log("Unhandled Err")
    }
  )
->Future.mapOk(_ => Js.log("OKAY!"));
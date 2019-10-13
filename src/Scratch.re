let auth =
  Cognito.makeConfig(
    ~clientId="3vjshpa4lgf92nfisjrg9os21a",
    ~region=UsEast2,
    ~poolId="us-east-2_pTFKTGtS3",
    (),
  );

let emailAttribute = Js.Dict.empty();
Js.Dict.set(emailAttribute, "Name", "email");
Js.Dict.set(emailAttribute, "Value", "reasoncog@pck.email");

let testRequest =
  auth
  ->Cognito.signUp(
      ~username="sarah6",
      ~password="Password12345@",
      ~attributes=[|emailAttribute|],
      (),
    )
  ->Future.tapOk(value => Js.log2("OK", value))
  ->Future.tapError(value => Js.log2("err", value));
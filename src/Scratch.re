let auth =
  Cognito.makeConfig(~clientId="1234", ~region=UsEast1, ~poolId="1234", ());

let testRequest =
  auth->Cognito.signUp(
    ~username="sometestuser",
    ~password="Password12345@",
    (),
  );
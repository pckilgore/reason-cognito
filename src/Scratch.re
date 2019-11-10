let auth =
  Cognito.Config.make(
    ~clientId="3vjshpa4lgf92nfisjrg9os21a",
    ~region=UsEast2,
    ~poolId="us-east-2_pTFKTGtS3",
    (),
  );

let emailAttribute = Js.Dict.empty();
Js.Dict.set(emailAttribute, "Name", "email");
Js.Dict.set(emailAttribute, "Value", "reasoncog@pck.email");

// let testRequest =
//   auth
//   ->Cognito.signUp(
//       ~username="sarah7",
//       ~password="Password12345@",
//       ~attributes=[|emailAttribute|],
//       (),
//     )
//   ->Future.tapOk(value => Js.log2("OK", value))
//   ->Future.tapError(value => Js.log2("err", value));

// let emailAttribute = Js.Dict.empty();
// Js.Dict.set(emailAttribute, "Name", "email");
// Js.Dict.set(emailAttribute, "Value", "aa.rmand.in.box@gmail.com");

// auth
// ->Cognito.confirmSignUp(~username="sarah7", ~confirmationCode="217900", ())
// ->Future.tapOk(value => Js.log2("OK", value))
// ->Future.tapError(value => Js.log2("err", value)) /* let signOut */;

let authParameters = Js.Dict.empty();
Js.Dict.set(authParameters, "USERNAME", "sarah7");
Js.Dict.set(authParameters, "PASSWORD", "Password12345@");

auth
->Cognito.initiateAuth(~authParameters, ~authFlow=USER_PASSWORD_AUTH, ())
->Future.tapOk(value => {
    Js.log2("result", value.authenticationResult);
    switch (value.challengeName) {
    | Some(name) => Js.log(name)
    | None => Js.log("No challenge name")
    };
    Js.log2("params", value.challengeParameters);
  })
->Future.tapError(value => Js.log2("err", value)) /*     ()*/ /*   auth->Cognito.changePassword*/ /*     ~oldPassword="Password12345#"*/ /*     ~accessToken="eyJraWQiOiJoNzdxa1lvSHJMS1Q4OXFwSE1PYkJIQjlRUlJxdDJDZzJsckJpeGlrQUJrPSIsImFsZyI6IlJTMjU2In0.eyJzdWIiOiJjNGVkYzQ1NS05YzA0LTQwYzktODc2NS03NjYzMTdjMDE3MDYiLCJldmVudF9pZCI6ImJjMmNmZDE4LTE0ZDgtNDU1ZS05NWY3LTdjYTFiYjQ0ZDFiYSIsInRva2VuX3VzZSI6ImFjY2VzcyIsInNjb3BlIjoiYXdzLmNvZ25pdG8uc2lnbmluLnVzZXIuYWRtaW4iLCJhdXRoX3RpbWUiOjE1Njk4NzgzODEsImlzcyI6Imh0dHBzOlwvXC9jb2duaXRvLWlkcC51cy1lYXN0LTIuYW1hem9uYXdzLmNvbVwvdXMtZWFzdC0yX3BURktUR3RTMyIsImV4cCI6MTU2OTg4MTk4MSwiaWF0IjoxNTY5ODc4MzgxLCJqdGkiOiIzMjAxMjljZC1iODhlLTRlMWYtODUwYy0zMzc2NzAzYmY2ODEiLCJjbGllbnRfaWQiOiIzdmpzaHBhNGxnZjkybmZpc2pyZzlvczIxYSIsInVzZXJuYW1lIjoic29tZXRlc3R1c2VyMiJ9.OCZ8ggv-qVuVOXlUvXbrWJVYEQFpODBi4_P4cU0OOYe5nKHeaaX6QGpRU-EhRBZ3SgpBmJQZI4KZeXLRz_aTF6xC_uu-nSu7etxESdbC9SpsZrWCeb5A7UeAQXQ6UmWM9HVVEp8-XrwkN6jKxUxcWGW9YnG6NXG7c5cGfYjcpbB1gBT60QrBx8_4eaX7E_ffWlFd41ikRhREWanUfbvHsBanVbwobUPI-VRx6lVNDjF7YUanUwkhLtl8KY8Gl3FydeHZFNbrmDRAHUpgZJpG2vft1hHraBPBiVxs_uuTgwzg7HMlTWbjhqZ-dII0nm0bfE_xh_bP7UGn3EH_AMsZAg"*/ /*   )*/ /* let signIn */ /*     ~username="sometestuser2"*/ /*     ~password="Password12345#"*/ /*   auth->Cognito.initiateAuth*/ /* works with new passwor*/ /*     ()*/ /*     ~newPassword="Password12345@"*/ /*     ~username="sometestuser2"*/ /* let changePassword */;

/*   )*/

/*   auth->Cognito.signOut()*/
/* get `accessToken` from `signUp` response object, pass it to `changePassword` */
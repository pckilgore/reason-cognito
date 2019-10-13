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

let emailAttribute = Js.Dict.empty();
Js.Dict.set(emailAttribute, "Name", "email");
Js.Dict.set(emailAttribute, "Value", "aa.rmand.in.box@gmail.com");

let confirmSignUp =
  auth->Cognito.confirmSignUp(
    ~username="sometestuser3",
    ~confirmationCode="301529",
    (),
  );
/* get `accessToken` from `signUp` response object, pass it to `changePassword` */
// let changePassword =
//   auth->Cognito.changePassword(
//     ~username="sometestuser2",
//     ~oldPassword="Password12345#",
//     ~newPassword="Password12345@",
//     ~accessToken="eyJraWQiOiJoNzdxa1lvSHJMS1Q4OXFwSE1PYkJIQjlRUlJxdDJDZzJsckJpeGlrQUJrPSIsImFsZyI6IlJTMjU2In0.eyJzdWIiOiJjNGVkYzQ1NS05YzA0LTQwYzktODc2NS03NjYzMTdjMDE3MDYiLCJldmVudF9pZCI6ImJjMmNmZDE4LTE0ZDgtNDU1ZS05NWY3LTdjYTFiYjQ0ZDFiYSIsInRva2VuX3VzZSI6ImFjY2VzcyIsInNjb3BlIjoiYXdzLmNvZ25pdG8uc2lnbmluLnVzZXIuYWRtaW4iLCJhdXRoX3RpbWUiOjE1Njk4NzgzODEsImlzcyI6Imh0dHBzOlwvXC9jb2duaXRvLWlkcC51cy1lYXN0LTIuYW1hem9uYXdzLmNvbVwvdXMtZWFzdC0yX3BURktUR3RTMyIsImV4cCI6MTU2OTg4MTk4MSwiaWF0IjoxNTY5ODc4MzgxLCJqdGkiOiIzMjAxMjljZC1iODhlLTRlMWYtODUwYy0zMzc2NzAzYmY2ODEiLCJjbGllbnRfaWQiOiIzdmpzaHBhNGxnZjkybmZpc2pyZzlvczIxYSIsInVzZXJuYW1lIjoic29tZXRlc3R1c2VyMiJ9.OCZ8ggv-qVuVOXlUvXbrWJVYEQFpODBi4_P4cU0OOYe5nKHeaaX6QGpRU-EhRBZ3SgpBmJQZI4KZeXLRz_aTF6xC_uu-nSu7etxESdbC9SpsZrWCeb5A7UeAQXQ6UmWM9HVVEp8-XrwkN6jKxUxcWGW9YnG6NXG7c5cGfYjcpbB1gBT60QrBx8_4eaX7E_ffWlFd41ikRhREWanUfbvHsBanVbwobUPI-VRx6lVNDjF7YUanUwkhLtl8KY8Gl3FydeHZFNbrmDRAHUpgZJpG2vft1hHraBPBiVxs_uuTgwzg7HMlTWbjhqZ-dII0nm0bfE_xh_bP7UGn3EH_AMsZAg",
//     (),
//   );

// works with new password
// let signIn =
//   auth->Cognito.initiateAuth(
//     ~username="sometestuser2",
//     ~password="Password12345#",
//     (),
//   );
// let signOut =
//   auth->Cognito.signOut();
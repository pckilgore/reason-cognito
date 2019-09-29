
[@bs.deriving abstract]
type authenticationResultDecoder = {
  [@bs.as "AccessToken"]
  accessToken: string,
  [@bs.as "ExpiresIn"]
  expiresIn: int,
  [@bs.as "IdToken"]
  idToken: string,
  [@bs.as "RefreshToken"]
  refreshToken: string,
  [@bs.as "TokenType"]
  tokenType: string,
};

[@bs.deriving abstract]
type signInResponseDecoder = {
  [@bs.as "AuthenticationResult"]
  authenticationResultDecoder,
  [@bs.as "ChallengeParameters"]
  challengeParameters: Js.Json.t,
};
type signInResponse = {
  authenticationResult,
  challengeParameters: Js.Json.t,
}
and authenticationResult = {
  accessToken: string,
  expiresIn: int,
  idToken: string,
  refreshToken: string,
  tokenType: string,
};

external makeSignInResponse: 't => signInResponseDecoder = "%identity";
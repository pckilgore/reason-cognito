// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var SRP = require("./srp/SRP.bs.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Fetch = require("bs-fetch/src/Fetch.js");
var Utils = require("./Utils.bs.js");
var Errors = require("./Errors.bs.js");
var Future = require("reason-future/src/Future.bs.js");
var Js_dict = require("bs-platform/lib/js/js_dict.js");
var FutureJs = require("reason-future/src/FutureJs.bs.js");
var Caml_array = require("bs-platform/lib/js/caml_array.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var CognitoJson_bs = require("./atd/CognitoJson_bs.bs.js");

((require('isomorphic-fetch')));

function toString(param) {
  switch (param) {
    case /* USER_PASSWORD_AUTH */0 :
        return "USER_PASSWORD_AUTH";
    case /* USER_SRP_AUTH */1 :
        return "USER_SRP_AUTH";
    case /* CUSTOM_AUTH */2 :
        return "CUSTOM_AUTH";
    
  }
}

var toJson = toString;

var AuthenticationFlowType = {
  toString: toString,
  toJson: toJson
};

function toString$1(param) {
  switch (param) {
    case /* UsEast1 */0 :
        return "us-east-1";
    case /* UsEast2 */1 :
        return "us-east-2";
    case /* UsWest2 */2 :
        return "us-west-2";
    case /* ApSouth1 */3 :
        return "ap-south-1";
    case /* ApNortheast1 */4 :
    case /* ApNortheast2 */5 :
        return "ap-northeast-2";
    case /* ApSoutheast1 */6 :
        return "ap-southeast-1";
    case /* ApSoutheast2 */7 :
        return "ap-southeast-2";
    case /* CaCentral1 */8 :
        return "ca-central-1";
    case /* EuCentral1 */9 :
        return "eu-central-1";
    case /* EuWest1 */10 :
        return "eu-west-1";
    case /* EuWest2 */11 :
        return "eu-west-2";
    
  }
}

var Region = {
  toString: toString$1
};

function getPoolId(t) {
  return Caml_array.caml_array_get(t[/* pool */0].split("_"), 1);
}

function make(pool, clientId, region, $staropt$star, param) {
  var authenticationFlowType = $staropt$star !== undefined ? $staropt$star : /* USER_SRP_AUTH */1;
  return /* record */[
          /* pool */pool,
          /* clientId */clientId,
          /* endpoint */"https://cognito-idp." + (toString$1(region) + ".amazonaws.com/"),
          /* authenticationFlowType */authenticationFlowType
        ];
}

var Config = {
  AuthenticationFlowType: AuthenticationFlowType,
  Region: Region,
  getPoolId: getPoolId,
  make: make
};

function toString$2(param) {
  switch (param) {
    case /* ChangePassword */0 :
        return "ChangePassword";
    case /* ConfirmForgotPassword */1 :
        return "ConfirmForgotPassword";
    case /* ConfirmSignUp */2 :
        return "ConfirmSignUp";
    case /* ForgotPassword */3 :
        return "ForgotPassword";
    case /* InitiateAuth */4 :
        return "InitiateAuth";
    case /* ResendConfirmationCode */5 :
        return "ResendConfirmationCode";
    case /* RespondToAuthChallenge */6 :
        return "RespondToAuthChallenge";
    case /* SignUp */7 :
        return "SignUp";
    
  }
}

var Operation = {
  toString: toString$2
};

function request(config, operation, params) {
  var headers = { };
  var target = "AWSCognitoIdentityProviderService." + toString$2(operation);
  headers["X-Amz-Target"] = target;
  headers["Content-Type"] = "application/x-amz-json-1.1";
  headers["X-Amz-User-Agent"] = "reason-cognito/0.1.x js";
  params["ClientId"] = config[/* clientId */1];
  var settings = Fetch.RequestInit.make(/* Post */2, Caml_option.some(headers), Caml_option.some(JSON.stringify(params)), undefined, undefined, undefined, undefined, /* NoCache */3, undefined, undefined, undefined)(/* () */0);
  console.log("Fetching...");
  console.log(settings);
  return Future.flatMapOk(FutureJs.fromPromise(fetch(config[/* endpoint */2], settings), (function (fetchError) {
                    return /* `ReasonCognitoClientError */[
                            -291143216,
                            fetchError
                          ];
                  })), (function (apiResponse) {
                return Future.mapOk(FutureJs.fromPromise(apiResponse.json(), (function (err) {
                                  return /* `ReasonCognitoClientError */[
                                          -291143216,
                                          err
                                        ];
                                })), (function (json) {
                              var code = apiResponse.status;
                              var status = code < 200 ? /* Informational */Block.__(0, [code]) : (
                                  code >= 200 && code < 300 ? /* Success */Block.__(1, [code]) : (
                                      code >= 300 && code < 400 ? /* Redirect */Block.__(2, [code]) : (
                                          code >= 400 && code < 500 ? /* ClientError */Block.__(3, [code]) : /* ServerError */Block.__(4, [code])
                                        )
                                    )
                                );
                              return /* record */[
                                      /* status */status,
                                      /* json */json
                                    ];
                            }));
              }));
}

var Client = {
  Operation: Operation,
  request: request
};

function signUp(config, username, password, $staropt$star, $staropt$star$1, analyticsEndpointId, clientMetadata, secretHash, param) {
  var attributes = $staropt$star !== undefined ? $staropt$star : /* array */[];
  var validationData = $staropt$star$1 !== undefined ? $staropt$star$1 : /* array */[];
  var params = { };
  if (analyticsEndpointId !== undefined) {
    params["AnalyticsMetadata"] = Caml_option.valFromOption(analyticsEndpointId);
  }
  if (clientMetadata !== undefined) {
    params["ClientMetadata"] = Caml_option.valFromOption(clientMetadata);
  }
  params["Password"] = password;
  if (secretHash !== undefined) {
    params["SecretHash"] = secretHash;
  }
  params["UserAttributes"] = Utils.jsonMapString(attributes);
  params["Username"] = username;
  params["ValidationData"] = Utils.jsonMapString(validationData);
  return Future.flatMapOk(request(config, /* SignUp */7, params), (function (res) {
                var match = res[/* status */0];
                var tmp;
                tmp = match.tag === /* Success */1 ? /* Ok */Block.__(0, [Curry._1(CognitoJson_bs.read_signUpResponse, res[/* json */1])]) : /* Error */Block.__(1, [Errors.SignUpErrors.makeFromJson(res[/* json */1])]);
                return Future.value(tmp);
              }));
}

function confirmSignUp(config, username, confirmationCode, $staropt$star, secretHash, clientMetadata, analyticsEndpointId, param) {
  var forceAliasCreation = $staropt$star !== undefined ? $staropt$star : false;
  var params = { };
  if (analyticsEndpointId !== undefined) {
    params["AnalyticsMetadata"] = Caml_option.valFromOption(analyticsEndpointId);
  }
  if (clientMetadata !== undefined) {
    params["ClientMetadata"] = Caml_option.valFromOption(clientMetadata);
  }
  params["ConfirmationCode"] = confirmationCode;
  params["ForceAliasCreation"] = forceAliasCreation;
  if (secretHash !== undefined) {
    params["SecretHash"] = secretHash;
  }
  params["Username"] = username;
  return Future.flatMapOk(request(config, /* ConfirmSignUp */2, params), (function (param) {
                var tmp;
                tmp = param[/* status */0].tag === /* Success */1 ? /* Ok */Block.__(0, [/* () */0]) : /* Error */Block.__(1, [Errors.ConfirmSignUp.makeFromJson(param[/* json */1])]);
                return Future.value(tmp);
              }));
}

function initiateAuth(config, authParameters, authFlow, clientMetadata, analyticsEndpointId, param) {
  var params = { };
  if (analyticsEndpointId !== undefined) {
    params["AnalyticsMetadata"] = Caml_option.valFromOption(analyticsEndpointId);
  }
  params["AuthFlow"] = toString(authFlow);
  params["AuthParameters"] = Utils.makeJsonStringsFromDictValues(authParameters);
  if (clientMetadata !== undefined) {
    params["ClientMetadata"] = Caml_option.valFromOption(clientMetadata);
  }
  return Utils.andIfOk(request(config, /* InitiateAuth */4, params), (function (param) {
                var json = param[/* json */1];
                if (param[/* status */0].tag === /* Success */1) {
                  return /* Ok */Block.__(0, [Curry._1(CognitoJson_bs.read_authenticationResponse, json)]);
                } else {
                  return /* Error */Block.__(1, [Errors.InitiateAuth.makeFromJson(json)]);
                }
              }));
}

function respondToAuthChallenge(config, challengeName, challengeResponses, session, analyticsEndpointId, clientMetadata, param) {
  var params = { };
  if (analyticsEndpointId !== undefined) {
    params["AnalyticsMetadata"] = Caml_option.valFromOption(analyticsEndpointId);
  }
  params["ChallengeName"] = Curry._1(CognitoJson_bs.write_challengeName, challengeName);
  if (challengeResponses !== undefined) {
    var match = challengeResponses;
    var data = { };
    data["USERNAME"] = match[/* userIdForSrp */1];
    data["PASSWORD_CLAIM_SECRET_BLOCK"] = match[/* secretBlock */0];
    data["TIMESTAMP"] = match[/* timestamp */3];
    data["PASSWORD_CLAIM_SIGNATURE"] = match[/* signature */2];
    params["ChallengeResponses"] = data;
  }
  if (clientMetadata !== undefined) {
    params["ClientMetadata"] = Caml_option.valFromOption(clientMetadata);
  }
  if (session !== undefined) {
    params["Session"] = session;
  }
  return Future.flatMapOk(request(config, /* RespondToAuthChallenge */6, params), (function (param) {
                var json = param[/* json */1];
                var tmp;
                tmp = param[/* status */0].tag === /* Success */1 ? /* Ok */Block.__(0, [Curry._1(CognitoJson_bs.read_authenticationResponse, json)]) : /* Error */Block.__(1, [Errors.RespondToAuthChallenge.makeFromJson(json)]);
                return Future.value(tmp);
              }));
}

function initiateSRPAuth(config, authParameters, $staropt$star, clientMetadata, analyticsEndpointId, param) {
  var authFlow = $staropt$star !== undefined ? $staropt$star : /* USER_SRP_AUTH */1;
  var srpConfig = SRP.make(SRP.KnownSafePrimes.bit3072);
  var match = Js_dict.get(authParameters, "PASSWORD");
  var srpPassword = match !== undefined ? (Js_dict.unsafeDeleteKey(authParameters, "PASSWORD"), match) : "";
  var params = { };
  if (analyticsEndpointId !== undefined) {
    params["AnalyticsMetadata"] = Caml_option.valFromOption(analyticsEndpointId);
  }
  params["AuthFlow"] = toString(authFlow);
  authParameters["SRP_A"] = SRP.hexStrFromBigInt(srpConfig[/* bigA */1]);
  params["AuthParameters"] = Utils.makeJsonStringsFromDictValues(authParameters);
  if (clientMetadata !== undefined) {
    params["ClientMetadata"] = Caml_option.valFromOption(clientMetadata);
  }
  return Future.flatMapOk(Utils.andIfOk(Utils.andIfOk(request(config, /* InitiateAuth */4, params), (function (param) {
                        var json = param[/* json */1];
                        if (param[/* status */0].tag === /* Success */1) {
                          return /* Ok */Block.__(0, [Curry._1(CognitoJson_bs.read_authSRPResponse, json)]);
                        } else {
                          return /* Error */Block.__(1, [Errors.InitiateAuth.makeFromJson(json)]);
                        }
                      })), (function (param) {
                    var challengeParameters = param[/* challengeParameters */1];
                    if (param[/* challengeName */0] !== -125280006) {
                      return /* Error */Block.__(1, [/* ReasonCognitoUnknownError */536808491]);
                    } else {
                      var pool = getPoolId(config);
                      var params_000 = /* username */challengeParameters[/* userIdForSrp */4];
                      var params_002 = /* bigB */SRP.bigIntFromHexStr(challengeParameters[/* srpB */2]);
                      var params_003 = /* salt */SRP.bigIntFromHexStr(challengeParameters[/* salt */0]);
                      var params = /* record */[
                        params_000,
                        /* password */srpPassword,
                        params_002,
                        params_003,
                        /* pool */pool
                      ];
                      var keyResult = SRP.makeAuthenticationKey(srpConfig, params);
                      if (keyResult.tag) {
                        return /* Error */Block.__(1, [/* ReasonCognitoSRPError */-447216412]);
                      } else {
                        return /* Ok */Block.__(0, [SRP.makeResponseParams(pool, challengeParameters, keyResult[0])]);
                      }
                    }
                  })), (function (challengeResponses) {
                return respondToAuthChallenge(config, /* PASSWORD_VERIFIER */-125280006, challengeResponses, undefined, undefined, undefined, /* () */0);
              }));
}

function changePassword(config, accessToken, previousPassword, proposedPassword, param) {
  var params = { };
  params["AccessToken"] = accessToken;
  params["PreviousPassword"] = previousPassword;
  params["ProposedPassword"] = proposedPassword;
  return Future.flatMapOk(request(config, /* ChangePassword */0, params), (function (param) {
                var tmp;
                tmp = param[/* status */0].tag === /* Success */1 ? /* Ok */Block.__(0, [/* () */0]) : /* Error */Block.__(1, [Errors.ChangePassword.makeFromJson(param[/* json */1])]);
                return Future.value(tmp);
              }));
}

function confirmForgotPassword(config, username, password, confirmationCode, analyticsEndpointId, clientMetadata, param) {
  var params = { };
  if (analyticsEndpointId !== undefined) {
    params["AnalyticsMetadata"] = Caml_option.valFromOption(analyticsEndpointId);
  }
  if (clientMetadata !== undefined) {
    params["ClientMetadata"] = Caml_option.valFromOption(clientMetadata);
  }
  params["ConfirmationCode"] = confirmationCode;
  params["Password"] = password;
  params["Username"] = username;
  return Future.flatMapOk(request(config, /* ConfirmForgotPassword */1, params), (function (param) {
                var tmp;
                tmp = param[/* status */0].tag === /* Success */1 ? /* Ok */Block.__(0, [/* () */0]) : /* Error */Block.__(1, [Errors.ConfirmForgotPassword.makeFromJson(param[/* json */1])]);
                return Future.value(tmp);
              }));
}

function forgotPassword(config, username, analyticsEndpointId, clientMetadata, param) {
  var params = { };
  if (analyticsEndpointId !== undefined) {
    params["AnalyticsMetadata"] = Caml_option.valFromOption(analyticsEndpointId);
  }
  if (clientMetadata !== undefined) {
    params["ClientMetadata"] = Caml_option.valFromOption(clientMetadata);
  }
  params["Username"] = username;
  return Future.flatMapOk(request(config, /* ForgotPassword */3, params), (function (param) {
                var json = param[/* json */1];
                var tmp;
                tmp = param[/* status */0].tag === /* Success */1 ? /* Ok */Block.__(0, [Curry._1(CognitoJson_bs.read_codeDeliveryResponse, json)]) : /* Error */Block.__(1, [Errors.ForgotPassword.makeFromJson(json)]);
                return Future.value(tmp);
              }));
}

function resendConfirmationCode(config, username, analyticsEndpointId, clientMetadata, param) {
  var params = { };
  if (analyticsEndpointId !== undefined) {
    params["AnalyticsMetadata"] = Caml_option.valFromOption(analyticsEndpointId);
  }
  if (clientMetadata !== undefined) {
    params["ClientMetadata"] = Caml_option.valFromOption(clientMetadata);
  }
  params["Username"] = username;
  return Future.flatMapOk(request(config, /* ResendConfirmationCode */5, params), (function (param) {
                var json = param[/* json */1];
                var tmp;
                tmp = param[/* status */0].tag === /* Success */1 ? /* Ok */Block.__(0, [Curry._1(CognitoJson_bs.read_codeDeliveryResponse, json)]) : /* Error */Block.__(1, [Errors.ResendConfirmationCode.makeFromJson(json)]);
                return Future.value(tmp);
              }));
}

exports.Config = Config;
exports.Client = Client;
exports.signUp = signUp;
exports.confirmSignUp = confirmSignUp;
exports.initiateAuth = initiateAuth;
exports.respondToAuthChallenge = respondToAuthChallenge;
exports.initiateSRPAuth = initiateSRPAuth;
exports.changePassword = changePassword;
exports.confirmForgotPassword = confirmForgotPassword;
exports.forgotPassword = forgotPassword;
exports.resendConfirmationCode = resendConfirmationCode;
/*  Not a pure module */

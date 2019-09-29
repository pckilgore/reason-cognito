// Generated by BUCKLESCRIPT VERSION 5.0.6, PLEASE EDIT WITH CARE
'use strict';

var $$Array = require("bs-platform/lib/js/array.js");
var Block = require("bs-platform/lib/js/block.js");
var Client = require("./Client.bs.js");
var Future = require("reason-future/src/Future.bs.js");
var Js_dict = require("bs-platform/lib/js/js_dict.js");

function signUp(username, password, $staropt$star, $staropt$star$1, param) {
  var attributes = $staropt$star !== undefined ? $staropt$star : /* array */[];
  var validationData = $staropt$star$1 !== undefined ? $staropt$star$1 : /* array */[];
  var params = { };
  var jsonAttribs = $$Array.map((function (attrib) {
          return Js_dict.map((function (av) {
                        return av;
                      }), attrib);
        }), attributes);
  params["Username"] = username;
  params["Password"] = password;
  params["UserAttributes"] = jsonAttribs;
  params["ValidationData"] = validationData;
  return Future.mapOk(Client.request(/* SignUp */-384133096, params), (function (resp) {
                var err = resp.__type;
                var msg = resp.message;
                console.log("Raw Response", resp);
                switch (err) {
                  case "InvalidParameterException" : 
                      return /* InvalidParameterException */Block.__(1, [msg]);
                  case "UsernameExistsException" : 
                      return /* UsernameExistsException */Block.__(2, [msg]);
                  default:
                    var cddDecoder = resp.CodeDeliveryDetails;
                    var codeDeliveryDetails_000 = /* attributeName */cddDecoder.AttributeName;
                    var codeDeliveryDetails_001 = /* deliveryMedium */cddDecoder.DeliveryMedium;
                    var codeDeliveryDetails_002 = /* destination */cddDecoder.Destination;
                    var codeDeliveryDetails = /* record */[
                      codeDeliveryDetails_000,
                      codeDeliveryDetails_001,
                      codeDeliveryDetails_002
                    ];
                    return /* Ok */Block.__(0, [/* record */[
                                /* codeDeliveryDetails */codeDeliveryDetails,
                                /* userConfirmed */resp.UserConfirmed,
                                /* userSub */resp.UserSub
                              ]]);
                }
              }));
}

function confirmSignUp(username, confirmationCode, param) {
  var params = { };
  params["Username"] = username;
  params["ConfirmationCode"] = confirmationCode;
  return Future.mapOk(Client.request(/* ConfirmSignUp */-606583560, params), (function (resp) {
                var err = resp.__type;
                var msg = resp.message;
                console.log("Raw Response", resp);
                switch (err) {
                  case "InvalidParameterException" : 
                      return /* InvalidParameterException */Block.__(1, [msg]);
                  case "UsernameExistsException" : 
                      return /* UsernameExistsException */Block.__(2, [msg]);
                  default:
                    var cddDecoder = resp.CodeDeliveryDetails;
                    var codeDeliveryDetails_000 = /* attributeName */cddDecoder.AttributeName;
                    var codeDeliveryDetails_001 = /* deliveryMedium */cddDecoder.DeliveryMedium;
                    var codeDeliveryDetails_002 = /* destination */cddDecoder.Destination;
                    var codeDeliveryDetails = /* record */[
                      codeDeliveryDetails_000,
                      codeDeliveryDetails_001,
                      codeDeliveryDetails_002
                    ];
                    return /* Ok */Block.__(0, [/* record */[
                                /* codeDeliveryDetails */codeDeliveryDetails,
                                /* userConfirmed */resp.UserConfirmed,
                                /* userSub */resp.UserSub
                              ]]);
                }
              }));
}

function changePassword(oldUserPassword, newUserPassword, param) {
  var params = { };
  params["PreviousPassword"] = oldUserPassword;
  params["ProposedPassword"] = newUserPassword;
  return Future.mapOk(Client.request(/* ChangePassword */-866061109, params), (function (resp) {
                var err = resp.__type;
                var msg = resp.message;
                console.log("Raw Response", resp);
                switch (err) {
                  case "InvalidParameterException" : 
                      return /* InvalidParameterException */Block.__(1, [msg]);
                  case "UsernameExistsException" : 
                      return /* UsernameExistsException */Block.__(2, [msg]);
                  default:
                    var cddDecoder = resp.CodeDeliveryDetails;
                    var codeDeliveryDetails_000 = /* attributeName */cddDecoder.AttributeName;
                    var codeDeliveryDetails_001 = /* deliveryMedium */cddDecoder.DeliveryMedium;
                    var codeDeliveryDetails_002 = /* destination */cddDecoder.Destination;
                    var codeDeliveryDetails = /* record */[
                      codeDeliveryDetails_000,
                      codeDeliveryDetails_001,
                      codeDeliveryDetails_002
                    ];
                    return /* Ok */Block.__(0, [/* record */[
                                /* codeDeliveryDetails */codeDeliveryDetails,
                                /* userConfirmed */resp.UserConfirmed,
                                /* userSub */resp.UserSub
                              ]]);
                }
              }));
}

function signIn(username, password, param) {
  var authParams = { };
  authParams["USERNAME"] = username;
  authParams["PASSWORD"] = password;
  var params = { };
  params["AuthParameters"] = authParams;
  return Future.mapOk(Client.request(/* InitiateAuth */-23567743, params), (function (resp) {
                var err = resp.__type;
                var msg = resp.message;
                console.log("Raw Response", resp);
                if (err === "InvalidParameterException") {
                  return /* InvalidParameterException */Block.__(1, [msg]);
                } else {
                  var authDecoder = resp.AuthenticationResult;
                  var authenticationResult_000 = /* accessToken */authDecoder.AccessToken;
                  var authenticationResult_001 = /* expiresIn */authDecoder.ExpiresIn;
                  var authenticationResult_002 = /* idToken */authDecoder.IdToken;
                  var authenticationResult_003 = /* refreshToken */authDecoder.RefreshToken;
                  var authenticationResult_004 = /* tokenType */authDecoder.TokenType;
                  var authenticationResult = /* record */[
                    authenticationResult_000,
                    authenticationResult_001,
                    authenticationResult_002,
                    authenticationResult_003,
                    authenticationResult_004
                  ];
                  return /* Ok */Block.__(0, [/* record */[
                              /* authenticationResult */authenticationResult,
                              /* challengeParameters */resp.ChallengeParameters
                            ]]);
                }
              }));
}

exports.signUp = signUp;
exports.confirmSignUp = confirmSignUp;
exports.changePassword = changePassword;
exports.signIn = signIn;
/* Client Not a pure module */

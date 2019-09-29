# reason-cognito

# Build

```
npm run build
```

# Watch

```
npm run watch
```

# Editor

If you use `vscode`, Press `Windows + Shift + B` it will build automatically# reason-cognito

## Initiating Auth / SignIn

### Headers

Operation is `InitiateAuth`.

```json
Host: cognito-idp.eu-west-1.amazonaws.com
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:70.0) Gecko/20100101 Firefox/70.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: http://localhost:3000/
Content-Type: application/x-amz-json-1.1
X-Amz-Target: AWSCognitoIdentityProviderService.InitiateAuth
X-Amz-User-Agent: aws-amplify/0.1.x js
Origin: http://localhost:3000
Content-Length: 905
DNT: 1
Connection: keep-alive
Pragma: no-cache
Cache-Control: no-cache
```

### Params

```json
{"AuthFlow":"USER_SRP_AUTH","ClientId":"73fn5q8ne8ph5vo99lm5hfct43","AuthParameters":{"USERNAME":"idkjs","SRP_A":"db25daec36205282d3f01f75562e9738b8be6654f1b789be51250d147f6412803c84852865fc9558105ee7fdb29a92f2fa743c3272f6c48e855a5acbafc09d1409cfc19b11ee7a110bcc5dce8"},"ClientMetadata":{}}
```

### Response Json

```json
{"JSON":{"ChallengeName":"PASSWORD_VERIFIER","ChallengeParameters":{"SALT":"3fb3cb028b0e4aaeabce3b573699a322","SECRET_BLOCK":"pJV3JYJzq4Srz8qE5q+JYj4HoK3NdlDa1OCRRvK07s1lcvgN4R/QbsC4oWebPA8hNYqMSt8fJpd0JlAFkzGapOMV6j73mtl+/OO0JX7rFjmsDFR2L56sAI=","SRP_B":"ddc979984abfc3daa781570e31cd30f506c4306e34f10ce49324fc6ed6228e31174b102b41ed5f1f38413a98125e","USERNAME":"idkjs","USER_ID_FOR_SRP":"idkjs"}},"Response payload":{"EDITOR_CONFIG":{"text":"eyJDaGFsbGVuZ2VOYW1lIjoiUEFTU1dPUkRfVkVSSUZJRVIiLCJDaGFsbGVuZ2VQYXJhbWV0ZXJ2ZTM0ZjEwY2U0OTMyNGZjNmVkNjIyOGUzMTE3NGIxMDJiNDFlZDVmMWYzODQxM2E5ODEyNWUiLCJVU0VSTkFNRSI6Imlka2pzIiwiVVNFUl9JRF9GT1JfU1JQIjoiaWRranMifX0=","mode":"application/json"}}}
```

### Response Object

```json
MGU0R3daMjcxUlN1a0RKQU9PWkI1NTZszODQxM2E5ODEyNWUiLCJVU0VSTkFNRSI6Imlka2pzIiwiVVNFUl9JRF9GT1JfU1JQIjoiaWRranMifX0
```

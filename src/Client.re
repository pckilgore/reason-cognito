// An abstraction over http requests.
// I don't really have a good idea what to do here yet, because there are
// really no good options.
//
// XMLHttpRequest => Events.  Yuck.  But widely supported.  The foundation of axios.
// fetch (axios...) => Promises.  Yuck.  Nothing's ever easy with bucklescript...
//
// So rather than think too hard about that right now I really just want to get started
// with something behind a reasonable interface and then plug it out later.
//
// We control the whole chain through the request and back, and AWS errors here are
// actually pretty sane, if you bother to hold to them (I'm looking at you, amplify.)

[%raw "require('isomorphic-fetch')"];

let idpEndpoint = "https://cognito-idp.us-east-2.amazonaws.com/";

type networkError =
  | NetworkError(Js.Promise.error);

let makeNetworkError = err => {
  NetworkError(err);
};

[@bs.deriving abstract]
type amazonHeaders = {
  [@bs.as "x-amzn-errortype"]
  errorType: array(string),
  [@bs.as "x-amzn-errormessage"]
  errorMessage: array(string),
};

[@bs.deriving abstract]
type amazonHeadersWrapper = {_headers: amazonHeaders};

external headersUnwrap: Fetch.headers => amazonHeadersWrapper = "%identity";

type reasonCognitoResponse =
  | NetworkError(string)
  | InvalidParameterException(string)
  | ReasonCognitoUnknownErrorException(string);

let request = (operation, params: Js.Dict.t(Js.Json.t)) => {
  let headers = Js.Dict.empty();

  Js.Dict.set(headers, "Content-Type", "application/x-amz-json-1.1");
  Js.Dict.set(headers, "X-Amz-User-Agent", "aws-amplify/0.1.x js");
  Js.Dict.set(
    headers,
    "X-Amz-Target",
    "AWSCognitoIdentityProviderService." ++ operation,
  );

  Fetch.fetchWithInit(
    idpEndpoint,
    Fetch.RequestInit.make(
      ~method_=Post,
      ~mode=NoCORS,
      ~cache=NoCache,
      ~headers=Fetch.HeadersInit.makeWithDict(headers),
      ~body=Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(params))),
      (),
    ),
  )
  ->FutureJs.fromPromise(makeNetworkError)
  ->Future.mapError(err => {
      Js.log2("Network Error", err);
      // TODO: More robustness here.
      Belt.Result.Error(
        NetworkError(
          "There was a network error in the Reason-Cognito Library.",
        ),
      );
    })
  ->Future.mapOk(Fetch.Response.headers)
  ->Future.map(Belt.Result.getExn)
  ->Future.map(headers => {
      let errors = headersUnwrap(headers)->_headersGet;
      let err = errors->errorTypeGet->Array.get(0);
      let msg = errors->errorMessageGet->Array.get(0);
      switch (err) {
      | "InvalidParameterException:" =>
        Belt.Result.Error(InvalidParameterException(msg))
      | _ =>
        Belt.Result.Error(
          ReasonCognitoUnknownErrorException(
            "AWS Cognito returned an undocumented error code.",
          ),
        )
      };
    });
};

type keyValuePair = {
  key: string,
  value: string,
};

module QueryString = {
  type t;
  let buildQueryString = ({key, value}) => key ++ "=" ++ value;

  let make = (values: list(keyValuePair)) =>
    switch (values) {
    | [head, ...rest] =>
      "?"
      ++ buildQueryString(head)
      ++ List.fold_left(
           (a, b) => a ++ "&" ++ buildQueryString(b),
           "&",
           rest,
         )
    | [] => ""
    };
};
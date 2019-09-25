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

type keyValue = {
  key: string,
  value: string,
};

type requestConfig = {
  headers: option(list(keyValue)),
  queryString: option(list(keyValue)),
};

let buildQueryString = ({key, value}) => key ++ "=" ++ value;

// exception UnhandledPromise;

// let handlePromiseFailure =
//   [@bs.open]
//   (
//     fun
//     | Not_found => {
//         Js.log2("Not Found", Not_found);
//         Js.Promise.resolve(None);
//       }
//   );

let request = (endpoint: string, config: requestConfig) => {
  let urlWithQueryString =
    switch (config.queryString) {
    | Some([head, ...rest]) =>
      endpoint
      ++ "?"
      ++ buildQueryString(head)
      ++ List.fold_left(
           (a, b) => a ++ "&" ++ buildQueryString(b),
           "&",
           rest,
         )
    | Some([])
    | None => endpoint
    };

  Js.Promise.(
    Fetch.fetch(urlWithQueryString)
    |> then_(Fetch.Response.text)
    |> then_(text => resolve(Some(text)))
    |> catch(error => {
         Js.log(error);
         resolve(None);
       })
  );
};
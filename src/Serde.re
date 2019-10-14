open Types;

let parseCognitoError = err =>
  switch (Js.Json.decodeObject(err)) {
  | Some(envelope) =>
    switch (
      Js.Dict.get(envelope, "__type"),
      Js.Dict.get(envelope, "message"),
    ) {
    | (Some(type_), Some(msg)) =>
      switch (Js.Json.decodeString(type_), Js.Json.decodeString(msg)) {
      | (Some(__type), Some(message)) => Some({__type, message})
      | _ => None
      }
    | _ => None
    }
  | None => None
  };

let makeErrKind = (err, makeErrorFunction) =>
  switch (parseCognitoError(err)) {
  | Some(err) => makeErrorFunction(err)
  | None => `ReasonCognitoUnknownError
  };

let makeSignupResponse = json => {
  switch (Js.Json.decodeObject(json)) {
  | Some(env) =>
    switch (
      Js.Dict.get(env, "UserConfirmed"),
      Js.Dict.get(env, "UserSub"),
      Js.Dict.get(env, "CodeDeliveryDetails"),
    ) {
    | (Some(confirmed), Some(sub), Some(delivery)) =>
      switch (
        Js.Json.decodeBoolean(confirmed),
        Js.Json.decodeString(sub),
        Js.Json.decodeObject(delivery),
      ) {
      | (Some(userConfirmed), Some(userSub), Some(delivery)) =>
        switch (
          Js.Dict.get(delivery, "AttributeName"),
          Js.Dict.get(delivery, "DeliveryMedium"),
          Js.Dict.get(delivery, "Destination"),
        ) {
        | (Some(attrib), Some(medium), Some(dest)) =>
          switch (
            Js.Json.decodeString(attrib),
            Js.Json.decodeString(dest),
            switch (Js.Json.decodeString(medium)) {
            | Some(medium) when medium == "EMAIL" => Email
            | Some(medium) when medium == "SMS" => SMS
            | Some(_)
            | None => UnknownDeliveryMedium
            },
          ) {
          | (Some(attributeName), Some(destination), deliveryMedium) =>
            Some({
              userSub,
              userConfirmed,
              codeDeliveryDetails: {
                attributeName,
                deliveryMedium,
                destination,
              },
            })
          | _ => None
          }
        | _ => None
        }
      | _ => None
      }
    | _ => None
    }
  | None => None
  };
};

let jsonMapString = arr =>
  Array.map(
    item => Js.Dict.map((. value) => Js.Json.string(value), item),
    arr,
  );
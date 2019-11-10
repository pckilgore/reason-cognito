let makeJsonStringsFromDictValues = map =>
  Js.Dict.map((. v) => Js.Json.string(v), map);

let jsonMapString = arr =>
  Array.map(
    item => Js.Dict.map((. value) => Js.Json.string(value), item),
    arr,
  );
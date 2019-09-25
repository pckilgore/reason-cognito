[@bs.deriving jsConverter]
type response_type = [ | [@bs.as "code"] `Code | [@bs.as "token"] `Token];

type config = {
  response_type,
  client_id: string,
};

let authorization = authConfig => {
  let request =
    Client.request(
      "https://reason-test.auth.us-east-1.amazoncognito.com/oauth2/authorize",
    );

  let response = request({queryString: authQueryString, headers: None});

  Js.log(response);
};
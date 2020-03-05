// let srpConfig = SRP.(make({...KnownSafePrimes.bit3072, generator: 2}));

let auth =
  Cognito.Config.make(
    ~clientId="3vjshpa4lgf92nfisjrg9os21a",
    ~region=UsEast2,
    ~pool="us-east-2_pTFKTGtS3",
    (),
  );
let authParameters = Js.Dict.empty();
Js.Dict.set(authParameters, "USERNAME", "sarah7");
Js.Dict.set(authParameters, "PASSWORD", "Password12345@!");
auth->Cognito.initiateSRPAuth(~authParameters, ())->Future.tap(Js.log);

// Stop weird EOF comment reformatting;
let eof = 0;
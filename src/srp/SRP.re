module Date: {let make: unit => string;} = {
  let monthIndex = [|
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec",
  |];
  let dayIndex = [|"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"|];
  let twoDigitString = (test: float) =>
    test < 10.0 ? "0" ++ Js.Float.toString(test) : Js.Float.toString(test);

  /**
   * Create datetime string in amazon-specific format:
   *  "ddd MMM D HH:mm:ss UTC YYYY"
   */
  let make = () => {
    open Js.Date;
    let now = make();
    let day = dayIndex[getUTCDay(now)->int_of_float];
    let month = monthIndex[getUTCMonth(now)->int_of_float];
    let date = getUTCDate(now);
    let hours = getUTCHours(now)->twoDigitString;
    let minutes = getUTCMinutes(now)->twoDigitString;
    let seconds = getUTCSeconds(now)->twoDigitString;
    let year = getUTCFullYear(now);

    {j|$day $month $date $hours:$minutes:$seconds UTC $year|j};
  };
};

module KnownSafePrimes = {
  // https://tools.ietf.org/html/rfc5054#appendix-A
  type t = {
    hex: string,
    generator: int,
  };
  let bit2048 = {
    hex:
      "AC6BDB41324A9A9BF166DE5E1389582FAF72B6651987EE07FC319294"
      ++ "3DB56050A37329CBB4A099ED8193E0757767A13DD52312AB4B03310D"
      ++ "CD7F48A9DA04FD50E8083969EDB767B0CF6095179A163AB3661A05FB"
      ++ "D5FAAAE82918A9962F0B93B855F97993EC975EEAA80D740ADBF4FF74"
      ++ "7359D041D5C33EA71D281E446B14773BCA97B43A23FB801676BD207A"
      ++ "436C6481F1D2B9078717461A5B9D32E688F87748544523B524B0D57D"
      ++ "5EA77A2775D2ECFA032CFBDBF52FB3786160279004E57AE6AF874E73"
      ++ "03CE53299CCC041C7BC308D82A5698F3A8D0C38271AE35F8E9DBFBB6"
      ++ "94B5C803D89F7AE435DE236D525F54759B65E372FCD68EF20FA7111F"
      ++ "9E4AFF73",
    generator: 2,
  };
  let bit3072 = {
    hex:
      "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E08"
      ++ "8A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B"
      ++ "302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9"
      ++ "A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE6"
      ++ "49286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8"
      ++ "FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D"
      ++ "670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C"
      ++ "180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF695581718"
      ++ "3995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D"
      ++ "04507A33A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7D"
      ++ "B3970F85A6E1E4C7ABF5AE8CDB0933D71E8C94E04A25619DCEE3D226"
      ++ "1AD2EE6BF12FFA06D98A0864D87602733EC86A64521F2B18177B200C"
      ++ "BBE117577A615D6C770988C0BAD946E208E24FA074E5AB3143DB5BFC"
      ++ "E0FD108E4B82D120A93AD2CAFFFFFFFFFFFFFFFF",
    generator: 5,
  };
  let bit4096 = {
    hex:
      "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B80DC1CD129024E08"
      ++ "8A67CC74020BBEA63B139B22514A08798E3404DDEF9519B3CD3A431B"
      ++ "302B0A6DF25F14374FE1356D6D51C245E485B576625E7EC6F44C42E9"
      ++ "A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C4B1FE6"
      ++ "49286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA8"
      ++ "FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D"
      ++ "670C354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C"
      ++ "180E86039B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF695581718"
      ++ "3995497CEA956AE515D2261898FA051015728E5A8AAAC42DAD33170D"
      ++ "04507A33A85521ABDF1CBA64ECFB850458DBEF0A8AEA71575D060C7D"
      ++ "B3970F85A6E1E4C7ABF5AE8CDB0933D71E8C94E04A25619DCEE3D226"
      ++ "1AD2EE6BF12FFA06D98A0864D87602733EC86A64521F2B18177B200C"
      ++ "BBE117577A615D6C770988C0BAD946E208E24FA074E5AB3143DB5BFC"
      ++ "E0FD108E4B82D120A92108011A723C12A787E6D788719A10BDBA5B26"
      ++ "99C327186AF4E23C1A946834B6150BDA2583E9CA2AD44CE8DBBBC2DB"
      ++ "04DE8EF92E8EFC141FBECAA6287C59474E6BC05D99B2964FA090C3A2"
      ++ "233BA186515BE7ED1F612970CEE2D7AFB81BDD762170481CD0069127"
      ++ "D5B05AA993B4EA988D8FDDC186FFB7DC90A6C08F4DF435C934063199"
      ++ "FFFFFFFFFFFFFFFF",
    generator: 5,
  };
};

/*
 * The data structure containing components of the SRP key exchange
 * protocol.  See http://srp.stanford.edu/design.html.
 */
type t = {
  bigN: BigInteger.t,
  bigA: BigInteger.t,
  smallA: BigInteger.t,
  smallK: BigInteger.t,
  smallG: BigInteger.t,
};

exception IllegalSRPParameter(string);

let bigIntFromHexStr = s => BigInteger.bigIntBaseN(`String(s), `Int(16));
let hexStrFromBigInt = bigInt => bigInt->BigInteger.toString(~base=16, ());

let makeRandomHexString = length =>
  CryptoJs.WordArray.(make->random(length)->toString);

let makeRandomSmallA = bigN => {
  let randomHex = makeRandomHexString(128);
  bigIntFromHexStr(randomHex)->BigInteger.mod_(`BigInt(bigN));
};

// Memoized operations over SRP (a, A) values.
module A: {
  type t = {
    small: BigInteger.t,
    big: BigInteger.t,
  };
  let clear: unit => unit;
  let makeValid: (BigInteger.t, BigInteger.t, option(t)) => t;
} = {
  type t = {
    small: BigInteger.t,
    big: BigInteger.t,
  };

  // We memoize a and A when we find a valid pair and return the same
  // pair absent explicit instructions to clear the value.
  let memoARecords = ref(None);

  /**
   * Reset memoized values.
   */
  let clear = () => {
    memoARecords := None;
  };

  /**
   * Attempt to generate a valid pair of (a, A) from given (g, N),
   * memoizes the result.
   */
  let rec makeValid = (smallG, bigN, preValidatedResult) => {
    BigInteger.(
      switch (preValidatedResult, memoARecords^) {
      | (_, Some(memoized)) => memoized
      | (Some(prevalidated), None) => prevalidated
      | (None, None) =>
        let smallA = makeRandomSmallA(bigN);
        let bigA = modPow(smallG, `BigInt(smallA), `BigInt(bigN));

        // See constraints from spec.  We retry with a new smallA if this is
        // true.
        let invalid = mod_(bigA, `BigInt(bigN))->equals(`Int(0));
        let retry =
          invalid
            ? None
            : {
              memoARecords := Some({small: smallA, big: bigA});
              Some({small: smallA, big: bigA});
            };
        makeValid(smallG, bigN, retry);
      }
    );
  };
};

let hash = value => {
  open CryptoJs;
  let hash = stringify(hex, sha256(value));
  String.make(64 - Js.String.length(hash), '0') ++ hash;
};

let makeSmallK = (bigN, smallG) => {
  let hash =
    hash("00" ++ hexStrFromBigInt(bigN) ++ "0" ++ hexStrFromBigInt(smallG));
  bigIntFromHexStr(hash);
};

let make = ({hex, generator}: KnownSafePrimes.t) => {
  let bigN = bigIntFromHexStr(hex);
  let smallG = BigInteger.bigIntBaseN(`Int(generator), `Int(16));
  let A.{small: smallA, big: bigA} = A.makeValid(smallG, bigN, None);
  let smallK = makeSmallK(bigN, smallG);
  {bigN, smallG, bigA, smallA, smallK};
};

/**
 * Make SRP (U)
 */
let makeBigU = (bigA, bigB) => {
  let hexA = hexStrFromBigInt(bigA);
  let hexB = hexStrFromBigInt(bigB);
  hash(hexA ++ hexB)->bigIntFromHexStr;
};

/**
 * More amazon-specific bullshit.
 */
let padHex = hexStr =>
  if (String.length(hexStr) mod 2 == 1) {
    "0" ++ hexStr;
  } else if (String.contains("89ABCDEFabcdef", hexStr.[0])) {
    "00" ++ hexStr;
  } else {
    hexStr;
  };

type keyParameters = {
  username: string,
  password: string,
  bigB: BigInteger.t,
  salt: BigInteger.t,
  pool: string,
};

/**
 * I don't quite understand what this is supposed to do over
 * regular old hmacing the key with the salt.  Basically some
 * crazy Amazon shit that doesn't appear in the spec.
 *
 */
let makeHKDF = (key, salt) => {
  let infoBits = {js|Caldera Derived Key\u0001|js};

  let prk = CryptoJs.hmacSha256(`String(key), `String(salt));
  let hmac = CryptoJs.hmacSha256(`String(infoBits), `WordArr(prk));
  let hmacString = CryptoJs.stringify(CryptoJs.hex, hmac);
  Js.String.slice(~from=0, ~to_=16, hmacString);
};

let makeBigS = (t, params, smallX, bigU) => {
  open BigInteger;
  // User:  S = (B - kg^x) ^ (a + ux)   (computes session key)
  // g^x
  let gx = modPow(t.smallG, `BigInt(t.bigN), `BigInt(smallX));
  // k * g^x
  let kgx = multiply(t.smallK, `BigInt(gx));
  // B - kg^x
  let bkgx = subtract(params.bigB, `BigInt(kgx));
  // ux
  let ux = multiply(bigU, `BigInt(smallX));
  // a + ux
  let aux = add(t.smallA, `BigInt(ux));
  // (B - kg^x) ^ (a + ux)
  let result = modPow(bkgx, `BigInt(t.bigN), `BigInt(aux));
  // Per spec
  mod_(result, `BigInt(t.bigN));
};

let makeAuthenticationKey = (t, params) => {
  open BigInteger;
  let testB = mod_(params.bigB, `BigInt(t.bigN))->equals(`Int(0));
  let bigU = makeBigU(t.bigA, params.bigB);
  let testU = bigU->equals(`Int(0));

  switch (testB, testU) {
  | (true, _)
  | (_, true) =>
    // See the spec. I don't see how to recover, yet.
    Belt.Result.Error(`ReasonCognitoSRPError)
  | (_, _) =>
    let usernamePassword =
      params.pool ++ params.username ++ ":" ++ params.password;
    let usernamePasswordHash = hash(usernamePassword);
    let paddedSalt = hexStrFromBigInt(params.salt)->padHex;
    let smallX = hash(paddedSalt ++ usernamePasswordHash)->bigIntFromHexStr;
    let bigS = makeBigS(t, params, smallX, bigU)->hexStrFromBigInt->padHex;
    let hkdf = makeHKDF(bigS, hexStrFromBigInt(bigU)->padHex);
    Belt.Result.Ok(hkdf);
  };
};

let parseUtf = CryptoJs.parse(CryptoJs.utf8);
let parseBase64 = CryptoJs.parse(CryptoJs.base64);

type challengeResponse = {
  secretBlock: string,
  userIdForSrp: string,
  signature: string,
  timestamp: string,
};

let makeResponseParams =
    (
      ~pool,
      ~challengeParameters: CognitoJson_bs.verifierChallengeParams,
      ~key,
    ) => {
  open CryptoJs;
  let timestamp = Date.make();
  let now = parseUtf(timestamp);
  let pool = parseUtf(pool);
  let username = parseUtf(challengeParameters.userIdForSrp);
  let secret = parseBase64(challengeParameters.secretBlock);

  let message =
    WordArray.(pool->concat(username)->concat(secret)->concat(now));
  let signature =
    stringify(base64, hmacSha256(`WordArr(message), `String(key)));
  {
    userIdForSrp: challengeParameters.userIdForSrp,
    secretBlock: challengeParameters.secretBlock,
    signature,
    timestamp,
  };
};
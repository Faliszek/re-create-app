[@deriving yojson]
type package = {
  name: string,
  version: string,
  main: string,
  license: string,
  //   keywords: array(string),
};

let make = (~appName, ~path) => {
  let json =
    package_to_yojson({
      name: appName,
      version: "1.0.0",
      main: "index.js",
      license: "MIT",
      //   keywords: [|"reasonml", "bucklescript", "react"|],
    })
    |> Yojson.Safe.pretty_to_string;

  let channel = open_out(path ++ "/package.json");
  output_string(channel, json);
  close_out(channel);
};

//package json from yarn
// "name": "my-app",
//   "version": "1.0.0",
//   "main": "index.js",
//   "license": "MIT"
//package json from npm
// {
//   "name": "other-app",
//   "version": "1.0.0",
//   "description": "",
//   "main": "index.js",
//   "scripts": {
//     "test": "echo \"Error: no test specified\" && exit 1"
//   },
//   "keywords": [],
//   "author": "",
//   "license": "ISC"

let _ = ();
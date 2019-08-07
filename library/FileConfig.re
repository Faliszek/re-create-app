module PackageJSON = {
  [@deriving yojson]
  type scripts = {
    start: string,
    compile: string,
    build: string,
  };

  [@deriving yojson]
  type package = {
    name: string,
    version: string,
    main: string,
    license: string,
    keywords: array(string),
    scripts,
  };

  let make = (~appName, ~path) => {
    let json =
      package_to_yojson({
        name: appName,
        version: "1.0.0",
        main: "index.js",
        license: "MIT",
        keywords: [|"reasonml", "bucklescript", "react"|],
        scripts: {
          start: "parcel watch ./public/index.html",
          compile: "bsb -make-world -clean-world -w",
          build: "yarn buckle:build && parcel build ./public/index.html",
        },
      })
      |> Yojson.Safe.pretty_to_string;

    let channel = open_out(path ++ "/package.json");
    output_string(channel, json);
    close_out(channel);
  };
};

module BSConfig = {
  [@deriving yojson]
  type reasonKey = {
    [@key "react-jsx"]
    reactJsx: int,
  };
  [@deriving yojson]
  type sources = {
    dir: string,
    subdirs: bool,
  };
  [@deriving yojson]
  type packageSpecsT = {
    [@key "module"]
    module_: string, //TODO: should be some variant
    [@key "in-source"]
    inSource: bool,
  };
  [@deriving yojson]
  type warnings = {error: string};

  [@deriving yojson]
  type bsconfig = {
    name: string,
    reason: reasonKey,
    sources,
    [@key "package-specs"]
    packageSpecs: packageSpecsT,
    suffix: string,
    [@key "bs-dependencies"]
    bsDependencies: array(string),
    warnings,
    namespace: bool,
    refmt: int,
  };

  let make = (~appName, ~path) => {
    let json =
      bsconfig_to_yojson({
        name: appName,
        reason: {
          reactJsx: 3,
        },
        sources: {
          dir: "src",
          subdirs: true,
        },
        packageSpecs: {
          module_: "commonjs",
          inSource: true,
        },
        suffix: ".bs.js",
        bsDependencies: [|"reason-react"|],
        warnings: {
          error: "+101",
        },
        namespace: true,
        refmt: 3,
      })
      |> Yojson.Safe.pretty_to_string;

    let channel = open_out(path ++ "/bsconfig.json");
    output_string(channel, json);
    close_out(channel);
  };
};
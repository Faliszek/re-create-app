module Unix = {
  let os = Sys.os_type;
  let copyTemplate = (~rootPath, ~projectPath) => {
    let slash = Filename.dir_sep;

    let c =
      switch (os) {
      | "Windows" =>
        "xcopy " ++ rootPath ++ "template " ++ projectPath ++ " /s /e"
      | _ =>
        "cp -R "
        ++ rootPath
        ++ "template"
        ++ slash
        ++ ". "
        ++ projectPath
        ++ slash
      };
    print_endline(c);
    c;
  };
  let checkYarn = "yarn -v -q";
  let checkNPM = "npm -v -q";
};
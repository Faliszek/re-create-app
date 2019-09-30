module Unix = {
  let os = Sys.os_type;
  let copyTemplate = (~rootPath, ~projectPath) => {
    print_endline("rootPath " ++ rootPath);
    print_endline("projectPath " ++ projectPath);

    let c =
      switch (os) {
      | "Windows" =>
        "xcopy " ++ rootPath ++ "template " ++ projectPath ++ " /s /e"
      | _ => "cp -R " ++ rootPath ++ "template/. " ++ projectPath ++ "/"
      };
    print_endline(c);
    c;
  };
  let checkYarn = "hash yarn";
  let checkNPM = "hash npm";
};
module Unix = {
  let copyTemplate = (~rootPath, ~projectPath) => {
    print_endline("from " ++ rootPath ++ " to " ++ projectPath);
    "cp -R " ++ rootPath ++ "/template/. " ++ projectPath ++ "/";
  };
  let checkYarn = "hash yarn";
  let checkNPM = "hash npm";
};
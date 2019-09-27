module Unix = {
  let copyTemplate = (~rootPath, ~projectPath) => {
    "cp -R " ++ rootPath ++ "/template/. " ++ projectPath ++ "/";
  };
  let checkYarn = "hash yarn";
  let checkNPM = "hash npm";
};
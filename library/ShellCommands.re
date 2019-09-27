module Unix = {
  let copyTemplate = (~rootPath, ~projectPath) => {
    "cp -R ../../../template/. " ++ projectPath ++ "/";
  };
  let checkYarn = "hash yarn";
  let checkNPM = "hash npm";
};
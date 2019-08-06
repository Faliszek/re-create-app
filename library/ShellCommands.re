module Unix = {
  let copyTemplate = (~rootPath, ~projectPath) => {
    "cp -r ./template/. " ++ projectPath;
  };
  let checkYarn = "hash yarn";
  let checkNPM = "hash npm";
};
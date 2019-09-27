module Unix = {
  let copyTemplate = (~rootPath, ~projectPath) => {
    "pwd  && cp -R ./template/. " ++ projectPath ++ "/";
  };
  let checkYarn = "hash yarn";
  let checkNPM = "hash npm";
};
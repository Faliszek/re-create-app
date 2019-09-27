module Unix = {
  let copyTemplate = (~rootPath, ~projectPath) => {
    "pwd && ls -lah && cp -R ../template/. " ++ projectPath ++ "/";
  };
  let checkYarn = "hash yarn";
  let checkNPM = "hash npm";
};
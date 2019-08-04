module Unix = {
  let copyTemplate = (~path) => "cp -r " ++ "./template/*.* " ++ path;
  let checkYarn = "hash yarn";
  let checkNPM = "hash npm";
};
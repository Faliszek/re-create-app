module Unix = {
  let copyTemplate = (~path) => "cp -r " ++ "./template/*.* " ++ path;
};
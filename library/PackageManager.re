type t = [ | `NPM | `Yarn];

let toString = m =>
  switch (m) {
  | `NPM => "npm"
  | `Yarn => "yarn"
  };

let toCommand = m =>
  switch (m) {
  | `NPM => "npm install "
  | `Yarn => "yarn add "
  };

let check = () => {
  let noPackageManagerDetected =
    Pastel.(<Pastel> "It looks like you don't have npm installed" </Pastel>);
  let result = ref(None);
  switch (Sys.command(ShellCommands.Unix.checkYarn)) {
  | 0 => result := Some(`Yarn)
  | _ =>
    switch (Sys.command(ShellCommands.Unix.checkNPM)) {
    | 0 => result := Some(`NPM)
    | _ =>
      print_endline(noPackageManagerDetected);
      result := None;
    }
  };

  result^;
};
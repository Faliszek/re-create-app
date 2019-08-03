type packageManager =
  | NPM
  | Yarn;

let managerToCommand = m =>
  switch (m) {
  | NPM => "npm init --yes"
  | Yarn => "yarn init --yes"
  };

let managerToString = m =>
  switch (m) {
  | NPM => "npm"
  | Yarn => "yarn"
  };

let quitWithError = () => {
  print_endline(
    Pastel.(<Pastel bold=true color=Red> "\nExiting. " </Pastel>),
  );
  exit(0);
};
let createDirForProject = (~appName) => {
  //   print_endline(appName);
  //   print_endline(pathToBuild);

  //   print_endline(pathToTemplate);
  let result = ref(Ok(""));
  let pathToBuild = Sys.getcwd();

  let appPathName = pathToBuild ++ "/" ++ appName;
  //Try create directory with current path and appName

  try(Unix.mkdir(appPathName, 0o755)) {
  //Of course, it may not goes wall so we try to handle this
  //with useful information for user about a error

  | Unix.Unix_error(err, _, _) =>
    let errorTitle = Pastel.(<Pastel bold=true color=Red> "ERROR" </Pastel>);
    result := Error("");
    let desc =
      switch (err) {
      | Unix.EEXIST =>
        Pastel.(
          <Pastel>
            <Pastel> "I tried to create your new project in " </Pastel>
            <Pastel bold=true> pathToBuild </Pastel>
            <Pastel> " with this cool name " </Pastel>
            <Pastel bold=true> appName </Pastel>
            <Pastel> " but I failed you ☹️ \n\n" </Pastel>
          </Pastel>
        )

      | _ =>
        result := Error("");
        Pastel.(<Pastel> "Error occured" </Pastel>);
      };

    let errMessage = err |> Unix.error_message;
    let usefulInfo =
      Pastel.(
        <Pastel>
          "Your system said: \n"
          <Pastel bold=true color=Red> errMessage "\n\n" </Pastel>
        </Pastel>
      );

    let tips =
      switch (err) {
      | Unix.EEXIST =>
        Pastel.(
          <Pastel>
            "Try run "
            <Pastel bold=true> "rm -rf ./" appName </Pastel>
            " and then, again"
            <Pastel bold=true> " npx create-reason-app " appName </Pastel>
            <Pastel> " or you can pick other name for your project" </Pastel>
          </Pastel>
        )
      | _ =>
        Pastel.(
          <Pastel>
            "I wish I can help but I have no idea what happend"
          </Pastel>
        )
      };
    print_endline(errorTitle);
    print_endline(desc);
    print_endline(usefulInfo);
    print_endline(tips);
  };
  //   Unix.system(ShellCommands.Unix.copyTemplate(~path=appName));
  result^;
};

let runYarnOnNpm = () => {
  let noPackageManagerDetected =
    Pastel.(<Pastel> "It looks like you don't have npm installed" </Pastel>);
  let result = ref(None);

  switch (Sys.command("hash yarn")) {
  | 0 => result := Some(Yarn)
  | _ => result := Some(NPM)
  };

  switch (Sys.command("hash npm")) {
  | 0 => result := Some(NPM)
  | _ => result := None
  };

  let runPackageManagerResult =
    switch (result^) {
    | Some(packageManager) =>
      packageManager |> managerToCommand |> Sys.command
    | None =>
      print_endline(noPackageManagerDetected);
      1;
    };

  print_int(runPackageManagerResult);
  //   try(yarn) {
  //    | exn =>   {
  //     }
  //     | 0 => Ok(Yarn)
  //     | _ => Error(Yarn)
  //     };
  //   } else {
  //     switch (managerToCommand(NPM)) {
  //     | 0 => Ok(NPM)
  //     | _ => Error(NPM)
  //     };
};

let startCreatingProject = (~appName) => {
  switch (createDirForProject(~appName)) {
  | Error(_) => quitWithError()

  | Ok(_) =>
    let pathToBuild = Sys.getcwd();

    let appPathName = pathToBuild ++ "/" ++ appName;
    //   tryCopyTemplate(~appName=name);
    let start =
      Pastel.(
        <Pastel>
          "Creating a new React app in "
          <Pastel bold=true> appPathName "\n" </Pastel>
        </Pastel>
      );
    Sys.command("cd " ++ pathToBuild ++ "/" ++ appName ++ " && pwd") |> ignore;
    // Sys.command("pwd") |> ignore;
    // runYarnOnNpm();

    // let output =
    //   switch (runYarnOnNpm()) {
    //   | Ok(packageManager) => "✅ initialized config file"
    //   | Error(packageManager) =>
    //     "❌ There was an error caused by " ++ managerToString(packageManager)
    //   };

    print_endline(start);
  // print_endline(output);
  };
};

let run = () =>
  switch (Sys.argv, Sys.os_type) {
  | ([|path, name|], "Unix") => startCreatingProject(~appName=name)

  | (_, "Windows") => print_endline("Sorry windows is not supported yet :(")
  | ([|path, name|], _) =>
    print_endline("Sorry your OS is not supported :(")
  | _ => print_endline("something went preety wrong")
  };
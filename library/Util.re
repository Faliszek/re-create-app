type packageManager =
  | NPM
  | Yarn;

let managerToString = m =>
  switch (m) {
  | NPM => "npm"
  | Yarn => "yarn"
  };

let managerToCommand = m =>
  switch (m) {
  | NPM => "npm install"
  | Yarn => "yarn add "
  };

let quitWithError = () => {
  print_endline(
    Pastel.(<Pastel bold=true color=Red> "\nExiting. " </Pastel>),
  );
  exit(0);
};

let createDirForProject = (~appName) => {
  let result = ref(Ok(""));
  let pathToBuild = Sys.getcwd();

  let appPathName = pathToBuild ++ "/" ++ appName;

  //Try create directory with current path and appName

  try(Unix.mkdir(appPathName, 0o777)) {
  //Of course, it may not goes well so we try to handle this
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
            <Pastel bold=true> " npx re-create-app " appName </Pastel>
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

let checkPackageManager = () => {
  let noPackageManagerDetected =
    Pastel.(<Pastel> "It looks like you don't have npm installed" </Pastel>);
  let result = ref(None);
  switch (Sys.command(ShellCommands.Unix.checkYarn)) {
  | 0 => result := Some(Yarn)
  | _ =>
    switch (Sys.command(ShellCommands.Unix.checkNPM)) {
    | 0 => result := Some(NPM)
    | _ =>
      print_endline(noPackageManagerDetected);
      result := None;
    }
  };

  result^;
};

let init = (~path) => {
  print_endline(
    Pastel.(
      <Pastel>
        "Creating a new React app in "
        <Pastel bold=true> path "\n" </Pastel>
      </Pastel>
    ),
  );
};

let instalingDevDependencies = (~packageManager) => {
  print_endline(
    Pastel.(
      <Pastel>
        "\n⏳ Installing dev dependencies "
        <Pastel bold=true> "parcel" </Pastel>
        " and "
        <Pastel bold=true> "bs-platform \n\n" </Pastel>
      </Pastel>
    ),
  );

  try(
    Unix.system(
      managerToCommand(packageManager) ++ " parcel-bundler bs-platform --dev",
    )
    |> ignore
  ) {
  | exn => exn |> Printexc.to_string |> print_endline
  };
};
let installingDependencies = (~packageManager) => {
  print_endline(
    Pastel.(
      <Pastel>
        "\n⏳ Installing "
        <Pastel bold=true> "reason-react \n\n" </Pastel>
      </Pastel>
    ),
  );

  try(
    Unix.system(
      managerToCommand(packageManager) ++ "reason-react react react-dom",
    )
    |> ignore
  ) {
  | exn => exn |> Printexc.to_string |> print_endline
  };
};

let tryCopyTemplate = (~rootPath, ~projectPath) => {
  let re = Str.regexp("/");
  let folders = rootPath |> Str.split_delim(re) |> Array.of_list;
  let templatePath =
    folders
    |> Array.sub(_, 0, Array.length(folders) - 5)
    |> Array.fold_left((acc, folder) => acc ++ folder ++ "/", "");

  print_endline("Template path is here " ++ templatePath);
  try(
    Unix.system(
      ShellCommands.Unix.copyTemplate(~rootPath=templatePath, ~projectPath),
    )
    |> ignore
  ) {
  | exn => exn |> Printexc.to_string |> print_endline
  };
  Sys.chdir(projectPath);
};

let startCreatingProject = (~rootPath, ~appName) => {
  print_endline(
    Pastel.(
      <Pastel> "Starting create project, this may take a while \n" </Pastel>
    ),
  );
  switch (createDirForProject(~appName)) {
  | Error(_) => quitWithError()

  | Ok(_) =>
    let pathToBuild = Sys.getcwd();

    let appPathName = pathToBuild ++ "/" ++ appName;

    switch (checkPackageManager()) {
    | Some(packageManager) =>
      FileConfig.PackageJSON.make(~appName, ~path=appPathName);

      FileConfig.BSConfig.make(~appName, ~path=appPathName);
      print_endline("\n✅ Created package.json succesfully!");
      Unix.chdir(appPathName) |> ignore;

      init(~path=appPathName);
      installingDependencies(~packageManager);
      instalingDevDependencies(~packageManager);
      tryCopyTemplate(~rootPath, ~projectPath=appPathName);

    | None =>
      print_endline(
        "\n❌ It looks like you don't have installed "
        ++ managerToString(NPM)
        ++ " on your system, you can install nodejs && npm here \n https://nodejs.org/en/download/package-manager/",
      )
    };
  };
};

let run = () => {
  switch (Sys.argv, Sys.os_type) {
  | (args, "Unix") when args |> Array.length == 1 =>
    print_endline(
      Pastel.(
        <Pastel>
          "You should provide name for your app, for example as second argument: \n\n"
          <Pastel bold=true> "npx re-create-app my-app" </Pastel>
        </Pastel>
      ),
    )
  | ([|path, name|], "Unix") =>
    print_endline("Runnig from " ++ path);
    startCreatingProject(~rootPath=path, ~appName=name);

  | (_, "Windows") => print_endline("Sorry Windows is not supported yet :(")
  | ([|path, name|], _) =>
    print_endline("Sorry your OS is not supported :(")
  | _ => print_endline("something went preety wrong")
  };
  print_endline("\n\nExiting.");
};
module Err = {
  let quit = () => {
    print_endline(
      Pastel.(<Pastel bold=true color=Red> "\nExiting. " </Pastel>),
    );
    exit(1);
  };
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

let installingDevDependencies = (~packageManager) => {
  print_endline(
    Pastel.(
      <Pastel>
        "\n⏳ Installing dev dependencies "
        <Pastel bold=true> "parcel" </Pastel>
        " and "
        <Pastel bold=true> "bs-platform " </Pastel>
        <Pastel> ", this may take a while \n\n" </Pastel>
      </Pastel>
    ),
  );

  try(
    Unix.system(
      PackageManager.toCommand(packageManager)
      ++ " parcel-bundler bs-platform --dev",
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
      PackageManager.toCommand(packageManager)
      ++ "reason-react react react-dom",
    )
    |> ignore
  ) {
  | exn => exn |> Printexc.to_string |> print_endline
  };
};

//rootPath is a path from where program is stored,
//and have /template inside
//projectPath is place where user call our cli pacakge

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
  ();
};

let startCreatingProject = (~rootPath, ~appName) => {
  print_endline(Pastel.(<Pastel> "Start creating project\n" </Pastel>));

  switch (createDirForProject(~appName)) {
  | Error(_) => Err.quit()
  | Ok(_) =>
    let pathToBuild = Sys.getcwd();
    let appPathName = pathToBuild ++ "/" ++ appName;

    switch (PackageManager.check()) {
    | Some(packageManager) =>
      Config.PackageJSON.make(~appName, ~path=appPathName);
      Config.BSConfig.make(~appName, ~path=appPathName);
      print_endline("\n✅ Created package.json succesfully!");
      Unix.chdir(appPathName) |> ignore;
      init(~path=appPathName);
      installingDependencies(~packageManager);
      installingDevDependencies(~packageManager);
      tryCopyTemplate(~rootPath, ~projectPath=appPathName);
    | None =>
      print_endline(
        "\n❌ It looks like you don't have installed "
        ++ PackageManager.toString(`NPM)
        ++ " on your system, you can install nodejs && npm here \n https://nodejs.org/en/download/package-manager/",
      )
    };
  };
};

let run = () => {
  switch (Sys.argv, Sys.os_type) {
  | (args, _) when args |> Array.length == 1 =>
    print_endline(
      Pastel.(
        <Pastel>
          "You should provide name for your app, for example as second argument: \n\n"
          <Pastel bold=true> "npx re-create-app my-app" </Pastel>
        </Pastel>
      ),
    )
  | ([|path, name|], _) =>
    startCreatingProject(~rootPath=path, ~appName=name)

  | (_, "Windows") => print_endline("Sorry Windows is not supported yet :(")
  | _ => print_endline("Something went preety wrong")
  };
};
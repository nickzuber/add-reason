
open Utils;
open GenericBindings;

[@bs.module]
external camelCase : string => string = "lodash.camelcase";

[@bs.val] [@bs.module "../../../../src/patches"]
external appendToPackageScripts : (string, string) => bool = "editPackageScripts";

[@bs.val] [@bs.module "../../../../src/patches"]
external generateConfigContents : (string, string) => string = "";

[@bs.val] [@bs.module "../../../../src/patches"]
external generateMerlinContents : (string, string) => string = "";

module InitCommand {
  let defaultCompiledPath = "/lib/js";
  let defaultNodeModulesPath = "node_modules";
  let bsConfigFile = "bsconfig.json";
  let merlinFile = ".merlin";

  let attemptToLink = (position, source, dest) : bool => {
    let (index, total) = position;
    let position = Printf.sprintf("[%d/%d] ", index, total) |> gray;
    let targetDirectory = dest |> Path.removeTopFromPath;
    
    /** Should exist. This needs to be true since we can/will have problems otherwise
      * Also note that this shouldn't ever really be false since the step for configuring this
      * source happens before this step. This being false means something went wrong in that previous
      * configuration step. */
    let existsSource = FsPolyfill.safeFileExists(source);
    /* Should not exist. It can be false, just show a warning */
    let existsDest = FsPolyfill.safeFileExists(dest);
    /* Should exist. Needs to tbe true */
    let isValidPathDest = targetDirectory |> FsPolyfill.safeFileExists;

    let prefix = position ++ getEmoji("link") ++ "Linking package... ";
    let success = switch (existsSource, existsDest, isValidPathDest) {
      | (_, _, false) =>
        Printf.sprintf("%s%s Couldn't find your node modules directory. Do you have a typo?\n%s%s",
          prefix, red("failed"), altCodeDirectional, bold(targetDirectory)) 
          |> Js.log;
        false
      | (true, false, _) =>
        createSymlink(source, dest);
        Printf.sprintf("%s%s", prefix, green("success")) 
          |> Js.log;
        true
      | (true, true, _) =>
        Printf.sprintf("%s%s There's already a link with that name in your node modules. Did you mean to `unlink`?\n%s%s",
          prefix, yellow("warning"), altCodeDirectional, bold(dest)) 
          |> Js.log;
        true
      | (false, false, _) =>
        Printf.sprintf("%s%s Couldn't find your source directory. Do you have a typo?\n%s%s",
          prefix, red("failed"), altCodeDirectional, bold(source)) 
          |> Js.log;
        false
      | (false, true, _) =>
        Printf.sprintf("%s%s Couldn't find your source directory and there was a file already in your node modules. Do you have any typos?\n%s%s\n%s%s",
          prefix, red("failed"), altCodeDirectionalNonTerminal, bold(source), altCodeDirectional, bold(dest))
          |> Js.log;
        false
    };
    success;
  };

  let attemptToCreateConfig = (position, filename, contents) => {
    try {
      let (index, total) = position;
      let position = Printf.sprintf("[%d/%d] ", index, total) |> gray;
      let prefix = position ++ getEmoji("truck") ++ "Creating config... ";

      /* Should not exist, but if it doesn't we just ignore and warn */
      let existsConfig = FsPolyfill.safeFileExists(filename);
      let _ = switch existsConfig {
        | false =>
          createAndWriteToFile(filename, contents);
          let filename = filename |> bold;
          Printf.sprintf("%s%s", prefix, green("success"))
            |> Js.log;
        | true =>
          Printf.sprintf("%s%s You already have a bsconfig file. Skipping this step.", prefix, yellow("warning"))
            |> Js.log;
      };
      true
    } {
      | _ => false
    }
  };

  let attemptToCreateMerlin = (position, filename, contents) => {
    try {
      let (index, total) = position;
      let position = Printf.sprintf("[%d/%d] ", index, total) |> gray;
      let prefix = position ++ getEmoji("lipstick") ++ "Creating linter... ";

      /* Should not exist, but if it doesn't we just ignore and warn */
      let existsMerlin = FsPolyfill.safeFileExists(filename);
      let _ = switch existsMerlin {
        | false =>
          createAndWriteToFile(filename, contents);
          let filename = filename |> bold;
          Printf.sprintf("%s%s", prefix, green("success"))
            |> Js.log;
        | true =>
          Printf.sprintf("%s%s You already have a merlin file. Skipping this step.", prefix, yellow("warning"))
            |> Js.log;
      };
      true
    } {
      | _ => false
    }
  };

  let buildRelativeSymlinkPaths = (name, directory) : (string, string) => {
    let source = Path.combinePaths(["..", defaultCompiledPath, directory], ~useLeadingSlash=false);
    let dest = Path.combinePaths([".", defaultNodeModulesPath, name], ~useLeadingSlash=false);
    (source, dest)
  };

  let getCompleteMessage = (finishWithFailure, name) : string => {
    switch finishWithFailure {
      | true =>
        Printf.sprintf("%s%s See which step went wrong", 
          getEmoji("no_entry_sign"), red("fail"))
      | false =>
        let pkg = Printf.sprintf("const %s = require('%s');", camelCase(name), name) |> bold;
        let example = Printf.sprintf("Import your compiled ReasonML code like:\n%s\n%s %s\n%s\n%s\n%s",
          gray("1 "++altLong), gray("2 "++altLong), pkg, gray("3 "++altLong), gray("4 "++altLong++" // rest of code..."), gray("5 "++altLong));
        Printf.sprintf("%s%s\n%s %s",
          getEmoji("sparkles"), green("done"), green("success"), example)
    };
  };

  let checkIfBsPlatformIsInstalled = (rootDirectory) : bool => {
    let bsPlatformLocation = Path.combinePaths([rootDirectory, defaultNodeModulesPath, "bs-platform"]);
    let exists = FsPolyfill.safeFileExists(bsPlatformLocation);
    exists
  };

  let performLinking = (position, name, directory, rootDirectory, isCancelled) : bool => {
    if (isCancelled) {
      let (index, total) = position;
      let position = Printf.sprintf("[%d/%d] ", index, total) |> gray;
      Printf.sprintf("%s%sLinking package... %s", position, getEmoji("link"), red("cancelled")) |> Js.log;
      false
    } else {
      open Path;
      let linkToNodeModules = combinePaths([rootDirectory, defaultNodeModulesPath]);
      let source = combinePaths([rootDirectory, defaultCompiledPath, directory]);
      let dest = combinePaths([linkToNodeModules, name]);
      let success = FsPolyfill.attemptToLink(position, source, dest);
      success
    }
  };

  let performConfigCreation = (position, name, directory, rootDirectory, isCancelled) : bool => {
    if (isCancelled) {
      let (index, total) = position;
      let position = Printf.sprintf("[%d/%d] ", index, total) |> gray;
      Printf.sprintf("%s%sCreating config... %s", position, getEmoji("truck"), red("cancelled")) |> Js.log;
      false
    } else {
      let absolutePath = Path.combinePaths([rootDirectory, bsConfigFile]);
      let packageLocation = Path.combinePaths([rootDirectory, "package.json"]);
      let configContents = generateConfigContents(directory, packageLocation);
      let success = FsPolyfill.attemptToCreateConfig(position, absolutePath, configContents);
      success
    }
  };

  let performMerlinCreation = (position, name, directory, rootDirectory, isCancelled) : bool => {
    if (isCancelled) {
      let (index, total) = position;
      let position = Printf.sprintf("[%d/%d] ", index, total) |> gray;
      Printf.sprintf("%s%sCreating linter... %s", position, getEmoji("lipstick"), red("cancelled")) |> Js.log;
      false
    } else {
      let absolutePath = Path.combinePaths([rootDirectory, merlinFile]);
      let nodeModulesLocation = Path.combinePaths([rootDirectory, defaultNodeModulesPath]);
      let configContents = generateMerlinContents(directory, nodeModulesLocation);
      let success = FsPolyfill.attemptToCreateMerlin(position, absolutePath, configContents);
      success
    }
  };

  let performEndpointSetup = (position, name, directory, rootDirectory, isCancelled) : bool => {
    open FsPolyfill;
    open Path;
    let (index, total) = position;
    let position = Printf.sprintf("[%d/%d]", index, total) |> gray;

    let directoryPath = combinePaths([rootDirectory, directory]);
    let lib = combinePaths([rootDirectory, "lib"]);
    let lib_js = combinePaths([lib, "js"]);
    let lib_js_name = combinePaths([lib_js, directory]);

    let existsDirectory = safeFileExists(directoryPath);
    let existsLib = safeFileExists(lib);
    let existsLibJs = safeFileExists(lib_js);
    let existsLibJsName = safeFileExists(lib_js_name);

    let success = switch (existsLib, existsLibJs, existsLibJsName, existsDirectory, isCancelled) {
      | (_, _, _, _, true) =>
        Printf.sprintf("%s %sValidating target... %s",
          position, getEmoji("open_file_folder"), red("cancelled"))
          |> Js.log;
        false
      | (_, _, _, false, _) =>
        Printf.sprintf("%s %sValidating target... %s Couldn't find your source directory of ReasonML code. Do you have a typo?\n%s%s",
          position, getEmoji("open_file_folder"), red("failed"), altCodeDirectional, bold(directoryPath))
          |> Js.log;
        false
      | (false, _, _, _, _) =>
        let createDirsSuccess = safeCreateDirectory(lib_js_name);
        let result = if (createDirsSuccess) {
          green("success")
        } else {
          Printf.sprintf("%s Something went wrong when trying to create target directories.\n%s%s",
            red("failed"), altCodeDirectional, bold(lib_js_name));
        };
        Printf.sprintf("%s %sValidating target... %s",
          position, getEmoji("open_file_folder"), result)
          |> Js.log;
        createDirsSuccess
      | (true, false, _, _, _) =>
        let createDirsSuccess = safeCreateDirectory(lib_js_name);
        let result = if (createDirsSuccess) {
          yellow("warning") ++ " You already have a `lib` directory, so we're going to use it.";
        } else {
          Printf.sprintf("%s Something went wrong when trying to create target directories.\n%s%s",
            red("failed"), altCodeDirectional, bold(lib_js_name));
        };
        Printf.sprintf("%s %sValidating target... %s",
          position, getEmoji("open_file_folder"), result)
          |> Js.log;
        true
      | (true, true, false, _, _) =>
        let createDirsSuccess = safeCreateDirectory(lib_js_name);
        let result = if (createDirsSuccess) {
          yellow("warning") ++ " You already have a `lib/js` directory, so we're going to use it.";
        } else {
          Printf.sprintf("%s Something went wrong when trying to create target directories.\n%s%s",
            red("failed"), altCodeDirectional, bold(lib_js_name));
        };
        Printf.sprintf("%s %sValidating target... %s",
          position, getEmoji("open_file_folder"), result)
          |> Js.log;
        true
      | (true, true, true, _, _) => 
        Printf.sprintf("%s %sValidating target... %s",
          position, getEmoji("open_file_folder"), green("success"))
          |> Js.log;
        true
    };
    success
  };

  let performPostInstall = (position, name, directory, rootDirectory, isCancelled) : bool => {
    let (index, total) = position;
    let (source, dest) = buildRelativeSymlinkPaths(name, directory);
    let position = Printf.sprintf("[%d/%d]", index, total) |> gray;
    let packageLocation = Path.combinePaths([rootDirectory, "package.json"]);
    let prefix = Printf.sprintf("%s %sAdding postinstall... ", position, getEmoji("nut_and_bolt"));
    let postInstallCommand = Printf.sprintf("node -e \"var s='%s',d='%s',fs=require('fs');if(fs.existsSync(d)===false){fs.symlinkSync(s,d,'dir')};\"", source, dest);
    if (isCancelled) {
      Printf.sprintf("%s%s", prefix, red("cancelled")) |> Js.log;
      false
    } else {
      let success = appendToPackageScripts(packageLocation, postInstallCommand);    
      switch success {
        | true => Printf.sprintf("%s%s", prefix, green("success")) |> Js.log
        | false =>
          let postInstallCommand' = Printf.sprintf("node -e \\\"var s='%s',d='%s',fs=require('fs');if(fs.existsSync(d)===false){fs.symlinkSync(s,d,'dir')};\\\"", source, dest);
          let prettyPostInstallCommand = Printf.sprintf("\"postinstall\": \"%s\"", postInstallCommand') |> bold;
          Printf.sprintf("%s%s Unable to automatically add postinstall script\n%sAdd this to your `package.json` scripts:\n   %s",
            prefix, yellow("warning"), altCodeDirectional, prettyPostInstallCommand)
            |> Js.log
      };
      true
    }
  };

  let execute = (steps, name, directory, rootDirectory) : bool => {
    let total = List.length(steps);
    let finishWithFailure = ref(false);
    let rec loop = (steps, index, isCancelled) : unit =>
      switch steps {
        | [cur] => 
          let success = cur((index, total), name, directory, rootDirectory, isCancelled);
          if (!success) { finishWithFailure := true }
        | [cur, ...rest] =>
          let success = cur((index, total), name, directory, rootDirectory, isCancelled);
          if (!success) { finishWithFailure := true };
          loop(rest, index + 1, finishWithFailure^)
        | _ => ()
      };
    loop(steps, 1, false);
    finishWithFailure^
  };

  let main = (name, directory, rootDirectory, version, linking) : unit => {
    Printf.sprintf("add-reason init v%s", version)
      |> white
      |> bold
      |> Js.log;
    let stepsAsFunctions = if (linking) {
      [ performEndpointSetup,
        performLinking,
        performConfigCreation,
        performMerlinCreation,
        performPostInstall ];
    } else {
      [ performEndpointSetup,
        performConfigCreation,
        performMerlinCreation ];
    };
    let finishWithFailure = execute(stepsAsFunctions, name, directory, rootDirectory);
    /* Show message regarding success of the init */
    getCompleteMessage(finishWithFailure, name)
      |> Js.log;
    /* Prompt is `bs-platform` is not installed */
    if (!checkIfBsPlatformIsInstalled(rootDirectory)) {
      let bsPlatform = "bs-platform" |> bold |> white;
      Printf.sprintf("%s You don't seem to have %s installed yet! Run the command:\n%s%s\n%s  or\n%s%s",
        yellow("warning"), bsPlatform, altCodeDirectionalNonTerminal, bold("yarn add bs-platform@2.1.0") |> white,
        altLong, altCodeDirectional, bold("npm install bs-platform@2.1.0") |> white)
        |> Js.log;
    };
    Js.log("")
  };
};


open Utils;
open GenericBindings;

[@bs.module]
external camelCase : string => string = "lodash.camelcase";

[@bs.val] [@bs.module "../../../../src/patches"]
external appendToPackageScripts : (string, string) => bool = "editPackageScripts";

[@bs.val] [@bs.module "../../../../src/patches"]
external generateConfigContents : (string, string) => string = "generateConfigContents";

module InitCommand {
  let defaultCompiledPath = "/lib/js";
  let defaultNodeModulesPath = "node_modules";
  let bsConfigFile = "bsconfig.json";

  let buildRelativeSymlinkPaths = (name, directory) : (string, string) => {
    let source = Path.combinePaths(["..", defaultCompiledPath, directory], ~useLeadingSlash=false);
    let dest = Path.combinePaths([".", defaultNodeModulesPath, name], ~useLeadingSlash=false);
    (source, dest)
  };

  let getCompleteMessage = (finishWithFailure, name) : string => {
    switch finishWithFailure {
      | true =>
        Printf.sprintf("%s%s See which step went wrong", 
          Global.Emojis.failure(), red("fail"))
      | false =>
        let pkg = Printf.sprintf("const %s = require('%s');", camelCase(name), name) |> bold;
        let example = Printf.sprintf("Import your compiled ReasonML code like:\n%s\n%s %s\n%s\n%s\n%s",
          gray("1 "++altLong), gray("2 "++altLong), pkg, gray("3 "++altLong), gray("4 "++altLong++" // rest of code..."), gray("5 "++altLong));
        Printf.sprintf("%s%s\n%s %s", 
          Global.Emojis.success(), green("done"), green("success"), example)
    };
  };

  let performLinking = (position, name, directory, rootDirectory) : bool => {
    open Path;
    let linkToNodeModules = combinePaths([rootDirectory, defaultNodeModulesPath]);
    let source = combinePaths([rootDirectory, defaultCompiledPath, directory]);
    let dest = combinePaths([linkToNodeModules, name]);
    let success = FsPolyfill.attemptToLink(position, source, dest);
    success
  };

  let performConfigCreation = (position, name, directory, rootDirectory) : bool => {
    let absolutePath = Path.combinePaths([rootDirectory, bsConfigFile]);
    let packageLocation = Path.combinePaths([rootDirectory, "package.json"]);
    let configContents = generateConfigContents(directory, packageLocation);
    let success = FsPolyfill.attemptToCreateConfig(position, absolutePath, configContents);
    success
  };

  let performEndpointSetup = (position, name, directory, rootDirectory) : bool => {
    let (index, total) = position;
    let position = Printf.sprintf("[%d/%d]", index, total) |> gray;
    /** @TODO
     * check for lib/           <- a
     * check for lib/js         <- b
     * check for lib/js/<name>  <- c
     * switch (a, b, c) { ...
     *   | (false, _, _) => create all 3 (success)
     *   | (true, false, _) => create /lib/js and /libjs/<name> (warning, lib exists and we gonna use it)
     *   | (true, true, false) => create /lib/js/<name> (success)
     *   | (true, true, true) => do nothing (success)
     */
    Printf.sprintf("%s %sValidating target... %s Unfinished", position, Global.Emojis.validate(), yellow("warning")) |> Js.log;
    true
  };

  let performPostInstall = (position, name, directory, rootDirectory) : bool => {
    let (index, total) = position;
    let (source, dest) = buildRelativeSymlinkPaths(name, directory);
    let position = Printf.sprintf("[%d/%d]", index, total) |> gray;
    let packageLocation = Path.combinePaths([rootDirectory, "package.json"]);
    let prefix = Printf.sprintf("%s %sAdding postinstall... ", position, Global.Emojis.postinstall());
    let postInstallCommand = Printf.sprintf("node -e \"var s='%s',d='%s',fs=require('fs');if(fs.existsSync(d)===false){fs.symlinkSync(s,d,'dir')};\"", source, dest);
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
  };

  let execute = (steps, name, directory, rootDirectory) : bool => {
    let total = List.length(steps);
    let finishWithFailure = ref(false);
    let rec loop = (steps, index) : unit =>
      switch steps {
        | [cur] => 
          let success = cur((index, total), name, directory, rootDirectory);
          if (!success) { finishWithFailure := true }
        | [cur, ...rest] =>
          let success = cur((index, total), name, directory, rootDirectory);
          if (!success) { finishWithFailure := true };
          loop(rest, index + 1)
        | _ => ()
      };
    loop(steps, 1);
    finishWithFailure^
  };

  let main = (name, directory, rootDirectory, version, useEmojis : option(string)) : unit => {
    Printf.sprintf("add-reason init v%s", version)
      |> white
      |> bold
      |> Js.log;
    Global.Emojis.use := switch useEmojis {
      | Some(_) => true
      | None => false
    };
    let stepsAsFunctions = [
      performConfigCreation,
      performEndpointSetup,
      performLinking,
      performPostInstall
    ];
    let finishWithFailure = execute(stepsAsFunctions, name, directory, rootDirectory);
    getCompleteMessage(finishWithFailure, name)
      |> Js.log;
  };
};

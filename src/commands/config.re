open Utils;

open Bindings;

/**
 * Create a basic building config file if one doesn't already exist.
 * TODO: Add a check for package file too; we can do a match statement here.
 */
let createBuildingConfig =
    (inSource, _name, source, root)
    : (bool, option(string)) => {
  paint("looking for building config");
  let configPath = Path.combinePaths([root, "bsconfig.json"]);
  let packagePath = Path.combinePaths([root, "package.json"]);
  let contents = generateConfigContents(source, inSource, packagePath);
  if (! Fs.safeFileExists(configPath)) {
    paint("creating building config");
    (
      Fs.safeCreateFile(configPath, contents),
      Some("created " ++ highlightColor("bsconfig.json"))
    );
  } else {
    (true, None);
  };
};

let main = (source, root, version) : unit => {
  Printf.sprintf("add-reason config v%s\n", version) |> white |> bold |> stdout;
  /* `inSource` will always be false here. */
  let stepsAsFunctions = [createBuildingConfig(false)];
  let (finishWithFailure, comments) =
    Utils.execute(stepsAsFunctions, (), source, root);
  finishWithFailure ? success() : failure();
  Utils.printList(comments);
};

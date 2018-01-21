open Utils;
open Bindings;

/**
 * Create a basic linting config file if one doesn't already exist.
 */
let createLintingConfig = (_name, source, root) : (bool, option(string)) => {
  paint("looking for linting config");
  let configPath = Path.combinePaths([root, ".merlin"]);
  let nodeModulesPath = Path.combinePaths([root, "node_modules"]);
  let contents = generateMerlinContents(source, nodeModulesPath);
  if (!Fs.safeFileExists(configPath)) {
    paint("creating linting config");
    (Fs.safeCreateFile(configPath, contents), Some("created " ++ yellow(".merlin")));
  } else {
    (true, None);
  }
};

let main = (source, root, version) : unit => {
  Printf.sprintf("add-reason linter v%s\n", version)
    |> white
    |> bold
    |> stdout;
  let stepsAsFunctions = [
    createLintingConfig
  ];
  let (finishWithFailure, comments) = Utils.execute(stepsAsFunctions, (), source, root);
  finishWithFailure ? success() : failure();
  Utils.printList(comments)
}

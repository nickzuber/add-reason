open Utils;
open Bindings;

/**
 * Create a basic linting config file if one doesn't already exist.
 */
let createLintingConfig = (_name, source, root) : bool => {
  paint("looking for linting config");
  let configPath = Path.combinePaths([root, ".merlin"]);
  let nodeModulesPath = Path.combinePaths([root, "node_modules"]);
  let contents = generateMerlinContents(source, nodeModulesPath);
  if (!Fs.safeFileExists(configPath)) {
    paint("creating linting config");
    Fs.safeCreateFile(configPath, contents);
  } else {
    true;
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
  let finishWithFailure = Utils.execute(stepsAsFunctions, (), source, root);
  finishWithFailure ? success() : ();
  stdout("\n")
}

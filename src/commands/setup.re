open Utils;
open Bindings;

/**
 * Ensure that the source directory (where the ReasonML code is) actually exists, and
 * we attempt to create the target directory too. If it already exists, then nothing
 * happens with it.
 */
let prepareTargetDirectory = (name, source, root) : bool => {
  paint("looking for source directory");
  let sourcePath = Path.combinePaths([root, source]);
  let outputPath = Path.combinePaths([root, "lib", "js", source]);
  if (Fs.safeFileExists(sourcePath)) {
    paint("creating target directory");
    Fs.safeCreateDirectory(outputPath);
  } else {
    false;
  }
};

/**
 * Create a basic building config file if one doesn't already exist.
 * TODO: Add a check for package file too; we can do a match statement here.
 */
let createBuildingConfig = (name, source, root) : bool => {
  paint("looking for building config");
  let configPath = Path.combinePaths([root, "bsconfig.json"]);
  let packagePath = Path.combinePaths([root, "package.json"]);
  let contents = generateConfigContents(source, packagePath);
  if (!Fs.safeFileExists(configPath)) {
    paint("creating building config");
    Fs.safeCreateFile(configPath, contents);
  } else {
    true;
  }
};

/**
 * Create a basic linting config file if one doesn't already exist.
 */
let createLintingConfig = (name, source, root) : bool => {
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

let main = (name, source, root, version, linking) : unit => {
  Printf.sprintf("add-reason setup v%s\n", version)
    |> white
    |> bold
    |> stdout;
  let stepsAsFunctions = if (linking) {
    [ prepareTargetDirectory,
      createBuildingConfig,
      createLintingConfig,
      Link.performLinking,
      Link.createPostinstall ]
  } else {
    [ prepareTargetDirectory,
      createBuildingConfig,
      createLintingConfig ]
  };
  let finishWithFailure = Utils.execute(stepsAsFunctions, name, source, root);
  finishWithFailure ? success() : ();
  stdout("\n")
}

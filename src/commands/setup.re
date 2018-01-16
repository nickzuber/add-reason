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

/**
 * Create a symlink in the local node modules directory that points to the
 * transpiled ReasonML code.
 */
let performLinking = (name, source, root) : bool => {
  paint("seeing if we can make a symlink");
  let nodeModulesPath = Path.combinePaths([root, "node_modules"]);
  let symlinkPath = Path.combinePaths([root, "node_modules", name]);
  let outputPath = Path.combinePaths([root, "lib", "js", source]);
  /* Can't find node_modules folder, then make one */
  if (!Fs.safeFileExists(nodeModulesPath)) {
    /* Temporary until we organize a better way to handle these edge case errors */
    let _ = Fs.safeCreateDirectory(nodeModulesPath);
  };
  /* Namespace already exists in node_modules, we complain and stop setup */
  if (Fs.safeFileExists(symlinkPath)) {
    failure("file with the name \"" ++ name ++ "\" already exists in your \
      node_modules, so we cannot create a symlink here.");
    false;
  } else {
    paint("making symlink to output directory");
    Fs.safeCreateSymlink(outputPath, symlinkPath)
      ? true
      : { failure("something went wrong when creating the symlink"); false }
  }
};

let createPostinstall = (name, source, root) : bool => {
  paint("preparing to add a postinstall script");
  let packagePath = Path.combinePaths([root, "package.json"]);
  /* We use relative paths here specifically for the postinstall script */
  let source = Path.combinePaths(["..", "lib", "js", source], ~useLeadingSlash=false);
  let dest = Path.combinePaths([".", "node_modules", name], ~useLeadingSlash=false);
  let command = Printf.sprintf("node -e \"var s='%s',d='%s',fs=require('fs');\
    if(fs.existsSync(d)===false){fs.symlinkSync(s,d,'dir')};\"", source, dest);
  paint("adding the postinstall script to your package file");
  appendToPackageScripts(packagePath, command);
};

let rec execute = (steps, name, source, root) : bool => {
  switch (steps) {
    | [] => true;
    | [step] => step(name, source, root);
    | [step, ...rest] => step(name, source, root)
      ? execute(rest, name, source, root)
      : false;
  }
};

let main = (name, source, root, version, linking) : unit => {
  Printf.sprintf("add-reason setup v%s\n", version)
    |> white
    |> bold
    |> stdout;
  let stepsAsFunctions = linking ? [
      prepareTargetDirectory,
      createBuildingConfig,
      createLintingConfig,
      performLinking,
      createPostinstall ]
    : [
      prepareTargetDirectory,
      createBuildingConfig,
      createLintingConfig ];
  let finishWithFailure = execute(stepsAsFunctions, name, source, root);
  finishWithFailure ? success() : ();
  stdout("\n")
}

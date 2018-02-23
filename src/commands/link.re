open Utils;

open Bindings;

/**
 * Create a symlink in the local node modules directory that points to the
 * transpiled ReasonML code.
 */
let performLinking = (name, source, root) : (bool, option(string)) => {
  paint("seeing if we can make a symlink");
  let nodeModulesPath = Path.combinePaths([root, "node_modules"]);
  let symlinkPath = Path.combinePaths([root, "node_modules", name]);
  let outputPath = Path.combinePaths([root, "lib", "js", source]);
  /* Can't find node_modules folder, then make one */
  if (! Fs.safeFileExists(nodeModulesPath)) {
    /* Temporary until we organize a better way to handle these edge case errors */
    let _ = Fs.safeCreateDirectory(nodeModulesPath);
    ();
  };
  /* Namespace already exists in node_modules, we complain and stop setup */
  if (Fs.safeFileExists(symlinkPath)) {
    (
      false,
      Some(
        Printf.sprintf(
          "%s already exists in your %s, so we can't create a symlink",
          highlightColor(name),
          highlightColor("node_modules")
        )
      )
    );
  } else {
    paint("making symlink to output directory");
    Fs.safeCreateSymlink(outputPath, symlinkPath) ?
      (
        true,
        Some(
          "import your reason code with "
          ++ highlightColor("require('" ++ name ++ "');")
        )
      ) :
      (false, Some("something went wrong when creating the symlink"));
  };
};

let createBuildCommand = (name, source, root) : (bool, option(string)) => {
  paint("preparing to add a build command");
  let packagePath = Path.combinePaths([root, "package.json"]);
  /* We use relative paths here specifically for the postinstall script */
  let source =
    Path.combinePaths(["..", "lib", "js", source], ~useLeadingSlash=false);
  let dest =
    Path.combinePaths([".", "node_modules", name], ~useLeadingSlash=false);
  (
    appendToPackageScripts("build-reason", packagePath, "bsb -make-world"),
    Some("Added a " ++ highlightColor("build-reason") ++ " command")
  );
};

let createPostinstallCommand = (name, source, root) : (bool, option(string)) => {
  paint("preparing to add a postinstall script");
  let packagePath = Path.combinePaths([root, "package.json"]);
  /* We use relative paths here specifically for the postinstall script */
  let source =
    Path.combinePaths(["..", "lib", "js", source], ~useLeadingSlash=false);
  let dest =
    Path.combinePaths([".", "node_modules", name], ~useLeadingSlash=false);
  let command =
    Printf.sprintf(
      "node -e \"var s='%s',d='%s',fs=require('fs');if(fs.existsSync(d)===false){fs.symlinkSync(s,d,'dir')};\"",
      source,
      dest
    );
  paint("adding the postinstall script to your package file");
  (
    appendToPackageScripts("postinstall", packagePath, command),
    Some("Added a " ++ highlightColor("postinstall") ++ " command")
  );
};

let main = (name, source, root, version) : unit => {
  Printf.sprintf("add-reason link v%s\n", version) |> white |> bold |> stdout;
  let stepsAsFunctions = [
    performLinking,
    createBuildCommand,
    createPostinstallCommand
  ];
  let (finishWithFailure, comments) =
    Utils.execute(stepsAsFunctions, name, source, root);
  finishWithFailure ? success() : failure();
  Utils.printList(comments);
};

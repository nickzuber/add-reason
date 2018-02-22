open Utils;

open Bindings;

/**
 * Ensure that the source directory (where the ReasonML code is) actually exists, and
 * we attempt to create the target directory too. If it already exists, then nothing
 * happens with it.
 */
let prepareTargetDirectory = (name, source, root) : (bool, option(string)) => {
  paint("looking for source directory");
  let sourcePath = Path.combinePaths([root, source]);
  let outputPath = Path.combinePaths([root, "lib", "js", source]);
  if (Fs.safeFileExists(sourcePath)) {
    paint("creating target directory");
    (Fs.safeCreateDirectory(outputPath), None);
  } else {
    (
      false,
      Some(
        "couldn't find " ++ highlightColor(source) ++ ", do you have a typo?"
      )
    );
  };
};

/**
 * Check to see if `bs-platform` has been installed locally or linked.
 */
let checkForBucklescript = (name, source, root) : (bool, option(string)) => {
  paint("checking bs-platform has been linked or installed");
  let bsPlatformPath =
    Path.combinePaths([root, "node_modules", "bs-platform"]);
  if (Fs.safeFileExists(bsPlatformPath)) {
    (true, None);
  } else {
    (
      /* Using line breaks here is generally a bad idea, but since we know this
         is always going to be the last message that is displayed, it should be fine visually. */
      true,
      Some(
        redBright("WARNING ")
        ++ "couldn't find "
        ++ magenta("bs-platform")
        ++ ", make sure you it's installed globally & linked with "
        ++ magenta("npm link bs-platform")
      )
    );
  };
};

let main = (name, source, root, version, linking) : unit => {
  Printf.sprintf("add-reason setup v%s\n", version) |> white |> bold |> stdout;
  let stepsAsFunctions =
    if (linking) {
      [
        prepareTargetDirectory,
        Config.createBuildingConfig,
        Linter.createLintingConfig,
        Link.performLinking,
        Link.createBuildCommand,
        Link.createPostinstallCommand,
        checkForBucklescript
      ];
    } else {
      [
        prepareTargetDirectory,
        Config.createBuildingConfig,
        Linter.createLintingConfig,
        Link.createBuildCommand,
        checkForBucklescript
      ];
    };
  let (finishWithFailure, comments) =
    Utils.execute(stepsAsFunctions, name, source, root);
  finishWithFailure ? success() : failure();
  Utils.printList(comments);
};

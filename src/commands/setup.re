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

let main = (name, source, root, version, linking) : unit => {
  Printf.sprintf("add-reason setup v%s\n", version)
    |> white
    |> bold
    |> stdout;
  let stepsAsFunctions = if (linking) {
    [ prepareTargetDirectory,
      Config.createBuildingConfig,
      Linter.createLintingConfig,
      Link.performLinking,
      Link.createPostinstall ]
  } else {
    [ prepareTargetDirectory,
      Config.createBuildingConfig,
      Linter.createLintingConfig ]
  };
  let finishWithFailure = Utils.execute(stepsAsFunctions, name, source, root);
  finishWithFailure ? success() : ();
  stdout("\n")
}

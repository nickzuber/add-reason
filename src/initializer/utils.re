
open GenericBindings;

[@bs.val] [@bs.module "fs"]
external createSymlink : (string, string) => unit = "symlinkSync";

[@bs.val] [@bs.module "fs"]
external throwIfFileDNE : string => bool = "lstatSync";

[@bs.val] [@bs.module "fs"]
external createAndWriteToFile : (string, string) => unit = "writeFileSync";

[@bs.val] [@bs.module "fs-extra"]
external createDirectory : (string) => unit = "ensureDirSync";

[@bs.val] [@bs.module "path"]
external sep : string = "";

module Path {
  /**
   * Removes any leading and trailing slashes on the input path string.
   * If you want to add a leading or trailing slash, do so manually after
   * running this function.
   */
  let cleanPath = (path) : string => {
    let size = String.length(path);
    let first = String.sub(path, 0, 1);
    let last = String.sub(path, size - 1, 1);
    let starting = if (first == sep) { 1; } else { 0; };
    let ending = if (last == sep) { size - starting - 1; } else { size - starting; };
    String.sub(path, starting, ending)
  };
  
  let combinePaths = (~useLeadingSlash=true, paths) : string => {
    let prefix = useLeadingSlash ? sep : "";
    let rec combine = (paths, result) => {
      switch paths {
        | [path] => result ++ cleanPath(path);
        | [path, ...rest] =>
          let result = result ++ cleanPath(path) ++ sep;
          combine(rest, result);
        | _ => result
      };
    };
    prefix ++ combine(paths, "");
  };

  let removeTopFromPath = (path) : string => {
    let path = sep ++ cleanPath(path);
    let pos = ref(String.length(path) - 1);
    let finalPath = ref(path);
    while (pos^ >= 0) {
      let cur = String.sub(path, pos^, 1);
      if (cur == sep) {
        finalPath := String.sub(path, 0, pos^);
        pos := -1
      } else {
        pos := pos^ - 1
      }
    };
    finalPath^
  };
};


module FsPolyfill {
  let safeFileExists = (file) : bool => {
    try { let _ = throwIfFileDNE(file); true
    } { | _ => false }
  };

  let safeCreateDirectory = (dir) : bool => {
    try { let _ = createDirectory(dir); true
    } { | _ => false }
  };

  let attemptToLink = (position, source, dest) : bool => {
    let (index, total) = position;
    let position = Printf.sprintf("[%d/%d] ", index, total) |> gray;
    let targetDirectory = dest |> Path.removeTopFromPath;
    
    /** Should exist. This needs to be true since we can/will have problems otherwise
      * Also note that this shouldn't ever really be false since the step for configuring this
      * source happens before this step. This being false means something went wrong in that previous
      * configuration step. */
    let existsSource = safeFileExists(source);
    /* Should not exist. It can be false, just show a warning */
    let existsDest = safeFileExists(dest);
    /* Should exist. Needs to tbe true */
    let isValidPathDest = targetDirectory |> safeFileExists;

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
      let existsConfig = safeFileExists(filename);
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
      let existsMerlin = safeFileExists(filename);
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
};

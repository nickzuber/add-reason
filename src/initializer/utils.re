
open GenericBindings;

[@bs.val] [@bs.module "fs"]
external createSymlink : (string, string) => unit = "symlinkSync";

[@bs.val] [@bs.module "fs"]
external throwIfFileDNE : string => bool = "lstatSync";

[@bs.val] [@bs.module "fs"]
external createAndWriteToFile : (string, string) => unit = "writeFileSync";


module Path {
  /**
   * Removes any leading and trailing slashes on the input path string and 
   * adds a leading slash before the cleaned path is returned. You can opt-out
   * of this leading slash if you want to. Doing that could be useful for when 
   * you want to have a relative path.
   */
  let cleanPath = (path) : string => {
    let size = String.length(path);
    let first = String.sub(path, 0, 1);
    let last = String.sub(path, size - 1, 1);
    let starting = if (first == "/") { 1; } else { 0; };
    let ending = if (last == "/") { size - starting - 1; } else { size - starting; };
    String.sub(path, starting, ending)
  };
  
  let combinePaths = (~useLeadingSlash=true, paths) : string => {
    let prefix = useLeadingSlash ? "/" : "";
    let rec combine = (paths, result) => {
      switch paths {
        | [path] => result ++ cleanPath(path);
        | [path, ...rest] =>
          let result = result ++ cleanPath(path) ++ "/";
          combine(rest, result);
        | _ => result
      };
    };
    prefix ++ combine(paths, "");
  };

  let removeTopFromPath = (path) : string => {
    let path = "/" ++ cleanPath(path);
    let pos = ref(String.length(path) - 1);
    let finalPath = ref(path);
    while (pos^ >= 0) {
      let cur = String.sub(path, pos^, 1);
      if (cur == "/") {
        finalPath := String.sub(path, 0, pos^);
        pos := -1
      } else {
        pos := pos^ - 1
      }
    };
    finalPath^
  };
};


module Fs_Polyfill {
  let safeFileExists = (file) : bool => {
    try { let _ = throwIfFileDNE(file); true
    } { | _ => false }
  };

  let attemptToLink = (position, source, dest) : bool => {
    let (index, total) = position;
    let position = Printf.sprintf("[%d/%d] ", index, total) |> gray;
    let targetDirectory = dest |> Path.removeTopFromPath;
    
    /* Should exist */
    let existsSource = safeFileExists(source);
    /* Should not exist */
    let existsDest = safeFileExists(dest);
    /* Should exist */
    let isValidPathDest = targetDirectory |> safeFileExists;

    let prefix = position ++ getEmoji("link") ++ "Linking package... ";
    let success = switch (existsSource, existsDest, isValidPathDest) {
      | (_, _, false) =>
      Printf.sprintf("%s%s Couldn't find your target's directory. Do you have a typo?\n%s%s",
          prefix, red("failed"), altCodeDirectional, bold(targetDirectory)) 
          |> Js.log;
        false
      | (true, false, _) =>
        createSymlink(source, dest);
        Printf.sprintf("%s%s", prefix, green("success")) 
          |> Js.log;
        true
      | (true, true, _) =>
        Printf.sprintf("%s%s There's already a file here. Did you mean to `unlink`?\n%s%s",
          prefix, red("failed"), altCodeDirectional, bold(dest)) 
          |> Js.log;
        false
      | (false, false, _) =>
        Printf.sprintf("%s%s Couldn't find your source directory. Do you have a typo?\n%s%s",
          prefix, red("failed"), altCodeDirectional, bold(source)) 
          |> Js.log;
        false
      | (false, true, _) =>
        Printf.sprintf("%s%s Couldn't find your source directory and there was a file already in your destination. Do you have any typos?\n%s%s\n%s%s",
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
      let prefix = position ++ getEmoji("pencil2") ++ "Creating config... ";

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
};

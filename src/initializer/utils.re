
open GenericBindings;

[@bs.val] [@bs.module "fs"]
external createSymlink : (string, string) => unit = "symlinkSync";

[@bs.val] [@bs.module "fs"]
external throwIfFileDNE : string => bool = "lstatSync";

module Fs_Polyfill {
  let safeFileExists = (file) : bool => {
    try { let _ = throwIfFileDNE(file); true
    } { | _ => false }
  };

  let attemptToLink = (position, source, dest) : bool => {
    let (index, total) = position;
    let position = Printf.sprintf("[%d/%d] ", index, total) |> gray;
    let existsSource = safeFileExists(source);  /* Should exist */
    let existsDest = safeFileExists(dest);  /* Should not exist */
    let prefix = position ++ getEmoji("link") ++ "Linking package... ";
    let success = switch (existsSource, existsDest) {
      | (true, false) =>
        createSymlink(source, dest);
        Printf.sprintf("%s%s",prefix, green("success")) 
          |> Js.log;
        true
      | (true, true) =>
        Printf.sprintf("%s%s There's already a file here. Did you mean to `unlink`?\n%s%s",
          prefix, red("failed"), altCodeDirectional, bold(dest)) 
          |> Js.log;
        false
      | (false, true) =>
        Printf.sprintf("%s%s Couldn't find your source directory. Do you have a typo?\n%s%s",
          prefix, red("failed"), altCodeDirectional, bold(source)) 
          |> Js.log;
        false
      | (false, false) =>
        Printf.sprintf("%s%s Couldn't find either directory. Do you have any typos?\n%s%s\n%s%s",
          prefix, red("failed"), altCodeDirectionalNonTerminal, bold(source), altCodeDirectional, bold(dest))
          |> Js.log;
        false
    };
    success;
  };
};

module Path {
  let cleanPath = (path) : string => {
    let size = String.length(path);
    let first = String.sub(path, 0, 1);
    let last = String.sub(path, size - 1, 1);
    let starting = if (first == "/") { 1; } else { 0; };
    let ending = if (last == "/") { size - starting - 1; } else { size - starting; };
    String.sub(path, starting, ending);
  };
  
  let combinePaths = (paths) : string => {
    let rec combine = (paths, result) => {
      switch paths {
        | [path] => result ++ "/" ++ cleanPath(path);
        | [path, ...rest] => 
          let result = result ++ "/" ++ cleanPath(path);
          combine(rest, result);
        | _ => result
      };
    };
    combine(paths, "");
  };
};




open GenericBindings;

[@bs.val] [@bs.module "fs"]
external createSymlink : (string, string) => unit = "symlinkSync";

[@bs.val] [@bs.module "fs"]
external throwIfFileDNE : string => bool = "lstatSync";

let safeFileExists = (file) => {
  try {
    let stats = throwIfFileDNE(file);
    true;
  } {
    | _ => false
  }
};

let attemptToLink = (source, dest) => {
  let existsSource = safeFileExists(source);  /* Should exist */
  let existsDest = safeFileExists(dest);  /* Should not exist */
  let success = switch (existsSource, existsDest) {
    | (true, false) => 
      /* Debug  Js.log(printGreen("ln") ++ " -sv " ++ source ++ " " ++ dest); */
      createSymlink(source, dest);
      Js.log(getEmoji("paperclip") ++ printGreen("successful link"));
      true;
    | (true, true) => 
      Js.log(getEmoji("paperclip") ++ printRed("failed link") ++ " there's already a file here. Did you mean to `unlink`?");
      Js.log("\t" ++ dest);
      false;
    | (false, true) =>
      Js.log(getEmoji("paperclip") ++ printRed("failed link") ++ " couldn't find your source directory. Do you have a typo?");
      Js.log("\t" ++ source);
      false;
    | (false, false) => 
      Js.log(getEmoji("paperclip") ++ printRed("failed link") ++ " couldn't find either directory. Do you have typos?");
      Js.log("\t" ++ source);
      Js.log("\t" ++ dest);
      false;
  };
  success;
};

let cleanPath = (path) => {
  let size = String.length(path);
  let first = String.sub(path, 0, 1);
  let last = String.sub(path, size - 1, 1);
  let starting = if (first == "/") { 1; } else { 0; };
  let ending = if (last == "/") { size - starting - 1; } else { size - starting; };
  String.sub(path, starting, ending);
};

let combinePaths = (paths) => {
  let rec combine = (paths, result) => {
    switch paths {
      | [path, ...rest] => 
        let result = result ++ "/" ++ cleanPath(path);
        combine(rest, result);
      | [path] => result ++ "/" ++ cleanPath(path);
      | _ => result
    };
  };
  combine(paths, "");
};
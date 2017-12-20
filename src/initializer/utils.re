
open GenericBindings;

[@bs.val] [@bs.module "fs"]
external createSymlink : (string, string) => unit = "symlinkSync";

[@bs.val] [@bs.module "fs"]
external validSymlinkExists : string => bool = "existsSync";

let attemptToLink = (source, dest) => {
  Js.log("CHECKING: " ++ dest);
  let exists = validSymlinkExists(dest);
  Js.log("EXISTS:   " ++ string_of_bool(exists));
  if (exists) {
    let msg = getEmoji("paperclip") ++ printRed("failed link") ++ " there's already a symlink here. Do you mean to `unlink`?";
    Js.log(msg);
    false;
  } else {
    createSymlink(source, dest);
    /* Debug */ Js.log(printGreen("ln") ++ " -sv " ++ source ++ " " ++ dest);
    let msg = getEmoji("paperclip") ++ printGreen("successful link");
    Js.log(msg);
    true;
  };
};

let cleanPath = (path) => {
  let size = String.length(path);
  let first = String.sub(path, 0, 1);
  let last = String.sub(path, size - 1, 1);
  let starting = if (first == "/") { 1; } else { 0; };
  let ending = if (last == "/") { size - starting - 1; } else { size - starting; };
  String.sub(path, starting, ending);
};

let combinePaths = (beginning, ending) => {
  let validBeginning = cleanPath(beginning);
  let validEndling = cleanPath(ending);
  validBeginning ++ "/" ++ validEndling;
};
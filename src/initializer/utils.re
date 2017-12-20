
open GenericBindings;

[@bs.val] [@bs.module "fs"]
external symlink : (string, string, (option(string) => unit)) => unit = "symlink";

let responseMessage = (err) => {
  let msg = switch err {
  | Some(e) => getEmoji("no_entry_sign") ++ "  " ++ printRed("failed link, " ++ e)
  | None => getEmoji("paperclip") ++ "  " ++ printGreen("successful link")
  };
  Js.log(msg);
};

let createSymlink = (source, dest) => {
  let source = "/" ++ source;
  let dest = "/" ++ dest;
  symlink(source, dest, (err) => {
    responseMessage(err)
  });
  Js.log(printGreen("ln") ++ " -sv " ++ source ++ " " ++ dest)
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
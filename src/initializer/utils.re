
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
  /** Removes any leading and trailing slashes on the input path string.
   *  If you want to add a leading or trailing slash, do so manually after
   *  running this function. */
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
};

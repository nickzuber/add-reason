open Bindings;

let execute = (steps, name, source, root) : (bool, list(string)) => {
  let rec process = (steps, feedback) => {
    switch (steps) {
      | [] => (true, feedback);
      | [step] => switch (step(name, source, root)) {
          | (success, Some(response)) => (success, [response, ...feedback]);
          | (success, None) => (success, feedback);
        }
      | [step, ...rest] => switch (step(name, source, root)) {
          | (true, Some(response)) => process(rest, [response, ...feedback]);
          | (true, None) => process(rest, feedback);
          | (false, Some(response)) => (false, [response, ...feedback]);
          | (false, None) => (false, feedback);
        }
    }
  };
  process(steps, []);
};

let isReasonFile = str : bool => {
  let pos = String.length(str);
  if (pos > 3) {
    String.sub(str, pos - 3, 3) == ".re";
  } else {
    false;
  }
};


let safeExists = (tbl, key) => {
  try { Hashtbl.find(tbl, key); true }
  { | _ => false }
};

let rec printList = messages : unit => {
  switch (messages) {
    | [] => stdout("\n");
    | [message] => stdout("\n" ++ barEnd ++ message ++ "\n\n");
    | [message, ...rest] => stdout("\n" ++ bar ++ message);
      printList(rest);
  }
};

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


module Fs {
  let safeFileExists = (file) : bool => {
    try { let _ = throwIfFileDNE(file); true
    } { | _ => false }
  };

  let safeCreateDirectory = (dir) : bool => {
    try { let _ = createDirectory(dir); true
    } { | _ => false }
  };

  let safeCreateFile = (file, contents) : bool => {
    try { let _ = createAndWriteToFile(file, contents); true
    } { | _ => false }
  };

  let safeCreateSymlink = (source, dest) : bool => {
    try { let _ = createSymlink(source, dest); true
    } { | _ => false }
  };
};

let deriveSourceDirectory = root => {
  let memo = Hashtbl.create(42);
  let rec traverse = node : int => {
    stdout(node ++ "\n");
    let isTargetFile = isReasonFile(node);
    let isDirectory = safeIsDirectory(node);
    let alreadySeen = safeExists(memo, node);
    switch (isTargetFile, isDirectory, alreadySeen) {
      | (_, _, true) => 0;
      | (true, _, _) => 1;
      | (_, false, _) => 0;
      | (_, true, _) => 0;
        /* List.fold_right((child, acc) => {
          /* let childScore = traverse(Path.combinePaths([root, child]));
          childScore + acc; */
          stdout(child ++ "\n");
          1
        }, readDir(node), 0); */
    }
  };
  /* traverse(root); */
  let lst = ["test", "testtt", "mu"];
  let lst2 = readDir(root);
  List.map((child) => {Js.log(child)}, lst2);
  1
};

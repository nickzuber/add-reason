// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var Fs                      = require("fs");
var List                    = require("bs-platform/lib/js/list.js");
var Path                    = require("path");
var Curry                   = require("bs-platform/lib/js/curry.js");
var Chalk                   = require("chalk");
var $$String                = require("bs-platform/lib/js/string.js");
var FsExtra                 = require("fs-extra");
var Bindings$ReasonTemplate = require("./bindings.bs.js");

function highlightColor(prim) {
  return Chalk.bold(prim);
}

function execute(steps, name, source, root) {
  var _steps = steps;
  var _feedback = /* [] */0;
  while(true) {
    var feedback = _feedback;
    var steps$1 = _steps;
    if (steps$1) {
      var rest = steps$1[1];
      var step = steps$1[0];
      if (rest) {
        var match = Curry._3(step, name, source, root);
        if (match[0] !== 0) {
          var match$1 = match[1];
          if (match$1) {
            _feedback = /* :: */[
              match$1[0],
              feedback
            ];
            _steps = rest;
            continue ;
            
          } else {
            _steps = rest;
            continue ;
            
          }
        } else {
          var match$2 = match[1];
          if (match$2) {
            return /* tuple */[
                    /* false */0,
                    /* :: */[
                      match$2[0],
                      feedback
                    ]
                  ];
          } else {
            return /* tuple */[
                    /* false */0,
                    feedback
                  ];
          }
        }
      } else {
        var match$3 = Curry._3(step, name, source, root);
        var match$4 = match$3[1];
        var success = match$3[0];
        if (match$4) {
          return /* tuple */[
                  success,
                  /* :: */[
                    match$4[0],
                    feedback
                  ]
                ];
        } else {
          return /* tuple */[
                  success,
                  feedback
                ];
        }
      }
    } else {
      return /* tuple */[
              /* true */1,
              feedback
            ];
    }
  };
}

function printList(messages) {
  var revMessages = List.rev(messages);
  var _messages = revMessages;
  while(true) {
    var messages$1 = _messages;
    if (messages$1) {
      var rest = messages$1[1];
      var message = messages$1[0];
      if (rest) {
        process.stdout.write("\n" + (Bindings$ReasonTemplate.bar + message));
        _messages = rest;
        continue ;
        
      } else {
        process.stdout.write("\n" + (Bindings$ReasonTemplate.barEnd + (message + "\n\n")));
        return /* () */0;
      }
    } else {
      process.stdout.write("\n");
      return /* () */0;
    }
  };
}

function cleanPath(path) {
  var size = path.length;
  var first = $$String.sub(path, 0, 1);
  var last = $$String.sub(path, size - 1 | 0, 1);
  var starting = first === Path.sep ? 1 : 0;
  var ending = last === Path.sep ? (size - starting | 0) - 1 | 0 : size - starting | 0;
  return $$String.sub(path, starting, ending);
}

function combinePaths($staropt$star, paths) {
  var useLeadingSlash = $staropt$star ? $staropt$star[0] : /* true */1;
  var prefix = useLeadingSlash !== 0 ? Path.sep : "";
  var combine = function (_paths, _result) {
    while(true) {
      var result = _result;
      var paths = _paths;
      if (paths) {
        var rest = paths[1];
        var path = paths[0];
        if (rest) {
          var result$1 = result + (cleanPath(path) + Path.sep);
          _result = result$1;
          _paths = rest;
          continue ;
          
        } else {
          return result + cleanPath(path);
        }
      } else {
        return result;
      }
    };
  };
  return prefix + combine(paths, "");
}

function removeTopFromPath(path) {
  var path$1 = Path.sep + cleanPath(path);
  var pos = path$1.length - 1 | 0;
  var finalPath = path$1;
  while(pos >= 0) {
    var cur = $$String.sub(path$1, pos, 1);
    if (cur === Path.sep) {
      finalPath = $$String.sub(path$1, 0, pos);
      pos = -1;
    } else {
      pos = pos - 1 | 0;
    }
  };
  return finalPath;
}

var Path$1 = /* module */[
  /* cleanPath */cleanPath,
  /* combinePaths */combinePaths,
  /* removeTopFromPath */removeTopFromPath
];

function safeFileExists(file) {
  try {
    +Fs.lstatSync(file);
    return /* true */1;
  }
  catch (exn){
    return /* false */0;
  }
}

function safeCreateDirectory(dir) {
  try {
    FsExtra.ensureDirSync(dir);
    return /* true */1;
  }
  catch (exn){
    return /* false */0;
  }
}

function safeCreateFile(file, contents) {
  try {
    Fs.writeFileSync(file, contents);
    return /* true */1;
  }
  catch (exn){
    return /* false */0;
  }
}

function safeCreateSymlink(source, dest) {
  try {
    Fs.symlinkSync(source, dest);
    return /* true */1;
  }
  catch (exn){
    return /* false */0;
  }
}

var Fs$1 = /* module */[
  /* safeFileExists */safeFileExists,
  /* safeCreateDirectory */safeCreateDirectory,
  /* safeCreateFile */safeCreateFile,
  /* safeCreateSymlink */safeCreateSymlink
];

exports.highlightColor = highlightColor;
exports.execute        = execute;
exports.printList      = printList;
exports.Path           = Path$1;
exports.Fs             = Fs$1;
/* fs Not a pure module */

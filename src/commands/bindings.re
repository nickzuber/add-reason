
/** General */
[@bs.val] [@bs.module "chalk"]
external white : string => string = "";

[@bs.val] [@bs.module "chalk"]
external green : string => string = "";

[@bs.val] [@bs.module "chalk"]
external red : string => string = "";

[@bs.val] [@bs.module "chalk"]
external gray : string => string = "";

[@bs.val] [@bs.module "chalk"]
external yellow : string => string = "";

[@bs.val] [@bs.module "chalk"]
external blue : string => string = "";

[@bs.val] [@bs.module "chalk"]
external cyan : string => string = "";

[@bs.val] [@bs.module "chalk"]
external magenta : string => string = "";

[@bs.val] [@bs.module "chalk"]
external bold : string => string = "";

[@bs.val] [@bs.module "chalk"]
external yellowBright : string => string = "";

[@bs.val] [@bs.module "chalk"]
external blueBright : string => string = "";

[@bs.val] [@bs.module "node-emoji"]
external getEmojiNative : string => string = "get";

let emoji = (emoji) => getEmojiNative(emoji) ++ "  ";
let altCodeDirectionalNonTerminal = [%bs.raw{| "├─ " |}];
let altCodeDirectional = [%bs.raw{| "└─ " |}];
let altLong = [%bs.raw{| "│" |}];

[@bs.module]
external camelCase : string => string = "lodash.camelcase";

/** Internal patches */
[@bs.val] [@bs.module "../../../../src/patches"]
external appendToPackageScripts : (string, string) => bool = "editPackageScripts";

[@bs.val] [@bs.module "../../../../src/patches"]
external generateConfigContents : (string, string) => string = "";

[@bs.val] [@bs.module "../../../../src/patches"]
external generateMerlinContents : (string, string) => string = "";

[@bs.val] [@bs.module "../../../../src/patches"]
external flush : unit => unit = "";

/** Processes and Globals */
[@bs.val]
external stdout : string => unit = "process.stdout.write";

[@bs.val] [@bs.scope "Date"]
external now : unit => int = "";

/**
 * This is incredibly hacky, and I apologize to anyone who looks upon this function. I am not
 * held liable for any loss or damage of eye sight. Hijack the CPU for loading icon swag.
 */
let sleep = ms => {
  let start = now();
  let cur = ref(now());
  while (cur^ - start < ms) {
    cur := now();
  };
};

[@bs.val] [@bs.scope "Math"]
external random : unit => float = "";

[@bs.val] [@bs.scope "Math"]
external floor : float => int = "";

let paint = (~useLoader=true, arg) => {
  let parts = [|
    {j|⠋|j},
    {j|⠙|j},
    {j|⠹|j},
    {j|⠸|j},
    {j|⠼|j},
    {j|⠴|j},
    {j|⠦|j},
    {j|⠧|j},
    {j|⠇|j},
    {j|⠏|j},
  |];
  for (i in 0 to Array.length(parts) - 1) {
    sleep(20);
    flush();
    stdout(parts[i] ++ " " ++ arg);
  };
};
let success = (arg) => {
  flush();
  stdout(emoji("sparkles") ++ green("done"));
};
let failure = (arg) => {
  flush();
  stdout(red("failed: ") ++ arg);
};

/** Path and Fs */
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

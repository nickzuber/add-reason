
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

let getEmoji = (emoji) => getEmojiNative(emoji) ++ "  ";
let altCodeDirectionalNonTerminal = [%bs.raw{| "├─ " |}];
let altCodeDirectional = [%bs.raw{| "└─ " |}];
let altLong = [%bs.raw{| "│" |}];
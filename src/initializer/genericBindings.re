
[@bs.val] [@bs.module "chalk"]
external green : string => string = "green";

[@bs.val] [@bs.module "chalk"]
external red : string => string = "red";

[@bs.val] [@bs.module "chalk"]
external white : string => string = "white";

[@bs.val] [@bs.module "chalk"]
external gray : string => string = "gray";

[@bs.val] [@bs.module "chalk"]
external yellow : string => string = "yellow";

[@bs.val] [@bs.module "chalk"]
external bold : string => string = "bold";

[@bs.val] [@bs.module "node-emoji"]
external getEmojiNative : string => string = "get";

let getEmoji = (emoji) => getEmojiNative(emoji) ++ "  ";
let altCodeDirectionalNonTerminal = [%bs.raw{| "├─ " |}];
let altCodeDirectional = [%bs.raw{| "└─ " |}];

[@bs.val] [@bs.module "chalk"]
external printGreen : string => string = "green";

[@bs.val] [@bs.module "chalk"]
external printRed : string => string = "red";

[@bs.val] [@bs.module "node-emoji"]
external getEmojiNative : string => string = "get";

/** Patch with padding */
let getEmoji = (emoji) => getEmojiNative(emoji) ++ "  ";
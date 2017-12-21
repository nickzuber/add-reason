
/* Symlink:
  ln -sv /Users/nick/projects/add-reason/lib/js/src/initializer /Users/nick/projects/add-reason/node_modules/initializer 
  ln -sv "$(pwd)/lib/js/src/initializer" "$(pwd)/node_modules/<ALIAS>"
                directory   ^^^^^^^^^^^         target
                            we need this
*/
open Utils;
open GenericBindings;

let getReasonSource = () => {
  "/lib/js/src/"
};

let complete = (success, name) => {
  let msg = switch success {
    | true => getEmoji("sparkles") ++ printGreen("done") ++ " import your compiled code as `" ++ name ++ "`";
    | false => getEmoji("no_entry_sign") ++ printRed("failure") ++ " see which step went wrong";
  };
  Js.log(msg);
};

let performConfigCreation = (name, directory, rootDirectory) => {
  /* let success = attemptToCreateConfig */
  Js.log(getEmoji("page_with_curl") ++ printGreen("created config") ++ " ");
};

let performLinking = (name, directory, rootDirectory) => {
  let linkToNodeModules = combinePaths([rootDirectory, "node_modules"]);
  let source = combinePaths([rootDirectory, "lib/js", directory]);
  let dest = combinePaths([linkToNodeModules, name]);
  let success = attemptToLink(source, dest);
  success;
};

let handle = (name, directory, rootDirectory) => {
  let configSuccess = performConfigCreation(name, directory, rootDirectory);
  let linkingSuccess = performLinking(name, directory, rootDirectory);
  complete(linkingSuccess, name);
};

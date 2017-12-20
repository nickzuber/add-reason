
/* Symlink:
  ln -sv /Users/nick/projects/add-reason/lib/js/src/initializer /Users/nick/projects/add-reason/node_modules/initializer 
  ln -sv "$(pwd)/lib/js/src/initializer" "$(pwd)/node_modules/<ALIAS>"
                directory   ^^^^^^^^^^^         target
                            do we need this
*/
open Utils;
open GenericBindings;

let getReasonSource = () => {
  "/lib/js/src/"
};

let handle = (name, directory, rootDirectory) => {
  let linkToNodeModules = combinePaths(rootDirectory, "node_modules");
  let source = combinePaths(rootDirectory, directory);
  let dest = combinePaths(linkToNodeModules, name);
  createSymlink(source, dest)
};

let complete = () => {
  getEmoji("sparkles") ++ printGreen("done") ++ " in 0.52s";
};

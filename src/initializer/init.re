
/* Symlink:
  ln -sv /Users/nick/projects/add-reasoninitializer /Users/nick/projects/add-reason/node_modules//lib/js/src/initializer
  ln -sv "$(pwd)/lib/js/src/<ALIAS>" "$(pwd)/node_modules/<ALIAS>"
                directory                       target
*/
let handle = (alias, directory) => {
  Js.log("handle " ++ alias ++ " -> " ++ directory);
  Utils.create_symlink(alias, directory);
};


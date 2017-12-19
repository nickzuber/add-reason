
/* Symlink:
  ln -sv /Users/nick/projects/add-reasoninitializer /Users/nick/projects/add-reason/node_modules//lib/js/src/initializer
  ln -sv "$(pwd)/lib/js/src/<ALIAS>" "$(pwd)/node_modules/<ALIAS>"
                directory                       target
*/

/**
 * Handle the init command by delegating what to do with the 
 * alias and the directory arguments.
 */
let handle = (alias, directory) => {
  Js.log("handle " ++ alias ++ " -> " ++ directory);
  Utils.create_symlink(alias, directory);
};

/**
 * Reconstruct a directory and a file to correctly append
 * the file to the end of the directory, forming a valid path.
 */
let createPath = (directory, file) => {
  /* split directory by slash */
  /* add file to end of directory list */
  /* rejoin new directory list with a slash */
  /*   ^-- backslash if windows? */
};
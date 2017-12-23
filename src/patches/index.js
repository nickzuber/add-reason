'use strict';

const fs = require('fs');
const jsonfile = require('jsonfile');
const jsonFormat = require('json-format');

/**
 * Takes a commander instance and patches help output to remove and adjust
 * some of the weird spacing.
 * @param commander The commander instance.
 */
const OutputPatcher = (commander) => {
  const originalHelpOutput = commander.helpInformation();
  const newNativeHelpMethod = function() {
    let newOutput = originalHelpOutput.replace(/\n  Commands/gmi, '  Commands');
    return newOutput;
  }
  commander.helpInformation = newNativeHelpMethod;
};

/**
 * @note Everything needs to be sync because of the ordering of the output.
 * @param file The absolute path to the `package.json` file.
 * @param command The command to add to the `scripts` key.
 * @return The success of the operations.
 */
const editPackageScripts = (file, command, callback) => {
  const config = {
    type: 'space',
    size: 2
  };
  try {
    const contents = jsonfile.readFileSync(file);
    // No existing scripts
    if (!contents.scripts) {
      contents.scripts = {
        postinstall: command
      };
    }
    // Already are some scripts
    else {
      // Already is a postinstall script
      if (contents.scripts.postinstall) {
        // Make sure we don't add this command more than once if it already exists
        if (contents.scripts.postinstall.indexOf(command) > -1) return true;
        command = `${contents.scripts.postinstall} && ${command}`;
      }
      contents.scripts.postinstall = command;
    }
    fs.writeFileSync(file, jsonFormat(contents, config));
    return true;
  } catch (e) {
    return false;
  }
};

module.exports.OutputPatcher = OutputPatcher;
module.exports.editPackageScripts = editPackageScripts;

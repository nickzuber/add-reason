'use strict';

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

module.exports.OutputPatcher = OutputPatcher;

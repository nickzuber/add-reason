'use strict';

const initializer = require('initializer');
const handleInit = initializer.handleInit();

function init (alias, directory, rootDirectory) {
  handleInit(alias, directory, rootDirectory);
}

module.exports = init;
'use strict';

const initializer = require('initializer');
const handleInit = initializer.handleInit();

function init (alias, directory, rootDirectory, version) {
  handleInit(alias, directory, rootDirectory, version);
}

module.exports = init;

'use strict';

const initializer = require('initializer');
const handleInit = initializer.handleInit();

function init (alias, directory) {
  const res = handleInit(alias, directory);
  console.log(res);
  console.log(__dirname);
}

module.exports = init;
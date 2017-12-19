'use strict';

const initializer = require('initializer');
const commands = require('../src/commands');

const handleInit = initializer.handleInit();

console.log('');
console.log(`Initializer: ${initializer}`);
console.log(handleInit('alias', '/directory/path'));

commands.init('reason-package', 'some/directory/path');

console.log('');

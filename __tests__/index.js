'use strict';

const path = require('path');
const initializer = require('initializer');
const chalk = require('chalk');
const commands = require('../src/commands');
const handleInit = initializer.handleInit();
const rootDirectory = path.join(__dirname, 'mockRoot');
const version = require('../src/config').VERSION;

const name = 'reason-package';
const directory = 'src/myCode';
console.log(`$> ${chalk.green('add-reason')} init ${name} ${directory}`)
commands.init(name, directory, rootDirectory, version);
// require('./mockRoot/index');


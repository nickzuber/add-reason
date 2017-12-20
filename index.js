#!/usr/bin/env node

'use strict';

const path = require('path');
const program = require('commander');
const commands = require('./src/commands');
const initializer = require('initializer');
const OutputPatcher = require('./src/patches').OutputPatcher;
const rootDirectory = path.join(__dirname);

program.version('1.0.0')
  .usage('add-reason [command] [options]')
  .option('--no-emojis', 'disable the emojis in the output');

program.command('init <package-name> <directory>')
  .description('set up Reason directory, config files, and symlink')
  .action((name, directory) => {
    console.log(rootDirectory);
    commands.init(name, directory, "root")
  });

 program.command('link <package-name> <directory>')
  .description('create a symlink with the given package name')
  .action((name, directory) => {});

 program.command('rename <new-alias>')
  .description('change the current Reason package name')
  .action((alias) => {});

program.command('unlink <package-name>')
  .description('removes the given symlink')
  .action((name) => {});
  
OutputPatcher(program);

// Set default command to --help
if (!process.argv.slice(2).length) {
  program.outputHelp();
}

program.parse(process.argv);
 
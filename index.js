#!/usr/bin/env node

'use strict';

const sade = require('sade');
const initializer = require('initializer');
const commands = require('./src/commands');
const handleInit = initializer.handleInit();

const program = sade('add-reason');

program.version('1.0.0')
  .option('--no-emojis', 'disable the emojis in the output')

program.command('init <package-name> <directory>')
  .describe('set up Reason directory, config files, and symlink')
  .action((name, directory) => commands.init(name, directory));

 program.command('link <package-name> <directory>')
  .describe('create a symlink with the given package name')
  .action((name, directory) => {});

 program.command('rename <new-alias>')
  .describe('change the current Reason package name')
  .action((alias) => {});

program.command('unlink <package-name>')
  .describe('removes the given symlink')
  .action((name) => {});

program.parse(process.argv);
 
#!/usr/bin/env node

'use strict';

const path = require('path');
const program = require('commander');
const chalk = require('chalk');
const getDistance = require('levenshtein-lite');
const commands = require('commands');
const OutputPatcher = require('./src/patches').OutputPatcher;
const rootDirectory = process.cwd();

const handleSetup = commands.handleSetup();
const handleLink = commands.handleLink();
const handleConfig = commands.handleConfig();
const handleLinter = commands.handleLinter();
const VERSION = require('./src/config').VERSION;

const unknownCommand = program => !program.args.map(arg => typeof arg).includes('object')

program.version(VERSION)
  .usage('add-reason [command] [options]')
  .option('--no-linking', 'don\'t create the symlink to your compiled ReasonML code');

program.command('setup <directory> [package-name]')
  .description('set up Reason directory, config files, and symlink')
  .action((directory, name) => {
    // given name or last source directory path name or simply `pkg`
    name = name || directory.replace(/\/+$/, '').split('/').pop() || 'pkg';
    handleSetup(name, directory, rootDirectory, VERSION, program.linking);
  });

 program.command('link <directory> [package-name]')
  .description('create a symlink with the given package name')
  .action((directory, name) => {
    // given name or last source directory path name or simply `pkg`
    name = name || directory.replace(/\/+$/, '').split('/').pop() || 'pkg';
    handleLink(name, directory, rootDirectory, VERSION);
  });

 program.command('linter <directory>')
  .description('create a merlin file for linting your ReasonML code')
  .action(directory => {
    handleLinter(directory, rootDirectory, VERSION);
  });

 program.command('config <directory>')
  .description('create a bsconfig file for building your ReasonML code')
  .action(directory => {
    handleConfig(directory, rootDirectory, VERSION);
  });

 program.command('rename <package-name>')
  .description('change the current Reason package name symlink')
  .action((name) => {});

program.command('unlink <package-name>')
  .description('removes the given symlink')
  .action((name) => {});

OutputPatcher(program);
program.parse(process.argv);

// Set default command to --help
if (!process.argv.slice(2).length) {
  program.outputHelp();
  process.exit(0);
}

if (unknownCommand(program)) {
  const unknownCommand = program.args[0];
  const possibleCommands = program.commands.reduce((acc, cmd) => {
    const name = cmd._name;
    if (getDistance(name, unknownCommand) < 3) {
      acc.push(name);
    }
    return acc;
  }, []);
  if (possibleCommands.length > 0) {
    process.stdout.write(`\n  Unknown command ${chalk.bold(unknownCommand)}. Did you mean to do any of these?\n\n`);
    possibleCommands.map(name => process.stdout.write(`    ${name}\n`))
    process.stdout.write('\n');
  } else {
    const directory = program.args[0];
    const name = program.args[1] || directory.replace(/\/+$/, '').split('/').pop() || 'pkg';
    handleSetup(name, directory, rootDirectory, VERSION, program.linking);
  }
  process.exit(0);
}

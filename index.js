#!/usr/bin/env node

'use strict';

const program = require('commander');
const initializer = require('initializer');

const handleInit = initializer.Shell[3]();

program
  .version('1.0.0')
  .usage('[command] [options]')
  .command('init <alias> <directory>')
  .description('Set up Reason directory, config files, and symlink.')
  .action((alias, directory) => {
    const res = handleInit(alias, directory);
    console.log(res);
  });

program
  .command('version')
  .description('Get the current version.')
  .action(() => console.log(`🎺  version: ${program.version()}`));

// Add a newline after the help command to make it look nicer
program.on('--help', () => console.log(''));

program.parse(process.argv);
 
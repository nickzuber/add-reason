
<img width="392" src="public/logo.png" alt="add-reason" />

> Simple tool for seamlessly integrating ReasonML into your existing JavaScript projects.

<a href="#"><img src="https://travis-ci.org/nickzuber/add-reason.svg?branch=master" /></a> <a href="#"><img src="https://img.shields.io/badge/project-active-brightgreen.svg" /></a> <a href="#"><img src="https://img.shields.io/badge/released-yes-green.svg" /></a> <a href="#"><img src="https://img.shields.io/badge/license-MIT%20Licence-blue.svg" /></a>

This is an easy to use [cli tool](https://en.wikipedia.org/wiki/Command-line_interface) that helps you seamlessly interop [ReasonML](https://reasonml.github.io/) code with an existing JavaScript project. You tell us where your ReasonML code is, and we'll handle the rest!

## Installation

```
$ npm install -g add-reason
```
or
```
$ yarn add global add-reason
```

## Usage

The most basic use case would be to simply add an interop between your JavaScript and ReasonML code. In this case, you'd probably want to go with the `setup` command and specify the location of your ReasonML code and optionally include the name of a package you'd want to give it.

Let's assume you had file structure like this, where all your ReasonML code is located in a single directory called `reasonCode` or something:

```
myCoolProject/
├── reasonCode/
│   ├── index.re
│   ├── file1.re
│   ├── file2.re
│   └── utils/
│       ├── fs.re
│       └── path.re
├── src/
│   ├── index.js
│   ├── main.js
│   ├── example1.js
│   ├── example2.js
│   └── example3.js
└── etc/
```

You'd simply go to your project root and type:

```
$ add-reason setup ./reasonCode
```

or, since `setup` is the default command, you can even write

```
$ add-reason ./reasonCode
```

and that's it! Now you can compile you ReasonML code with

```
$ npm run build-reason
# or
$ bsb -make-world
```

and then import your compiled ReasonML code with

```js
const reasonCode = require('reasonCode');
// ...
```

If you don't specify a package name you want to give your code, we just use the directory name. You can choose any name you want though, so long as it doesn't collide with the name of a package you already have in your `node_modules/`!

```
$ add-reason setup ./reasonCode my-cool-code
```

```js
const myCoolCode = require('my-cool-code');
// ...
```

You can do a variety of other things too, the more basic usage is:

```
Usage: add-reason [command] [options]

  Commands:

    setup <directory> [package-name]  set up Reason directory, config files, and symlink
    link <directory> [package-name]   create a symlink with the given package name
    linter <directory>                create a merlin file for linting your ReasonML code
    config <directory>                create a bsconfig file for building your ReasonML code

  Options:

    -h, --help     output usage information
    -V, --version  output the version number
    --no-linking   don't create the symlink to your compiled ReasonML code
```

## How It Works

This is a very simple tool. All it does is make sure you have the typical basic config related files for writing  ReasonML and creates a symlink in your `node_modules/` targeting your compiled ReasonML code so you can easily import and use it within your JavaScript.

There are a few steps that happen when you call `setup`:

 1. Prepare target directory
 2. Make sure we have a config file
 3. Make sure we have a linting file
 4. Create symlink
 5. Create build command
 6. Create postinstall
 7. Check for `bs-platform`

Preparing the target directory just makes sure that the path `./lib/js/[your ReasonML directory name]` exists. This is where BuckleScript will put your compiled ReasonML/OCaml code, so this is the spot where we want to create a symlink later to down the line.

Next we just want to make sure you're all set up with at least the most basic of configurations. We check to see if you have a `.merlin` file for linting and a `bsconfig.json` for ReasonML configuration. If you don't, no worries! We will just create a basic file for each one that you don't have and set you up with it.

Then we just create that symlink  that hooks in your compiled ReasonML code to your `node_modules/` for easy access. We also create a postinstall script that will add this symlink every time someone installs your project, given that the link doesn't already exist. You always have the option to skip this symlinking step with the `--no-linking` flag too if you want.

We also add a script to your `package.json` called `build-reason` that simply uses Bucklescript to compile your code. So you can then call `npm run build-reason` after calling `setup` to have your ReasonML code compiled and ready to use!

The last thing that we do is check to make sure that you have [`bs-platform`](https://bucklescript.github.io/) integrated into your project. If you don't have it globally installed and linked to your current project, you're prompted to install and link it with

```
$ npm install -g bs-platform
$ npm link bs-platform
```

## FAQ

> I ran `setup` and everything worked fine, but when I try to import my code I get `Error: Cannot find module`

This is almost always because your ReasonML directory doesn't have an `index.re` file. Normally, this isn't a necessary thing, but if you want to be able to write

```js
const reasonCode = require('reasonCode');
// ...
```

your directory needs to have the `index` file for node to grab! You don't _need_ to have an `index.re` file though, instead you can just access the files you want directly.

```js
const reasonCode = require('reasonCode/file1.js');
// ...
```

> I ran `setup` but my code isn't compiled!

Don't forget to compile your code using `bs-platform`! You can use the `build-reason` command that should have been added to your `package.json` after using the setup command.

```
$ npm run build-reason
```

Remember that `add-reason` doesn't actually compile your code or deal with that bit. Alternatively, you can compile your code yourself manually with BuckleScript something like

```
$ bsb -make-world
```

## License

This software is free to use under the MIT License. See [this reference](https://opensource.org/licenses/MIT) for license text and copyright information.

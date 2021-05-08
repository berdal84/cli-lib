
![workflow status](https://github.com/berdal84/clib/actions/workflows/build.yml/badge.svg?branch=main)
![workflow status](https://github.com/berdal84/clib/actions/workflows/ctest.yml/badge.svg?branch=main)

## Brief

CLIB is a try to make a library to facilitate command line argument parsing.

_Note: this kind of this lib already exists, I want to learn how to do this "on my own"._

## Details

First, I want to be able to define a set of flags or typed params.
Some examples:

- flag fullscreen can be enable using `-f | --fullscreen`,
usage: `app -f`

- screen size `-s=640x480 | --screen-size=640x480`

Then I will add commands with the possibility to declare flags/params and to perform actions.

Ex: `myprogram command --flag --param=7`

Later, why not to create constraints between flags/params.

## Help

An example is provided in `examples/example_01.cpp`


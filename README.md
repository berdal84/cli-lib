A try to make a library to facilitate command line argument parsing.

First, I want to be able to define a set of flags or typed params.
Some examples:

- flag fullscreen can be enable using `-f | --fullscreen`,
usage: `app -f`

- screen size `-s=640x480 | --screen-size=640x480`

Then I will add commands to able to get flags/params and run some code.
Ex: `myprogram command --flag --param=7`

Later, why not to create constraints between flags/params.

Note: this kind of this might exist, but I want to learn how to do this.
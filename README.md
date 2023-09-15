# scfetch
![alt text](https://github.com/merci-libre/scfetch/blob/main/blobs/screenshot_077.png)
### Current version: 0.7
An entire re-implementation of sfetch written in C-Lang.
Currently only supports art prompts, but will introduce more features later.

Installation is done through the install script:
```
chmod +x install.sh
./install.sh
```
or you can manually make the file with:
```
gcc main.c -o scfetch
sudo cp scfetch /usr/local/bin
```
however, it is recommended that you install the program using the install script as 
the install script automatically makes the required directories for the software to work.

## About the program:

### This program is designed for Unix-like systems, specifically **Linux**! Do not use with Windows!
It's a terminal program that on startup, greets you with customizable prompts that you can manually add, remove, or even modify using system utilities.
In other words, all you need is a text file in the directory `$HOME/.config/shellrandomgreeter/prompts` and scfetch will read the text file as is.

Currently this project is in it's infancy so there still needs to be a roll-out to the development. Currently, the project is in what I call 'stage-0' which translates into an 'in-development' phase.
I open-sourced the project to keep track of the progress for both future employers, and myself.

## Manual Installation
The installation process is pretty easy if you want to do it manually.

The program searches for the following directory:
`/home/{user}/.config/shellrandomgreeter/prompts`, then searches for files within this directory to write out
in plaintext to your terminal. 
### Installing to *nix system
  1. In your terminal enter the following command `$ mkdir -p $HOME/.config/shellrandomgreeter/prompts`
  2. Create a prompt in `$HOME/.config/shellrandomgreeter/prompts/` with `$ touch $HOME/.config/shellrandomgreeter/prompts/{prompt_name}`
  3. Fill prompt with text.
### Compiling the binary
  1. Compile the binary in the git folder with `gcc -o scfetch main.c`
  2. move the binary to /usr/local/bin
Voila. Installation complete!


### Usage
Once you have the binary installed, you can invoke it in your shell with `scfetch`.
Running the command `scfetch -h` will provide you with a help menu for the options.
If you would like to get a specific prompt, you can list out the index with `scfetch -l`
and choose the index number with `scfetch -f <index number>`

### Adding prompts

In `$HOME/.config/shellrandomgreeter/prompts`, do the following:
```
touch {file_name}
```
and then edit the file `{file_name}` with a text editor. When scfetch is ran, the file `{file_name}` is read in plaintext.

### Removing prompts
Same concept as *Adding prompts* just use `rm {file_name}`

## Development cycle and versions.
### Stage-0 
Stage 0 is purely development and will cover all versions until version 1.0 is released. In this stage, development is focused on:
- Getting the program to work functionally.
- Adding features from the original project, such as system information, time since boot, getting environment variables etc.

### Stage-1
Stage 1 is the post-release stage, where things begin to be fleshed out into a safe, and working program.
Here is where I will check for:
- Bugs
- Memory leaks
- Potential security vulnerabilities
- and other nasty things.

Once I have obtained a list of these issues, I will then patch them periodically based on it's severity and release a new version of the software under a new version number which works like so: ```version 1.3``` where 1 is the major release number, and 3 being the patch number. 

### Stage-2
Stage 2 is the final stage in my development loop, as after this stage 0 is reached as I plan for future additions to the project. Stage 2 is where I will be planning new features that are fundamentally changing on both program functionality and leading to the next major release of the software. In short, this is where version 2.0, 3.0, etc are planned. 

## Potential questions:
### Will you have Windows support?
As of writing this, the answer is flat-out no. Well, maybe-- but I have to be in the mood to learn the Windows library and that will have to be an entire development cycle dedicated in order to have maximum portability across all operating systems. See '**Add support for 'xyz'**'
### Will you add compatibility for other Operating Systems?
I need to first test it to see if it will work on my system, but I will for sure test it on other systems in order to completely guarantee portability of the software onto systems that are configured different to mine.
### Add support for 'xyz'
If you want to add support for your OS or if the program doesn't work for your system feel free to fork the project and add support for it, it's the whole reason I open-sourced it.
### How do I submit bug reports?
https://github.com/merci-libre/scfetch/issues
### How do I contribute?
Either submit a pull-request or fork the project. Either works.

# scfetch

Current version: 0.3
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
sudo cp scfetch /usr/bin
```
however, it is recommended that you install the program using the install script as 
the install script manually makes the required directories for the software to work.

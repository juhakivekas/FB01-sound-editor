# FB01 Sound Editor V2.1

A patch ediotor for the Yamaha FB-01 synthesizer module.

## Installation

In order to install this software you need :

- The soure code in this repository
- Qt4 developement files (qt4-dev)
- Alsa sound library development files (libasound2-dev)
- X11 development files (libx11-dev)

### Compiling & installing procedure :

Open a terminal in the project source folder and type the following commands :

make all
sudo make install

If sudo is not available on your system, you can log in as super-user (with the
su command which is not recommended) and type :

make install

### Cleaning and removing procedure :

To completely remove the software from your system, type the following commands :

make uninstall

To clean the object files and other intermediary compiler files of the source
folder you can enter the command :

make clean

### Other makefile rules

make release	: produces the same executable as the make all directive
make debug		: produces a debug enable version of the executable

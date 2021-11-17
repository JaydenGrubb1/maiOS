# Requirements

The following tools are needed to compile the GCC cross compiler:
- build-essential
- bison
- flex
- libgmp3-dev
- libmpc-dev
- libmpfr-dev
- texinfo

On top of that, the following tools are needed to compile/run the operating system:
- nasm
- xorriso
- grub-pc-bin
- grub-common

To install any of these tools simply run 
```console
sudo apt install $TOOL_HERE
```
replacing `$TOOL_HERE` with the needed dependecy, or install them all at once using
```console
sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo nasm xorriso grub-pc-bin grub-common -y
```
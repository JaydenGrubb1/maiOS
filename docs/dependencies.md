# Dependencies
This document outlines the dependencies needed to compile and run the operating system. On my system (WSL - Ubuntu 20.04) these were installed through the APT package manager using the following command.
```console
sudo apt install XXXX
```
Be sure to adjust this and/or the below commands to match your system.

## Operating System
In order to compile and package the operating system the following packages will be needed.

- nasm
- xorriso
- grub-pc-bin
- grub-common

```console
sudo apt install nasm xorriso grub-pc-bin grub-common
```

## Cross Compiler
On top of this, a cross compiler is needed in order to compile to a generic ELF64 format. For this we will use a custom build of GCC. In order to build the GCC cross compiler, we will need the following packages.

- build-essential
- bison
- flex
- libgmp3-dev
- libmpc-dev
- libmpfr-dev
- texinfo

```console
sudo apt install build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
```
## Emulator
Finally, to run the operating system an emulator of some kind is needed. Currently, the project's Makefile has been configured to run it via QEMU. The following packages are needed for QEMU to run correctly.

- qemu
- qemu-kvm

```console
sudo apt install qemu qemu-kvm
```

## Other
Any other dependencies. At the moment this is just the GNU debugger.
- gdb

```console
sudo apt install gdb
```

## All Dependencies
```console
sudo apt install nasm xorriso grub-pc-bin grub-common build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo qemu qemu-kvm gdb
```
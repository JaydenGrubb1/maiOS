# Building

## TLDR
1. Install dependencies (see [dependencies](dependencies.md))
2. Run `tools/compiler-setup.sh`
3. Compile with `cmake` from the `build` directory

## 1 - Installing Dependencies
Install the necessary dependencies for your system. See [dependencies](dependencies.md) for more information.

## 2 - Setup Cross-Compiler
From the root directory of the project, run the following command:
```bash
./tools/compiler-setup.sh
```
This will download, patch, and compile the cross-compiler. The cross-compiler will be installed in the `tools` directory.

> [!NOTE]
> This will take a while, especially on slower machines.

## 3 - Compiling
Create the build directory and initialize `cmake`:
```bash
mkdir build
cd build
cmake ..
```
This will generate the build files. Now you can compile the project with `make`:
```bash
make
```
Additionnaly, you can just compile the kernel, initrd or iso with the following commands:
```bash
make kernel
make initrd
make iso
```

## 4 - Running
To run the kernel in QEMU, run the following command from the build directory:
```bash
make run
```
Optionally, QEMU can be run with debugging enabled using the following command:
```bash
make debug
```
This will start QEMU and wait for a debugger to connect to it. The provided vscode configuration can be used to connect to the debugger simply by pressing `F5`.

## 5 - Installing
TODO
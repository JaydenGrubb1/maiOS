# sudo apt update
# sudo apt upgrade -y
# sudo apt install build-essential -y
# sudo apt install bison -y
# sudo apt install flex -y
# sudo apt install libgmp3-dev -y
# sudo apt install libmpc-dev -y
# sudo apt install libmpfr-dev -y
# sudo apt install texinfo -y

GCC_VERSION="gcc-10"
BINUTILS_VERSION="binutils-2_37-branch"
PREFIX="$PWD/tools/compiler"
TARGET="x86_64-elf"

cd tools

git clone git://sourceware.org/git/binutils-gdb.git
cd binutils-gdb
git checkout $BINUTILS_VERSION
mkdir binutils-build
cd binutils-build
../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
cd ..
mv binutils-build ../binutils-build
cd ..

git clone git://gcc.gnu.org/git/gcc.git
cd gcc
git checkout releases/$GCC_VERSION
mkdir gcc-build
cd gcc-build
../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
cd ..
mv gcc-build ../gcc-build
cd ..
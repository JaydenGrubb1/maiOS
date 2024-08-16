GCC_VERSION="gcc-14"
BINUTILS_VERSION="binutils-2_43-branch"
PREFIX="$PWD/tools/compiler"
TARGET="x86_64-elf"

cd tools

# Download Binutils
git clone git://sourceware.org/git/binutils-gdb.git
cd binutils-gdb
git checkout $BINUTILS_VERSION

# Build Binutils
mkdir binutils-build
cd binutils-build
../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
cd ..
mv binutils-build ../binutils-build
cd ..

# Download GCC
git clone git://gcc.gnu.org/git/gcc.git
cd gcc
git checkout releases/$GCC_VERSION

# Apply patches
git apply ../patches/gcc/mno-red-zone.patch

# Build GCC
mkdir gcc-build
cd gcc-build
../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-bootstrap --disable-hosted-libstdcxx --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make all-target-libstdc++-v3
make install-gcc
make install-target-libgcc
make install-target-libstdc++-v3
cd ..
mv gcc-build ../gcc-build
cd ..
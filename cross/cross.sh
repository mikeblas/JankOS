#!/bin/sh

# Options
PREFIX="$HOME/opt/cross"
# TARGET="i686-elf"
TARGET="x86_64-elf"
# DEST="$HOME/osdev"
DEST="$HOME/osdev64"

# Add to the path, and add it to the bash profile
export PATH="$PREFIX/bin:$PATH"

# Prepare the environment
# export CC=gcc-4.9
# export CXX=g++-4.9
# export CPP=cpp-4.9
# export LD=gcc-4.9
export CC=gcc
export CXX=g++
export CPP=cpp
export LD=gcc

# Brew
# if brew list -1 | grep -q "^gcc49\$"; then
#     echo ""
#     echo "\033[1;32mgcc49 already installed\033[0m"
# else
#     echo ""
#     echo "\033[1;31mgcc49 not present... installing\033[0m"
#     brew tap homebrew/versions
#     brew install --enable-cxx  gcc49
# fi

echo ""
echo "\033[1;33mChecking for required libraries...\033[0m"
# for PKG in mpfr gmp libmpc; do
#     if brew list -1 | grep -q "^${PKG}\$"; then
#         echo "\033[1;32m${PKG} already installed\033[0m"
#     else
#         echo "\033[1;31m${PKG} not present... installing\033[0m"
#         brew install ${PKG}
#     fi
# done

# Download the sources
GCC_6_4_SOURCE="https://ftp.gnu.org/gnu/gcc/gcc-9.3.0/gcc-9.3.0.tar.gz"
BINUTILS_2_29_SOURCE="https://ftp.gnu.org/gnu/binutils/binutils-2.35.tar.gz"
GCC_ARCHIVE=${DEST}/gcc49.tar.gz
BIN_UTILS_ARCHIVE=${DEST}/binutils.tar.gz

# Ensure the destination folder exists
if [ ${PURGE} == "yes" ]; then
    rm -rf ${DEST}
fi

mkdir -p ${DEST}

echo "looking for ${BIN_UTILS_ARCHIVE}"

if [ ! -f ${BIN_UTILS_ARCHIVE} ]; then
    echo ""
    echo "\033[1;36mDownloading ${BINUTILS_2_29_SOURCE}\033[0m"
    curl -# ${BINUTILS_2_29_SOURCE} > ${BIN_UTILS_ARCHIVE}
fi

echo "looking for ${GCC_ARCHIVE}"

if [ ! -f ${GCC_ARCHIVE} ]; then
    echo ""
    echo "\033[1;36mDownloading ${GCC_6_4_SOURCE}\033[0m"
    curl -# ${GCC_6_4_SOURCE} > ${GCC_ARCHIVE}
fi

# Unpack the packages and prepare for building the toolchain
BUILD_SRC=${DEST}
BIN_UTILS_DIR=${BUILD_SRC}/binutils-2.35-build
GCC_DIR=${BUILD_SRC}/gcc-9.3-build

echo "GCC at ${GCC_DIR}"
echo "BIN at ${BIN_UTILS_DIR}"

mkdir -p ${BIN_UTILS_DIR} ${GCC_DIR}

echo ""
echo "\033[1;33mExpanding downloaded packages\033[0m"
cd ${DEST}
tar -zxf ${BIN_UTILS_ARCHIVE}
tar -zxf ${GCC_ARCHIVE}


echo ""
echo "\033[1;33mConfiguring & Installing binutils\033[0m"
cd ${BIN_UTILS_DIR}
../binutils-2.35/configure --target=${TARGET} --prefix="${PREFIX}"\
    --disable-nls --disable-werror
make
make install

cd ${GCC_DIR}
../gcc-9.3.0/configure --target=$TARGET --prefix="$PREFIX"\
    --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

# add the tools to the users PATH and bash profile
echo ""
echo "\033[1;33mAdding tools to your profile & PATH\033[0m"
echo "export PATH=\"\$PATH:$PREFIX/bin\"" >> ~/.bash_profile


# Clean up... we've finished everything
cd ${DIR}
# rm -rf ${DEST}


# Give a final wrap up message to the user with instructions.
cat <<EOF
\033[1;32mGCC Cross Compiler & Supporting Tools installed!\033[0m
The installation has now finished and the toolchain is now ready for use.
Unless you specified otherwise, the gcc compiler can be used by calling
\033[1;35mi686-elf-gcc\033[0m.
However there is one final step that you need to do before you can use it. That
is to run the following command:
    source ~/.bash_profile
After that you will be good to go!
EOF
	

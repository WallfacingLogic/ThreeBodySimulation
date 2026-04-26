#!/bin/bash
# 为 MinGW 交叉编译 GMP 和 MPFR 库

set -e

INSTALL_PREFIX="/opt/mingw-libs"
MINGW_HOST="x86_64-w64-mingw32"
BUILD_DIR="/tmp/mingw-libs-build"

echo "========================================="
echo "为 MinGW 编译 GMP 和 MPFR 库"
echo "安装目录: $INSTALL_PREFIX"
echo "========================================="

# 创建构建目录
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 下载并编译 GMP
echo ""
echo "步骤 1: 下载并编译 GMP..."
if [ ! -f gmp-6.3.0.tar.xz ]; then
    wget https://gmplib.org/download/gmp/gmp-6.3.0.tar.xz
fi
tar xf gmp-6.3.0.tar.xz
cd gmp-6.3.0

./configure \
    --host=$MINGW_HOST \
    --prefix=$INSTALL_PREFIX \
    --enable-static \
    --disable-shared \
    --disable-cxx

make -j$(nproc)
make install

echo "GMP 编译完成！"

# 下载并编译 MPFR
echo ""
echo "步骤 2: 下载并编译 MPFR..."
cd "$BUILD_DIR"
if [ ! -f mpfr-4.2.1.tar.xz ]; then
    wget https://www.mpfr.org/mpfr-current/mpfr-4.2.1.tar.xz
fi
tar xf mpfr-4.2.1.tar.xz
cd mpfr-4.2.1

./configure \
    --host=$MINGW_HOST \
    --prefix=$INSTALL_PREFIX \
    --with-gmp=$INSTALL_PREFIX \
    --enable-static \
    --disable-shared

make -j$(nproc)
make install

echo "MPFR 编译完成！"

# 清理
cd /
rm -rf "$BUILD_DIR"

echo ""
echo "========================================="
echo "编译完成！库文件安装在: $INSTALL_PREFIX"
echo "========================================="
echo ""
echo "头文件目录: $INSTALL_PREFIX/include"
echo "库文件目录: $INSTALL_PREFIX/lib"
echo ""
ls -la "$INSTALL_PREFIX/lib/"*.a

# MXE 交叉编译工具链文件
# 用于在 Linux 上编译 Windows 程序

# 设置目标系统
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# 设置 MXE 路径（可以根据实际情况修改）
set(MXE_PATH "/opt/mxe" CACHE PATH "Path to MXE installation")
set(MXE_TARGET "x86_64-w64-mingw32.static" CACHE STRING "MXE target triplet")

# 设置编译器
set(CMAKE_C_COMPILER "${MXE_PATH}/usr/bin/${MXE_TARGET}-gcc")
set(CMAKE_CXX_COMPILER "${MXE_PATH}/usr/bin/${MXE_TARGET}-g++")
set(CMAKE_RC_COMPILER "${MXE_PATH}/usr/bin/${MXE_TARGET}-windres")

# 设置查找路径
set(CMAKE_FIND_ROOT_PATH "${MXE_PATH}/usr/${MXE_TARGET}")

# 调整默认查找行为
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# 设置可执行文件后缀
set(CMAKE_EXECUTABLE_SUFFIX ".exe")

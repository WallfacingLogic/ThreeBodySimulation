cmake_minimum_required(VERSION 3.15)
project(ThreeBodySimulation LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录为 bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# 添加项目头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

# 原生编译：查找 MPFR 库（需要 GMP 作为依赖）
find_package(PkgConfig REQUIRED)
pkg_check_modules(MPFR REQUIRED mpfr)

# 包含目录 - 添加 x86_64-linux-gnu 路径以找到 gmp.h
include_directories(${MPFR_INCLUDE_DIRS})
include_directories(/usr/include/x86_64-linux-gnu)

# 源文件
set(SOURCES
    src/main.cpp
    src/ThreeBodySolver.cpp
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接 MPFR 和 GMP 库
target_link_libraries(${PROJECT_NAME} ${MPFR_LIBRARIES})

# 添加编译选项
if(MSVC)
    # Microsoft Visual C++ 编译器选项
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    # GCC/Clang 编译器选项
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)
endif()

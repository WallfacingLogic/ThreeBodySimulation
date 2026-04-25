cmake_minimum_required(VERSION 3.15)
project(ThreeBodySimulation LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置可执行文件输出目录为 bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

# 查找 MPFR 库（需要 GMP 作为依赖）
find_package(PkgConfig REQUIRED)
pkg_check_modules(MPFR REQUIRED mpfr)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${MPFR_INCLUDE_DIRS})

# 源文件
set(SOURCES
    src/main.cpp
    src/ThreeBodySolver.cpp
)

# 头文件
set(HEADERS
    include/Constants.h
    include/ThreeBodySolver.h
    include/Vector3.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

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

# 输出配置信息
message(STATUS "MPFR Include Dirs: ${MPFR_INCLUDE_DIRS}")
message(STATUS "MPFR Libraries: ${MPFR_LIBRARIES}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# 1. 指定最低支持的 CMake 版本
cmake_minimum_required(VERSION 3.10)
# 2. 定义项目名称 + 编程语言（C语言）
project(hello C)
# 3. 自动查找当前目录下所有 C 源文件，存入变量 SRCS
aux_source_directory(. SRCS)
# 4. 根据源文件生成可执行文件 hello
add_executable(hello ${SRCS})

# 其他常见Cmake语法

# 添加头文件目录
# include_directories(./inc)  # 让编译器找到 .h 文件
# 生成动态库 / 静态库
# add_library(utils STATIC utils.c)  # 静态库 .a/.lib
# add_library(utils SHARED utils.c)  # 动态库 .so/.dll
# 链接库文件
# target_link_libraries(main utils)  # 给 main 链接 utils 库
# 设置C语言标准
# set(CMAKE_C_STANDARD 99)   # C99
# set(CMAKE_CXX_STANDARD 11) # C++11
# 打印调试信息
# message("源文件列表：${SRCS}")

# Cmake 运行命令

# 1. 创建 build 文件夹（推荐外部构建）
# mkdir build && cd build

# 2. 生成 Makefile
# cmake ..

# 3. 编译
# make

# 4. 运行程序
# ./demo
cmake_minimum_required(VERSION 3.10)
project(10_Shm C)

# 每个演示程序独立编译为单独的可执行文件
add_executable(main main.c)

# ❗️shm_open需要librt实时库
target_link_libraries(main rt)

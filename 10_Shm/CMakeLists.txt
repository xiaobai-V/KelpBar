cmake_minimum_required(VERSION 3.10)
project(10_Shm C)

# 每个演示程序独立编译为单独的可执行文件
add_executable(main main.c)

# ❗️shm_open需要librt实时库
# target_link_libraries(main rt)

# ldd (Ubuntu GLIBC 2.39-0ubuntu8.7) 2.39
# glibc 2.18 及以上版本 shm_open, shm_unlink 等 POSIX 实时函数已经从 librt 合并进了 libc

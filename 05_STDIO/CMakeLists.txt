cmake_minimum_required(VERSION 3.1.5)
project(main C)
aux_source_directory(. SRCS)
add_executable(main ${SRCS})
cmake_minimum_required(VERSION 3.10)
project(JobShop C)

set(CMAKE_C_STANDARD 99)

add_executable(JobShop main.c IN.h OUT.h utils.h evolution.h IN.c evolution.c utils.c)
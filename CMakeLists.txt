cmake_minimum_required(VERSION 3.7)
project(KsP)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    njvm.c
    njvm.h)

add_executable(KsP ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.19)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED YES)

project(ofs)

include_directories(src)

add_subdirectory(src)
cmake_minimum_required(VERSION 3.20)
project(GADTs)

set(CMAKE_EXPORT_COMPILE_COMMANDS On)

add_executable(gadt main.cpp)
set_property(TARGET gadt PROPERTY CXX_STANDARD 20)

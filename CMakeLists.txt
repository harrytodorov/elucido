# CMake version check
cmake_minimum_required(VERSION 3.7)

# Project name
project(Elucido)

# Add the actual elucido project
add_subdirectory(src)

# Add elucido tests project
add_subdirectory(tests)

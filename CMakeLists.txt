# CMake version check
cmake_minimum_required(VERSION 3.7)

# Project name
project(elucido_all)

# Add the actual elucido project
add_subdirectory(src)

# Add elucido test project
add_subdirectory(test)

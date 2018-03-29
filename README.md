# Refactoring
The code base currently undergoes refactoring and some of the components could be ill-functioning or not functioning at all.

# Building elucido
elucido uses [Cmake](https://cmake.org/) as its build system. Following external libraries are used:
 * OpenGL Mathematics ver. 0.9.8 ([glm](https://glm.g-truc.net/0.9.8/index.html)): a header only C++ math library. The library is ued to provide robust implementation for manipulations on matrices and vectors.  
 * [png++](https://www.nongnu.org/pngpp/) ver. 0.2: a C++ wrapper for the [libpng](http://www.libpng.org/pub/png/libpng.html) library. The library is used to save rendered images into *.png graphics file format.
 * [Google Test](https://github.com/google/googletest) (gtest): a C++ testing framework. 

The source file for both libraries glm and png++ reside in the _include/_ directory. Although to be able to use and build png++ one needs to have [libpng](http://www.libpng.org/pub/png/libpng.html) installed. Installation instructions for it can be found on the [libpng](http://www.libpng.org/pub/png/libpng.html) webpage.

To be able to run the tests, one needs to have [gtest](https://github.com/google/googletest) installed.

# Testing
There is a folder inside _tests_ called _test_resources_, which contains files needed for testing (e.g. example test scenes, object files, etc.). This folder needs to be placed in the folder, where the compiled test-executable resides.  
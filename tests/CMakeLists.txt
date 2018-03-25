# CMake version check
cmake_minimum_required(VERSION 3.7)

# Project name
project(elucido_test)

# Add sources for tests files
set(ELUCIDO_TEST_SOURCE_FILES
        RayTest.cpp
        AABBTest.cpp
        SceneParserTest.cpp
        TransformationTest.cpp
        SampleTest.cpp
        CameraTest.cpp
        RendererTest.cpp
        LightTest.cpp
        ObjectTest.cpp
        AccelerationStructureTest.cpp)

# Create tests executable
add_executable(elucido_test TestMain.cpp ${ELUCIDO_TEST_SOURCE_FILES})

# Target the google tests library, locally availably
target_link_libraries(elucido_test gtest gtest_main)
target_link_libraries(elucido_test elucido_lib)

# Set up glm library
include_directories(../include)
link_directories(../include)

# Set up png++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(elucido_test ${PNG_LIBRARY})


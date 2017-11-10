# CMake version check
cmake_minimum_required(VERSION 3.7)

# Project name
project(elucido)

# Enable C++11 standard
set(CMAKE_CXX_STANDARD 11)

# Set source files
set(ELUCIDO_SOURCE_FILES
        cameras/Camera.cpp
        cameras/OrthographicCamera.cpp
        cameras/PerspectiveCamera.cpp
        lights/Light.cpp
        lights/DirectionalLight.cpp
        lights/PointLight.cpp
        objects/Object.cpp
        objects/AABBox.cpp
        objects/Sphere.cpp
        objects/Triangle.cpp
        objects/TriangleMesh.cpp
        acceleration/AccelerationStructure.cpp
        acceleration/Grid.cpp
        ImagePlane.cpp
        Ray.cpp)

# Set header files
set(ELUCIDO_HEADER_FILES
        ../include/elucido/Camera.h
        ../include/elucido/OrthographicCamera.h
        ../include/elucido/PerspectiveCamera.h
        ../include/elucido/Light.h
        ../include/elucido/DirectionalLight.h
        ../include/elucido/PointLight.h
        ../include/elucido/Object.h
        ../include/elucido/AABBox.h
        ../include/elucido/Sphere.h
        ../include/elucido/Triangle.h
        ../include/elucido/TriangleMesh.h
        ../include/elucido/AccelerationStructure.h
        ../include/elucido/Grid.h
        ../include/elucido/ImagePlane.h
        ../include/elucido/Ray.h
        ../include/elucido/Utilities.h)

# Include own header files
include_directories(../include/elucido)

# Create executable
add_executable(elucido main.cpp ${ELUCIDO_SOURCE_FILES})

add_library(elucido_lib STATIC
        ${ELUCIDO_SOURCE_FILES})

# Set up glm library
include_directories(../include)
link_directories(../include)

# Set up png++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(elucido ${PNG_LIBRARY})

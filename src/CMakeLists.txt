# CMake version check
cmake_minimum_required(VERSION 3.7)

# Project name
project(elucido)

# Enable C++11 standard
set(CMAKE_CXX_STANDARD 11)

# Set source files
set(ELUCIDO_SOURCE_FILES
        extra/Utilities.cpp
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
        extra/ImagePlane.cpp
        extra/Ray.cpp
        extra/Scene.cpp)

# Set header files
set(ELUCIDO_HEADER_FILES
        extra/Utilities.h
        cameras/Camera.h
        cameras/OrthographicCamera.h
        cameras/PerspectiveCamera.h
        lights/Light.h
        lights/DirectionalLight.h
        lights/PointLight.h
        objects/Object.h
        objects/AABBox.h
        objects/Sphere.h
        objects/Triangle.h
        objects/TriangleMesh.h
        acceleration/AccelerationStructure.h
        acceleration/Grid.h
        extra/ImagePlane.h
        extra/Ray.h
        extra/Scene.h)

# Create executable
add_executable(elucido main.cpp ${ELUCIDO_SOURCE_FILES})

add_library(elucido_lib STATIC ${ELUCIDO_SOURCE_FILES})

# Set up glm library
include_directories(../include)
link_directories(../include)

# Set up png++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(elucido ${PNG_LIBRARY})

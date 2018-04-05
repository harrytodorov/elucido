# CMake version check
cmake_minimum_required(VERSION 3.7)

# Project name
project(elucido)

# Enable C++11 standard
set(CMAKE_CXX_STANDARD 11)

# Set source files
set(ELUCIDO_SOURCE_FILES
        core/Utilities.cpp
        core/ImagePlane.cpp
        core/Sample.cpp
        core/Ray.cpp
        objects/Object.cpp
        objects/Sphere.cpp
        objects/Triangle.cpp
        objects/TriangleMesh.cpp
        accelerators/AABBox.cpp
        accelerators/AccelerationStructure.cpp
        accelerators/Grid.cpp
        accelerators/DynamicGrid.cpp
        accelerators/CompactGrid.cpp
        cameras/Camera.cpp
        cameras/OrthographicCamera.cpp
        cameras/PerspectiveCamera.cpp
        lights/Light.cpp
        lights/DirectionalLight.cpp
        lights/PointLight.cpp
        core/Common.cpp
        core/Renderer.cpp
        core/Scene.cpp)

# Set header files
set(ELUCIDO_HEADER_FILES
        core/Utilities.h
        core/ImagePlane.h
        core/Sample.h
        core/Ray.h
        objects/Object.h
        objects/Sphere.h
        objects/Triangle.h
        objects/TriangleMesh.h
        accelerators/AABBox.h
        accelerators/AccelerationStructure.h
        accelerators/Grid.h
        accelerators/DynamicGrid.h
        accelerators/CompactGrid.h
        lights/Light.h
        lights/DirectionalLight.h
        lights/PointLight.h
        cameras/Camera.h
        cameras/OrthographicCamera.h
        cameras/PerspectiveCamera.h
        core/Common.h
        core/Renderer.h
        core/Scene.h)

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

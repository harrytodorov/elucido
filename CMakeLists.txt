cmake_minimum_required(VERSION 3.7)
project(elucido)
set(CMAKE_CXX_STANDARD 11)

# Set source files
set(ELUCIDO_SOURCE_FILES
        src/cameras/Camera.cpp
        src/cameras/OrthographicCamera.cpp
        src/cameras/PerspectiveCamera.cpp
        src/lights/Light.cpp
        src/lights/DirectionalLight.cpp
        src/lights/PointLight.cpp
        src/objects/Object.cpp
        src/objects/AABBox.cpp
        src/objects/Sphere.cpp
        src/objects/Triangle.cpp
        src/objects/TriangleMesh.cpp
        src/acceleration/AccelerationStructure.cpp
        src/ImagePlane.cpp
        src/Ray.cpp)

set(ELUCIDO_HEADER_FILES
        include/elucido/Camera.h
        include/elucido/OrthographicCamera.h
        include/elucido/PerspectiveCamera.h
        include/elucido/Light.h
        include/elucido/DirectionalLight.h
        include/elucido/PointLight.h
        include/elucido/Object.h
        include/elucido/AABBox.h
        include/elucido/Sphere.h
        include/elucido/Triangle.h
        include/elucido/TriangleMesh.h
        include/elucido/AccelerationStructure.h
        include/elucido/ImagePlane.h
        include/elucido/Ray.h
        include/elucido/Utilities.h)

# Create executable
add_executable(elucido src/main.cpp ${ELUCIDO_SOURCE_FILES})

# Set up glm library
include_directories(include)
link_directories(include)

# Set up png++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(elucido ${PNG_LIBRARY})

# Set up gtest
enable_testing()
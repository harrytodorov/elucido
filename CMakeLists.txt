cmake_minimum_required(VERSION 3.7)
project(elucido)
set(CMAKE_CXX_STANDARD 11)

# Add source files to project
set(SOURCE_FILES src/Utilities.h src/Ray.h src/Ray.cpp src/ImagePlane.h src/ImagePlane.cpp
        src/cameras/Camera.cpp src/cameras/Camera.h src/cameras/OrthographicCamera.cpp src/cameras/OrthographicCamera.h src/cameras/PerspectiveCamera.cpp src/cameras/PerspectiveCamera.h
        src/objects/Object.h src/objects/Object.cpp src/objects/Sphere.cpp src/objects/Sphere.h src/objects/Triangle.cpp src/objects/Triangle.h src/objects/TriangleMesh.h src/objects/TriangleMesh.cpp src/objects/AABBox.h src/objects/AABBox.cpp src/objects/Grid.h src/objects/Grid.cpp
        src/lights/Light.h src/lights/Light.cpp src/lights/PointLight.cpp src/lights/PointLight.h src/lights/DirectionalLight.cpp src/lights/DirectionalLight.h
        src/main.cpp)

# Create executable
add_executable(elucido ${SOURCE_FILES})

# Set up glm library
include_directories(include)
link_directories(include)

# Set up png++
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(elucido ${PNG_LIBRARY})
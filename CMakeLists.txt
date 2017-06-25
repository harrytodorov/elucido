cmake_minimum_required(VERSION 3.7)
project(elucido)
set(CMAKE_CXX_STANDARD 11)

# Add source files to project
set(SOURCE_FILES src/main.cpp src/Ray.cpp src/Ray.h src/cameras/Camera.cpp src/cameras/Camera.h src/objects/Object.cpp src/objects/Object.h src/objects/Sphere.cpp src/objects/Sphere.h src/cameras/OrthographicCamera.cpp src/cameras/OrthographicCamera.h src/Utilities.h src/objects/Triangle.cpp src/objects/Triangle.h src/cameras/PerspectiveCamera.cpp src/cameras/PerspectiveCamera.h src/ImagePlane.cpp src/ImagePlane.h src/lights/Light.cpp src/lights/Light.h src/lights/PointLight.cpp src/lights/PointLight.h src/Material.cpp src/Material.h src/lights/DirectionalLight.cpp src/lights/DirectionalLight.h)

# Create executable
add_executable(elucido ${SOURCE_FILES})

# Set up an output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "builds")

# Set up glm library
include_directories(include)
link_directories(include)

# for now the project will save to ppm, without using external image libs
## Set up ImageMagick library
#add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
#add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
#find_package(ImageMagick COMPONENTS Magick++)
#include_directories(${ImageMagick_INCLUDE_DIRS})
#target_link_libraries(elucido ${ImageMagick_LIBRARIES})
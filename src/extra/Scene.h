// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ALL_SCENE_H
#define ELUCIDO_ALL_SCENE_H

#include <string>
#include <unordered_map>
#include <fstream>
#include "../objects/Object.h"
#include "../lights/Light.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "ImagePlane.h"
#include "../cameras/Camera.h"

class Scene {

//=============================================================================
// Constructors & destructors
//=============================================================================
 public:
  Scene() :
      name({}),
      objects({}),
      lights({}),
      image_plane(nullptr),
      camera(nullptr),
      acceleration_structure(nullptr)
      {};
  ~Scene() = default;

//=============================================================================
// Function declarations
//=============================================================================
  bool load_scene(const scene_description &description);

//=============================================================================
// Data members
//=============================================================================
 private:
  std::string name;
  std::map<std::string, Object *> objects;
  std::map<std::string, Light *> lights;
  std::unique_ptr<ImagePlane> image_plane;
  std::unique_ptr<Camera> camera;
  std::unique_ptr<AccelerationStructure> acceleration_structure;
};

#endif //ELUCIDO_ALL_SCENE_H
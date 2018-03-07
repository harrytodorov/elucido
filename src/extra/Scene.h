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
  Scene() = default;
  ~Scene() = default;

//=============================================================================
// Function declarations
//=============================================================================
  bool load_scene(const std::string &filename);

//=============================================================================
// Data members
//=============================================================================
 private:
  std::string scene_name;
  std::unordered_map<std::string, Object *> objects;
  std::unordered_map<std::string, Light *> lights;
  std::unordered_map<std::string, glm::vec3> colors;
  std::unordered_map<std::string, glm::vec4> vectors;
  ImagePlane image_plane;
  Camera *camera;
};

#endif //ELUCIDO_ALL_SCENE_H

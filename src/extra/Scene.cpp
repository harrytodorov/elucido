// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Scene.h"
#include "../objects/Sphere.h"

bool Scene::load_scene(const scene_description &description) {

  // Validate scene description.

  // Initialize the scene.
  this->name = description.name;

  /// Object.
  for (auto const &object : description.objects) {
    Object *obj;

    // Extract material.
    material obj_mat;
    if (object.material != nullptr) {
      obj_mat.mt = object.material->type;

    }
    switch (object.type) {

      case ObjectType::sphere: {
        obj = new Sphere();

      }
    }
  }
  return false;
}

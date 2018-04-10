// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <iostream>
#include <memory>
#include "glm/ext.hpp"    // glm::to_string

#include "objects/Object.h"
#include "objects/Sphere.h"
#include "accelerators/CompactGrid.h"
#include "core/Scene.h"
#include "accelerators/KDtreeMidpoint.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <scene file>" << std::endl;
    exit(1);
  }

  std::string filename = argv[1];
  auto result = read_scene_from_file(filename);
  if (result.first.first == success) {
    auto scene = new Scene();
    auto ret = scene->load_scene(result.second[0]);
    if (ret)
      scene->render_image();
  } else {
    std::cout << "Line " << result.first.second << ": "
              << STATUS_CODES_MAP.at(result.first.first) << std::endl;
  }
  return 0;
}

// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "glm/ext.hpp"  // glm::to_string()

#include "extra/Utilities.h"

int main(int argc, char **argv) {
  auto vec = glm::vec4(5.f, -3.f, 0.f, 0.f);
  std::cout << "Normalized: " << glm::to_string(glm::normalize(vec))
            << std::endl;

//  if (argc != 2) {
//    std::cout << "Usage: " << argv[0] << " <scene file>" << std::endl;
//    exit(1);
//  }
//  std::string filename = argv[1];
//  auto result = read_scene_from_file(filename);
//
//  return 0;
}

// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Scene.h"

bool Scene::load_scene(const std::string &filename) {
  // Open file.
  std::ifstream input_file;
  input_file.open(filename, std::ifstream::in);

  // Check, if the input file exist and is not damaged in some way.
  if (!input_file.good()) {
    return false;
  }

  std::string line;   // One line of the file.
  std::string token;
  while (std::getline(input_file, line)) {
    // Convert line into tokens.
    std::stringstream tokens(line);

    // Each line of the input scene file can start with one of the following
    // words: set, create, transform, for and render
    tokens >> token;

    if (token.compare("set")) {

    } else if (token.compare("create")) {

    } else if (token.compare("transform")) {

    } else if (token.compare("for")) {

    } else if (token.compare("render")) {

    } else {
      // An undefined start word has being used.
      return false;
    }
  }

  // Close file.
  input_file.close();
  return true;
}

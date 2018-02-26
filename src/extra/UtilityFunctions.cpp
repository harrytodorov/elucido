// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "../../include/elucido/Utilities.h"
/**
 *
 * @param filename
 * @return
 */
std::pair<SceneParserErrorCodes, std::vector<scene_description>>
        read_scene_from_file(const std::string &filename) {
  // Open file.
  std::ifstream input_file;
  input_file.open(filename, std::ifstream::in);

  // Check, if the input file exist and is not damaged in some way.
  if (!input_file.good()) {
    return {file_problem, {}};
  }

  std::map<std::string, color_description>                    colors;
  std::map<std::string, vector_description>                   vectors;
  std::map<std::string, material_description>                 materials;
  std::map<std::string, light_description>                    lights;
  std::map<std::string, object_description>                   objects;
  std::map<std::string, camera_description>                   cameras;
  std::map<std::string, image_plane_description>              image_planes;
  std::map<std::string, acceleration_structure_description>   acceleration_structures;
  std::map<std::string, animation_description>                animations;
  std::map<std::string, scene_description>                    scenes;
  std::string                                                 line;
  std::string                                                 action_token;
  while (std::getline(input_file, line)) {
    // Check if it's an empty line.
    if (line.empty()) continue;

    // Tokenize the line.
    std::stringstream tokenized_line(line);

    // Get the start word of a line, and check if it is a valid one.
    tokenized_line >> action_token;
    if (START_WORDS.find(action_token) == START_WORDS.end()) {
      return {invalid_statement, {}};
    }
    SceneFileActionWord action = START_WORDS.find(action_token)->second;
    // If the line is a comment, skip it.
    if (action == START_WORDS.find("#")->second) {
      continue;
    }
    // Execute the line statement.
    switch (action) {
      // Execute 'create' statement.
      case SceneFileActionWord::create: {
        std::string thing;
        std::string name;

        tokenized_line >> thing;
        tokenized_line >> name;

        // If either of the strings is empty, the create statement was
        // invalid, so one exists the function.
        if (thing.empty() || name.empty()) {
          return {invalid_syntax, {}};
        }
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end()) {
          return {invalid_statement, {}};
        }

        switch (AVAILABLE_THINGS.find(thing)->second) {
          case SceneThings::camera_d: {
            if (cameras.find(name) != cameras.end()) {
              return {duplicate, {}};
            }
            cameras.insert({name, camera_description(name)});
          } break;

          case SceneThings::color_d: {
            if (colors.find(name) != colors.end()) {
              return {duplicate, {}};
            }
            colors.insert({name, color_description(name)});
          } break;

          case SceneThings::vector_d: {
            if (vectors.find(name) != vectors.end()) {
              return {duplicate, {}};
            }
            vectors.insert({name, vector_description(name)});
          } break;

          case SceneThings::material_d: {
            if (materials.find(name) != materials.end()) {
              return {duplicate, {}};
            }
            materials.insert({name, material_description(name)});
          } break;

          case SceneThings::light_d: {
            if (lights.find(name) != lights.end()) {
              return {duplicate, {}};
            }
            lights.insert({name, light_description(name)});
          } break;

          case SceneThings::object_d: {
            if (objects.find(name) != objects.end()) {
              return {duplicate, {}};
            }
            objects.insert({name, object_description(name)});
          } break;

          case SceneThings::image_plane_d: {
            if (image_planes.find(name) != image_planes.end()) {
              return {duplicate, {}};
            }
            image_planes.insert({name, image_plane_description(name)});
          } break;

          case SceneThings::acceleration_structure_d: {
            if (acceleration_structures.find(name) != acceleration_structures.end()) {
              return {duplicate, {}};
            }
            acceleration_structures.insert({name, acceleration_structure_description(name)});
          } break;

          case SceneThings::animation_d: {
            if (animations.find(name) != animations.end()) {
              return {duplicate, {}};
            }
            animations.insert({name, animation_description(name)});
          } break;

          case SceneThings::scene_d: {
            if (scenes.find(name) != scenes.end()) {
              return {duplicate, {}};
            }
            scenes.insert({name, scene_description(name)});
          } break;
        }
      } break;

        // Execute 'set' statement.
      case SceneFileActionWord::set: {
        std::string thing;
        std::string name;
        std::string property;
        std::string property_value;

        tokenized_line >> thing;
        tokenized_line >> name;
        tokenized_line >> property;
        tokenized_line >> property_value;

        // If either of the strings is empty, the set statement is invalid; so
        // one exists the function.
        if (thing.empty() || name.empty() || property.empty() || property_value.empty()) {
          return {invalid_syntax, {}};
        }
        // Check if the thing to be set is supported.
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end()) {
          return {invalid_statement, {}};
        }

        switch (AVAILABLE_THINGS.find(thing)->second) {

          case SceneThings::camera_d: {
            if (cameras.find(name) == cameras.end()) {
              return {thing_not_created, {}};
            }
            if (CAMERA_SET_PROPERTIES.find(property) == CAMERA_SET_PROPERTIES.end()) {
              return {invalid_set_property, {}};
            }
            // Set the camera type.
            if (CAMERA_SET_PROPERTIES.find(property) == CAMERA_SET_PROPERTIES.find("type")) {
              if (CAMERA_TYPES_MAP.find(property_value) == CAMERA_TYPES_MAP.end()) {
                return {invalid_set_property_value, {}};
              }
              cameras.find(name)->second.type = CAMERA_TYPES_MAP.find(property_value)->second;
              break;
            } else {
              // Check if camera type is set.
              if (cameras.find(name)->second.type == not_set_ct) {
                return {invalid_set_property_value, {}};
              }

              // Zoom factor can only be set on orthographic cameras.
              if (CAMERA_SET_PROPERTIES.find(property) == CAMERA_SET_PROPERTIES.find("zoom_factor") &&
                  cameras.find(name)->second.type == orthographic) {
                // Set zoom factor.
                cameras.find(name)->second.property.first = zoom_factor;
                cameras.find(name)->second.property.second =
                    static_cast<float_t>(std::atof(property_value.c_str()));
              }
              // Field of view can only be set on perspective cameras.
              else if (CAMERA_SET_PROPERTIES.find(property) == CAMERA_SET_PROPERTIES.find("fov") &&
                       cameras.find(name)->second.type == perspective) {
                // Set fov.
                cameras.find(name)->second.property.first = field_of_view;
                cameras.find(name)->second.property.second =
                    static_cast<float_t>(std::atof(property_value.c_str()));
              }
              // Invalid.
              else {
                return {invalid_set_property_value, {}};
              }
            }
          } break;

          case SceneThings::color_d: {
            if (colors.find(name) == colors.end()) {
              return {thing_not_created, {}};
            }
            // Color has only just one property: rgb.
            if (property != "rgb") {
              return {invalid_set_property, {}};
            }
            // Extract r,g,b values from string.
            // A comma ',' is used as a delimiter.
            std::vector<std::string> color_values;
            std::istringstream color_values_stream(property_value);
            std::string color_token;
            while (std::getline(color_values_stream, color_token, ',')) {
              color_values.push_back(color_token);
            }
            // Check if only 3 values are set (r,g,b)
            if (color_values.size() != 3) {
              return {invalid_set_property_value, {}};
            }
            // Check if the values are in range [0, 255]
            int r = std::stoi(color_values[0]);
            int g = std::stoi(color_values[1]);
            int b = std::stoi(color_values[2]);
            if ((r < 0) || (r > 255) ||
                (g < 0) || (g > 255) ||
                (b < 0) || (b > 255)) {
              return {invalid_set_property_value, {}};
            }
            // Set rgb values.
            colors.find(name)->second.r = r;
            colors.find(name)->second.b = b;
            colors.find(name)->second.g = g;
          } break;

          case SceneThings::vector_d: {
            if (vectors.find(name) == vectors.end()) {
              return {thing_not_created, {}};
            }
            // Vector has just one property: coordinates.
            if (property != "coordinates") {
              return {invalid_set_property, {}};
            }
            // Extract x,y,z values from string.
            // A comma ',' is used as a delimiter.
            std::vector<std::string> vector_values;
            std::istringstream vector_values_stream(property_value);
            std::string vector_token;
            while (std::getline(vector_values_stream, vector_token, ',')) {
              vector_values.push_back(vector_token);
            }
            // Check if only 3 values are set (x,y,z)
            if (vector_values.size() != 3) {
              return {invalid_set_property_value, {}};
            }
            // Set vector's x,y,z values.
            vectors.find(name)->second.x = std::stof(vector_values[0]);
            vectors.find(name)->second.y = std::stof(vector_values[1]);
            vectors.find(name)->second.z = std::stof(vector_values[2]);
          } break;

          case SceneThings::material_d: {
            if (materials.find(name) == materials.end()) {
              return {thing_not_created, {}};
            }
            if (MATERIAL_PROPERTIES_MAP.find(property) == MATERIAL_PROPERTIES_MAP.end() &&
                  property != "type" && property != "color") {
              return {invalid_set_property, {}};
            }

            // Set material's type.
            if (property == "type") {
              if (MATERIAL_TYPES_MAP.find(property_value) != MATERIAL_TYPES_MAP.end()) {
                materials.find(name)->second.type = MATERIAL_TYPES_MAP.find(property_value)->second;
              } else {
                return {invalid_set_property_value, {}};
              }
            }
            // Set material's color.
            // The color should exist.
            else if (property == "color") {
              if (colors.find(property_value) != colors.end()) {
                materials.find(name)->second.color = &(colors.find(property_value)->second);
              } else {
                return {invalid_set_property_value, {}};
              }
            }
            // Set the remaining material properties.
            else {
              MaterialProperty mp = MATERIAL_PROPERTIES_MAP.find(property)->second;
              float_t mp_value    = std::stof(property_value.c_str());
              materials.find(name)->second.properties[mp] = mp_value;
            }
          } break;

          case SceneThings::light_d: {
            std::cout << "Name: " << lights.find(name)->second.name << std::endl
                      << "Type: " << lights.find(name)->second.type << std::endl
                      << "Color: " << ((lights.find(name)->second.color == nullptr) ? "ok" : "not ok") << std::endl
                      << "Intensity: " << lights.find(name)->second.intensity << std::endl
                      << "Property(type): " << lights.find(name)->second.property.first << std::endl
                      << "Property(value): " << ((lights.find(name)->second.property.second == nullptr) ? "ok" : "not ok") << std::endl
                      << "Transformations size: " << lights.find(name)->second.transformations.size() << std::endl;
          } break;

          case SceneThings::object_d: {
            std::cout << "Name: " << objects.find(name)->second.name << std::endl
                      << "Type: " << objects.find(name)->second.type << std::endl
                      << "Material: " << ((objects.find(name)->second.material == nullptr) ? "ok" : "not ok") << std::endl
                      << "Center: " <<  ((objects.find(name)->second.center == nullptr) ? "ok" : "not ok") << std::endl
                      << "Radius: " << objects.find(name)->second.radius << std::endl
                      << "Vertices: " << ((objects.find(name)->second.vertices == nullptr) ? "ok" : "not ok") << std::endl
                      << "File name: " << objects.find(name)->second.file_name << std::endl
                      << "Interpolation: " << unsigned(objects.find(name)->second.interpolation) << std::endl
                      << "Transformations size: " << objects.find(name)->second.transformations.size() << std::endl;
          } break;

          case SceneThings::image_plane_d: {
            std::cout << "Name: " << image_planes.find(name)->second.name << std::endl
                      << "Output type: " << image_planes.find(name)->second.output_type << std::endl
                      << "Horizontal: " << unsigned(image_planes.find(name)->second.horizontal) << std::endl
                      << "Vertical: " << unsigned(image_planes.find(name)->second.vertical) << std::endl
                      << "Gamma: " << unsigned(image_planes.find(name)->second.gamma) << std::endl
                      << "Number samples: " << unsigned(image_planes.find(name)->second.number_samples) << std::endl;
          } break;

          case SceneThings::acceleration_structure_d: {
            std::cout << "Name: " << acceleration_structures.find(name)->second.name << std::endl
                      << "Type: " << unsigned(acceleration_structures.find(name)->second.type) << std::endl
                      << "Alpha: " << acceleration_structures.find(name)->second.alpha << std::endl
                      << "Max resolution: " << unsigned(acceleration_structures.find(name)->second.max_resolution) << std::endl;
          } break;

          case SceneThings::animation_d: {
            std::cout << "Name: " << animations.find(name)->second.name << std::endl
                      << "Number of images in sequence: " << unsigned(animations.find(name)->second.num_of_images_in_sequence) << std::endl
                      << "Objects size: " << animations.find(name)->second.objects.size() << std::endl
                      << "Lights size: " << animations.find(name)->second.lights.size() << std::endl
                      << "Camera(name): " << animations.find(name)->second.camera.first << std::endl
                      << "Camera(transformations): " << unsigned(animations.find(name)->second.camera.second.size()) << std::endl;
          } break;

          case SceneThings::scene_d: {
            std::cout << "Name: " << scenes.find(name)->second.name << std::endl
                      << "Camera: " << ((scenes.find(name)->second.camera == nullptr) ? "ok" : "not ok" ) << std::endl
                      << "Image plane: " << ((scenes.find(name)->second.image_plane == nullptr) ? "ok" : "not ok" ) << std::endl
                      << "Acceleration structure: " << ((scenes.find(name)->second.acceleration_structure == nullptr) ? "ok" : "not ok" ) << std::endl
                      << "Objects size: " << scenes.find(name)->second.objects.size() << std::endl
                      << "Lights size: " << scenes.find(name)->second.lights.size() << std::endl
                      << "Animations size: " << scenes.find(name)->second.animations.size() << std::endl;
          } break;
        }

      } break;

        // Execute 'transform' statement.
      case SceneFileActionWord::transform: {

      } break;

        // Execute 'animate' statement.
      case SceneFileActionWord::animate: {

      } break;
    }
  }

  // Close file.
  input_file.close();

  std::vector<scene_description> result;

  return {success, result};
}
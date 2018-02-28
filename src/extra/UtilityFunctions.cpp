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
 * Reads a text file following given protocol (see Report) and extracts
 * scene descriptions(s) from it.
 * @param filename: The text file containing scene description(s).
 * @return:         A (1) pair of a
 *                      (1) pair of
 *                          (2) status code and
 *                          (2) the line at which an error occurred
 *                              (in case of an error) and a
 *                      (1) vector of scene descriptions; in case an error
 *                      occurred, an empty vector is returned.
 */
std::pair<std::pair<SceneParserStatusCodes, size_t>,
          std::vector<scene_description>>
        read_scene_from_file(const std::string &filename) {
  // Open file.
  std::ifstream input_file;
  input_file.open(filename, std::ifstream::in);

  // Check, if the input file exist and is not damaged in some way.
  if (!input_file.good()) {
    return {{file_problem, 0}, {}};
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
  size_t                                                      line_number{0};
  while (std::getline(input_file, line)) {
    // Increment the line number.
    line_number++;

    // Check if it's an empty line.
    if (line.empty()) continue;

    // Tokenize the line.
    std::stringstream tokenized_line(line);

    // Get the start word of a line, and check if it is a valid one.
    tokenized_line >> action_token;
    if (START_WORDS.find(action_token) == START_WORDS.end()) {
      return {{invalid_statement, line_number}, {}};
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
          return {{invalid_syntax, line_number}, {}};
        }
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end()) {
          return {{invalid_statement, line_number}, {}};
        }

        switch (AVAILABLE_THINGS.find(thing)->second) {
          case SceneThings::camera_d: {
            if (cameras.find(name) != cameras.end()) {
              return {{duplicate, line_number}, {}};
            }
            cameras.insert({name, camera_description(name)});
          } break;

          case SceneThings::color_d: {
            if (colors.find(name) != colors.end()) {
              return {{duplicate, line_number}, {}};
            }
            colors.insert({name, color_description(name)});
          } break;

          case SceneThings::vector_d: {
            if (vectors.find(name) != vectors.end()) {
              return {{duplicate, line_number}, {}};
            }
            vectors.insert({name, vector_description(name)});
          } break;

          case SceneThings::material_d: {
            if (materials.find(name) != materials.end()) {
              return {{duplicate, line_number}, {}};
            }
            materials.insert({name, material_description(name)});
          } break;

          case SceneThings::light_d: {
            if (lights.find(name) != lights.end()) {
              return {{duplicate, line_number}, {}};
            }
            lights.insert({name, light_description(name)});
          } break;

          case SceneThings::object_d: {
            if (objects.find(name) != objects.end()) {
              return {{duplicate, line_number}, {}};
            }
            objects.insert({name, object_description(name)});
          } break;

          case SceneThings::image_plane_d: {
            if (image_planes.find(name) != image_planes.end()) {
              return {{duplicate, line_number}, {}};
            }
            image_planes.insert({name, image_plane_description(name)});
          } break;

          case SceneThings::acceleration_structure_d: {
            if (acceleration_structures.find(name) != acceleration_structures.end()) {
              return {{duplicate, line_number}, {}};
            }
            acceleration_structures.insert({name, acceleration_structure_description(name)});
          } break;

          case SceneThings::animation_d: {
            if (animations.find(name) != animations.end()) {
              return {{duplicate, line_number}, {}};
            }
            animations.insert({name, animation_description(name)});
          } break;

          case SceneThings::scene_d: {
            if (scenes.find(name) != scenes.end()) {
              return {{duplicate, line_number}, {}};
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
          return {{invalid_syntax, line_number}, {}};
        }
        // Check if the thing to be set is supported.
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end()) {
          return {{invalid_statement, line_number}, {}};
        }

        switch (AVAILABLE_THINGS.find(thing)->second) {

          case SceneThings::camera_d: {
            if (cameras.find(name) == cameras.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (CAMERA_SET_PROPERTIES.find(property) == CAMERA_SET_PROPERTIES.end()) {
              return {{invalid_set_property, line_number}, {}};
            }
            // Set the camera type.
            if (CAMERA_SET_PROPERTIES.find(property) == CAMERA_SET_PROPERTIES.find("type")) {
              if (CAMERA_TYPES_MAP.find(property_value) == CAMERA_TYPES_MAP.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              cameras.find(name)->second.type = CAMERA_TYPES_MAP.find(property_value)->second;
            }
            // Check if camera type is set.
            if (cameras.find(name)->second.type == not_set_ct) {
              return {{invalid_set_property_value, line_number}, {}};
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
              return {{invalid_set_property_value, line_number}, {}};
            }
          } break;

          case SceneThings::color_d: {
            if (colors.find(name) == colors.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            // Color has only just one property: rgb.
            if (property != "rgb") {
              return {{invalid_set_property, line_number}, {}};
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
              return {{invalid_set_property_value, line_number}, {}};
            }
            // Check if the values are in range [0, 255]
            int r = std::stoi(color_values[0]);
            int g = std::stoi(color_values[1]);
            int b = std::stoi(color_values[2]);
            if ((r < 0) || (r > 255) ||
                (g < 0) || (g > 255) ||
                (b < 0) || (b > 255)) {
              return {{invalid_set_property_value, line_number}, {}};
            }
            // Set rgb values.
            colors.find(name)->second.r = r;
            colors.find(name)->second.b = b;
            colors.find(name)->second.g = g;
          } break;

          case SceneThings::vector_d: {
            if (vectors.find(name) == vectors.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            // Vector has just one property: coordinates.
            if (property != "coordinates") {
              return {{invalid_set_property, line_number}, {}};
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
              return {{invalid_set_property_value, line_number}, {}};
            }
            // Set vector's x,y,z values.
            vectors.find(name)->second.x = std::stof(vector_values[0]);
            vectors.find(name)->second.y = std::stof(vector_values[1]);
            vectors.find(name)->second.z = std::stof(vector_values[2]);
          } break;

          case SceneThings::material_d: {
            if (materials.find(name) == materials.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (MATERIAL_PROPERTIES_MAP.find(property) == MATERIAL_PROPERTIES_MAP.end() &&
                  property != "type" && property != "color") {
              return {{invalid_set_property, line_number}, {}};
            }

            // Set material's type.
            if (property == "type") {
              if (MATERIAL_TYPES_MAP.find(property_value) != MATERIAL_TYPES_MAP.end()) {
                materials.find(name)->second.type = MATERIAL_TYPES_MAP.find(property_value)->second;
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            }
            // Set material's color.
            // The color should exist.
            else if (property == "color") {
              if (colors.find(property_value) != colors.end()) {
                materials.find(name)->second.color = &(colors.find(property_value)->second);
              } else {
                return {{invalid_set_property_value, line_number}, {}};
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
            if (lights.find(name) == lights.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (LIGHT_PROPERTIES_MAP.find(property) == LIGHT_PROPERTIES_MAP.end() &&
                property != "type" && property != "color") {
              return {{invalid_set_property, line_number}, {}};
            }

            // Set light's type.
            if (property == "type") {
              if (LIGHT_TYPES_MAP.find(property_value) != LIGHT_TYPES_MAP.end()) {
                lights.find(name)->second.type = LIGHT_TYPES_MAP.find(property_value)->second;
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            }
            // Set light's color.
            // Color should exist.
            else if (property == "color") {
              if (colors.find(property_value) != colors.end()) {
                lights.find(name)->second.color = &(colors.find(property_value)->second);
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            } else {
              // Check if type is set before setting position or direction
              // property of a light.
              if (lights.find(name)->second.type == not_set_lt) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              // Check if the vector in the property value exists.
              if (vectors.find(property_value) == vectors.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }

              // Property and type should match.
              if ((property == "position" && lights.find(name)->second.type == point) ||
                  (property == "direction" && lights.find(name)->second.type != directional)) {
                LightProperty lp = LIGHT_PROPERTIES_MAP.find(property)->second;
                vector_description lp_value = vectors.find(property_value)->second;
                lights.find(name)->second.property.first = lp;
                lights.at(name).property.second = &(lp_value);
              } else {
                return {{invalid_set_property_value, line_number}, {}};
                }
            }
          } break;

          case SceneThings::object_d: {
            if (objects.find(name) == objects.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (OBJECT_PROPERTIES_MAP.find(property) == OBJECT_PROPERTIES_MAP.end() &&
                property != "type" && property != "material") {
              return {{invalid_set_property, line_number}, {}};
            }

            // Set object's type.
            if (property == "type") {
              if (OBJECT_TYPES_MAP.find(property_value) != OBJECT_TYPES_MAP.end()) {
                objects.at(name).type = OBJECT_TYPES_MAP.at(property_value);
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            }
            // Set object's material.
            // Material should exist.
            else if (property == "material") {
              if (materials.find(property_value) != materials.end()) {
                objects.at(name).material = &(materials.at(property_value));
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            } else {
              // Check if type is set.
              if (objects.at(name).type == not_set_ot) {
                return {{invalid_set_property_value, line_number}, {}};
              }

              // Check if type and property match.
              // SPHERE
              if (objects.at(name).type == sphere) {
                if (OBJECT_PROPERTIES_MAP.at(property) == radius) {
                  objects.at(name).radius = std::stof(property_value);
                } else if (OBJECT_PROPERTIES_MAP.at(property) == center) {
                  // Vector should be defined before.
                  if (vectors.find(property_value) == vectors.end()) {
                    return {{invalid_set_property_value, line_number}, {}};
                  }
                  objects.at(name).center = &(vectors.at(property_value));
                } else {
                  return {{invalid_set_property_value, line_number}, {}};
                }
              }
              // TRIANGLE
              else if (objects.at(name).type == triangle) {
                if (OBJECT_PROPERTIES_MAP.at(property) == vertices) {
                  // Extract vertices(vectors) values from string.
                  // A comma ',' is used as a delimiter.
                  std::vector<std::string> vertices_values;
                  std::istringstream vertices_values_stream(property_value);
                  std::string vertex_token;
                  while (std::getline(vertices_values_stream, vertex_token, ',')) {
                    vertices_values.push_back(vertex_token);
                  }

                  // Check if only 3 values are set (v1,v2,v3)
                  if (vertices_values.size() != 3) {
                    return {{invalid_set_property_value, line_number}, {}};
                  }

                  // Check if the vertices exist.
                  if (vectors.find(vertices_values[0]) == vectors.end() ||
                      vectors.find(vertices_values[1]) == vectors.end() ||
                      vectors.find(vertices_values[2]) == vectors.end()) {
                    return {{invalid_set_property_value, line_number}, {}};
                  }

                  // Set the vertices.
                  objects.at(name).vertices = new vector_description*[3];
                  objects.at(name).vertices[0] = &(vectors.at(vertices_values[0]));
                  objects.at(name).vertices[1] = &(vectors.at(vertices_values[1]));
                  objects.at(name).vertices[2] = &(vectors.at(vertices_values[2]));
                } else {
                  return {{invalid_set_property_value, line_number}, {}};
                }
              }
              // TRIANGLE MESH
              else if (objects.at(name).type == triangle_mesh) {
                if (OBJECT_PROPERTIES_MAP.at(property) == file_name) {
                  objects.at(name).file_name = property_value;
                } else if (OBJECT_PROPERTIES_MAP.at(property) == interpolation) {
                  int interpolation = std::stoi(property_value);
                  if (interpolation != 0) interpolation = 1;
                  objects.at(name).interpolation = interpolation;
                } else {
                  return {{invalid_set_property_value, line_number}, {}};
                }
              }
            }
          } break;

          case SceneThings::image_plane_d: {
            if (image_planes.find(name) == image_planes.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (IMAGE_PLANE_PROPERTIES.find(property) == IMAGE_PLANE_PROPERTIES.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            // OUTPUT TYPE
            if (IMAGE_PLANE_PROPERTIES.at(property) == output_type) {
              // Check if desired output type is available.
              if (IMAGE_PLANE_OUT_TYPES.find(property_value) == IMAGE_PLANE_OUT_TYPES.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }

              if (IMAGE_PLANE_OUT_TYPES.at(property_value) == ppm_o) {
                image_planes.at(name).output_type = ppm_o;
              } else if (IMAGE_PLANE_OUT_TYPES.at(property_value) == png_o) {
                image_planes.at(name).output_type = png_o;
              }
            }
            // RESOLUTION HORIZONTAL
            else if (IMAGE_PLANE_PROPERTIES.at(property) == horizontal) {
              image_planes.at(name).horizontal =
                  static_cast<uint32_t>(std::stoi(property_value));
            }
            // RESOLUTION VERTICAL
            else if (IMAGE_PLANE_PROPERTIES.at(property) == vertical) {
              image_planes.at(name).vertical =
                  static_cast<uint32_t>(std::stoi(property_value));
            }
            // GAMMA
            else if (IMAGE_PLANE_PROPERTIES.at(property) == gamma) {
              int gamma_value = std::stoi(property_value);
              if (gamma_value < 0) gamma_value = 0;
              if (gamma_value > 2) gamma_value = 2;
              image_planes.at(name).gamma = gamma_value;
            }
            // NUMBER SAMPLES
            else if (IMAGE_PLANE_PROPERTIES.at(property) == number_samples) {
              image_planes.at(name).number_samples =
                  static_cast<uint32_t>(std::stoi(property_value));
            }
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

  return {{success, line_number}, result};
}
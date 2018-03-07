// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <string>
#include <map>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <memory>


#include "../../include/elucido/Utilities.h"

/**
 * TODO: clean-up code, where possible
 * TODO: make code, a bit more readable; comment+spacing
 * TODO: look at CameraProperty and CameraSetProperties; why?
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

        switch (AVAILABLE_THINGS.at(thing)) {

          case SceneThings::camera_d: {
            if (cameras.find(name) == cameras.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (CAMERA_SET_PROPERTIES.find(property) == CAMERA_SET_PROPERTIES.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            // CAMERA TYPE
            if (CAMERA_SET_PROPERTIES.at(property) == camera_type) {
              if (CAMERA_TYPES_MAP.find(property_value) == CAMERA_TYPES_MAP.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              cameras.at(name).type = CAMERA_TYPES_MAP.at(property_value);
            } else {
              // Check if camera type is set.
              if (cameras.at(name).type == not_set_ct) {
                return {{invalid_set_property_value, line_number}, {}};
              }

              // ZOOM FACTOR
              if (CAMERA_SET_PROPERTIES.at(property) == camera_zoom_factor &&
                  cameras.at(name).type == orthographic) {
                cameras.at(name).property.first = zoom_factor;
                cameras.at(name).property.second =
                    static_cast<float_t>(std::atof(property_value.c_str()));
              }
              // FIELD OF VIEW
              else if (CAMERA_SET_PROPERTIES.at(property) == camera_fov &&
                  cameras.at(name).type == perspective) {
                cameras.at(name).property.first = field_of_view;
                cameras.at(name).property.second =
                    static_cast<float_t>(std::atof(property_value.c_str()));
              }
              // CAMERA TYPE AND PARAMETER SHOULD MATCH
              else {
                return {{invalid_set_property_value, line_number}, {}};
              }
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
            colors.at(name).r = r;
            colors.at(name).b = b;
            colors.at(name).g = g;
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
            vectors.at(name).x = std::stof(vector_values[0]);
            vectors.at(name).y = std::stof(vector_values[1]);
            vectors.at(name).z = std::stof(vector_values[2]);
          } break;

          case SceneThings::material_d: {
            if (materials.find(name) == materials.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (MATERIAL_PROPERTIES_MAP.find(property) == MATERIAL_PROPERTIES_MAP.end() &&
                  property != "type" && property != "color") {
              return {{invalid_set_property, line_number}, {}};
            }

            // MATERIAL TYPE
            if (property == "type") {
              if (MATERIAL_TYPES_MAP.find(property_value) != MATERIAL_TYPES_MAP.end()) {
                materials.at(name).type = MATERIAL_TYPES_MAP.at(property_value);
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            }
            // MATERIAL COLOR
            // The color should exist.
            else if (property == "color") {
              if (colors.find(property_value) != colors.end()) {
                materials.at(name).color = std::make_shared<color_description>(colors.at(property_value));
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            }
            // REMAINING PROPERTIES
            else {
              MaterialProperty mp = MATERIAL_PROPERTIES_MAP.at(property);
              float_t mp_value    = std::stof(property_value.c_str());
              materials.at(name).properties[mp] = mp_value;
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

            // LIGHT TYPE
            if (property == "type") {
              if (LIGHT_TYPES_MAP.find(property_value) != LIGHT_TYPES_MAP.end()) {
                lights.at(name).type = LIGHT_TYPES_MAP.at(property_value);
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            }
            // LIGHT COLOR
            else if (LIGHT_PROPERTIES_MAP.at(property) == color) {
              if (colors.find(property_value) != colors.end()) {
                lights.at(name).color = std::make_shared<color_description>(colors.at(property_value));
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            }
            else if (LIGHT_PROPERTIES_MAP.at(property) == intensity) {
              lights.at(name).intensity = std::stof(property_value);
            }
            // POSITION & DIRECTION
            else {
              // CHECK IF TYPE AND PROPERTY MATCH
              if (lights.find(name)->second.type == not_set_lt) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              if (vectors.find(property_value) == vectors.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }

              if ((LIGHT_PROPERTIES_MAP.at(property) == position && lights.at(name).type == point) ||
                  (LIGHT_PROPERTIES_MAP.at(property) == direction && lights.at(name).type == directional)) {
                LightProperty lp = LIGHT_PROPERTIES_MAP.at(property);
                vector_description lp_value = vectors.at(property_value);
                lights.find(name)->second.property.first = lp;
                lights.at(name).property.second = std::make_shared<vector_description>(lp_value);
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

            // OBJECT'S TYPE
            if (property == "type") {
              if (OBJECT_TYPES_MAP.find(property_value) != OBJECT_TYPES_MAP.end()) {
                objects.at(name).type = OBJECT_TYPES_MAP.at(property_value);
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            }
            // OBJECT'S MATERIAL
            else if (property == "material") {
              if (materials.find(property_value) != materials.end()) {
                objects.at(name).material = std::make_shared<material_description>(materials.at(property_value));
              } else {
                return {{invalid_set_property_value, line_number}, {}};
              }
            } else {
              if (objects.at(name).type == not_set_ot) {
                return {{invalid_set_property_value, line_number}, {}};
              }

              // SPHERE
              if (objects.at(name).type == sphere) {
                if (OBJECT_PROPERTIES_MAP.at(property) == radius) {
                  objects.at(name).radius = std::stof(property_value);
                } else if (OBJECT_PROPERTIES_MAP.at(property) == center) {
                  // Vector should be defined before.
                  if (vectors.find(property_value) == vectors.end()) {
                    return {{invalid_set_property_value, line_number}, {}};
                  }
                  objects.at(name).center = std::make_shared<vector_description>(vectors.at(property_value));
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
            if (IMAGE_PLANE_PROPERTIES_MAP.find(property) == IMAGE_PLANE_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            // OUTPUT TYPE
            if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == output_type) {
              // Check if desired output type is available.
              if (IMAGE_PLANE_OUT_TYPES_MAP.find(property_value) == IMAGE_PLANE_OUT_TYPES_MAP.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }

              if (IMAGE_PLANE_OUT_TYPES_MAP.at(property_value) == ppm_o) {
                image_planes.at(name).output_type = ppm_o;
              } else if (IMAGE_PLANE_OUT_TYPES_MAP.at(property_value) == png_o) {
                image_planes.at(name).output_type = png_o;
              }
            }
            // RESOLUTION HORIZONTAL
            else if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == horizontal) {
              image_planes.at(name).horizontal =
                  static_cast<uint32_t>(std::stoi(property_value));
            }
            // RESOLUTION VERTICAL
            else if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == vertical) {
              image_planes.at(name).vertical =
                  static_cast<uint32_t>(std::stoi(property_value));
            }
            // GAMMA
            else if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == use_gamma) {
              int gamma_value = std::stoi(property_value);
              if (gamma_value < 0 || gamma_value > 1)
                return {{invalid_set_property_value, line_number}, {}};
              image_planes.at(name).use_gamma = gamma_value;
            }
            // NUMBER SAMPLES
            else if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == number_samples) {
              image_planes.at(name).number_samples =
                  static_cast<uint32_t>(std::stoi(property_value));
            }
          } break;

          case SceneThings::acceleration_structure_d: {
            if (acceleration_structures.find(name) == acceleration_structures.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (AC_PROPERTIES_MAP.find(property) == AC_PROPERTIES_MAP.end() &&
                property != "type") {
              return {{invalid_set_property, line_number}, {}};
            }

            // TYPE
            if (property == "type") {
              if (AC_TYPES_MAP.find(property_value) == AC_TYPES_MAP.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }

              if (AC_TYPES_MAP.at(property_value) == grid) {
                acceleration_structures.at(name).type = grid;
              }
            }
            else {
              // Check if type is defined.
              if (acceleration_structures.at(name).type == not_set_act) {
                return {{invalid_set_property_value, line_number}, {}};
              }

              // ALPHA
              if (AC_PROPERTIES_MAP.at(property) == alpha) {
                acceleration_structures.at(name).alpha = std::stof(property_value);
              }
              // MAX RESOLUTION
              else if (AC_PROPERTIES_MAP.at(property) == max_resolution) {
                acceleration_structures.at(name).max_resolution =
                    static_cast<uint32_t>(std::stoi(property_value));
              }
            }
          } break;


          case SceneThings::animation_d: {
            if (animations.find(name) == animations.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (ANIMATION_PROPERTIES_MAP.find(property) == ANIMATION_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            // NUMBER OF IMAGES IN SEQUENCE
            if (ANIMATION_PROPERTIES_MAP.at(property) == number_of_images_in_seq) {
              int noiis_value = std::stoi(property_value);
              animations.at(name).num_of_images_in_sequence =
                  static_cast<uint32_t>(noiis_value);
            }
          } break;

          case SceneThings::scene_d: {
            if (scenes.find(name) == scenes.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (SCENE_PROPERTIES_MAP.find(property) == SCENE_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            // CAMERA
            if (SCENE_PROPERTIES_MAP.at(property) == scene_camera) {
              // Check if camera exists.
              if (cameras.find(property_value) == cameras.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              scenes.at(name).camera = std::make_shared<camera_description>(cameras.at(property_value));
            }
            // IMAGE PLANE
            else if (SCENE_PROPERTIES_MAP.at(property) == scene_image_plane) {
              // Check if image plane exists.
              if (image_planes.find(property_value) == image_planes.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              scenes.at(name).image_plane = std::make_shared<image_plane_description>(image_planes.at(property_value));
            }
            // ACCELERATION STRUCTURE
            else if (SCENE_PROPERTIES_MAP.at(property) == scene_as) {
              // Check if acceleration structure exists.
              if (acceleration_structures.find(property_value) == acceleration_structures.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              scenes.at(name).acceleration_structure = std::make_shared<acceleration_structure_description>(acceleration_structures.at(property_value));
            }
            // OBJECT
            else if (SCENE_PROPERTIES_MAP.at(property) == scene_objects) {
              // Check if object exists.
              if (objects.find(property_value) == objects.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              scenes.at(name).objects.push_back(objects.at(property_value));
            }
            // LIGHT
            else if (SCENE_PROPERTIES_MAP.at(property) == scene_lights) {
              // Check if light exists.
              if (lights.find(property_value) == lights.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              scenes.at(name).lights.push_back(lights.at(property_value));
            }
            // ANIMATION
            else if (SCENE_PROPERTIES_MAP.at(property) == scene_animations) {
              // Check if animation exists.
              if (animations.find(property_value) == animations.end()) {
                return {{invalid_set_property_value, line_number}, {}};
              }
              scenes.at(name).animations.push_back(animations.at(property_value));
            }
          } break;
        }

      } break;

        // Execute 'transform' statement.
      case SceneFileActionWord::transform: {
        std::string thing;
        std::string name;
        std::string type;
        std::string axes;
        std::string amount;

        tokenized_line >> thing;
        tokenized_line >> name;
        tokenized_line >> type;
        tokenized_line >> axes;
        tokenized_line >> amount;

        // If either of the strings is empty, the transform statement is invalid.
        if (thing.empty() || name.empty() || type.empty() || axes.empty() ||
            amount.empty()) {
          return {{invalid_syntax, line_number}, {}};
        }
        // Check if the thing to be transform is supported.
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end()) {
          return {{invalid_statement, line_number}, {}};
        }
        // Check if the transformation type is valid.
        if (TRANSFORMATION_TYPES_MAP.find(type) == TRANSFORMATION_TYPES_MAP.end()) {
          return {{invalid_transformation_type, line_number}, {}};
        }
        // Check if the transformation axis is valid.
        if (AXES_MAP.find(axes) == AXES_MAP.end()) {
          return {{invalid_transformation_axix, line_number}, {}};
        }

        switch (AVAILABLE_THINGS.at(thing)) {
          case SceneThings::light_d: {
            // Check if the light to be transformed exists.
            if (lights.find(name) == lights.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            // Check if the transformation type is valid.
            // Light supports only rotation and translation.
            if (TRANSFORMATION_TYPES_MAP.at(type) == scale) {
              return {{invalid_transformation_type, line_number}, {}};
            }

            // Create transformation.
            transformation_description transform_light = {};
            transform_light.type = TRANSFORMATION_TYPES_MAP.at(type);
            transform_light.axis = AXES_MAP.at(axes);
            transform_light.amount = std::stof(amount);

            // Apply transformation to light.
            lights.at(name).transformations.push_back(transform_light);
          } break;

          case SceneThings::object_d: {
            // Check if the object to be transformed exists.
            if (objects.find(name) == objects.end()) {
              return {{thing_not_created, line_number}, {}};
            }

            // Create transformation.
            transformation_description transform_object = {};
            transform_object.type = TRANSFORMATION_TYPES_MAP.at(type);
            transform_object.axis = AXES_MAP.at(axes);
            transform_object.amount = std::stof(amount);

            // Apply transformation to object.
            objects.at(name).transformations.push_back(transform_object);
          } break;

          case SceneThings::camera_d: {
            // Check if camera to be transformed exists.
            if (cameras.find(name) == cameras.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            // Check if the transformation type is valid.
            // Camera supports only rotation and translation.
            if (TRANSFORMATION_TYPES_MAP.at(type) == scale) {
              return {{invalid_transformation_type, line_number}, {}};
            }

            // Create transformation.
            transformation_description transform_camera = {};
            transform_camera.type = TRANSFORMATION_TYPES_MAP.at(type);
            transform_camera.axis = AXES_MAP.at(axes);
            transform_camera.amount = std::stof(amount);

            // Apply transformation to camera.
            cameras.at(name).transformations.push_back(transform_camera);
          } break;

          default: return {{invalid_thing_to_transform, line_number}, {}};
        }
      } break;

      // Execute 'animate' statement.
      case SceneFileActionWord::animate: {
        std::string anim_name;
        std::string thing;
        std::string thing_name;
        std::string trans_type;
        std::string axes;
        std::string amount;

        tokenized_line >> anim_name;
        tokenized_line >> thing;
        tokenized_line >> thing_name;
        tokenized_line >> trans_type;
        tokenized_line >> axes;
        tokenized_line >> amount;

        // If either of the strings is empty, the animate statement is invalid.
        if (anim_name.empty() || thing.empty() || thing_name.empty() ||
            trans_type.empty() || axes.empty() || amount.empty()) {
          return {{invalid_syntax, line_number}, {}};
        }
        // Check if the animation exists.
        if (animations.find(anim_name) == animations.end()) {
          return {{thing_not_created, line_number}, {}};
        }
        // Check if the thing to be animated is supported.
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end()) {
          return {{invalid_statement, line_number}, {}};
        }
        // Check if the transformation type is valid.
        if (TRANSFORMATION_TYPES_MAP.find(trans_type) == TRANSFORMATION_TYPES_MAP.end()) {
          return {{invalid_transformation_type, line_number}, {}};
        }
        // Check if the transformation axis is valid.
        if (AXES_MAP.find(axes) == AXES_MAP.end()) {
          return {{invalid_transformation_axix, line_number}, {}};
        }

        switch (AVAILABLE_THINGS.at(thing)) {
          case SceneThings::light_d: {
            // Check if the light to be animated exists.
            if (lights.find(thing_name) == lights.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            // Check if the transformation type is valid.
            // Light supports only rotation and translation.
            if (TRANSFORMATION_TYPES_MAP.at(trans_type) == scale) {
              return {{invalid_transformation_type, line_number}, {}};
            }

            // Create transformation.
            transformation_description animate_light = {};
            animate_light.type = TRANSFORMATION_TYPES_MAP.at(trans_type);
            animate_light.axis = AXES_MAP.at(axes);
            animate_light.amount = std::stof(amount);

            // Apply transformation to the animation.
            animations.at(anim_name).lights[thing_name].push_back(animate_light);
          } break;

          case SceneThings::object_d: {
            // Check if the object to be animated exists.
            if (objects.find(thing_name) == objects.end()) {
              return {{thing_not_created, line_number}, {}};
            }

            // Create transformation.
            transformation_description animate_object = {};
            animate_object.type = TRANSFORMATION_TYPES_MAP.at(trans_type);
            animate_object.axis = AXES_MAP.at(axes);
            animate_object.amount = std::stof(amount);

            // Apply transformation to the animation.
            animations.at(anim_name).objects[thing_name].push_back(animate_object);
          } break;

          case SceneThings::camera_d: {
            // Check if the camera to be animated exists.
            if (cameras.find(thing_name) == cameras.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            // Check if the transformation type is valid.
            // Camera supports only rotation and translation.
            if (TRANSFORMATION_TYPES_MAP.at(trans_type) == scale) {
              return {{invalid_transformation_type, line_number}, {}};
            }

            // Create transformation.
            transformation_description animate_camera = {};
            animate_camera.type = TRANSFORMATION_TYPES_MAP.at(trans_type);
            animate_camera.axis = AXES_MAP.at(axes);
            animate_camera.amount = std::stof(amount);

            if (animations.at(anim_name).camera.first.empty()) {
              animations.at(anim_name).camera.first = thing_name;
            } else {
              // Check if the camera name matches.
              if (animations.at(anim_name).camera.first != thing_name) {
                return {{invalid_animation_camera, line_number}, {}};
              }
            }

            // Apply transformation to the animation.
            animations.at(anim_name).camera.second.push_back(animate_camera);
          } break;

          default: return {{invalid_thing_to_transform, line_number}, {}};
        }
      } break;
    }
  }

  // Close file.
  input_file.close();

  std::vector<scene_description> result_scenes;

  // Place the scene description(s) in a result vector;
  for (auto const &scene : scenes) {
    result_scenes.push_back(scene.second);
  }

  return {{success, line_number}, result_scenes};
}
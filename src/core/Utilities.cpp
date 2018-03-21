// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Utilities.h"

//==============================================================================
transformation_description create_transformation_desc(
    const std::string &transformation_type,
    const std::string &axes,
    const std::string &amount) {
  transformation_description td = {};
  td.type   = TRANSFORMATION_TYPES_MAP.at(transformation_type);
  td.axis   = AXES_MAP.at(axes);
  td.amount = std::stof(amount);
  return td;
}

//==============================================================================
bool set_camera_property(
    const std::string &name,
    const std::string &property,
    const std::string &property_value,
    std::map<std::string, camera_description> &cameras
) {

  /// Type.
  if (CAMERA_SET_PROPERTIES.at(property) == camera_type) {
    if (CAMERA_TYPES_MAP.find(property_value) == CAMERA_TYPES_MAP.end()) {
      return false;
    }
    cameras.at(name).type = CAMERA_TYPES_MAP.at(property_value);
  } else {
    // Check if camera type is set.
    if (cameras.at(name).type == not_set_ct) {
      return false;
    }

    /// Zoom factor; set the property.
    if (CAMERA_SET_PROPERTIES.at(property) == camera_zoom_factor &&
        cameras.at(name).type == orthographic) {
      cameras.at(name).property.first = zoom_factor;
    }
    /// Field of view; set the property.
    else if (CAMERA_SET_PROPERTIES.at(property) == camera_fov &&
             cameras.at(name).type == perspective) {
      cameras.at(name).property.first = field_of_view;
    }
    // Camera type and parameter should match.
    else {
      return false;
    }

    // If everything went fine, set the property value.
    float prop_val = static_cast<float_t>(std::atof(property_value.c_str()));
    cameras.at(name).property.second = prop_val;
  }
  return true;
}

//==============================================================================
std::vector<std::string> split_string(
    const std::string &string,
    const char &delim) {
  std::vector<std::string> values;
  std::istringstream values_stream(string);
  std::string val_token;
  while (std::getline(values_stream, val_token, delim)) {
    values.push_back(val_token);
  }
  return values;
}

//==============================================================================
bool set_color_property(const std::string &property_value,
                        const std::string &name,
                        std::map<std::string, color_description> &colors) {
  auto color_val = split_string(property_value, ',');
  if (color_val.size() != 3) return false;

  // Check if the values are in range [0, 255]
  int r = std::stoi(color_val[0]);
  int g = std::stoi(color_val[1]);
  int b = std::stoi(color_val[2]);
  if ((r < 0) || (r > 255) ||
      (g < 0) || (g > 255) ||
      (b < 0) || (b > 255)) {
    return false;
  }
  // Set rgb values.
  colors.at(name).r = r;
  colors.at(name).b = b;
  colors.at(name).g = g;
  return true;
}

//==============================================================================
bool set_vector_property(const std::string &property_value,
                         const std::string &name,
                         std::map<std::string, vector_description> &vectors) {

  auto vector_val = split_string(property_value, ',');
  if (vector_val.size() != 3) return false;

  vectors.at(name).x = std::stof(vector_val[0]);
  vectors.at(name).y = std::stof(vector_val[1]);
  vectors.at(name).z = std::stof(vector_val[2]);
  return true;
}

//==============================================================================
bool set_material_property(const std::string &property,
                           const std::string &property_value,
                           const std::string &name,
                           const std::map<std::string, color_description> &colors,
                           std::map<std::string, material_description> &materials) {

  /// Material type.
  if (MATERIAL_PROPERTIES_MAP.at(property) == mat_type &&
      MATERIAL_TYPES_MAP.find(property_value) != MATERIAL_TYPES_MAP.end()) {
    materials.at(name).type = MATERIAL_TYPES_MAP.at(property_value);
  /// Material color.
  } else if (MATERIAL_PROPERTIES_MAP.at(property) == mat_color &&
             colors.find(property_value) != colors.end()) {
    materials.at(name).color = std::make_shared<color_description>(colors.at(property_value));
  /// Remaining properties.
  } else if (MATERIAL_PROPERTIES_MAP.at(property) != mat_type &&
           MATERIAL_PROPERTIES_MAP.at(property) != mat_color) {
    MaterialProperty mp = MATERIAL_PROPERTIES_MAP.at(property);
    float_t mp_value    = std::stof(property_value.c_str());
    materials.at(name).properties[mp] = mp_value;
  } else {
    return false;
  }
  return true;
}

//==============================================================================
bool set_light_property(const std::string &property,
                        const std::string &property_value,
                        const std::string &name,
                        const std::map<std::string, color_description> &colors,
                        const std::map<std::string, vector_description> &vectors,
                        std::map<std::string, light_description> &lights) {
  /// Light type.
  if (LIGHT_PROPERTIES_MAP.at(property) == light_type &&
      LIGHT_TYPES_MAP.find(property_value) != LIGHT_TYPES_MAP.end()) {
    lights.at(name).type = LIGHT_TYPES_MAP.at(property_value);
  /// Light color.
  } else if (LIGHT_PROPERTIES_MAP.at(property) == light_color &&
             colors.find(property_value) != colors.end()) {
    lights.at(name).color = std::make_shared<color_description>(colors.at(property_value));
  /// Light intensity.
  } else if (LIGHT_PROPERTIES_MAP.at(property) == intensity) {
    lights.at(name).intensity = std::stof(property_value);
  /// Position & Direction.
  } else if ((LIGHT_PROPERTIES_MAP.at(property) == position && lights.at(name).type == point) ||
             (LIGHT_PROPERTIES_MAP.at(property) == direction && lights.at(name).type == directional)) {
    // Check if type is set and there is a defined vector
    // for the position/direction.
    if (lights.find(name)->second.type == not_set_lt ||
        vectors.find(property_value) == vectors.end()) {
      return false;
    }

    LightProperty lp = LIGHT_PROPERTIES_MAP.at(property);
    vector_description lp_value = vectors.at(property_value);
    lights.find(name)->second.property.first = lp;
    lights.at(name).property.second = std::make_shared<vector_description>(lp_value);
    } else {
      return false;
    }
  return true;
}

//==============================================================================
bool set_object_property(const std::string &property,
                         const std::string &property_value,
                         const std::string &name,
                         const std::map<std::string, vector_description> &vectors,
                         const std::map<std::string, material_description> &materials,
                         std::map<std::string, object_description> &objects) {

  /// Object type.
  if (OBJECT_PROPERTIES_MAP.at(property) == object_type &&
      OBJECT_TYPES_MAP.find(property_value) != OBJECT_TYPES_MAP.end()) {
    objects.at(name).type = OBJECT_TYPES_MAP.at(property_value);
  /// Object material.
  } else if (OBJECT_PROPERTIES_MAP.at(property) == object_mat &&
             materials.find(property_value) != materials.end()) {
    objects.at(name).material = std::make_shared<material_description>(materials.at(property_value));
  } else if (objects.at(name).type != not_set_ot) {

    // Sphere.
    if (objects.at(name).type == sphere) {
      /// Sphere radius.
      if (OBJECT_PROPERTIES_MAP.at(property) == radius) {
        objects.at(name).radius = std::stof(property_value);
      /// Sphere center.
      } else if (OBJECT_PROPERTIES_MAP.at(property) == center &&
                 vectors.find(property_value) != vectors.end()) {
        objects.at(name).center = std::make_shared<vector_description>(vectors.at(property_value));
      } else {
        return false;
      }
    // Triangle.
    } else if (objects.at(name).type == triangle) {
      /// Triangle vertices.
      if (OBJECT_PROPERTIES_MAP.at(property) == vertices) {
        auto vertices = split_string(property_value, ',');
        if (vertices.size() != 3) return false;

        // Check if the vertices exist.
        if (vectors.find(vertices[0]) == vectors.end() ||
            vectors.find(vertices[1]) == vectors.end() ||
            vectors.find(vertices[2]) == vectors.end()) {
          return false;
        }

        // Overwrites the nullptr will be at position 0.
        objects.at(name).vertices[0] = std::make_shared<vector_description>(vectors.at(vertices[0]));
        objects.at(name).vertices.push_back(std::make_shared<vector_description>(vectors.at(vertices[1])));
        objects.at(name).vertices.push_back(std::make_shared<vector_description>(vectors.at(vertices[2])));
      } else {
        return false;
      }
    // Triangle mesh.
    } else if (objects.at(name).type == triangle_mesh) {
      /// Triangle mesh file name.
      if (OBJECT_PROPERTIES_MAP.at(property) == file_name) {
        objects.at(name).file_name = property_value;
      /// Triangle mesh interpolation.
      } else if (OBJECT_PROPERTIES_MAP.at(property) == interpolation) {
        int interpolation = std::stoi(property_value);
        if (interpolation != 0) interpolation = 1;
        objects.at(name).interpolation = interpolation;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

//==============================================================================
bool set_image_plane_property(const std::string &property,
                              const std::string &property_value,
                              const std::string &name,
                              std::map<std::string, image_plane_description> &image_planes) {

  /// Output type.
  if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == output_type &&
      IMAGE_PLANE_OUT_TYPES_MAP.find(property_value) != IMAGE_PLANE_OUT_TYPES_MAP.end()) {
    if (IMAGE_PLANE_OUT_TYPES_MAP.at(property_value) == ppm_o) {
      image_planes.at(name).output_type = ppm_o;
    } else if (IMAGE_PLANE_OUT_TYPES_MAP.at(property_value) == png_o) {
      image_planes.at(name).output_type = png_o;
    }
  /// Horizontal resolution.
  } else if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == horizontal) {
    auto val = static_cast<uint32_t>(std::stoi(property_value));
    image_planes.at(name).horizontal = val;
  /// Vertical resolution.
  } else if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == vertical) {
    auto val = static_cast<uint32_t>(std::stoi(property_value));
    image_planes.at(name).vertical = val;
  /// Gamma.
  } else if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == use_gamma) {
    auto val = std::stoi(property_value);
    if (val != 0) val = 1;
    image_planes.at(name).use_gamma = val;
  /// Number of samples.
  } else if (IMAGE_PLANE_PROPERTIES_MAP.at(property) == number_samples) {
    auto val = static_cast<uint32_t>(std::stoi(property_value));
    image_planes.at(name).number_samples = val;
  } else {
    // In case an invalid output type is specified.
    return false;
  }
  return true;
}

//==============================================================================
bool set_accel_str_property(const std::string &property,
                            const std::string &property_value,
                            const std::string &name,
                            std::map<std::string, acceleration_structure_description> &acc_strs) {
  /// Type.
  if (AC_PROPERTIES_MAP.at(property) == as_type &&
      AC_TYPES_MAP.find(property_value) != AC_TYPES_MAP.end()) {
    if (AC_TYPES_MAP.at(property_value) == grid) {
      acc_strs.at(name).type = grid;
    }
  } else if (acc_strs.at(name).type != not_set_act) {
    /// Alpha.
    if (AC_PROPERTIES_MAP.at(property) == alpha) {
      acc_strs.at(name).alpha = std::stof(property_value);
    /// Maximal resolution.
    } else if (AC_PROPERTIES_MAP.at(property) == max_resolution) {
      acc_strs.at(name).max_resolution =
          static_cast<uint32_t>(std::stoi(property_value));
    }
  } else {
    return false;
  }
  return true;
}

//==============================================================================
bool set_animation_property(const std::string &property,
                            const std::string &property_value,
                            const std::string &name,
                            std::map<std::string, animation_description> &animations) {
  /// Number of images in a sequence.
  if (ANIMATION_PROPERTIES_MAP.at(property) == number_of_images_in_seq) {
    auto val = static_cast<uint32_t>(std::stoi(property_value));
    animations.at(name).num_of_images_in_sequence = val;
  }
  return true;
}

//==============================================================================
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

    // Check if the line starts with a known start action word.
    if (START_WORDS.find(action_token) == START_WORDS.end()) {
      return {{invalid_statement, line_number}, {}};
    }
    SceneFileActionWord action = START_WORDS.find(action_token)->second;

    // Execute the line statement.
    switch (action) {

      // If the line is a comment, skip it.
      case SceneFileActionWord::comment:
        continue;

      // Execute 'create' statement.
      case SceneFileActionWord::create: {
        std::string thing, name;

        tokenized_line >> thing;
        tokenized_line >> name;

        if (thing.empty() || name.empty())
          return {{invalid_syntax, line_number}, {}};
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end())
          return {{invalid_statement, line_number}, {}};

        bool r;
        switch (AVAILABLE_THINGS.at(thing)) {
          case SceneThings::camera_d:
            r = cameras.emplace(name, name).second;
            break;
          case SceneThings::color_d:
            r = colors.emplace(name, name).second;
            break;
          case SceneThings::vector_d:
            r = vectors.emplace(name, name).second;
            break;
          case SceneThings::material_d:
            r = materials.emplace(name, name).second;
            break;
          case SceneThings::light_d:
            r = lights.emplace(name, name).second;
            break;
          case SceneThings::object_d:
            r = objects.emplace(name, name).second;
            break;
          case SceneThings::image_plane_d:
            r = image_planes.emplace(name, name).second;
            break;
          case SceneThings::acceleration_structure_d:
            r = acceleration_structures.emplace(name, name).second;
            break;
          case SceneThings::animation_d:
            r = animations.emplace(name, name).second;
            break;
          case SceneThings::scene_d:
            r = scenes.emplace(name, name).second;
            break;
        }
        if (!r) return {{duplicate, line_number}, {}};
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

        if (thing.empty() || name.empty() || property.empty() ||
            property_value.empty()) {
          return {{invalid_syntax, line_number}, {}};
        }
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end()) {
          return {{invalid_statement, line_number}, {}};
        }

        bool r = true;
        switch (AVAILABLE_THINGS.at(thing)) {

          // CAMERA.
          case SceneThings::camera_d: {
            if (cameras.find(name) == cameras.end()) {
              return {{thing_not_created, line_number}, {}};
            }

            if (CAMERA_SET_PROPERTIES.find(property) == CAMERA_SET_PROPERTIES.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            r = set_camera_property(name,
                                    property,
                                    property_value,
                                    cameras);
          } break;

          // COLOR.
          case SceneThings::color_d: {
            if (colors.find(name) == colors.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            /// RGB.
            if (property != "rgb") {
              return {{invalid_set_property, line_number}, {}};
            }

            r = set_color_property(property_value, name, colors);
          } break;

          case SceneThings::vector_d: {
            if (vectors.find(name) == vectors.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            /// Coordinates.
            if (property != "coordinates") {
              return {{invalid_set_property, line_number}, {}};
            }

            r = set_vector_property(property_value, name, vectors);
          } break;

          case SceneThings::material_d: {
            if (materials.find(name) == materials.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (MATERIAL_PROPERTIES_MAP.find(property) == MATERIAL_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            r = set_material_property(property,
                                      property_value,
                                      name,
                                      colors,
                                      materials);
          } break;

          case SceneThings::light_d: {
            if (lights.find(name) == lights.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (LIGHT_PROPERTIES_MAP.find(property) == LIGHT_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            r = set_light_property(property,
                                   property_value,
                                   name,
                                   colors,
                                   vectors,
                                   lights);
          } break;

          case SceneThings::object_d: {
            if (objects.find(name) == objects.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (OBJECT_PROPERTIES_MAP.find(property) == OBJECT_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            r = set_object_property(property,
                                    property_value,
                                    name,
                                    vectors,
                                    materials,
                                    objects);
          } break;

          case SceneThings::image_plane_d: {
            if (image_planes.find(name) == image_planes.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (IMAGE_PLANE_PROPERTIES_MAP.find(property) == IMAGE_PLANE_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            r = set_image_plane_property(property,
                                         property_value,
                                         name,
                                         image_planes);
          } break;

          case SceneThings::acceleration_structure_d: {
            if (acceleration_structures.find(name) == acceleration_structures.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (AC_PROPERTIES_MAP.find(property) == AC_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            r = set_accel_str_property(property,
                                       property_value,
                                       name,
                                       acceleration_structures);
          } break;


          case SceneThings::animation_d: {
            if (animations.find(name) == animations.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (ANIMATION_PROPERTIES_MAP.find(property) == ANIMATION_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            r = set_animation_property(property,
                                       property_value,
                                       name,
                                       animations);
          } break;

          case SceneThings::scene_d: {
            if (scenes.find(name) == scenes.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            if (SCENE_PROPERTIES_MAP.find(property) == SCENE_PROPERTIES_MAP.end()) {
              return {{invalid_set_property, line_number}, {}};
            }

            /// Camera.
            if (SCENE_PROPERTIES_MAP.at(property) == scene_camera &&
                cameras.find(property_value) != cameras.end()) {
              scenes.at(name).camera = std::make_shared<camera_description>(cameras.at(property_value));
            /// Image plane.
            } else if (SCENE_PROPERTIES_MAP.at(property) == scene_image_plane &&
                     image_planes.find(property_value) != image_planes.end()) {
              scenes.at(name).image_plane = std::make_shared<image_plane_description>(image_planes.at(property_value));
            /// Acceleration structure.
            } else if (SCENE_PROPERTIES_MAP.at(property) == scene_as &&
                     acceleration_structures.find(property_value) != acceleration_structures.end()) {
              scenes.at(name).acceleration_structure = std::make_shared<acceleration_structure_description>(acceleration_structures.at(property_value));
            /// Object.
            } else if (SCENE_PROPERTIES_MAP.at(property) == scene_objects &&
                     objects.find(property_value) != objects.end()) {
              scenes.at(name).objects.push_back(objects.at(property_value));
            /// Light.
            } else if (SCENE_PROPERTIES_MAP.at(property) == scene_lights &&
                       lights.find(property_value) != lights.end()) {
              scenes.at(name).lights.push_back(lights.at(property_value));
            /// Animation.
            } else if (SCENE_PROPERTIES_MAP.at(property) == scene_animations &&
                       animations.find(property_value) != animations.end()) {
              scenes.at(name).animations.push_back(animations.at(property_value));
            } else {
              r = false;
            }
          } break;
        }

        // Check for invalid set property.
        if (!r) return {{invalid_set_property_value, line_number}, {}};
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

        if (thing.empty() || name.empty() || type.empty() || axes.empty() ||
            amount.empty()) {
          return {{invalid_syntax, line_number}, {}};
        }
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end()) {
          return {{invalid_statement, line_number}, {}};
        }
        if (TRANSFORMATION_TYPES_MAP.find(type) == TRANSFORMATION_TYPES_MAP.end()) {
          return {{invalid_transformation_type, line_number}, {}};
        }
        if (AXES_MAP.find(axes) == AXES_MAP.end()) {
          return {{invalid_transformation_axix, line_number}, {}};
        }

        switch (AVAILABLE_THINGS.at(thing)) {
          case SceneThings::light_d: {
            if (lights.find(name) == lights.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            // Check if the transformation type is valid.
            // Light supports only rotation and translation.
            if (TRANSFORMATION_TYPES_MAP.at(type) == scale) {
              return {{invalid_transformation_type, line_number}, {}};
            }

            auto transform_light = create_transformation_desc(type,
                                                              axes,
                                                              amount);

            lights.at(name).transformations.push_back(transform_light);
          } break;

          case SceneThings::object_d: {
            if (objects.find(name) == objects.end()) {
              return {{thing_not_created, line_number}, {}};
            }

            auto transform_object = create_transformation_desc(type,
                                                               axes,
                                                               amount);

            objects.at(name).transformations.push_back(transform_object);
          } break;

          case SceneThings::camera_d: {
            if (cameras.find(name) == cameras.end()) {
              return {{thing_not_created, line_number}, {}};
            }
            // Check if the transformation type is valid.
            // Camera supports only rotation and translation.
            if (TRANSFORMATION_TYPES_MAP.at(type) == scale) {
              return {{invalid_transformation_type, line_number}, {}};
            }

            auto transform_camera = create_transformation_desc(type,
                                                               axes,
                                                               amount);

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

        if (anim_name.empty() || thing.empty() || thing_name.empty() ||
            trans_type.empty() || axes.empty() || amount.empty()) {
          return {{invalid_syntax, line_number}, {}};
        }
        if (animations.find(anim_name) == animations.end()) {
          return {{thing_not_created, line_number}, {}};
        }
        if (AVAILABLE_THINGS.find(thing) == AVAILABLE_THINGS.end()) {
          return {{invalid_statement, line_number}, {}};
        }
        if (TRANSFORMATION_TYPES_MAP.find(trans_type) == TRANSFORMATION_TYPES_MAP.end()) {
          return {{invalid_transformation_type, line_number}, {}};
        }
        if (AXES_MAP.find(axes) == AXES_MAP.end()) {
          return {{invalid_transformation_axix, line_number}, {}};
        }

        switch (AVAILABLE_THINGS.at(thing)) {
          case SceneThings::light_d: {
            if (lights.find(thing_name) == lights.end())
              return {{thing_not_created, line_number}, {}};

            // Check if the transformation type is valid.
            // Light supports only rotation and translation.
            if (TRANSFORMATION_TYPES_MAP.at(trans_type) == scale)
              return {{invalid_transformation_type, line_number}, {}};

            auto animate_light = create_transformation_desc(trans_type,
                                                            axes,
                                                            amount);

            animations.at(anim_name).lights[thing_name].push_back(animate_light);
          } break;

          case SceneThings::object_d: {
            if (objects.find(thing_name) == objects.end()) {
              return {{thing_not_created, line_number}, {}};
            }

            auto animate_object = create_transformation_desc(trans_type,
                                                             axes,
                                                             amount);

            animations.at(anim_name).objects[thing_name].push_back(animate_object);
          } break;

          case SceneThings::camera_d: {
            if (cameras.find(thing_name) == cameras.end())
              return {{thing_not_created, line_number}, {}};

            // Check if the transformation type is valid.
            // Camera supports only rotation and translation.
            if (TRANSFORMATION_TYPES_MAP.at(trans_type) == scale)
              return {{invalid_transformation_type, line_number}, {}};

            auto animate_camera = create_transformation_desc(trans_type,
                                                             axes,
                                                             amount);

            if (animations.at(anim_name).camera.first.empty()) {
              animations.at(anim_name).camera.first = thing_name;
            } else {
              // Check if the camera name matches.
              if (animations.at(anim_name).camera.first != thing_name) {
                return {{invalid_animation_camera, line_number}, {}};
              }
            }

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

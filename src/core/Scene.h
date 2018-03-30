// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ALL_SCENE_H
#define ELUCIDO_ALL_SCENE_H

#include <string>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "../cameras/Camera.h"
#include "../objects/Object.h"
#include "../lights/Light.h"
#include "ImagePlane.h"

class Scene {
 public:
  Scene() :
      name({}),
      objects({}),
      lights({}),
      image_plane(nullptr),
      camera(nullptr),
      acceleration_structure(nullptr),
      scene_bb(AABBox())
      {};

  ~Scene() = default;

  void add_object(const std::shared_ptr<Object> object);
  bool generate_object(const object_description &object);

  void add_light (const std::shared_ptr<Light>  light);
  bool generate_light(const light_description &light);

  void set_camera(const std::shared_ptr<Camera> _camera);
  bool generate_camera(const std::shared_ptr<camera_description> &c,
                       const uint32_t &image_width,
                       const uint32_t &image_height);

  void set_image_plane(const std::shared_ptr<ImagePlane> _ip);

  void set_as(const std::shared_ptr<AccelerationStructure> _ac);
  bool generate_as(const std::shared_ptr<acceleration_structure_description> &as);
  void print_as_construction_info(const as_construct_info &i,
                                    const AccelerationStructureType &type);
  void print_tm_loading_info(const mesh_loading_info &i,
                             const size_t &loading_time,
                             const std::string &fn);

  bool load_scene(const scene_description &description);
  void print_scene_info(const scene_info &i);
  void extend_scene_bb();
  void prepare_scene();
  void convert_color01_range(glm::vec3 &color);
  void render_image();
  void print_render_info(const render_info &ri, const size_t &render_time);

 private:
  std::string                             name;
  std::vector<std::shared_ptr<Object>>    objects;
  std::vector<std::shared_ptr<Light>>     lights;
  std::shared_ptr<ImagePlane>             image_plane;
  std::shared_ptr<Camera>                 camera;
  std::shared_ptr<AccelerationStructure>  acceleration_structure;
  AABBox                                  scene_bb;
  scene_info                              si;
};

#endif //ELUCIDO_ALL_SCENE_H

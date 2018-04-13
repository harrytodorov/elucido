// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_ALL_SCENE_H
#define ELUCIDO_ALL_SCENE_H

#include <string>
#include <unordered_map>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "../cameras/Camera.h"
#include "../cameras/OrthographicCamera.h"
#include "../cameras/PerspectiveCamera.h"

#include "../objects/Object.h"
#include "../objects/Sphere.h"
#include "../objects/Triangle.h"
#include "../objects/TriangleMesh.h"

#include "../lights/Light.h"
#include "../lights/PointLight.h"
#include "../lights/DirectionalLight.h"

#include "../accelerators/AccelerationStructure.h"
#include "../accelerators/DynamicGrid.h"
#include "../accelerators/CompactGrid.h"
#include "../accelerators/KDtreeMidpoint.h"

#include "Renderer.h"
#include "ImagePlane.h"

class Scene {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  Scene() :
      name({}),
      objects({}),
      lights({}),
      image_plane(nullptr),
      camera(nullptr),
      acceleration_structure(nullptr),
      animations({}),
      scene_bb(AABBox())
      {};

  ~Scene() = default;

//==============================================================================
// Function declarations
//==============================================================================
 public:
  void add_object(const std::shared_ptr<Object> object);
  void add_object_for_animation(const size_t &object_index,
                                const std::string &object_name);
  void add_light (const std::shared_ptr<Light>  light);
  void add_light_for_animation(const size_t &light_index,
                               const std::string &light_name);

  void set_camera(const std::shared_ptr<Camera> _camera,
                  const std::string &_camera_name);
  void set_image_plane(const std::shared_ptr<ImagePlane> _ip);
  void set_as(const std::shared_ptr<AccelerationStructure> _ac);

  bool load_scene(const scene_description &description);
  inline const std::string& get_name() { return name; }
  void render_image(const std::string &image_name);
  void render_image_sequence(const size_t &animation_index);

 private:
  bool generate_object(const object_description &object);
  void apply_object_transformations(const std::shared_ptr<Object> &ptr,
                                    const std::vector<transformation_description> &desc);
  bool generate_light(const light_description &light);
  void apply_light_transformations(const std::shared_ptr<Light> &ptr,
                                   const std::vector<transformation_description> &desc);
  bool generate_camera(const std::shared_ptr<camera_description> &c,
                       const uint32_t &image_width,
                       const uint32_t &image_height);
  void apply_camera_transformations(const std::shared_ptr<Camera> &ptr,
                                    const std::vector<transformation_description> &desc);
  bool generate_as(const std::shared_ptr<acceleration_structure_description> &as);

  void extend_scene_bb();
  void prepare_scene();
  void convert_color01_range(glm::vec3 &color);

  void print_render_info(const render_info &ri, const size_t &render_time);
  void print_scene_info(const scene_info &i);
  void print_as_construction_info(const as_construct_info &i,
                                  const AccelerationStructureType &type);
  void print_tm_loading_info(const mesh_loading_info &i,
                             const size_t &loading_time,
                             const std::string &fn);

//==============================================================================
// Data members
//==============================================================================
 private:
  std::string                             name;
  std::vector<std::shared_ptr<Object>>    objects;
  std::vector<std::shared_ptr<Light>>     lights;
  std::shared_ptr<Camera>                 camera;
  std::shared_ptr<ImagePlane>             image_plane;
  std::shared_ptr<AccelerationStructure>  acceleration_structure;
  std::unordered_map<std::string, size_t> animated_objects;
  std::unordered_map<std::string, size_t> animated_lights;
  std::string                             camera_name;
  std::vector<animation_description>      animations;
  AABBox                                  scene_bb;
  scene_info                              si;
};

#endif //ELUCIDO_ALL_SCENE_H

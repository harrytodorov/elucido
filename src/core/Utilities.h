// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_UTILITIES_H
#define ELUCIDO_UTILITIES_H

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include <random>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Object;

const float_t kEpsilon = glm::epsilon<float_t>();               // epsilon value; used to deal with some edge cases
const float_t infinity = std::numeric_limits<float_t>::max();   // infinity value

const glm::vec3 white(1);                                       // white color
const glm::vec3 lightslategray(0.467f, 0.533f, 0.6f);

const float_t bias = 0.0001;            // shadow bias is used for avoiding self-shadows
const uint32_t max_depth = 5;           // maximum depth of recursion
const glm::vec3 bgc(lightslategray);    // background color

const std::string vertex("v");
const std::string vertex_normal("vn");
const std::string face("f");

// Available status codes for parsing scene files +
// mappings for the status code in human language.
enum SceneParserStatusCodes {
  file_problem,                   // 0
  invalid_statement,              // 1
  invalid_syntax,                 // 2
  duplicate,                      // 3
  thing_not_created,              // 4
  invalid_set_property,           // 5
  invalid_set_property_value,     // 6
  invalid_thing_to_transform,     // 7
  invalid_transformation_type,    // 8
  invalid_transformation_axix,    // 9
  invalid_animation_camera,       // 10
  success                         // 11
};
const std::map<SceneParserStatusCodes, std::string> STATUS_CODES_MAP = {
    {file_problem,                "There was a problem with loading the file."},
    {invalid_statement,           "An invalid statement."},
    {invalid_syntax,              "Invalid syntax."},
    {duplicate,                   "Duplicate"},
    {thing_not_created,           "The thing is not created."},
    {invalid_set_property,        "The set property you're using is not defined."},
    {invalid_set_property_value,  "The value of the set property is not valid."},
    {invalid_thing_to_transform,  "The thing does not support transformation."},
    {invalid_transformation_type, "The transformation type is not supported"},
    {invalid_transformation_axix, "You have entered invalid transformation axis."},
    {invalid_animation_camera,    "You can only animate one camera per animation."},
    {success,                     "File was successfully parsed."}
};

// The available start words (statements) within the scene format +
// the corresponding mappings needed for the parser.
enum SceneFileActionWord {
  create,
  set,
  transform,
  animate,
  comment
};
const std::map<std::string, SceneFileActionWord> START_WORDS = {
    {"create",    create},
    {"set",       set},
    {"transform", transform},
    {"animate",   animate},
    {"#",         comment}
};

// The available things to apply statements on within the scene format +
// the corresponding mappings needed for the parser.
enum SceneThings {
  camera_d,
  color_d,
  vector_d,
  material_d,
  light_d,
  object_d,
  image_plane_d,
  acceleration_structure_d,
  animation_d,
  scene_d
};
const std::map<std::string, SceneThings> AVAILABLE_THINGS = {
    {"camera",                  camera_d},
    {"color",                   color_d},
    {"vector",                  vector_d},
    {"material",                material_d},
    {"light",                   light_d},
    {"object",                  object_d},
    {"image_plane",             image_plane_d},
    {"acceleration_structure",  acceleration_structure_d},
    {"animation",               animation_d},
    {"scene",                   scene_d}
};

// Available camera set properties +
// corresponding mappings for the parser.
enum CameraSetProperties {
  camera_type,
  camera_zoom_factor,
  camera_fov
};
const std::map<std::string, CameraSetProperties> CAMERA_SET_PROPERTIES = {
    {"type",        camera_type},
    {"zoom_factor", camera_zoom_factor},
    {"fov",         camera_fov}
};

// Available camera types +
// corresponding mappings for the parser.
enum CameraType {
  not_set_ct,
  perspective,
  orthographic
};
const std::map<std::string, CameraType> CAMERA_TYPES_MAP = {
    {"orthographic",  orthographic},
    {"perspective",   perspective}
};

// Available camera properties.
enum CameraProperty {
  not_set_cp,
  zoom_factor,
  field_of_view
};

// Available material types +
// corresponding mappings for the parser.
enum MaterialType {
  not_set_mt,
  phong,        // Phong material
  reflective,   // Reflection matrial
  refractive,   // Refractive material
};
const std::map<std::string, MaterialType> MATERIAL_TYPES_MAP = {
    {"phong",       phong},
    {"reflective",  reflective},
    {"refractive",  refractive}
};

// Available material properties +
// corresponding mappings for the parser.
enum MaterialProperty {
  ambient,
  diffuse,
  spec_const,
  spec_exp,
  refl_ind,
  ior,
  mat_color,
  mat_type
};
const std::map<std::string, MaterialProperty> MATERIAL_PROPERTIES_MAP = {
    {"ambient",     ambient},
    {"diffuse",     diffuse},
    {"spec_const",  spec_const},
    {"spec_exp",    spec_exp},
    {"refl_ind",    refl_ind},
    {"ior",         ior},
    {"color",       mat_color},
    {"type",        mat_type}
};

// Available light types +
// corresponding mappings for the parser.
enum LightType {
  not_set_lt,
  point,
  directional
};
const std::map<std::string, LightType> LIGHT_TYPES_MAP = {
    {"point",       point},
    {"directional", directional}
};

// Available light properties +
// corresponding mappings for the parser.
enum LightProperty {
  not_set_lp,
  position,
  direction,
  intensity,
  light_color,
  light_type
};
const std::map<std::string, LightProperty> LIGHT_PROPERTIES_MAP = {
    {"position",  position},
    {"direction", direction},
    {"intensity", intensity},
    {"color",     light_color},
    {"type",      light_type}
};

// Available object types +
// corresponding mappings for the parser.
enum ObjectType {
  not_set_ot,
  sphere,
  triangle,
  triangle_mesh
};
const std::map<std::string, ObjectType> OBJECT_TYPES_MAP = {
    {"sphere",        sphere},
    {"triangle",      triangle},
    {"triangle_mesh", triangle_mesh}
};

// Available object properties +
// corresponding mappings for the parser.
enum ObjectProperty {
  radius,
  center,
  vertices,
  file_name,
  interpolation,
  object_type,
  object_mat
};
const std::map<std::string, ObjectProperty> OBJECT_PROPERTIES_MAP = {
    {"radius",          radius},
    {"center",          center},
    {"vertices",        vertices},
    {"file_name",       file_name},
    {"interpolation",   interpolation},
    {"type",            object_type},
    {"material",        object_mat}
};

// Available image plane output types +
// corresponding mappings for the parser.
enum OutputType {
  not_set_out,
  ppm_o,
  png_o
};
const std::map<std::string, OutputType> IMAGE_PLANE_OUT_TYPES_MAP = {
    {"ppm", ppm_o},
    {"png", png_o}
};

// Available image plane properties +
// corresponding mappings for the parser.
enum ImagePlaneProperty {
  output_type,
  horizontal,
  vertical,
  use_gamma,
  number_samples,
  sampling_strategy,
  pixel_filter
};
const std::map<std::string, ImagePlaneProperty> IMAGE_PLANE_PROPERTIES_MAP = {
    {"output_type",       output_type},
    {"horizontal",        horizontal},
    {"vertical",          vertical},
    {"gamma",             use_gamma},
    {"number_samples",    number_samples},
    {"sampling_strategy", sampling_strategy},
    {"pixel_filter",      pixel_filter}
};

// Available sampling strategies + corresponding mappings for the parser.
enum SamplingStrategy {
  random_sampling,
  half_jittered_sampling,
  nrooks_sampling,
  multi_jittered_sampling
};
const std::map<std::string, SamplingStrategy> SAMPLING_STRATEGIES_MAP = {
    {"random",          random_sampling},
    {"half_jittered",   half_jittered_sampling},
    {"nrooks",          nrooks_sampling},
    {"multi_jittered",  multi_jittered_sampling}
};

// Available pixel filtering strategies + corresponding mappings for the parser.
enum PixelFilter {
  box_filter,
  triangle_filter,
  gaussian_filter,
  mitchell_filter,
  sinc_filter
};
const std::map<std::string, PixelFilter> PIXEL_FILTER_MAP = {
    {"box",       box_filter},
    {"triangle",  triangle_filter},
    {"gaussian",  gaussian_filter},
    {"mitchell",  mitchell_filter},
    {"sinc",      sinc_filter}
};

// Available accelerators structure properties +
// corresponding mappings.
enum AccelerationStructureProperties {
  alpha,
  max_resolution,
  as_type
};
const std::map<std::string, AccelerationStructureProperties> AC_PROPERTIES_MAP = {
    {"alpha",           alpha},
    {"max_resolution",  max_resolution},
    {"type",            as_type}
};

// Available accelerators structure types +
// corresponding mappings.
enum AccelerationStructureType {
  not_set_act,
  grid,
  compact_grid,
  kdtree_midpoint
};
const std::map<std::string, AccelerationStructureType> AC_TYPES_MAP = {
    {"grid",            grid},
    {"compact_grid",    compact_grid},
    {"kdtree_midpoint", kdtree_midpoint}
};

// Available animation properties +
// corresponding mappings.
enum AnimationProperties {
  number_of_images_in_seq,
};
const std::map<std::string, AnimationProperties> ANIMATION_PROPERTIES_MAP = {
    {"number_of_images_in_seq", number_of_images_in_seq}};

// Available scene properties +
// corresponding mappings.
enum SceneProperties {
  scene_camera,
  scene_image_plane,
  scene_as,
  scene_objects,
  scene_lights,
  scene_animations
};
const std::map<std::string, SceneProperties> SCENE_PROPERTIES_MAP = {
    {"camera",                  scene_camera},
    {"image_plane",             scene_image_plane},
    {"acceleration_structure",  scene_as},
    {"objects",                 scene_objects},
    {"lights",                  scene_lights},
    {"animations",              scene_animations}
};

// Available transformation type + corresponding mappings.
enum TransformationType {
  not_set_tt,
  translation,
  rotation,
  scale
};
const std::map<std::string, TransformationType> TRANSFORMATION_TYPES_MAP = {
    {"translate", translation},
    {"rotate",    rotation},
    {"scale",     scale}
};

// Available transformation axes + corresponding mappings.
enum Axis {
  X,
  Y,
  Z,
  uniform
};
const std::map<std::string, Axis> AXES_MAP = {
    {"X",   X},
    {"Y",   Y},
    {"Z",   Z},
    {"uniform", uniform}
};

enum RayType {
  primary,
  shadow,
  reflection,
  refraction
};

struct color_description {
  std::string   name;
  uint8_t       r;
  uint8_t       g;
  uint8_t       b;
  color_description(const std::string &_name) :
      name(_name),
      r(0),
      g(0),
      b(0) {}
};

struct vector_description {
  std::string   name;
  float_t       x;
  float_t       y;
  float_t       z;
  vector_description(const std::string &_name) :
      name(_name),
      x(0),
      y(0),
      z(0) {}
};

struct transformation_description {
  Axis                axis;
  TransformationType  type;
  float_t             amount;
  transformation_description() :
      axis(X),
      type(not_set_tt),
      amount(0.f) {}
};

struct material_description {
  std::string                         name;
  MaterialType                        type;
  std::map<MaterialProperty, float_t> properties;
  std::shared_ptr<color_description>  color;
  material_description(const std::string &_name) :
      name(_name),
      type(not_set_mt),
      properties({}),
      color(nullptr) {}
};

struct camera_description {
  std::string                               name;
  CameraType                                type;
  std::pair<CameraProperty, float_t>        property;
  std::vector<transformation_description>   transformations;
  camera_description(const std::string &_name) :
      name(_name),
      type(not_set_ct),
      property({not_set_cp, 0.f}),
      transformations({}) {}
};

struct light_description {
  std::string                                     name;
  LightType                                       type;
  std::shared_ptr<color_description>              color;
  float_t                                         intensity;
  std::pair<LightProperty,
            std::shared_ptr<vector_description>>  property;
  std::vector<transformation_description>         transformations;
  light_description(const std::string &_name) :
      name(_name),
      type(not_set_lt),
      color(nullptr),
      intensity(0.f),
      property({not_set_lp, nullptr}),
      transformations({}) {}
};

struct object_description {
  std::string                                       name;
  ObjectType                                        type;
  std::shared_ptr<material_description>             material;
  float_t                                           radius;
  std::shared_ptr<vector_description>               center;
  std::vector<std::shared_ptr<vector_description>>  vertices;
  std::string                                       file_name;
  uint8_t                                           interpolation;  // One wants to encode 3 states:
                                                                    // 0: interpolation is not set
                                                                    // 1: interpolation is set to true
                                                                    // 2: interpolation is set to false
  std::vector<transformation_description>           transformations;
  object_description(const std::string &_name) :
      name(_name),
      type(not_set_ot),
      material(nullptr),
      radius(0.f),
      center(nullptr),
      vertices({nullptr}),
      file_name({}),
      interpolation(0),
      transformations({}) {}
};

struct image_plane_description {
  std::string         name;
  OutputType          output_type;
  uint32_t            horizontal;
  uint32_t            vertical;
  uint8_t             use_gamma;  // One wants to encode 3 states:
                                  // 0: gamma is not set
                                  // 1: gamma is set to true
                                  // 2: gamma is set to false
  uint32_t            number_samples;
  SamplingStrategy    sampling_strategy;
  PixelFilter         pixel_filter;
  image_plane_description(const std::string &_name) :
      name(_name),
      output_type(not_set_out),
      horizontal(0),
      vertical(0),
      use_gamma(0),
      number_samples(1),
      sampling_strategy(random_sampling),
      pixel_filter(box_filter)
  {}
};

struct acceleration_structure_description {
  std::string                 name;
  AccelerationStructureType   type;
  float_t                     alpha;
  uint32_t                    max_resolution;
  acceleration_structure_description(const std::string &_name) :
      name(_name),
      type(not_set_act),
      alpha(0.f),
      max_resolution(0) {}
};

struct animation_description {
  std::string                                                       name;
  uint32_t                                                          num_of_images_in_sequence;
  std::map<std::string, std::vector<transformation_description>>    objects;
  std::map<std::string, std::vector<transformation_description>>    lights;
  std::pair<std::string, std::vector<transformation_description>>   camera;
  animation_description(const std::string &_name) :
      name(_name),
      num_of_images_in_sequence(0),
      objects({}),
      lights({}),
      camera({{}, {}}) {}
};

struct scene_description {
  std::string                                           name;
  std::shared_ptr<camera_description>                   camera;
  std::shared_ptr<image_plane_description>              image_plane;
  std::shared_ptr<acceleration_structure_description>   acceleration_structure;
  std::vector<object_description>                       objects;
  std::vector<light_description>                        lights;
  std::vector<animation_description>                    animations;
  scene_description(const std::string &_name) :
      name(_name),
      camera(nullptr),
      image_plane(nullptr),
      acceleration_structure(nullptr),
      objects({}),
      lights({}),
      animations({}) {}
};

struct mesh_loading_info {
  uint32_t nv{0};     // Number of vertices.
  uint32_t nvn{0};    // Number of vertex normals.
  uint32_t nf{0};     // Number of faces.
  uint32_t nt{0};     // Number of triangles.
  bool     l{false};  // Successfully loaded mesh.
};

struct render_info {
  uint64_t npr{0};    // Number of primary rays.
  uint64_t nsr{0};    // Number of shadow rays
  uint64_t nrr{0};    // Number of reflection rays.
  uint64_t nrrr{0};   // Number of refraction rays.

  uint64_t nrpt{0};   // Number of ray-primitive intersection tests.
  uint64_t nroi{0};   // Number of ray-object intersections; ray-bounding box intersection does not count
                      // as a valid ray-object intersection; so just ray-object intersections are counted
};

struct isect_info {
  glm::vec4               ip;   // Intersection point.
  glm::vec4               ipn;  // Normal at the intersection point.
  float_t                 tn;   // Nearest distance from the ray's origin to
                                // the intersection point.
  float_t                 u;    // Barycentric coordinate u of a triangle.
  float_t                 v;    // Barycentric coordinate v of a triangle.
  uint32_t                ti;   // Index of the triangle in a triangulated mesh.
  bool                    fp;   // Flip normal.
  uint64_t                nrpt; // Number of ray-primitive intersection tests.
                                // It's used inside an acceleration structure.
  std::shared_ptr<Object> ho;   // Pointer to the object hit by a ray.
  isect_info() :
      ip{infinity},
      ipn{0},
      tn{infinity},
      u{infinity},
      v{infinity},
      ti{},
      fp{false},
      ho{nullptr},
      nrpt{0}
  {}
};

struct as_construct_info {
  size_t    d{0};     // Duration of the construction in ms.

  // Grid-related information.
  uint32_t  r[3];    // Grid's resolution.
  uint32_t  nfc{0};  // Number of non-empty cells.
  float_t   npnc{0}; // Number of primitives per non-empty cell.
};

struct scene_info {
  uint32_t no{0}; // Number of objects.
  uint32_t nl{0}; // Number of light sources.
  uint32_t np{0}; // Number of primitives.
};

struct material {
  glm::vec3       c{white};       // material's color
  MaterialType    mt{phong};      // material's type (see MaterialType enum)
  float_t         ac{0.2f};       // ambient constant
  float_t         dc{0.8f};       // diffuse constant
  float_t         sc{0.2f};       // specular constant
  float_t         se{10.f};       // specular exponent
  float_t         ri{0.8f};       // reflection index
  float_t         ior{1.00029f};  // index of refraction
};

struct ip_sample {
  glm::vec2 position;
  glm::vec3 radiance;
  bool      evaluated;
  ip_sample() :
      position{0},
      radiance{0},
      evaluated{false} {}
  ip_sample(const glm::vec3 &_radiance) :
      position{0},
      radiance{_radiance},
      evaluated{true} {}
  ip_sample(const glm::vec3 &_radiance,
            const glm::vec2 &_position) :
      position{_position},
      radiance{_radiance},
      evaluated{true} {}
};

//------------------------------------------------------------------------------
// FUNCTION DEFINITIONS
//------------------------------------------------------------------------------
/**
 * TODO: move to a parser class.
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
read_scene_from_file(const std::string &f);

//------------------------------------------------------------------------------

#endif //ELUCIDO_UTILITIES_H
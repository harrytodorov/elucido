// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_UTILITIES_H
#define ELUCIDO_UTILITIES_H

#include <string>
#include <map>
#include <vector>
#include "glm/gtc/epsilon.hpp"

class Object;

const float_t kEpsilon = glm::epsilon<float_t>();               // epsilon value; used to deal with some edge cases
const float_t infinity = std::numeric_limits<float_t>::max();   // infinity value

const glm::vec3 red(1.f, 0, 0);                                 // red color
const glm::vec3 green(0, 1.f, 0);                               // green color
const glm::vec3 blue(0, 0, 1.f);                                // blue color
const glm::vec3 white(1);                                       // white color
const glm::vec3 black(0);                                       // black color
const glm::vec3 sienna(0.627f, 0.321f, 0.176f);                 // sienna color
const glm::vec3 bluish(0.357f, 0.439f, 0.569f);                 // bluish color
const glm::vec3 deadgold(0.647f, 0.604f, 0.486f);
const glm::vec3 violet(0.573f, 0.384f, 0.757f);
const glm::vec3 orangish(0.929f, 0.615f, 0.306f);
const glm::vec3 whitish(0.780f, 0.812f, 0.867f);
const glm::vec3 lightslategray(0.467f, 0.533f, 0.6f);
const glm::vec3 greyish(0.65f);
const glm::vec3 skyblue(0.258f, 0.674f, 0.831f);
const glm::vec3 pink(1.f,105.f/255,180.f/255);
const glm::vec3 gold(1.f, 0.843f, 0.f);
const glm::vec3 rabit_color(0.847f, 0.105f, 0.376f);
const glm::vec3 bg_green(0.230f, 0.656f, 0.320f);               // Bulgaria's
                                                                // green

const float_t bias = 0.0001;            // shadow bias is used for avoiding self-shadows
const uint32_t max_depth = 5;           // maximum depth of recursion
const glm::vec3 bgc(lightslategray);    // background color

const std::string vertex("v");
const std::string vertex_normal("vn");
const std::string face("f");

// Available error codes for parsing scene files.
enum SceneParserErrorCodes {
  file_problem,
  invalid_statement,
  invalid_syntax,
  duplicate,
  thing_not_created,
  invalid_set_property,
  invalid_set_property_value,
  success
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
    {"create", create},
    {"set", set},
    {"transform", transform},
    {"for", animate},
    {"#", comment}
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
    {"image-plane",             image_plane_d},
    {"acceleration-structure",  acceleration_structure_d},
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
    {"type", camera_type},
    {"zoom_factor", camera_zoom_factor},
    {"fov", camera_fov}
};

// Available camera types +
// corresponding mappings for the parser.
enum CameraType {
  not_set_ct,
  perspective,
  orthographic
};
const std::map<std::string, CameraType> CAMERA_TYPES_MAP = {
    {"orthographic", orthographic},
    {"perspective", perspective}
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
  pm,     // Phong material
  rm,     // Reflection matrial
  rrm,    // Refractive material
};
const std::map<std::string, MaterialType> MATERIAL_TYPES_MAP = {
    {"phong", pm},
    {"reflective", rm},
    {"refractive", rrm}
};

// Available material properties +
// corresponding mappings for the parser.
enum MaterialProperty {
  ambient,
  diffuse,
  spec_const,
  spec_exp,
  refl_ind,
  ior
};
const std::map<std::string, MaterialProperty> MATERIAL_PROPERTIES_MAP = {
    {"ambient", ambient},
    {"diffuse", diffuse},
    {"spec_const", spec_const},
    {"spec_exp", spec_exp},
    {"refl_ind", refl_ind},
    {"ior", ior}
};

enum RayType {
  primary,
  shadow,
  reflection,
  refraction
};

enum ObjectType {
  not_set_ot,
  sphere,
  triangle,
  triangle_mesh
};

enum Axis {
  X,
  Y,
  Z,
  XY,
  XZ,
  YZ,
  XYZ
};

enum LightType {
  not_set_lt,
  point,
  directional
};

enum LightProperty {
  not_set_lp,
  position,
  direction
};

enum ObjectProperty {
  radius,
  center,
  vertices,
  file_name,
  interpolation
};

enum TransformationType {
  not_set_tt,
  translation,
  rotation,
  scale
};

enum OutputType {
  not_set_out,
  ppm_o,
  png_o
};

enum AccelerationStructureType {
  not_set_act,
  grid
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
  color_description                   *color;
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
  std::string                                   name;
  LightType                                     type;
  color_description                             *color;
  float_t                                       intensity;
  std::pair<LightProperty, vector_description *>  property;
  std::vector<transformation_description>       transformations;
  light_description(const std::string &_name) :
      name(_name),
      type(not_set_lt),
      color(nullptr),
      intensity(0.f),
      property({not_set_lp, nullptr}),
      transformations({}) {}
};

struct object_description {
  std::string                             name;
  ObjectType                              type;
  material_description                    *material;
  float_t                                 radius;
  vector_description                      *center;
  vector_description                      *vertices;
  std::string                             file_name;
  uint8_t                                 interpolation;  // One wants to encode 3 states:
                                                          // 0: interpolation is not set
                                                          // 1: interpolation is set to true
                                                          // 2: interpolation is set to false
  std::vector<transformation_description> transformations;
  object_description(const std::string &_name) :
      name(_name),
      type(not_set_ot),
      material(nullptr),
      radius(0),
      center(nullptr),
      vertices(nullptr),
      file_name({}),
      interpolation(0),
      transformations({}) {}
};

struct image_plane_description {
  std::string   name;
  OutputType    output_type;
  uint32_t      horizontal;
  uint32_t      vertical;
  uint8_t       gamma;  // One wants to encode 3 states:
                        // 0: gamma is not set
                        // 1: gamma is set to true
                        // 2: gamma is set to false
  uint32_t      number_samples;
  image_plane_description(const std::string &_name) :
      name(_name),
      output_type(not_set_out),
      horizontal(0),
      vertical(0),
      gamma(0),
      number_samples(0) {}
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
  std::string                         name;
  camera_description                  *camera;
  image_plane_description             *image_plane;
  acceleration_structure_description  *acceleration_structure;
  std::vector<object_description>     objects;
  std::vector<light_description>      lights;
  std::vector<animation_description>  animations;
  scene_description(const std::string &_name) :
      name(_name),
      camera(nullptr),
      image_plane(nullptr),
      acceleration_structure(nullptr),
      objects({}),
      lights({}),
      animations({}) {}
};

struct loading_info {
  uint32_t nv{0};     // number of vertices
  uint32_t nvn{0};    // number of vertex normals
  uint32_t nf{0};     // number of faces
  uint32_t nt{0};     // number of triangles
};

struct render_info {
  uint64_t npr{0};    // number of primary rays
  uint64_t nsr{0};    // number of secondary rays
  uint64_t nrr{0};    // number of reflection rays
  uint64_t nrrr{0};   // number of refraction rays
  uint64_t no{0};     // number of objects in the scene
  uint64_t nls{0};    // number of light sources in the scene
  uint64_t nrpt{0};   // number of ray-primitive intersection tests
  uint64_t nroi{0};   // number of ray-object intersections; ray-bounding box intersection does not count
                      // as a valid ray-object intersection; so just ray-object intersections are counted
};

struct isect_info {
  glm::vec4       ip{glm::vec4(infinity)};    // intersection point
  glm::vec4       ipn{glm::vec4(0)};          // normal at the intersection point
  float_t         tn{infinity};               // nearest distance from the ray's origin to the intersection point
  float_t         u{infinity};                // barycentric coordinate u: Area_ACP / Area_ABC
  float_t         v{infinity};                // barycentric coordinate v: Area_ABP / Area_ABC
  uint32_t        ti{(uint32_t) -1};          // index of the triangle in a triangulated mesh
  const Object    *ho = nullptr;              // pointer to the object hit by the ray
};

struct grid_info {
  size_t  r[3];     // grid's resolution
  size_t  nfc{0};   // number of cells, which contain one or more primitives
  size_t  np{0};    // number of primitives contained in the grid
  float_t nppc{0};  // average number of primitives per non-empty cell
};

struct material {
  glm::vec3       c{white};       // material's color
  MaterialType    mt{pm};         // material's type (see MaterialType enum)
  float_t         ac{0.2f};       // ambient constant
  float_t         dc{0.8f};       // diffuse constant
  float_t         sc{0.2f};       // specular constant
  float_t         se{10.f};       // specular exponent
  float_t         ri{0.8f};       // reflection index
  float_t         ior{1.00029f};  // index of refraction
};

#endif //ELUCIDO_UTILITIES_H
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

//const uint32_t X    = 0;
//const uint32_t Y    = 1;
//const uint32_t Z    = 2;
//const uint32_t XY   = 3;
//const uint32_t XZ   = 4;
//const uint32_t YZ   = 5;
//const uint32_t XYZ  = 6;

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

enum RayType: uint8_t {
    primary,
    shadow,
    reflection,
    refraction
};

enum MaterialType: uint8_t {
    pm,     // Phong material
    rm,     // Reflection matrial
    rrm,    // Refractive material
};

enum MaterialProperty: uint8_t {
  ambient,
  diffuse,
  spec_const,
  spec_exp,
  reflection_index,
  ior
};

enum ObjectType: uint8_t {
    def,
    sphere,
    triangle,
    triangle_mesh
};

enum CameraType: uint8_t {
  perspective,
  orthographic
};

enum CameraProperty: uint8_t {
  zoom_factor,
  field_of_view
};

enum Axis: uint8_t {
  X,
  Y,
  Z,
  XY,
  XZ,
  YZ,
  XYZ
};

enum LightType: uint8_t {
  point,
  directional
};

enum LightProperty: uint8_t {
  position,
  direction
};

enum TransformationType: uint8_t {
  translation,
  rotation,
  scale
};

struct color_description {
  std::string name;
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

struct vector_description {
  std::string name;
  float_t x;
  float_t y;
  float_t z;
};

struct transformation_description {
  Axis axis;
  TransformationType type;
  float_t amount;
};

struct material_description {
  std::string name;
  MaterialType type;
  std::map<MaterialProperty, float_t> properties;
  color_description color;
};

struct camera_description {
  std::string name;
  CameraType type;
  std::pair<CameraProperty, float_t> property;
  std::vector<transformation_description> transformations;
};

struct light_description {
  std::string name;
  LightType type;
  color_description color;
  float_t intensity;
  std::pair<LightProperty, vector_description> property;
  std::vector<transformation_description> transformations;
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
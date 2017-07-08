//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_UTILITIES_H
#define ELUCIDO_UTILITIES_H

#include <string>
#include "glm/gtc/epsilon.hpp"

const float_t kEpsilon = glm::epsilon<float_t>();               // epsilon value; used to deal with some edge cases
const float_t infinity = std::numeric_limits<float_t>::max();   // infinity value

const uint32_t X    = 0;
const uint32_t Y    = 1;
const uint32_t Z    = 2;
const uint32_t XY   = 3;
const uint32_t XZ   = 4;
const uint32_t YZ   = 5;
const uint32_t XYZ  = 6;

const glm::vec3 red(1.f, 0, 0);                 // red color
const glm::vec3 green(0, 1.f, 0);               // green color
const glm::vec3 blue(0, 0, 1.f);                // blue color
const glm::vec3 white(1);                       // white color
const glm::vec3 black(0);                       // black color
const glm::vec3 sienna(0.627f, 0.321f, 0.176f); // sienna color
const glm::vec3 bluish(0.357f, 0.439f, 0.569f); // bluish color; rgb(91, 112, 145)
const glm::vec3 deadgold(0.647f, 0.604f, 0.486f);
const glm::vec3 violet(0.573f, 0.384f, 0.757f);
const glm::vec3 orangish(0.929f, 0.615f, 0.306f);
const glm::vec3 whitish(0.780f, 0.812f, 0.867f);
const glm::vec3 lightslategray(0.467f, 0.533f, 0.6f);

const float_t shadow_bias = 0.0001;             // shadow bias is used for avoiding self-shadows

const std::string vertex("v");
const std::string vertex_normal("vn");
const std::string face("f");

struct loading_info {
    uint32_t num_vertices{0};
    uint32_t num_of_vn{0};
    uint32_t num_of_faces{0};
    uint32_t num_of_triangles{0};
};

struct render_info {
    uint32_t primary_rays{0};
    uint32_t shadow_rays{0};
    uint32_t num_of_objects{0};
    uint32_t num_of_light_sources{0};
    uint32_t num_of_ray_object_tests{0};
    uint32_t num_of_ray_object_intersections{0};
};

enum RayType: uint8_t {
    primary,
    shadow
};

enum MaterialType: uint8_t {
    phong,
};


#endif //ELUCIDO_UTILITIES_H
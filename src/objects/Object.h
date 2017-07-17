//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_OBJECT_H
#define ELUCIDO_OBJECT_H

#include <glm/detail/type_vec.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/vec4.hpp>

#include <cstdio>
#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>

#include "../materials/Material.h"
#include "../Ray.h"
#include "../materials/PhongMaterial.h"
#include "AABBox.h"

class Object {
public:
    Material    *om;    // object's material
    glm::mat4   mt;     // model transform matrix for an object
                        // places and transforms the object in world coordinate space
    glm::mat4   nmt;    // transformation matrix for object's normals
    AABBox      bb;     // bounding box of the object
    bool        in;     // interpolate normals

    Object() : om(new PhongMaterial()), mt(glm::mat4(1)), bb(AABBox()) {}
    Object(Material *m) : om(m), mt(glm::mat4(1)), nmt(glm::mat4(1)), bb(AABBox()), in(false) {}
    Object(Material *m, const bool &_in) : om(m), mt(glm::mat4(1)), nmt(glm::mat4(1)), bb(AABBox()), in(_in) {}
    virtual ~Object() {}

    virtual bool intersect(const Ray &r, isect_info &i) = 0;
    virtual void get_surface_properties(isect_info &i) const = 0;
    virtual void apply_camera_transformation(const glm::mat4 &ctm, const glm::mat4 &tictm) = 0;
    virtual void apply_transformations() = 0;
    virtual void translate(const float_t &translation, const uint32_t &axes_of_translation) = 0;
    virtual void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation) = 0;
    virtual void scale(const float_t &scaling_factor, const uint32_t &axes_of_scale) = 0;
};


#endif //ELUCIDO_OBJECT_H

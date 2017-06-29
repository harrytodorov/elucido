//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_TRIANGLE_H
#define ELUCIDO_TRIANGLE_H

#define GLM_SWIZZLE_XYZW

#include "Object.h"

class Triangle : public Object{
public:
    glm::vec4 v0, v1, v2;   // vertices of the triangle
    glm::vec4 normal;       // normal of the triangle

    // Vertices have to be defined in counterclockwise direction, starting from vertex v0 going to v2
    // otherwise strange artefacts could occur

    Triangle() :
            Object(),
            v0(-1, -1, 0, 1),
            v1( 1, -1, 0, 1),
            v2( 0,  1, 0, 1),
            normal(glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0)))
    {}
    Triangle(Material *m) :
            Object(m),
            v0(-1, -1, 0, 1),
            v1( 1, -1, 0, 1),
            v2( 0,  1, 0, 1),
            normal(glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0)))
    {}
    Triangle(glm::vec4 &vec0,
             glm::vec4 &vec1,
             glm::vec4 &vec2) :
            Object(),
            v0(vec0),
            v1(vec1),
            v2(vec2),
            normal(glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0)))
    {}
    Triangle(glm::vec4 &vec0,
             glm::vec4 &vec1,
             glm::vec4 &vec2,
             Material *m) :
            Object(m),
            v0(vec0),
            v1(vec1),
            v2(vec2),
            normal(glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0)))
    {}
    virtual ~Triangle() {}

    bool intersect(const Ray &r, float_t &t, glm::vec4 &p_hit);
    void get_surface_properties(const glm::vec4 &hit_point, const glm::vec4 &view_direction, glm::vec4 &hit_normal);
    void apply_camera_transformation(glm::mat4 &t);
    void apply_transformations();
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
    void scale(const float_t &scaling_factor, const uint32_t &axes_of_scale);
};


#endif //ELUCIDO_TRIANGLE_H

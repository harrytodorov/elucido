//
// Created by Haralambi Todorov on 25/05/2017.
//

#ifndef ELUCIDO_TRIANGLE_H
#define ELUCIDO_TRIANGLE_H

#include "Object.h"

class Triangle : public Object{
public:
    glm::vec4 v0, v1, v2;       // vertices of the triangle
    glm::vec4 normal;           // normal of the triangle

    // Vertices have to be defined in counterclockwise direction, starting from vertex v0 going to v2
    // otherwise strange artefacts could occur

    Triangle() :
            Object(),
            v0(-1, -1, 0, 1),
            v1( 1, -1, 0, 1),
            v2( 0,  1, 0, 1)
    {
        normal = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0));
        reshape_bb();
    }
    Triangle(Material *m) :
            Object(m),
            v0(-1, -1, 0, 1),
            v1( 1, -1, 0, 1),
            v2( 0,  1, 0, 1)
    {
        normal = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0));
        reshape_bb();
    }
    Triangle(glm::vec4 &vec0,
             glm::vec4 &vec1,
             glm::vec4 &vec2) :
            Object(),
            v0(vec0),
            v1(vec1),
            v2(vec2)
    {
        reshape_bb();
    }
    Triangle(glm::vec4 &vec0,
             glm::vec4 &vec1,
             glm::vec4 &vec2,
             Material *m) :
            Object(m),
            v0(vec0),
            v1(vec1),
            v2(vec2)
    {
        normal = glm::normalize(glm::vec4(glm::cross(glm::vec3(v1)-glm::vec3(v0), glm::vec3(v2)-glm::vec3(v0)), 0));
        reshape_bb();
    }
    virtual ~Triangle() {}

    bool intersect(const Ray &r, isect_info &i);
    void get_surface_properties(isect_info &i) const;
    void apply_camera_transformation(const glm::mat4 &ictm, const glm::mat4 &itictm);
    void apply_transformations();
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(const float_t &angle_of_rotation, const uint32_t &axes_of_rotation);
    void scale(const float_t &scaling_factor, const uint32_t &axes_of_scale);

private:
    inline void reshape_bb() {
        // we don't want the bounding box to maintain its old shape
        bb.reset();

        // extend bb of triangle
        bb.extend_by(v0);
        bb.extend_by(v1);
        bb.extend_by(v2);
    }
};

#endif //ELUCIDO_TRIANGLE_H

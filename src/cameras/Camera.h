//
// Created by Haralambi Todorov on 18/05/2017.
//

#ifndef ELUCIDO_CAMERA_H
#define ELUCIDO_CAMERA_H

#include <glm/vec3.hpp>
#include <math.h>
#include <glm/mat4x4.hpp>
#include "../objects/Object.h"
#include "../ImagePlane.h"
#include "../lights/Light.h"
#include <vector>


class Camera {
public:
    glm::vec4                   eye;        // eye / camera position
    glm::vec4                   lookat;     // the point at which the camera looks
    glm::mat4                   ctm;        // camera's transformation matrix
    glm::mat4                   tictm;      // the transpose of the inverse of the camera's transformation matrix

    // constructors & destructors
    // - default camera position is at (0, 0, 0) in world space
    // - default camera direction is the negative z-axis (0, 0, -1)
    Camera() :
        eye(0, 0, 0, 1),
        lookat(0, 0, -1, 1),
        ctm(1),
        tictm(1)
    {}
    Camera(const glm::vec4 &p, const glm::vec4 &d) :
        eye(p),
        lookat(d),
        ctm(1),
        tictm(1)
    {}
    ~Camera() {}

    virtual render_info render_scene(const std::vector<Object *, std::allocator<Object *>> &objects,
                                     const std::vector<Light *, std::allocator<Light *>> &lights,
                                     ImagePlane &ip) = 0;
    void compute_color_at_surface(const std::vector<Light *> &lights, const std::vector<Object *> &objects,
                                      const Material *object_material, const glm::vec4 view_direction, glm::vec3 &color,
                                      uint32_t &num_of_ray_object_tests, uint32_t &num_of_ray_object_intersections,
                                      const isect_info &ii);
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(float_t rot_angle, uint32_t axes_of_rotation);

    inline glm::mat4 inverse_ctm() {
        // return the inverse of the camera's transformation matrix
        // needed to apply it to objects and light sources
        return glm::inverse(ctm);
    };

    inline glm::mat4 inverse_tictm() {
        // return the inverse of the transpose of the inverse of the camera's transformation matrix
        return glm::inverse(glm::transpose(tictm));
    };
};
#endif //ELUCIDO_CAMERA_H
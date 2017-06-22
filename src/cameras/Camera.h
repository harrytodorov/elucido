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
    glm::vec4                   up;         // up vector
    glm::mat4                   ctm;        // camera's transformation matrix

    // constructors & destructors
    // - default camera position is at (0, 0, 0) in world space
    // - default camera direction is the negative z-axis (0, 0, -1)
    // - we compute the orthonormal base vectors when constructing the camera
    Camera() :
        eye(0, 0, 0, 1),
        lookat(0, 0, -1, 1),
        up(0, 1, 0, 0),
        ctm(1)
    {}
    Camera(const glm::vec4 &p, const glm::vec4 &d) :
        eye(p),
        lookat(d),
        up(0, 1, 0, 0),
        ctm(1)
    {}
    ~Camera() {}

    virtual void render_scene(std::vector<Object*> &objects, std::vector<Light*> lights, ImagePlane &ip) = 0;
    void translate(const float_t &translation, const uint32_t &axes_of_translation);
    void rotate(float_t rot_angle, uint32_t axes_of_rotation);

// functions need to be accessed in derived classes
protected:
    glm::mat4 inverse_ctm();
};
#endif //ELUCIDO_CAMERA_H
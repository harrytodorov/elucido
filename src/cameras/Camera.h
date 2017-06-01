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
    glm::vec4                   lookat;     // the direction into which the camera points
    glm::vec4                   u,v,w;      // orthonormal basis vectors
    glm::vec4                   up;         // up vector

    // constructors & destructors
    // - default camera position is at (0, 0, 0) in world space
    // - default camera direction is the negative z-axis (0, 0, -1)
    Camera() :
        eye(0, 0, 0, 1),
        lookat(0, 0, -1, 0),
        up(0, 1, 0, 0)
    {}
    Camera(const glm::vec4 &p, const glm::vec4 &d) :
        eye(p),
        lookat(d),
        up(0, 1, 0, 0)
    {}
    ~Camera() {}

    virtual void render_scene(std::vector<Object*> &objects, std::vector<Light*> lights, ImagePlane &ip) = 0;
    void compute_uvw();
};
#endif //ELUCIDO_CAMERA_H
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
    glm::vec3                   eye;        // eye / camera position
    glm::vec3                   lookat;     // the direction into which the camera points
    glm::vec3                   u,v,w;      // orthonormal basis vectors
    glm::vec3                   up;         // up vector

    // constructors & destructors
    // - default camera position is at (0, 0, 0) in world space
    // - default camera direction is the negative z-axis (0, 0, -1)
    Camera() {
        eye         = glm::vec3(0);
        lookat      = glm::vec3(0, 0, -1);
        up          = glm::vec3(0, 1, 0);
    }
    Camera(const glm::vec3 &p, const glm::vec3 &d) {
        eye         = p;
        lookat      = d;
        up          = glm::vec3(0, 1, 0);
    }
    ~Camera() {}

    virtual void render_scene(std::vector<Object*> &objects, std::vector<Light*> lights, ImagePlane &ip) = 0;
    void compute_uvw();
};
#endif //ELUCIDO_CAMERA_H
//
// Created by Haralambi Todorov on 18/05/2017.
//

#include "Camera.h"

void Camera::compute_uvw() {
    w = eye - lookat;
    glm::normalize(w);

    // conversation to vec3, because the cross product of vectors is only defined for 3d vectors
    glm::vec3 _w(w.x, w.y, w.z);
    glm::vec3 _up(up.x, up.y, up.z);
    glm::vec3 _u, _v;

    _u = glm::cross(_up, _w);

    glm::normalize(_u);
    _v = glm::cross(_w, _u);

    // convert back to 4d vectors
    w = glm::vec4(_w.x, _w.y, _w.z, 0);
    u = glm::vec4(_u.x, _u.y, _u.z, 0);
    v = glm::vec4(_v.x, _v.y, _v.z, 0);
}

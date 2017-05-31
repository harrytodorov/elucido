//
// Created by Haralambi Todorov on 18/05/2017.
//

#include "Camera.h"

void Camera::compute_uvw() {
    w = eye - lookat;
    glm::normalize(w);
    u = glm::cross(up, w);
    glm::normalize(u);
    v = glm::cross(w, u);
}

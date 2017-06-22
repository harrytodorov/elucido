//
// Created by Haralambi Todorov on 18/05/2017.
//

#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"


void Camera::rotate(float_t rot_angle, uint32_t axes_of_rotation) {
    // create 3d vector to determine the axis of rotation
    glm::vec3 tmv(0);

    switch (axes_of_rotation) {
        case X :
            tmv.x = 1;
            break;
        case Y :
            tmv.y = 1;
            break;
        case Z :
            tmv.z = 1;
            break;
        case XY :
            tmv.x = 1;
            tmv.y = 1;
            break;
        case XZ :
            tmv.x = 1;
            tmv.z = 1;
            break;
        case YZ :
            tmv.y = 1;
            tmv.z = 1;
            break;
        case XYZ :
            tmv = glm::vec3(1);
            break;
        default:
            printf("You're using an undefined axis of translation.");
            break;
    }
    // TODO: finish the rotation of a matrix

}

void Camera::translate(const float_t &translation, const uint32_t &axes_of_translation) {
    // create 3d vector to determine the axes of translation
    glm::vec3 tmv(0);

    switch (axes_of_translation) {
        case X :
            tmv.x = translation;
            break;
        case Y :
            tmv.y = translation;
            break;
        case Z :
            tmv.z = translation;
            break;
        case XY :
            tmv.x = translation;
            tmv.y = translation;
            break;
        case XZ :
            tmv.x = translation;
            tmv.z = translation;
            break;
        case YZ :
            tmv.y = translation;
            tmv.z = translation;
            break;
        case XYZ :
            tmv = glm::vec3(translation);
            break;
        default:
            printf("You're using an undefined axis of translation.");
            break;
    }

    // assign the newly created translation matrix to the camera's transformation matrix
    ctm = glm::translate(ctm, tmv);

    // apply the translation to the camera's parameters; actually those can be leaved unchanged, because
    // before rendering one would use the inverse of the camera transform to bring the camera back to its original
    // position; but for consistency, they'll be transformed as well
    eye = ctm * eye;
    lookat = ctm * lookat;
    up = glm::normalize(ctm * up);
}

glm::mat4 Camera::inverse_ctm() {
    // first calculate the inverse of the camera's transformation matrix
    glm::mat4 inverse = glm::inverse(ctm);

    // apply the inverse of the camera's transformation matrix to all affected camera parameters
    eye = inverse * eye;
    lookat = inverse * lookat;
    up = glm::normalize(ctm * up);

    // return the inverse of the camera's transformation matrix
    // needed to apply it to objects and light sources
    return inverse;
}
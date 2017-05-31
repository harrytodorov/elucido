//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_ORTHOGRAPHICCAMERA_H
#define ELUCIDO_ORTHOGRAPHICCAMERA_H


#include "Camera.h"

class OrthographicCamera : public Camera {
public:

    // default constructor
    OrthographicCamera() : Camera() {}
    OrthographicCamera(const glm::vec3 &p, const glm::vec3 &d) : Camera(p, d) {}

    void render_scene(std::vector<Object*> &objects, std::vector<Light*> lights, ImagePlane &ip);
};

#endif //ELUCIDO_ORTHOGRAPHICCAMERA_H

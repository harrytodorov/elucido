//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_ORTHOGRAPHICCAMERA_H
#define ELUCIDO_ORTHOGRAPHICCAMERA_H


#include "Camera.h"

class OrthographicCamera : public Camera {
public:
    float_t zf;   // zoom factor or the size of a pixel will be used only in the orthographic
                        // camera for using it to zoom in and out of an object

    // default constructor
    OrthographicCamera() : Camera(), zf(1.f) {}
    OrthographicCamera(const float_t &z) : Camera(), zf(z) {}

    void render_scene(const std::vector<Object *, std::allocator<Object *>> &objects,
                      const std::vector<Light *, std::allocator<Light *>> &lights,
                      ImagePlane &ip);
};

#endif //ELUCIDO_ORTHOGRAPHICCAMERA_H

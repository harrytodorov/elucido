//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_ORTHOGRAPHICCAMERA_H
#define ELUCIDO_ORTHOGRAPHICCAMERA_H


#include "Camera.h"

class OrthographicCamera : public Camera {
    float_t zf{1.f};    // zoom factor or the size of a pixel will be used only in the orthographic
                        // camera for using it to zoom in and out of an object
public:
    // default constructor
    OrthographicCamera() = default;
    explicit OrthographicCamera(const float_t &z) : zf(z) {}

    render_info render_scene(const std::vector<Object *, std::allocator<Object *>> &objects,
                             const std::vector<Light *, std::allocator<Light *>> &lights,
                             ImagePlane &ip);

    inline void set_zoom_factor(const float_t &z) {
        this->zf = z;
    }
};

#endif //ELUCIDO_ORTHOGRAPHICCAMERA_H

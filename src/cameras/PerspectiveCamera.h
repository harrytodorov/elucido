//
// Created by Haralambi Todorov on 26/05/2017.
//

#ifndef ELUCIDO_PERSPECTIVECAMERA_H
#define ELUCIDO_PERSPECTIVECAMERA_H


#include "../ImagePlane.h"
#include "../objects/Object.h"
#include "Camera.h"

class PerspectiveCamera : public Camera {
public:
    float_t fov;     // angle of view / field of view

    PerspectiveCamera() :
            Camera(),
            fov(90.f)
    {}
    PerspectiveCamera(const float_t &f) :
            Camera(),
            fov(f)
    {}

    render_info render_scene(const std::vector<Object *, std::allocator<Object *>> &objects,
                             const std::vector<Light *, std::allocator<Light *>> &lights,
                             ImagePlane &ip);
};


#endif //ELUCIDO_PERSPECTIVECAMERA_H

//
// Created by Haralambi Todorov on 19/05/2017.
//

#ifndef ELUCIDO_ORTHOGRAPHICCAMERA_H
#define ELUCIDO_ORTHOGRAPHICCAMERA_H


#include "Camera.h"

class OrthographicCamera : public Camera {
public:
    OrthographicCamera() : Camera() {}

    OrthographicCamera(const uint32_t &image_plane_width,
                       const uint32_t &image_plane_height,
                       glm::vec3 &camera_position) :
        Camera(image_plane_width, image_plane_height, camera_position)
    {}

//    void render_scene(std::vector<Object> &objects);
    void render_scene1(Object &object);
};


#endif //ELUCIDO_ORTHOGRAPHICCAMERA_H

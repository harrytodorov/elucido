//
// Created by Haralambi Todorov on 18/05/2017.
//

#ifndef ELUCIDO_CAMERA_H
#define ELUCIDO_CAMERA_H

#include <glm/vec3.hpp>
#include <math.h>
#include <glm/mat4x4.hpp>
#include "../objects/Object.h"
#include <vector>


class Camera {
public:
    glm::vec3                   cp;     // camera position
    glm::vec3                   cd;     // the direction into which the camera points
    glm::vec3*                  fb;     // frame buffer
    uint32_t                    ip_x;   // image plane width
    uint32_t                    ip_y;   // image plane height
    float_t                     ps;     // pixel size; in orthographic projection
                                    // the pixel size could be used for zooming

    // constructors & destructors
    // - default camera position is at (0, 0, 0) in world space
    // - default resolution of the image plane if not specified is 640x480
    // - default camera direction is the negative z-axis (0, 0, -1)
    Camera(const uint32_t &image_plane_width, const uint32_t &image_plane_height) {
        ip_x    = image_plane_width;
        ip_y    = image_plane_height;
        cp      = glm::vec3(0);
        cd      = glm::vec3(0, 0, -1);
        ps      = 1.0;
        fb      = new glm::vec3[ip_x * ip_y];
    }

    Camera(const uint32_t &image_plane_width, const uint32_t &image_plane_height, glm::vec3 &camera_position) {
        ip_x    = image_plane_width;
        ip_y    = image_plane_height;
        cp      = camera_position;
        cd      = glm::vec3(0, 0, -1);
        ps      = 1.0;
        fb      = new glm::vec3[ip_x * ip_y];
    }

    Camera() :
            Camera(640, 480)
    {}

    ~Camera() {}

//    virtual void render_scene(std::vector<std::unique_ptr<Object>> &objects)=0;
    virtual void render_scene1(Object &object) = 0;
};
#endif //ELUCIDO_CAMERA_H

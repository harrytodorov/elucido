//
// Created by Haralambi Todorov on 19/05/2017.
//

#include <iostream>
#include "OrthographicCamera.h"

void OrthographicCamera::render_scene(std::vector<Object*> &objects) {
    // x & y position of the image plane
    // according to this positions each ray would be shoot
    // in this basic scenario the z-axis stays fixed
    float_t             x_pos = 0;
    float_t             y_pos = 0;
    float_t             infinity = std::numeric_limits<float_t>::max();
    float_t             t_near;
    float_t             t_curr;
    const Object        *hit_object = nullptr;
    Ray                 ray;
    glm::vec3           hit_color;


    // Set the ray direction same as the direction of the camera
    ray.dir = cd;

    // get pointer to the frame buffer
    glm::vec3 *pixels = fb;


    // iterate through each pixel of the image plane
    for (int r = 0; r < ip_y; r++) {
        for (int c = 0; c < ip_x; c++) {

            // calculate the x/y coordinates for each pixel supposing the image plane is
            // positioned orthogonal to the z-plane and the image plane is placed at (0, 0, 0)
            // TODO: you have to make the orthogonal projection universal, so the camera is able to move through space
            x_pos = (float_t) (ps * (c - 0.5 * (ip_x - 1)));
            y_pos = (float_t) (ps * (r - 0.5 * (ip_y - 1)));

            // set the ray origin for each separate pixel
            // z position is fixed
            ray.orig = glm::vec3(x_pos, y_pos, cp.z);

            // set the nearest point initially at infinity
            t_near = infinity;

            // default color is camera's default background color
            hit_color = bc;

            // no object is hit
            hit_object = nullptr;

            // iterate through all objects and find the closest intersection
            for (auto& object : objects) {
                if (object->intersect(ray, t_near)) {
                    hit_object = object;
                }
            }

            // get the color from the closest intersected object, if any
            if (hit_object != nullptr)
                hit_color = hit_object->color;

            // assign the color to the frame buffer
            *(pixels++) = hit_color;
        }
    }
}
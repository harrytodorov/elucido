//
// Created by Haralambi Todorov on 19/05/2017.
//

#include <iostream>
#include "OrthographicCamera.h"
#include "../Utilities.h"


void OrthographicCamera::render_scene(std::vector<Object*> &objects, std::vector<Light*> lights, ImagePlane &ip) {
    // x & y position of the image plane
    // according to this positions each ray would be shoot
    // in this basic scenario the z-axis stays fixed
    float_t             x_pos = 0;
    float_t             y_pos = 0;
    float_t             infinity = std::numeric_limits<float_t>::max();
    float_t             t_near;
    const Object        *hit_object = nullptr;
    Ray                 ray;
    glm::vec3           hit_color;
    glm::vec4           hit_point;
    glm::vec4           hit_normal;


    // Set the ray direction same as the direction of the camera
    ray.dir = lookat;

    // get pointer to the frame buffer
    glm::vec3 *pixels = ip.fb;

    // iterate through each pixel of the image plane
    //
    // the top    left  corner of the image plane has the coordinates: (0     , vres-1)
    // the bottom right corner of the image plane has the coordinates: (hres-1,      0)
    // that's because of the fact that cameras flip the image by 180 degrees?
    // so to show the image with positive y-axis pointing upwards and
    // positive x-axis pointing left we iterate
    // first through each row starting at: vres-1 until 0
    // and in the inner loop through each column starting at: 0 until hres-1
    for (int r = ip.vres; r > 0; --r) {
        for (int c = 0; c < ip.hres; c++) {

            // calculate the x/y coordinates for each pixel supposing the image plane is
            // positioned orthogonal to the z-plane and the image plane is placed at (0, 0, 0)
            // TODO: you have to make the orthogonal projection universal, so the camera is able to move through space
            x_pos = (float_t) (ip.s * (c - 0.5 * (ip.hres - 1)));
            y_pos = (float_t) (ip.s * (r - 0.5 * (ip.vres - 1)));

            // set the ray origin for each separate pixel
            // z position is fixed
            ray.orig.x = x_pos;
            ray.orig.y = y_pos;
            ray.orig.z = eye.z;

            // set the nearest point initially at infinity
            t_near = infinity;

            // default color is image plane's default background color
            hit_color = ip.bc;

            // no object is hit
            hit_object = nullptr;

            // iterate through all objects and find the closest intersection
            for (auto& object : objects) {
                if (object->intersect(ray, t_near, hit_point, hit_normal)) {
                    hit_object = object;
                }
            }

            // get the color from the closest intersected object, if any
            // calculate the illumination per pixel using Phong illumination model
            if (hit_object != nullptr) {
                // set the hit color to 0 before adding the ambient, defuse and specular components
                // in case the default background color is not black
                hit_color.x = hit_color.y = hit_color.z = 0;

                // holders for diffuse and specular values
                glm::vec3 diffuse(0), specular(0);

                // calculate the ambient
                hit_color += ka * hit_object->color;

                // iterate through all light sources and calculate specular and defuse components
                for (auto& light : lights) {
                    glm::vec4 light_direction;
                    glm::vec3 light_intensity;
                    light->illuminate(hit_point, light_direction, light_intensity, t_near);

                    // calculate the diffuse component
                    diffuse += light_intensity * std::max(0.0f, glm::dot(hit_normal, -light_direction));
                }
                // add diffuse the the hit color
                hit_color += kd * diffuse;
            }

            // assign the color to the frame buffer
            *(pixels++) = hit_color;
        }
    }
}
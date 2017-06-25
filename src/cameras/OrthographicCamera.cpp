//
// Created by Haralambi Todorov on 19/05/2017.
//

#include <iostream>
#include "OrthographicCamera.h"


void OrthographicCamera::render_scene(std::vector<Object*> &objects, std::vector<Light*> lights, ImagePlane &ip) {
    float_t             curr_x = 0.f;
    float_t             curr_y = 0.f;
    float_t             t_near;
    float_t             ar;                     // image plane's aspect ratio
    Object              *hit_object = nullptr;
    Ray                 ray;
    glm::vec3           hit_color;
    glm::vec4           hit_point;
    glm::vec4           hit_normal;
    glm::mat4           icm;                    // inverse camera's transformation matrix

    // first position the camera at the origin
    // and get the inverse camera's transformation matrix
    icm = inverse_ctm();

    // apply the inverse camera's transformation matrix to all objects
    // and light sources in the scene
    for (auto& object : objects) {
        object->apply_camera_transformation(icm);
    }
    for (auto& light : lights) {
        light->apply_camera_transformation(icm);
    }

    // Set the ray direction same as the direction of the camera
    // and normalize it, shouldn't be necessary, but just in case
    ray.d = glm::normalize(lookat - eye);

    // get pointer to the frame buffer
    glm::vec3 *pixels = ip.fb;

    // calculate the image plane's aspect ratio
    // force float division
    ar = (ip.hres * 1.f) / ip.vres;

    for (int r = 0; r < ip.vres; r++) {
        for (int c = 0; c < ip.hres; c++) {

            // calculate the x/y coordinates for each pixel;
            // the image plane is positioned orthogonal to the z-plane and is placed at (0, 0, 0)
            // we also incorporate a scaling factor for the orthographic camera, which scales the
            // the pixel cell in world coordinate space; this would be used for zooming with orthographic camera
            curr_x = (2.f * ((c + 0.5f) / ip.hres) - 1) * ar * zf;
            curr_y = (1 - 2.f * ((r + 0.5f) / ip.vres)) * zf;

            // set the ray origin for each separate pixel
            // z position is fixed
            ray.o.x = curr_x;
            ray.o.y = curr_y;
            ray.o.z = 0.f;

            // set the nearest point initially at infinity
            t_near = infinity;

            // default color is image plane's default background color
            hit_color = ip.bc;

            // no object is hit
            hit_object = nullptr;

            // iterate through all objects and find the closest intersection
            for (auto& object : objects) {

                // check objects for intersection with tha ray
                if (object->intersect(ray, t_near, hit_point)) {
                    hit_object = object;
                }
            }

            // get the color from the closest intersected object, if any
            // calculate the illumination per pixel using Phong illumination model
            if (hit_object != nullptr) {

                // the view direction in case of ray-tracing is the opposite of the ray's direction
                glm::vec4 view_direction = -ray.d;

                // get the hit normal of the intersection point
                hit_object->get_surface_properties(hit_point, view_direction, hit_normal);

                // set the hit color to black before adding the ambient, defuse and specular components
                // in case the default background color is not black
                hit_color = black;

                // holders for diffuse & specular values;
                glm::vec3 diffuse(0), specular(0);

                // calculate the ambient
                hit_color += hit_object->om.ac * hit_object->om.c;

                // iterate through all light sources and calculate specular and defuse components
                for (auto& light : lights) {

                    glm::vec4 light_direction(0);
                    glm::vec3 light_intensity(0);
                    light->illuminate(hit_point, light_direction, light_intensity, t_near);

                    // dot product based on Lambert's cosine law for Lambertian reflectance
                    const float_t dot_pr = glm::dot(hit_normal, light_direction);

                    // calculate the diffuse component
                    diffuse += hit_object->om.c * light_intensity * std::max(0.f, dot_pr);

                    // calculate the specular component
                    glm::vec4 l_reflection = glm::normalize((2.f * dot_pr * hit_normal) - light_direction);
                    specular += light_intensity * std::max(0.f, std::powf(glm::dot(l_reflection, view_direction), hit_object->om.se));
                }
                // add diffuse the the hit color
                hit_color += hit_object->om.dc * diffuse + hit_object->om.sc * specular;
            }

            // assign the color to the frame buffer
            *(pixels++) = glm::clamp(hit_color, 0.f, 255.f);
        }
    }

    // after rendering reverse all objects and light sources to
    // their original positions;
    // use the camera transformation matrix to
    // bring them to their original positions
    for (auto& object : objects) {
        object->apply_camera_transformation(ctm);
    }
    for (auto& light : lights) {
        light->apply_camera_transformation(ctm);
    }
}
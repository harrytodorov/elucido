//
// Created by Haralambi Todorov on 19/05/2017.
//

#include "OrthographicCamera.h"


render_info OrthographicCamera::render_scene(const std::vector<Object *, std::allocator<Object *>> &objects,
                                             const std::vector<Light *, std::allocator<Light *>> &lights,
                                             ImagePlane &ip) {
    float_t             curr_x;
    float_t             curr_y;
    float_t             ar;                     // image plane's aspect ratio
    Object              *hit_object = nullptr;
    Ray                 ray;
    glm::vec3           hit_color;
    glm::vec4           hit_point;
    glm::vec4           hit_normal;
    glm::mat4           ictm;                   // inverse camera's transformation matrix
    glm::mat4           itictm;                 // inverse of the the transpose of the inverse of
                                                // the camera's transformation matrix
    render_info         ri;                     // rendering information
    isect_info          ii;                     // intersection information

    // get the inverse matrices
    ictm = inverse_ctm();
    itictm = inverse_tictm();

    // apply the inverse camera's transformation matrix to all objects
    // and light sources in the scene
    for (auto& object : objects) {
        object->apply_camera_transformation(ictm, tictm);
    }
    for (auto& light : lights) {
        light->apply_camera_transformation(ictm, tictm);
    }

    // Set the ray direction same as the direction of the camera
    // and normalize it, shouldn't be necessary, but just in case
    ray.set_dir(glm::normalize(lookat - eye));

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
            ray.set_orig(glm::vec4(curr_x, curr_y, eye.z, 1));

            // default color is image plane's default background color
            hit_color = ip.bc;

            // get the color from the closest intersected object, if any
            // calculate the illumination per pixel using Phong illumination model
            if (ray.trace(objects, ii, ri)) {

                // the view direction in case of ray-tracing is the opposite of the ray's direction
                glm::vec4 view_direction = -ray.dir();

                // get the hit normal of the intersection point
                ii.ho->get_surface_properties(ii);

                // get the color at the hit surface
                compute_color_at_surface(lights, objects, view_direction, ii, hit_color, ri);
            }

            // assign the color to the frame buffer
            *(pixels++) = glm::clamp(hit_color, 0.f, 1.f);
        }
    }

    // after rendering reverse all objects and light sources to
    // their original positions;
    // use the camera transformation matrix to
    // bring them to their original positions
    for (auto& object : objects) {
        object->apply_camera_transformation(ctm, itictm);
    }
    for (auto& light : lights) {
        light->apply_camera_transformation(ctm, itictm);
    }

    // get rendering information
    ri.num_of_light_sources = (uint32_t) lights.size();
    ri.num_of_objects = (uint32_t) objects.size();
    ri.primary_rays = ip.vres * ip.hres;

    return ri;
}
//
// Created by Haralambi Todorov on 26/05/2017.
//

#include "PerspectiveCamera.h"

render_info PerspectiveCamera::render_scene(const std::vector<Object *, std::allocator<Object *>> &objects,
                                            const std::vector<Light *, std::allocator<Light *>> &lights, ImagePlane &ip) {
    float_t             curr_x;
    float_t             curr_y;
    float_t             ar;                     // image plane's aspect ratio
    float_t             sf;                     // scaling factor obtained by the tan(fov/2)
    Object              *hit_object = nullptr;
    Ray                 ray;
    glm::vec3           hit_color;
    glm::vec4           cp;
    glm::mat4           ictm;                   // inverse camera's transformation matrix
    glm::mat4           itictm;                 // inverse of the the transpose of the inverse of
                                                // the camera's transformation matrix
    render_info         ri;                     // rendering information
    isect_info          ii;                     // intersection information

    // first position the camera at the origin
    // and get the inverse camera's transformation matrix
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

    // set the origin of the rays
    ray.set_orig(eye);

    // get pointer to the frame buffer
    glm::vec3 *pixels = ip.fb;

    // calculate the image plane's aspect ratio
    // force float division
    ar = (ip.hres * 1.f) / ip.vres;

    // calculate the scaling factor for the image plane
    // using the field of view; fov is in range(0, 180)
    sf = glm::tan(glm::radians(fov * 0.5f));

    for (int r = 0; r < ip.vres; r++) {
        for (int c = 0; c < ip.hres; c++) {

            // calculate the x/y coordinates for each pixel
            // the image plane is positioned orthogonal to the z-plane and is one unit away in
            // negative z-direction from the origin at (0, 0, -1)
            curr_x = (2.f * ((c + 0.5f) / ip.hres) - 1) * ar * sf;
            curr_y = (1.f - 2.f * ((r + 0.5f) / ip.vres)) * sf;

            // current position in the center of the image plane's cell
            cp = glm::vec4(curr_x, curr_y, -1.f, 1.f);

            // compute the direction of the ray; vector from the eye to the current image
            // plane cell's c; of course direction should be normalized as well
            ray.set_dir(glm::normalize(cp - eye));

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
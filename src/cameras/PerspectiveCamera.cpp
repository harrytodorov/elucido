//
// Created by Haralambi Todorov on 26/05/2017.
//

#include "PerspectiveCamera.h"

render_info PerspectiveCamera::render_scene(const std::vector<Object *, std::allocator<Object *>> &objects,
                                            const std::vector<Light *, std::allocator<Light *>> &lights, ImagePlane &ip) {
    float_t             curr_x;
    float_t             curr_y;
    float_t             t_near;
    float_t             ar;                     // image plane's aspect ratio
    float_t             sf;                     // scaling factor obtained by the tan(fov/2)
    uint32_t            ti;                     // triangle index
    Object              *hit_object = nullptr;
    Ray                 ray;
    glm::vec3           hit_color;
    glm::vec4           hit_point;
    glm::vec4           hit_normal;
    glm::vec4           cp;
    glm::mat4           icm;                    // inverse camera's transformation matrix
    render_info         ri;                     // rendering information

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

    // set the origin of the rays
    ray.set_origin(eye);

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
            ray.set_direction(glm::normalize(cp - eye));

            // set the nearest point initially at infinity
            t_near = infinity;

            // default color is image plane's default background color
            hit_color = ip.bc;

            // no object is hit
            hit_object = nullptr;

            // iterate through all objects and find the closest intersection
            for (auto &object : objects) {

                // increment the number of ray-object tests
                __sync_fetch_and_add(&ri.num_of_ray_object_tests, 1);

                // first iterate through object's bounding boxes and check if there is an intersection
                if (object->bb.intersect(ray)) {
                    hit_color = red;

                    // if there is an intersection with the bounding box,
                    // check if there is an intersection with the object itself
                    if (object->intersect(ray, t_near, hit_point, ti)) {
                        hit_object = object;

                        // increment the number of ray-object tests
                        __sync_fetch_and_add(&ri.num_of_ray_object_tests, 1);

                        // increment the number of ray-object intersections
                        __sync_fetch_and_add(&ri.num_of_ray_object_intersections, 1);

                        // increment number of shadow rays; for each hit object we definitely have a shadow ray
                        __sync_fetch_and_add(&ri.shadow_rays, 1);
                    }
                }
            }

            // get the color from the closest intersected object, if any
            // calculate the illumination per pixel using Phong illumination model
            if (hit_object != nullptr) {

                // the view direction in case of ray-tracing is the opposite of the ray's direction
                glm::vec4 view_direction = -ray.get_direction();

                // get the hit normal of the intersection point
                hit_object->get_surface_properties(hit_point, view_direction, ti, hit_normal);

                // get the color at the hit surface
                compute_color_at_surface(lights, objects, hit_object->om, hit_point, hit_normal, view_direction,
                                         hit_color, ri.num_of_ray_object_tests, ri.num_of_ray_object_intersections);

                // accumulate number of shadow rays; for each hit object we definitely have a shadow ray
                ri.shadow_rays += 1;

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
        object->apply_camera_transformation(ctm);
    }
    for (auto& light : lights) {
        light->apply_camera_transformation(ctm);
    }
    // get rendering information
    ri.num_of_light_sources = (uint32_t) lights.size();
    ri.num_of_objects = (uint32_t) objects.size();
    ri.primary_rays = ip.vres * ip.hres;

    return ri;
}
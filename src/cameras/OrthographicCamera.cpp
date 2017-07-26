//
// Created by Haralambi Todorov on 19/05/2017.
//

#include "OrthographicCamera.h"


render_info OrthographicCamera::render_scene(const std::vector<Object *, std::allocator<Object *>> &objects,
                                             const std::vector<Light *, std::allocator<Light *>> &lights,
                                             ImagePlane &ip) {
    float_t             curr_x;
    float_t             curr_y;
    float_t             ar;                                     // image plane's aspect ratio
    Ray                 ray;
    glm::vec3           pc;                                     // pixel color
    render_info         ri;                                     // rendering information
    std::random_device rd;                                      // obtain a random number from hardware
    std::mt19937 eng(rd());                                     // seed generator
    std::uniform_real_distribution<float_t> pr(0.05f, 0.95f);   // define ranges for pixel x/y

    // position all objects in the scene relative to the camera's position at the origin; inverse view transformation
    apply_inverse_view_transform(objects, lights);

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

            // use half-jittered sampling to reduce aliasing artifacts
            for (uint32_t ny = 0; ny < ip.ns; ny++) {
                for (uint32_t nx = 0; nx < ip.ns; nx++) {

                    // calculate the x/y coordinates for each sample per pixel;
                    // the image plane is positioned orthogonal to the z-plane and is placed at (0, 0, -1)
                    // we also incorporate a scaling factor for the orthographic camera, which scales the
                    // the pixel cell in world coordinate space; this would be used for zooming with orthographic camera
                    curr_x = (2.f * ((c*ip.ns + nx + pr(eng)) / (ip.hres*ip.ns)) - 1.f) * ar * zf;
                    curr_y = (1.f - 2.f * ((r*ip.ns + ny + pr(eng)) / (ip.vres*ip.ns))) * zf;

                    // set the ray origin for each sample position
                    ray.set_orig(glm::vec4(curr_x, curr_y, -1.f, 1.f));

                    // cast a ray into the scene and get the color value for it
                    pc += cast_ray(ray, lights, objects, 0, ri);
                }
            }

            // use simple box filter to get the average of the samples per pixel
            pc /= ip.ns*ip.ns;

            *(pixels++) = glm::clamp(pc, 0.f, 1.f);
        }
    }

    // reverse the inverse view transform; bring objects to their original positions
    reverse_inverse_view_transform(objects, lights);

    // get rendering information
    ri.nls = (uint32_t) lights.size();
    ri.no = (uint32_t) objects.size();

    return ri;
}
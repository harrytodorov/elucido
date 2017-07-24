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
    Ray                 ray;
    glm::vec3           hit_color;
    render_info         ri;                     // rendering information

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

            // calculate the x/y coordinates for each pixel;
            // the image plane is positioned orthogonal to the z-plane and is placed at (0, 0, 0)
            // we also incorporate a scaling factor for the orthographic camera, which scales the
            // the pixel cell in world coordinate space; this would be used for zooming with orthographic camera
            curr_x = (2.f * ((c + 0.5f) / ip.hres) - 1) * ar * zf;
            curr_y = (1 - 2.f * ((r + 0.5f) / ip.vres)) * zf;

            // set the ray origin for each separate pixel
            // z position is fixed
            ray.set_orig(glm::vec4(curr_x, curr_y, eye.z, 1));

            // cast a ray into the scene and get the color value for it
            hit_color = cast_ray(ray, lights, objects, 0, ri);
            *(pixels++) = glm::clamp(hit_color, 0.f, 1.f);
        }
    }

    // reverse the inverse view transform; bring objects to their original positions
    reverse_inverse_view_transform(objects, lights);

    // get rendering information
    ri.nls = (uint32_t) lights.size();
    ri.no = (uint32_t) objects.size();
    ri.npr = ip.vres * ip.hres;

    return ri;
}
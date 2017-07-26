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
    Ray                 ray;
    glm::vec3           hit_color;
    glm::vec4           cp;
    render_info         ri;                     // rendering information

    // position all objects in the scene relative to the camera's position at the origin; inverse view transformation
    apply_inverse_view_transform(objects, lights);

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

            // use half-jittered sampling to reduce aliasing artifacts
            for (uint32_t ny = 0; ny < ip.ns; ny++) {
                for (uint32_t nx = 0; nx < ip.ns; nx++) {




                }
            }








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
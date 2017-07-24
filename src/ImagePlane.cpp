//
// Created by Haralambi Todorov on 26/05/2017.
//

#include "ImagePlane.h"

void ImagePlane::save_to_ppm(const char *fn) {
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ri(fn, std::ios::out | std::ios::binary);
    ri << "P6\n" << hres << " " << vres << "\n255\n";
    for (uint32_t i = 0; i < hres * vres; ++i) {
        auto r = (char)(255 * glm::clamp(fb[i].r, 0.f, 1.f));
        auto g = (char)(255 * glm::clamp(fb[i].g, 0.f, 1.f));
        auto b = (char)(255 * glm::clamp(fb[i].b, 0.f, 1.f));
        ri << r << g << b;
    }
    ri.close();
}

void ImagePlane::save_to_png(const char *fn) {
    png::image<png::rgb_pixel> ri(hres, vres);

    for (size_t y = 0; y < vres; ++y) {
        for (size_t x = 0; x < hres; ++x) {

            // color at current pixel in the framebuffer
            glm::vec3 cacp = fb[y*hres + x];

            // convert float value of pixel in a png::byte [0, 255]
            auto r = (unsigned char) (255 * glm::clamp(cacp.r, 0.f, 1.f));
            auto g = (unsigned char) (255 * glm::clamp(cacp.g, 0.f, 1.f));
            auto b = (unsigned char) (255 * glm::clamp(cacp.b, 0.f, 1.f));

            // assign color values to image
            ri[y][x] = png::rgb_pixel(r, g, b);
        }
    }

    // save image to disk
    ri.write(fn);
}

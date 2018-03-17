// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "ImagePlane.h"
#include <glm/exponential.hpp>

//==============================================================================
void ImagePlane::save_to_ppm(const char *fn) {
  // Save result to a PPM image
  std::ofstream ri(fn, std::ios::out | std::ios::binary);
  ri << "P6\n" << hres << " " << vres << "\n255\n";
  for (uint32_t i = 0; i < hres * vres; ++i) {
    auto r = static_cast<char>(255 * encode_gamma(fb[i].r));
    auto g = static_cast<char>(255 * encode_gamma(fb[i].g));
    auto b = static_cast<char>(255 * encode_gamma(fb[i].b));
    ri << r << g << b;
  }
  ri.close();
}

//==============================================================================
void ImagePlane::save_to_png(const char *fn) {
  png::image<png::rgb_pixel> ri(hres, vres);

  for (size_t y = 0; y < vres; ++y) {
    for (size_t x = 0; x < hres; ++x) {
      // col at current pixel in the framebuffer
      glm::vec3 cacp = fb[y * hres + x];

      // convert float value of pixel in a png::byte [0, 255]
      auto r = (unsigned char) (255 * encode_gamma(cacp.r));
      auto g = (unsigned char) (255 * encode_gamma(cacp.g));
      auto b = (unsigned char) (255 * encode_gamma(cacp.b));

      // assign col values to image
      ri[y][x] = png::rgb_pixel(r, g, b);
    }
  }

  // save image to disk
  ri.write(fn);
}

//==============================================================================
float_t ImagePlane::encode_gamma(const float_t &c) {
  if (c <= 0.0031308f) return 12.92f * c;
  return 1.055f * glm::pow(c, 0.4166667f) - 0.055f;
}

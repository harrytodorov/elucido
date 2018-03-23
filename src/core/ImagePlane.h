// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_IMAGEPLANE_H
#define ELUCIDO_IMAGEPLANE_H

#include <cstdint>
#include <cmath>
#include <memory>
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/common.hpp>

#include "png++/png.hpp"

class ImagePlane {
 public:
//==============================================================================
// Data members
//==============================================================================
  uint32_t hres;              // Horizontal image resolution.
  uint32_t vres;              // Vertical image resolution.
  glm::vec3 bc;               // Background color.
  uint32_t ns;                // Number of samples per pixel.
  std::vector<glm::vec3> fb;  // The frame buffer.

//==============================================================================
// Constructors & destructors
//==============================================================================
// - default resolution of the image plane if not specified is 640x480
// - default background col is black (0, 0, 0)
  ImagePlane(const uint32_t &ip_x, const uint32_t &ip_y) {
    hres = ip_x;
    vres = ip_y;
    fb.reserve(hres*vres);
  }
  ImagePlane() : ImagePlane(640, 480) {}

  ~ImagePlane() = default;

//==============================================================================
// Function declarations
//==============================================================================
  void save_to_ppm(const std::string &fn);
  void save_to_png(const std::string &fn);
 private:
  float_t encode_gamma(const float_t &c);
};

#endif //ELUCIDO_IMAGEPLANE_H

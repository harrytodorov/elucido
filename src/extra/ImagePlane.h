// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_IMAGEPLANE_H
#define ELUCIDO_IMAGEPLANE_H

#include <cstdint>
#include <cmath>
#include <fstream>
#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include "png++/png.hpp"

class ImagePlane {
 public:
//=============================================================================
// Data members
//=============================================================================
  uint32_t hres;       // horizontal image resolution
  uint32_t vres;       // vertical image resolution
  uint32_t ns;         // number of samples per pixel
  glm::vec3 *fb;       // frame buffer

//=============================================================================
// Constructors & destructors
//=============================================================================
// - default resolution of the image plane if not specified is 640x480
// - default background color is black (0, 0, 0)
  ImagePlane(const uint32_t &ip_x, const uint32_t &ip_y) {
    hres = ip_x;
    vres = ip_y;
    fb = new glm::vec3[ip_x*ip_y];
  }

//=============================================================================
  ImagePlane() : ImagePlane(640, 480) {}

//=============================================================================
  ~ImagePlane() = default;

//=============================================================================
// Function declarations
//=============================================================================
  void save_to_ppm(const char fn[50]);
  void save_to_png(const char fn[50]);
 private:
  float_t encode_gamma(const float_t &c);
};

#endif //ELUCIDO_IMAGEPLANE_H

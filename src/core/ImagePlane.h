// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_IMAGEPLANE_H
#define ELUCIDO_IMAGEPLANE_H

#include <cstdint>
#include <cmath>
#include <vector>
#include <fstream>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/common.hpp>

#include "png++/png.hpp"

#include "Utilities.h"
#include "Sample.h"

class ImagePlane {
 public:
//==============================================================================
// Data members
//==============================================================================
  uint32_t hres;              // Horizontal image resolution.
  uint32_t vres;              // Vertical image resolution.
  glm::vec3 bc;               // Background color.
  uint32_t ns;                // Number of samples per pixel.
  std::vector<glm::vec2> us;  // Unit samples.
  SamplingStrategy ss;        // Sampling strategy.
  PixelFilter pf;             // Pixel filter.
  OutputType ot;              // Output type.
  std::vector<glm::vec3> fb;  // The frame buffer.

//==============================================================================
// Constructors & destructors
//==============================================================================
// - default resolution of the image plane if not specified is 640x480
// - default background col is black (0, 0, 0)
  ImagePlane(const uint32_t &ip_x, const uint32_t &ip_y) {
    hres = ip_x;
    vres = ip_y;
    fb.assign(hres*vres, bc);
  }
  ImagePlane() : ImagePlane(640, 480) {}

  ~ImagePlane() = default;

//==============================================================================
// Function declarations
//==============================================================================
  void save_to_ppm(const std::string &fn);
  void save_to_png(const std::string &fn);
  void set_number_of_samples(const uint32_t &_ns);
  void set_sampling_strategy(const SamplingStrategy &_ss);
  void generate_unit_samples();
  void set_pixel_filter(const PixelFilter &_pf);
 private:
  float_t encode_gamma(const float_t &c);

};

#endif //ELUCIDO_IMAGEPLANE_H

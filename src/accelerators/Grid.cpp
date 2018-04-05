// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Grid.h"

//==============================================================================
void Grid::compute_resolution(const AABBox &box,
                              const uint32_t &number_primitives) {
  // Precompute bounding box's inverse volume and cube root from the
  // formula used for determining resolution size.
  float_t invVolume = 1.f / box.getVolume();
  float_t cubeRoot = std::cbrt(number_primitives * alpha * invVolume);

  // A temporary variable to store the resolution as a floating point
  // before conversion.
  float_t resFloat[3];

  // x
  resFloat[0] = box.getDiagonal().x * cubeRoot;
  // y
  resFloat[1] = box.getDiagonal().y * cubeRoot;
  // z
  resFloat[2] = box.getDiagonal().z * cubeRoot;

  // Convert resolution from floating number to an integer.
  for (size_t i = 0; i < 3; i++) {
    resFloat[i] = glm::clamp(resFloat[i], 1.f,
                             static_cast<float_t>(maxResolution));

    // Static cast to uint32_t is equivalent to std::floor of the
    // floating value.
    resolution[i] = static_cast<uint32_t>(resFloat[i]);
  }

  // Compute cell's dimension.
  cellDimension.x = box.getDiagonal().x / resolution[0];
  cellDimension.y = box.getDiagonal().y / resolution[1];
  cellDimension.z = box.getDiagonal().z / resolution[2];
}

//==============================================================================
uint32_t *Grid::compute_primitive_bound_cell(const glm::vec4 &primitive_bound,
                                             const glm::vec4 &box_bound) const {
  uint32_t *pb = new uint32_t[3];
  glm::vec4 pcc = (primitive_bound - box_bound) / cellDimension;

  // X
  pb[0] = static_cast<uint32_t>(glm::clamp(pcc.x, 0.f, 1.f * (resolution[0] - 1)));

  // Y
  pb[1] = static_cast<uint32_t>(glm::clamp(pcc.y, 0.f, 1.f * (resolution[1] - 1)));

  // Z
  pb[2] = static_cast<uint32_t>(glm::clamp(pcc.z, 0.f, 1.f * (resolution[2] - 1)));

  return pb;
}

//==============================================================================
void Grid::traversal_initialization(glm::vec4 &delta_t,
                                    glm::vec4 &next_crossing_t,
                                    uint32_t *current_cell,
                                    int32_t *step,
                                    int32_t *exit,
                                    const Ray &ray,
                                    const float_t &t_bb,
                                    const glm::vec4 &box_min_bound) const {
  // Ray-grid intersection point.
  glm::vec4 rgip = ray.orig() + t_bb*ray.dir();

  for (size_t axis = 0; axis < 3; axis++) {
    // Compute the scalar distance between the ray-grid intersection point
    // and the grid's bounding box minimum bound.
    float_t rg = rgip[axis] - box_min_bound[axis];

    current_cell[axis] = static_cast<uint32_t>(glm::clamp(
        glm::floor(rg / cellDimension[axis]),
        0.f,
        1.f * (resolution[axis] - 1)));

    // Ray's direction is positive in this axis.
    if (ray.dir()[axis] >= 0) {
      delta_t[axis] = cellDimension[axis] * ray.inv_dir()[axis];
      next_crossing_t[axis] = t_bb +
          ((current_cell[axis] + 1) * cellDimension[axis] - rg) * ray.inv_dir()[axis];
      step[axis] = 1;
      exit[axis] = resolution[axis];
    } else {
      delta_t[axis] = -cellDimension[axis] * ray.inv_dir()[axis];
      next_crossing_t[axis] = t_bb +
          (current_cell[axis] * cellDimension[axis] - rg) * ray.inv_dir()[axis];
      step[axis] = -1;
      exit[axis] = -1;
    }
  }
}

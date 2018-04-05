// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "DynamicGrid.h"

//==============================================================================
void DynamicGrid::construct(const AABBox &box,
                     const std::vector<std::shared_ptr<Object>> &objects,
                     const uint32_t &number_primitives,
                     as_construct_info &info) {
  // Compute primitives.
  compute_primitives(number_primitives, objects);

  // Grid's bounding box is slightly larger than the bounding box encapsulating
  // the objects. (e.g. axis aligned triangle would have volume 0).
  bbox.bounds[0] = box.bounds[0] - kEpsilon;
  bbox.bounds[1] = box.bounds[1] + kEpsilon;

  compute_resolution(bbox, primitives.size());

  // Allocate memory for cells in the grid.
  size_t numOfCells = resolution[0] * resolution[1] * resolution[2];
  cells.assign(numOfCells, nullptr);

  uint32_t *min_cell = nullptr;
  uint32_t *max_cell = nullptr;

  // Iterate over primitives to insert them in the corresponding grid's cell.
  for (auto const &primitive : primitives) {
    min_cell = compute_primitive_bound_cell(primitive.getBB().bounds[0],
                                            bbox.bounds[0]);
    max_cell = compute_primitive_bound_cell(primitive.getBB().bounds[1],
                                            bbox.bounds[0]);

    // Iterate over corresponding grid cells and add primitive to it.
    for (uint32_t z = min_cell[2]; z <= max_cell[2]; ++z) {
      for (uint32_t y = min_cell[1]; y <= max_cell[1]; ++y) {
        for (uint32_t x = min_cell[0]; x <= max_cell[0]; ++x) {
          // Compute the cell's index.
          size_t cellIndex = offset(x, y, z);

          // Initialize cell, if needed.
          if (cells[cellIndex] == nullptr) {
            cells[cellIndex] = std::make_shared<Cell>(Cell());
          }
          cells[cellIndex]->insert(primitive);
        }
      }
    }
  }

  // Iterate once more over all cells to gather statistical information
  // about the grid (e.g. number of non-empty cells, av number of primitives
  // per cell)
  for (size_t i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
    if (cells[i] == nullptr) continue;
    info.nfc++;
    info.npnc += cells[i]->primitives.size();
  }
  info.npnc /= (1.f * info.nfc);
  info.r[0] = resolution[0];
  info.r[1] = resolution[1];
  info.r[2] = resolution[2];
}

//==============================================================================
bool DynamicGrid::traverse(const Ray &r, isect_info &ii) const {
  // Scalar distance from the ray's origin to the nearest hit point of
  // the ray with the grid's bounding box.
  float_t   tBoundingBox;
  uint64_t  intersected_primitives{0};

  // Check if the ray intersect's the grid at all.
  if (!bbox.intersect(r, tBoundingBox))
    return false;

  glm::vec4 deltaT, nextCrossingT;
  uint32_t *currentCell = new uint32_t[3];
  int32_t *step         = new int32_t[3];
  int32_t *exit         = new int32_t[3];

  traversal_initialization(deltaT, nextCrossingT, currentCell, step, exit, r,
                           tBoundingBox, bbox.bounds[0]);

  // Reset the intersection information.
  ii = isect_info();

  // The actual traversal of the grid.
  while (true) {
    uint32_t cellIndex = offset(currentCell[0], currentCell[1], currentCell[2]);

    // Check if there are any primitives in the current cell and if yes
    // check if the ray intersects any of the primitives in the cell.
    if (cells[cellIndex] != nullptr) {
      cells[cellIndex]->intersect(r, ii);
      intersected_primitives += cells[cellIndex]->primitives.size();
    }

    // Find the plane with the smallest crossing.
    size_t planeIndex{0};
    for (size_t i = 0; i < 3; i++) {
      if (nextCrossingT[i] < nextCrossingT[planeIndex]) {
        planeIndex = i;
      }
    }

    // Advance the grid.
    if (ii.tn < nextCrossingT[planeIndex]) break;
    currentCell[planeIndex] += step[planeIndex];
    if (currentCell[planeIndex] == exit[planeIndex]) break;
    nextCrossingT[planeIndex] += deltaT[planeIndex];
  }

  ii.nrpt = intersected_primitives;
  return (ii.ho != nullptr);
}
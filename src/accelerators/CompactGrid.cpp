// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "CompactGrid.h"

//==============================================================================
void CompactGrid::construct(const AABBox &box,
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

  // Initialize cell's array and set all entries to 0.
  cells.reset(new uint32_t[resolution[0] * resolution[1] * resolution[2] + 1]());

  uint32_t *min_cell = nullptr;
  uint32_t *max_cell = nullptr;

  // Iterate over primitives and increment the overlapping cells.
  for (uint32_t i = 0; i < primitives.size(); i++) {
    min_cell = compute_primitive_bound_cell(primitives[i].getBB().bounds[0],
                                            bbox.bounds[0]);
    max_cell = compute_primitive_bound_cell(primitives[i].getBB().bounds[1],
                                            bbox.bounds[0]);

    // Iterate over corresponding grid cells.
    for (uint32_t z = min_cell[2]; z <= max_cell[2]; ++z) {
      for (uint32_t y = min_cell[1]; y <= max_cell[1]; ++y) {
        for (uint32_t x = min_cell[0]; x <= max_cell[0]; ++x) {
          // Compute the cell's index.
          size_t cellIndex = offset(x, y, z);
         cells[cellIndex]++;
        }
      }
    }
  }

  // Accumulate the size for the object lists.
  for (size_t i = 1; i <= primitives.size(); i++) {
    cells[i] += cells[i - 1];
  }

  // Initialize the object's list array.
  uint32_t le = resolution[0] * resolution[1] * resolution[2];
  object_lists.reset(new uint32_t[cells[le]]);

  // Iterate over the primitives in reverse order and
  //  1. decrement the offset for of the cell entry;
  //  2. add the object to the corresponding cell's object list.
  for (size_t i = primitives.size(); i --> 0 ;) {
    min_cell = compute_primitive_bound_cell(primitives[i].getBB().bounds[0],
                                            bbox.bounds[0]);
    max_cell = compute_primitive_bound_cell(primitives[i].getBB().bounds[1],
                                            bbox.bounds[0]);

    // Iterate over corresponding grid cells.
    for (uint32_t z = min_cell[2]; z <= max_cell[2]; ++z) {
      for (uint32_t y = min_cell[1]; y <= max_cell[1]; ++y) {
        for (uint32_t x = min_cell[0]; x <= max_cell[0]; ++x) {
          // Compute the cell's index.
          size_t cellIndex = offset(x, y, z);

          cells[cellIndex]--;
          object_lists[cells[cellIndex]] = i;
        }
      }
    }
  }
}

//==============================================================================
bool CompactGrid::traverse(const Ray &r, isect_info &i) const {
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
  i = isect_info();

  // The actual traversal of the grid.
  while (true) {
    uint32_t ci = offset(currentCell[0], currentCell[1], currentCell[2]);

    // Intersect all objects in the cell.
    for (size_t j = cells[ci]; j < cells[ci + 1]; j++) {
      primitives[object_lists[j]].intersect(r, i);
      intersected_primitives++;
    }

    // Find the plane with the smallest crossing.
    size_t planeIndex{0};
    for (size_t i = 0; i < 3; i++) {
      if (nextCrossingT[i] < nextCrossingT[planeIndex]) {
        planeIndex = i;
      }
    }

    // Advance the grid.
    if (i.tn < nextCrossingT[planeIndex]) break;
    currentCell[planeIndex] += step[planeIndex];
    if (currentCell[planeIndex] == exit[planeIndex]) break;
    nextCrossingT[planeIndex] += deltaT[planeIndex];
  }

  i.nrpt = intersected_primitives;
  return (i.ho != nullptr);
}

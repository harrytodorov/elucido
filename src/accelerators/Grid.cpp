// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Grid.h"

void Grid::construct(const AABBox &box,
                     const std::vector<std::shared_ptr<Object>> &objects,
                     const uint32_t &number_primitives,
                     as_construct_info &info) {
  // Assign bounding box.
  bbox = box;

  // Compute primitives.
  compute_primitives(number_primitives, objects);

  // Precompute bounding box's inverse volume and cube root from the
  // formula used for determining resolution size.
  float_t invVolume = 1.f / bbox.getVolume();
  float_t cubeRoot = std::cbrt(primitives.size() * alpha * invVolume);

  // A temporary variable to store the resolution as a floating point
  // before conversion.
  float_t resFloat[3];

  // x
  resFloat[0] = bbox.getDiagonal().x * cubeRoot;
  // y
  resFloat[1] = bbox.getDiagonal().y * cubeRoot;
  // z
  resFloat[2] = bbox.getDiagonal().z * cubeRoot;

  // Convert resolution from floating number to an integer.
  for (size_t i = 0; i < 3; i++) {
    if (resFloat[i] > static_cast<float_t>(maxResolution)) {
      resFloat[i] = static_cast<float_t>(maxResolution);
    }

    if (resFloat[i] < 1.f) {
      resFloat[i] = 1.f;
    }

    // Static cast to uint32_t is equivalent to std::floor of the
    // floating value.
    resolution[i] = static_cast<uint32_t>(resFloat[i]);
  }

  // Compute cell's dimension.
  cellDimension.x = bbox.getDiagonal().x / resolution[0];
  cellDimension.y = bbox.getDiagonal().y / resolution[1];
  cellDimension.z = bbox.getDiagonal().z / resolution[2];

  // Allocate memory for cells in the grid.
  size_t numOfCells = resolution[0] * resolution[1] * resolution[2];
  cells.assign(numOfCells, nullptr);

  glm::vec3 minPrimCellCoord{0}, maxPrimCellCoord{0};
  size_t minCell[3];
  size_t maxCell[3];

  // Iterate over primitives to insert them in the corresponding grid's cell.
  for (auto primitive : primitives) {
    // Represent the bounding box in grid's coordinates, relative to
    // the cell dimension.
    minPrimCellCoord = glm::vec4((primitive.getBB().bounds[0] - bbox.bounds[0]) / cellDimension);
    maxPrimCellCoord = glm::vec4((primitive.getBB().bounds[1] - bbox.bounds[0]) / cellDimension);

    // Convert the minimum and maximum coordinates for the cell's encapsulating
    // a primitive into integers.

    // min, x
    minCell[0] = static_cast<size_t>(
        glm::clamp(minPrimCellCoord.x,
                   0.f,
                   1.f * (resolution[0] - 1)));
    // min, y
    minCell[1] = static_cast<size_t>(
        glm::clamp(minPrimCellCoord.y,
                   0.f,
                   1.f * (resolution[1] - 1)));
    // min, z
    minCell[2] = static_cast<size_t>(
        glm::clamp(minPrimCellCoord.z,
                   0.f,
                   1.f * (resolution[2] - 1)));
    // max, x
    maxCell[0] = static_cast<size_t>(
        glm::clamp(maxPrimCellCoord.x,
                   0.f,
                   1.f * (resolution[0] - 1)));
    // max, y
    maxCell[1] = static_cast<size_t>(
        glm::clamp(maxPrimCellCoord.y,
                   0.f,
                   1.f * (resolution[1] - 1)));
    // max, z
    maxCell[2] = static_cast<size_t>(
        glm::clamp(maxPrimCellCoord.z,
                   0.f,
                   1.f * (resolution[2] - 1)));

    // Iterate over corresponding grid cells and add primitive to it.
    for (size_t z = minCell[2]; z <= maxCell[2]; ++z) {
      for (size_t y = minCell[1]; y <= maxCell[1]; ++y) {
        for (size_t x = minCell[0]; x <= maxCell[0]; ++x) {
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
bool Grid::traverse(const Ray &r, isect_info &ii) const {
  // Scalar distance from the ray's origin to the nearest hit point of
  // the ray with the grid's bounding box.
  float_t   tBoundingBox;
  uint64_t  primitive_hit{0};

  // Check if the ray intersect's the grid at all.
  if (!bbox.intersect(r, tBoundingBox))
    return false;

  glm::vec4 deltaT, nextCrossingT;
  int currentCell[3];
  int step[3];  // Steps could be also negative.
  int exit[3];  // The same argument.

  // Compute delta t, the parametric distance along the ray between two
  // planes perpendicular to x,y or z.
  // Compute the next crossing of the ray with one of the planes perpendicular
  // to x,y, or z.
  // Take in account that the sign of the
  // ray's direction plays a role into determining the proper value.
  for (size_t axis = 0; axis < 3; axis++) {
    float_t rayOrigToGrid = (r.orig()[axis] + r.dir()[axis]*tBoundingBox) -
                            bbox.bounds[0][axis];
    currentCell[axis] = static_cast<int>(glm::clamp(
        std::floor(rayOrigToGrid / cellDimension[axis]),
        0.f,
        1.f * (resolution[axis] - 1)));

    if (r.dir()[axis] >= 0) {
      // Ray's direction is positive in axis-axis.
      deltaT[axis] = cellDimension[axis] * r.inv_dir()[axis];
      nextCrossingT[axis] = tBoundingBox +
              ((currentCell[axis] + 1) * cellDimension[axis] - rayOrigToGrid) *
              r.inv_dir()[axis];
      step[axis] = 1;
      exit[axis] = resolution[axis];
    }
    else {
      // Ray's direction is negative in axis-axis.
      deltaT[axis] = -cellDimension[axis] * r.inv_dir()[axis];
      nextCrossingT[axis] = tBoundingBox +
          (currentCell[axis] * cellDimension[axis] - rayOrigToGrid) *
          r.inv_dir()[axis];
      step[axis] = -1;
      exit[axis] = -1;
    }
  }

  // Reset the intersection information.
  ii = isect_info();

  // The actual traversal of the grid.
  while (1) {
    uint32_t cellIndex = offset(currentCell[0], currentCell[1], currentCell[2]);

    // Check if there are any primitives in the current cell and if yes
    // check if the ray intersects any of the primitives in the cell.
    if (cells[cellIndex] != nullptr) {
      cells[cellIndex]->intersect(r, ii);
      primitive_hit += cells[cellIndex]->primitives.size();
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

  ii.nrpt = primitive_hit;
  return (ii.ho != nullptr);
}
// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Grid.h"

grid_info Grid::constructGrid() {
  // Initialize the grid statistical gathering structure.
  grid_info info;

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
    if (resFloat[i] > 128.f) resFloat[i] = 128.f;
    if (resFloat[i] < 1.f) resFloat[i] = 1.f;
    // Static cast to size_t is equivalent to std::floor of the
    // floating value.
    resolution[i] = static_cast<size_t>(resFloat[i]);
    info.r[i] = resolution[i];
  }

  // Compute cell's dimension.
  cellDimension.x = bbox.getDiagonal().x / resolution[0];
  cellDimension.y = bbox.getDiagonal().y / resolution[1];
  cellDimension.z = bbox.getDiagonal().z / resolution[2];

  // Allocate memory for cells in the grid.
  size_t numOfCells = resolution[0] * resolution[1] * resolution[2];
  cells = new Cell*[numOfCells];
  // Set the cell's content initially to NULL.
  memset(cells, 0x0, sizeof(Cell*) * numOfCells);

  glm::vec3 minPrimCellCoord{0}, maxPrimCellCoord{0};
  size_t minCell[3];
  size_t maxCell[3];

  // Iterate over primitives to insert them in the corresponding grid's cell.
  for (auto primitive : primitives) {
    // Represent the bounding box in grid's coordinates, relative to
    // the cell dimension.
    minPrimCellCoord = glm::vec4((primitive.getBB()->bounds[0] - bbox.bounds[0]) / cellDimension);
    maxPrimCellCoord = glm::vec4((primitive.getBB()->bounds[1] - bbox.bounds[0]) / cellDimension);

    // Convert the minimum and maximum coordinates for the cell's encapsulating
    // a primitive into integers.

    // min, x
    minCell[0] = static_cast<size_t>(glm::clamp(minPrimCellCoord.x,
                                                0.f,
                                                1.f * (resolution[0] - 1)));
    // min, y
    minCell[1] = static_cast<size_t>(glm::clamp(minPrimCellCoord.y,
                                                0.f,
                                                1.f * (resolution[1] - 1)));
    // min, z
    minCell[2] = static_cast<size_t>(glm::clamp(minPrimCellCoord.z,
                                                0.f,
                                                1.f * (resolution[2] - 1)));
    // max, x
    maxCell[0] = static_cast<size_t>(glm::clamp(maxPrimCellCoord.x,
                                                0.f,
                                                1.f * (resolution[0] - 1)));
    // max, y
    maxCell[1] = static_cast<size_t>(glm::clamp(maxPrimCellCoord.y,
                                                0.f,
                                                1.f * (resolution[1] - 1)));
    // max, z
    maxCell[2] = static_cast<size_t>(glm::clamp(maxPrimCellCoord.z,
                                                0.f,
                                                1.f * (resolution[2] - 1)));

    // Iterate over corresponding grid cells and add primitive to it.
    for (size_t z = minCell[2]; z <= maxCell[2]; ++z) {
      for (size_t y = minCell[1]; y <= maxCell[1]; ++y) {
        for (size_t x = minCell[0]; x <= maxCell[0]; ++x) {
          // Compute the cell's index.
          size_t cellIndex = z * resolution[2] + y * resolution[1] + x;
          // Initialize cell, if needed.
          if (cells[cellIndex] == NULL) cells[cellIndex] = new Cell;
          cells[cellIndex]->insert(primitive);
        }
      }
    }
  }

  // Iterate once more over all cells to gather statistical information
  // about the grid (e.g. number of non-empty cells, av number of primitives
  // per cell)
  for (size_t i = 0; i < resolution[0] * resolution[1] * resolution[2]; i++) {
    if (cells[i] == NULL) continue;
    info.nfc++;
    info.nppc += cells[i]->primitives.size();
  }
  info.nppc /= (1.f * info.nfc);
  info.np = primitives.size();

  return info;
}

bool Grid::intersect(const Ray &r, isect_info &i) const {
  // Scalar distance from the ray's origin to the nearest hit point of
  // the ray with the grid's bounding box.
  float_t tBoundingBox;

  // Check if the ray intersect's the grid at all.
  if (!bbox.intersect(r, tBoundingBox))
    return false;

  glm::vec4 deltaT, nextCrossingT;
  size_t currentCell[3];
  int step[3];  // Steps could be also negative.
  int exit[3];  // The same argument.


  // Compute delta t, the parametric distance along the ray between two
  // planes perpendicular to x,y or z.
  // Compute the next crossing of the ray with one of the planes perpendicular
  // to x,y, or z.
  // Take in account that the sign of the
  // ray's direction plays a role into determining the proper value.
  for (size_t i = 0; i < 3; i++) {
    float_t rayOrigToGrid = (r.orig()[i] + r.dir()[i]*tBoundingBox) -
                            bbox.bounds[0][i];
    currentCell[i] = glm::clamp(static_cast<size_t>(rayOrigToGrid / cellDimension[i]),
                                static_cast<size_t>(0),
                                static_cast<size_t>(resolution[i] - 1));

    // Ray's direction is negative in i-axis.
    if (r.dir()[i] < 0) {
      deltaT[i] = -cellDimension[i] * r.inv_dir()[i];
      nextCrossingT[i] = (currentCell[i] * cellDimension[i] - rayOrigToGrid) *
                          r.inv_dir()[i];
      step[i] = -1;
      exit[i] = -1;
    } else {
      // Ray's direction is positive in i-axis.
      deltaT[i] = cellDimension[i] / r.inv_dir()[i];
      nextCrossingT[i] = ((currentCell[i] + 1) * cellDimension[i] -
                          rayOrigToGrid) * r.inv_dir()[i];
      step[i] = 1;
      exit[i] = resolution[i];
    }
  }

  // The actual traversal of the grid.
  while (1) {
    size_t cellIndex = currentCell[2] * resolution[2] +
                       currentCell[1] * resolution[1] + currentCell[1];
    if (cells[cellIndex] != nullptr) {
      cells[cellIndex]->intersect(r, i);
    }
    return (i.ho != nullptr);
  }
}
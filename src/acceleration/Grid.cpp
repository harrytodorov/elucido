// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Grid.h"

Grid::Grid(const AABBox &box,
           const std::vector<Object *> &objects) :
    AccelerationStructure(box, objects), cells(NULL) {

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

  // Convert resolution from floating number to an integer
  for (size_t i = 0; i < 3; i++) {
    if (resFloat[i] > 128.f) resFloat[i] = 128.f;
    if (resFloat[i] < 1.f) resFloat[i] = 1.f;
    // Static cast to size_t is equivalent to std::floor of the
    // floating value.
    resolution[i] = static_cast<size_t>(resFloat[i]);
  }

  // Compute cell's dimension
  cellDimension.x = bbox.getDiagonal().x / resolution[0];
  cellDimension.y = bbox.getDiagonal().y / resolution[1];
  cellDimension.z = bbox.getDiagonal().z / resolution[2];

  // Allocate memory for cells in the grid.
  size_t numOfCells = resolution[0] * resolution[1] * resolution[2];
  cells = new Grid::Cell*[numOfCells];
  // Set the cell's content initially to NULL.
  memset(cells, 0x0, sizeof(Grid::Cell*) * numOfCells);

  glm::vec3 minCellCoord{0}, maxCellCoord{0};
  size_t minCell[3];
  size_t maxCell[3];
  // Iterate over primitives to insert them in the corresponding grid's cell.
  for (auto primitive : primitives) {
    // Represent the bounding box in grid's coordinates, relative to
    // the cell dimension.
    minCellCoord = glm::vec4((primitive.getBB()->bounds[0] - bbox.bounds[0]) / cellDimension);
    maxCellCoord = glm::vec4((primitive.getBB()->bounds[1] - bbox.bounds[0]) / cellDimension);

    // Convert the minimum and maximum coordinates for the cell's encapsulating
    // a primitive into integers.

    // min, x
    minCell[0] = static_cast<size_t>(glm::clamp(minCellCoord.x,
                                                0.f,
                                                1.f * resolution[0]));
    // min, y
    minCell[1] = static_cast<size_t>(glm::clamp(minCellCoord.y,
                                                0.f,
                                                1.f * resolution[1]));
    // min, z
    minCell[2] = static_cast<size_t>(glm::clamp(minCellCoord.z,
                                                0.f,
                                                1.f * resolution[2]));
    // max, x
    maxCell[0] = static_cast<size_t>(glm::clamp(maxCellCoord.x,
                                                0.f,
                                                1.f * resolution[0]));
    // max, y
    maxCell[1] = static_cast<size_t>(glm::clamp(maxCellCoord.y,
                                                0.f,
                                                1.f * resolution[1]));
    // max, z
    maxCell[2] = static_cast<size_t>(glm::clamp(maxCellCoord.z,
                                                0.f,
                                                1.f * resolution[2]));


    // Iterate over corresponding grid cells and add primitive to it
    for (size_t z = minCell[2]; z <= maxCell[2]; z++) {
      for (size_t y = minCell[1]; y <= maxCell[1]; y++) {
        for (size_t x = minCell[0]; x <= maxCell[0]; x++) {
          // Compute the cell's index.
          size_t cellIndex = z * resolution[2] + y * resolution[1] + x;
          // Initialize cell, if needed.
          if (cells[cellIndex] == NULL) cells[cellIndex] = new Grid::Cell;
          cells[cellIndex]->insert(primitive);
        }
      }
    }
  }
}

// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <memory>

#include "../src/accelerators/Grid.h"
#include "../src/objects/TriangleMesh.h"
#include "../src/accelerators/AABBox.h"

//==============================================================================
TEST(AccelerationStructure, convertToPrimitives) {
  const char *fp = "test_resources/cube.obj";
  std::shared_ptr<Object> cube = std::make_shared<TriangleMesh>(TriangleMesh(fp));

  AABBox box;
  Grid *grid_structure = new Grid();

  auto primitives = grid_structure->convert_to_primitive(cube);

  EXPECT_EQ(primitives.size(), 12);

  delete grid_structure;
}

// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include "../include/elucido/AABBox.h"

//==============================================================================
TEST(AABBox, extend_by) {
  glm::vec4 extent1(0, 0, 0, 1);
  glm::vec4 extent2(1, 1, 1, 1);
  glm::vec4 to_extend(2, 2, 2, 1);

  AABBox to_compare(extent1, extent2);
  AABBox result(extent1, to_extend);
  to_compare.extend_by(to_extend);

  EXPECT_FLOAT_EQ(to_compare.bounds[0].x,
                  result.bounds[0].x);
  EXPECT_FLOAT_EQ(to_compare.bounds[0].y,
                  result.bounds[0].y);
  EXPECT_FLOAT_EQ(to_compare.bounds[0].z,
                  result.bounds[0].z);
  EXPECT_FLOAT_EQ(to_compare.bounds[0].w,
                  result.bounds[0].w);
}

//==============================================================================
TEST(AABBox, getVolume) {
  glm::vec4 e1(0, 0, 0, 1);
  glm::vec4 e2(1, 1, 1, 1);
  glm::vec4 e3(2, 1.5, 1, 1);

  AABBox box(e1, e1);
  EXPECT_FLOAT_EQ(0.f, box.getVolume());
  box.extend_by(e2);
  EXPECT_FLOAT_EQ(1.f, box.getVolume());
  box.extend_by(e3);
  EXPECT_FLOAT_EQ(3.f, box.getVolume());
}
// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include "../include/elucido/AABBox.h"

namespace {
class AABBTest : public ::testing::Test {

};

TEST(AABBTest, extend_by) {
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
}
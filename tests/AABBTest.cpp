// Copyright (c) 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include "glm/ext.hpp"    // glm::to_string

#include "../src/accelerators/AABBox.h"
#include "../src/core/Ray.h"

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

//==============================================================================
TEST(AABBox, getDiagonal) {
  glm::vec4 min_extent(0.f);
  glm::vec4 max_extent(2.f);

  AABBox box;
  box.extend_by(min_extent);
  box.extend_by(max_extent);

  auto diagonal = box.getDiagonal();
  EXPECT_FLOAT_EQ(diagonal.x, 2.f);
  EXPECT_FLOAT_EQ(diagonal.y, 2.f);
  EXPECT_FLOAT_EQ(diagonal.z, 2.f);
}

//==============================================================================
TEST(AABBox, intersect) {
  glm::vec4 min_extent(0.f);
  glm::vec4 max_extent(2.f);

  AABBox box;
  box.extend_by(min_extent);
  box.extend_by(max_extent);

  Ray r;
  r.set_orig(glm::vec4(1.f, 1.f, -1.f, 1.f));
  r.set_dir (glm::vec4(0.f, 0.f,  1.f, 0.f));

  auto intersected = box.intersect(r);
  EXPECT_TRUE(intersected);
}

//==============================================================================
TEST(AABBox, intersectWithMinDistance) {
  glm::vec4 min_extent(0.f);
  glm::vec4 max_extent(2.f);

  AABBox box;
  box.extend_by(min_extent);
  box.extend_by(max_extent);

  Ray r;
  r.set_orig(glm::vec4(1.f, 1.f, -1.f, 1.f));
  r.set_dir (glm::vec4(0.f, 0.f,  1.f, 0.f));

  float_t min_distance(0.f);
  auto intersected = box.intersect(r, min_distance);

  EXPECT_TRUE(intersected);
  EXPECT_FLOAT_EQ(min_distance, 1.f);
}

//==============================================================================
TEST(AABBox, intersectParallel) {
  glm::vec4 min_extent(0.f);
  glm::vec4 max_extent(2.f);

  AABBox box;
  box.extend_by(min_extent);
  box.extend_by(max_extent);

  Ray r;
  r.set_orig(glm::vec4(1.f, 2.000001f, -1.f, 1.f));
  r.set_dir (glm::vec4(0.f, 0.f,  1.f, 0.f));

  auto intersected = box.intersect(r);
  EXPECT_TRUE(!intersected);
}
// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include "glm/ext.hpp"    // glm::to_string

#include "../src/objects/Sphere.h"

//==============================================================================
TEST(Sphere, basicInitialization) {
  auto s = new Sphere();

  EXPECT_EQ(s->object_type(), sphere);
  EXPECT_FLOAT_EQ(s->radius(), 1.f);

  EXPECT_FLOAT_EQ(s->center().x, 0.f);
  EXPECT_FLOAT_EQ(s->center().y, 0.f);
  EXPECT_FLOAT_EQ(s->center().z, 0.f);
  EXPECT_FLOAT_EQ(s->center().w, 1.f);
}

//==============================================================================
TEST(Sphere, intesect) {
  Ray ray;
  ray.set_orig({0.f, 0.f, 0.f, 1.f});
  ray.set_dir( {1.f, 0.f, 0.f, 0.f});

  auto s = new Sphere();
  s->set_radius(1.f);
  s->set_center({5.f, 0.f, 0.f, 1.f});

  auto ii = isect_info();
  auto intersected = s->intersect(ray, ii);

  EXPECT_TRUE(intersected);
  EXPECT_FLOAT_EQ(ii.tn, 4.f);

  EXPECT_FLOAT_EQ(ii.ip.x, 4.f);
  EXPECT_FLOAT_EQ(ii.ip.y, 0.f);
  EXPECT_FLOAT_EQ(ii.ip.z, 0.f);
  EXPECT_FLOAT_EQ(ii.ip.w, 1.f);

  EXPECT_FLOAT_EQ(ii.ipn.x, -1.f);
  EXPECT_FLOAT_EQ(ii.ipn.y,  0.f);
  EXPECT_FLOAT_EQ(ii.ipn.z,  0.f);
  EXPECT_FLOAT_EQ(ii.ipn.w,  0.f);
}


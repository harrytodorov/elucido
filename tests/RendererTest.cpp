// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include "glm/ext.hpp"    // glm::to_string

#include "../src/core/Renderer.h"

//==============================================================================
TEST(Lambertian, testMutlipleDirections) {
  auto float_err        = 0.0001f;
  auto surface_normal   = glm::vec4(0.f,    1.f,    0.f,     0.f);
  auto ld_parallel      = glm::vec4(1.f,    0.f,     0.f,    0.f);
  auto ld_normal        = glm::vec4(0.f,    1.f,     0.f,    0.f);
  auto ld_arbitrary     = glm::vec4(0.483f, 0.782f, -0.391f, 0.f);

  auto renderer = new Renderer(nullptr, nullptr, {}, {});

  float_t la1 = renderer->labertian_amount(surface_normal, ld_parallel);
  float_t la2 = renderer->labertian_amount(surface_normal, ld_normal);
  float_t la3 = renderer->labertian_amount(surface_normal, ld_arbitrary);

  EXPECT_NEAR(la1, 0.f,    float_err);
  EXPECT_NEAR(la2, 1.f,    float_err);
  EXPECT_NEAR(la3, 0.782f, float_err);
}

//==============================================================================
TEST(Reflection, testMutlipleDirections) {
  auto float_err        = 0.0001f;
  auto surface_normal   = glm::vec4( 0.f,     1.f,    0.f, 0.f);
  auto tr1              = glm::vec4(-1.f,     0.f,    0.f, 0.f);
  auto tr2              = glm::vec4(-0.707f,  0.707f, 0.f, 0.f);
  auto tr3              = glm::vec4( 0.f,     1.f,    0.f, 0.f);
  auto tr4              = glm::vec4(-0.707f, -0.707f, 0.f, 0.f);

  auto renderer = new Renderer(nullptr, nullptr, {}, {});

  auto r1 = renderer->reflect(surface_normal, tr1);
  auto r2 = renderer->reflect(surface_normal, tr2);
  auto r3 = renderer->reflect(surface_normal, tr3);
  auto r4 = renderer->reflect(surface_normal, tr4);

  // Reflected vector 1.
  EXPECT_NEAR(r1.x,  1.f, float_err);
  EXPECT_NEAR(r1.y,  0.f, float_err);
  EXPECT_NEAR(r1.z,  0.f, float_err);
  EXPECT_NEAR(r1.w,  0.f, float_err);

  // Reflected vector 2.
  EXPECT_NEAR(r2.x,  0.707f, float_err);
  EXPECT_NEAR(r2.y,  0.707f, float_err);
  EXPECT_NEAR(r2.z,  0.f,    float_err);
  EXPECT_NEAR(r2.w,  0.f,    float_err);

  // Reflected vector 3.
  EXPECT_NEAR(r3.x,  0.f, float_err);
  EXPECT_NEAR(r3.y,  1.f, float_err);
  EXPECT_NEAR(r3.z,  0.f, float_err);
  EXPECT_NEAR(r3.w,  0.f, float_err);

  // Reflected vector 4.
  EXPECT_NEAR(r4.x,  0.707f, float_err);
  EXPECT_NEAR(r4.y, -0.707f, float_err);
  EXPECT_NEAR(r4.z,  0.f,    float_err);
  EXPECT_NEAR(r4.w,  0.f,    float_err);
}


// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>

#include "../src/lights/PointLight.h"

//==============================================================================
TEST(PointLight, translation) {
  auto pl_pos = glm::vec4(0, 0, 0, 1);
  auto *pl = new PointLight();
  pl->p = pl_pos;

  pl->translate(3, XYZ);
  pl->apply_transformations();

  EXPECT_FLOAT_EQ(pl->p.x, 3.f);
  EXPECT_FLOAT_EQ(pl->p.y, 3.f);
  EXPECT_FLOAT_EQ(pl->p.z, 3.f);
  EXPECT_FLOAT_EQ(pl->p.w, 1.f);
}

//==============================================================================
TEST(PointLight, rotation) {
  auto pl_pos = glm::vec4(0, 0, 0, 1);
  auto *pl = new PointLight();
  pl->p = pl_pos;

  pl->rotate(90, Y);
  pl->apply_transformations();

  EXPECT_FLOAT_EQ(pl->p.x, 0.f);
  EXPECT_FLOAT_EQ(pl->p.y, 0.f);
  EXPECT_FLOAT_EQ(pl->p.z, 0.f);
  EXPECT_FLOAT_EQ(pl->p.w, 1.f);
}

//==============================================================================
TEST(PointLight, translationAndRotation) {
  auto pl_pos = glm::vec4(0, 0, 0, 1);
  auto *pl = new PointLight();
  pl->p = pl_pos;

  pl->translate(3, XYZ);
  pl->rotate(90, Y);
  pl->translate(2, XY);
  pl->apply_transformations();

  EXPECT_FLOAT_EQ(pl->p.x, 5.f);
  EXPECT_FLOAT_EQ(pl->p.y, 5.f);
  EXPECT_FLOAT_EQ(pl->p.z, 3.f);
  EXPECT_FLOAT_EQ(pl->p.w, 1.f);
}

//==============================================================================
TEST(PointLight, cameraTransformationIdentity) {
  auto pl_pos = glm::vec4(1, 1, -1, 1);
  auto pl = new PointLight();
  pl->p = pl_pos;

  glm::mat4 im(1);
  pl->apply_camera_transformation(im);

  EXPECT_FLOAT_EQ(pl->p.x,  1.f);
  EXPECT_FLOAT_EQ(pl->p.y,  1.f);
  EXPECT_FLOAT_EQ(pl->p.z, -1.f);
  EXPECT_FLOAT_EQ(pl->p.w,  1.f);
}

//==============================================================================
TEST(PointLight, cameraTransformationTranslation) {
  auto pl_pos = glm::vec4(0, 0, 1, 1);
  auto *pl = new PointLight();
  pl->p = pl_pos;

  glm::vec3 tv(0, 0, 3);
  glm::mat4 tm = glm::translate(glm::mat4(1), tv);
  tm = glm::inverse(tm);
  pl->apply_camera_transformation(tm);

  EXPECT_FLOAT_EQ(pl->p.x,  0.f);
  EXPECT_FLOAT_EQ(pl->p.y,  0.f);
  EXPECT_FLOAT_EQ(pl->p.z, -2.f);
  EXPECT_FLOAT_EQ(pl->p.w,  1.f);
}

//==============================================================================
TEST(PointLight, cameraTransformationRotation) {
  auto float_err = 0.000001f;

  auto pl_pos = glm::vec4(0, 0, 1, 1);
  auto *pl = new PointLight();
  pl->p = pl_pos;

  auto tv = glm::vec3(0, 1, 0);
  auto rm = glm::rotate(glm::mat4(1), glm::radians(90.f), tv);
  rm = glm::inverse(rm);
  pl->apply_camera_transformation(rm);

  EXPECT_NEAR(pl->p.x, -1.f, float_err);
  EXPECT_NEAR(pl->p.y,  0.f, float_err);
  EXPECT_NEAR(pl->p.z,  0.f, float_err);
  EXPECT_NEAR(pl->p.w,  1.f, float_err);
}

//==============================================================================
TEST(PointLight, cameraTransformationTranslationAndRotation) {
  auto float_err = 0.000001f;

  auto pl_pos = glm::vec4(0, 0, 2, 1);
  auto *pl = new PointLight();
  pl->p = pl_pos;

  auto tv = glm::vec3(0, 0, 3);
  auto mat = glm::translate(glm::mat4(1), tv);
  tv.z = 0.f;
  tv.y = 1.f;

  auto rm = glm::rotate(mat, glm::radians(90.f), tv);

  rm = glm::inverse(rm);
  pl->apply_camera_transformation(rm);

  EXPECT_NEAR(pl->p.x, 1.f, float_err);
  EXPECT_NEAR(pl->p.y, 0.f, float_err);
  EXPECT_NEAR(pl->p.z, 0.f, float_err);
  EXPECT_NEAR(pl->p.w, 1.f, float_err);
}

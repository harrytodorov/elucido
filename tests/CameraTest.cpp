// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>

#include "../src/cameras/PerspectiveCamera.h"
#include "../src/cameras/OrthographicCamera.h"

//==============================================================================
TEST(PerspectiveCamera, getRay) {
  auto float_err = 0.001f;

}

//==============================================================================
TEST(OrthographicCamera, getRay) {
  auto float_err = 0.001f;

  float_t zoom_factor   = 1.f;
  uint32_t image_width  = 6;
  uint32_t image_height = 3;
  auto camera = OrthographicCamera(zoom_factor, image_width, image_height);

  // Camera ray 1.
  uint32_t cr1_pixel_x = 0;
  uint32_t cr1_pixel_y = 2;
  float_t cr1_sample_x = 0.5f;
  float_t cr1_sample_y = 0.5f;
  Ray cr1 = camera.get_ray(cr1_pixel_x, cr1_pixel_y, cr1_sample_x, cr1_sample_y);

  EXPECT_EQ(cr1.rt, primary);

  EXPECT_FLOAT_EQ(cr1.dir().x,  0.f);
  EXPECT_FLOAT_EQ(cr1.dir().y,  0.f);
  EXPECT_FLOAT_EQ(cr1.dir().z, -1.f);
  EXPECT_FLOAT_EQ(cr1.dir().w,  0.f);

  EXPECT_NEAR(cr1.orig().x, -1.666f, float_err);
  EXPECT_NEAR(cr1.orig().y, -0.666f, float_err);
  EXPECT_NEAR(cr1.orig().z,  0.f,    float_err);
  EXPECT_NEAR(cr1.orig().w,  1.f,    float_err);

  // Camera ray 2.
  uint32_t cr2_pixel_x = 1;
  uint32_t cr2_pixel_y = 1;
  float_t cr2_sample_x = 0.f;
  float_t cr2_sample_y = 0.f;
  Ray cr2 = camera.get_ray(cr2_pixel_x, cr2_pixel_y, cr2_sample_x, cr2_sample_y);

  EXPECT_EQ(cr2.rt, primary);

  EXPECT_FLOAT_EQ(cr2.dir().x,  0.f);
  EXPECT_FLOAT_EQ(cr2.dir().y,  0.f);
  EXPECT_FLOAT_EQ(cr2.dir().z, -1.f);
  EXPECT_FLOAT_EQ(cr2.dir().w,  0.f);

  EXPECT_NEAR(cr2.orig().x, -1.333f, float_err);
  EXPECT_NEAR(cr2.orig().y,  0.333f, float_err);
  EXPECT_NEAR(cr2.orig().z,  0.f,    float_err);
  EXPECT_NEAR(cr2.orig().w,  1.f,    float_err);

  // Camera ray 3.
  uint32_t cr3_pixel_x = 5;
  uint32_t cr3_pixel_y = 0;
  float_t cr3_sample_x = 1.f;
  float_t cr3_sample_y = 0.f;
  Ray cr3 = camera.get_ray(cr3_pixel_x, cr3_pixel_y, cr3_sample_x, cr3_sample_y);

  EXPECT_EQ(cr3.rt, primary);

  EXPECT_FLOAT_EQ(cr3.dir().x,  0.f);
  EXPECT_FLOAT_EQ(cr3.dir().y,  0.f);
  EXPECT_FLOAT_EQ(cr3.dir().z, -1.f);
  EXPECT_FLOAT_EQ(cr3.dir().w,  0.f);

  EXPECT_NEAR(cr3.orig().x,  2.f, float_err);
  EXPECT_NEAR(cr3.orig().y,  1.f, float_err);
  EXPECT_NEAR(cr3.orig().z,  0.f, float_err);
  EXPECT_NEAR(cr3.orig().w,  1.f, float_err);

  // Camera ray 4.
  uint32_t cr4_pixel_x = 4;
  uint32_t cr4_pixel_y = 1;
  float_t cr4_sample_x = 0.75f;
  float_t cr4_sample_y = 0.25f;
  Ray cr4 = camera.get_ray(cr4_pixel_x, cr4_pixel_y, cr4_sample_x, cr4_sample_y);

  EXPECT_EQ(cr4.rt, primary);

  EXPECT_FLOAT_EQ(cr4.dir().x,  0.f);
  EXPECT_FLOAT_EQ(cr4.dir().y,  0.f);
  EXPECT_FLOAT_EQ(cr4.dir().z, -1.f);
  EXPECT_FLOAT_EQ(cr4.dir().w,  0.f);

  EXPECT_NEAR(cr4.orig().x,  1.166f, float_err);
  EXPECT_NEAR(cr4.orig().y,  0.166f, float_err);
  EXPECT_NEAR(cr4.orig().z,  0.f, float_err);
  EXPECT_NEAR(cr4.orig().w,  1.f, float_err);
}

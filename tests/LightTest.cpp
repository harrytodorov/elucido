// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include "glm/ext.hpp"    // glm::to_string

#include "../src/lights/DirectionalLight.h"
#include "../src/lights/PointLight.h"

//==============================================================================
TEST(DirectionalLight, directionDistanceAndIntensity) {
  float_t float_err = 0.001f;

  auto surface_point = glm::vec4(0.f, 0.f, 0.f, 1.f);

  auto directional_light = new DirectionalLight();
  directional_light->set_color(glm::vec3(1.f)); // White.
  directional_light->set_intensity(10.f);
  directional_light->set_direction(glm::vec4(-0.707f, -0.707f, 0.f, 0.f));

  auto direction = directional_light->get_direction(surface_point);
  EXPECT_NEAR(direction.x, 0.707f, float_err);
  EXPECT_NEAR(direction.y, 0.707f, float_err);
  EXPECT_NEAR(direction.z, 0.f,    float_err);
  EXPECT_NEAR(direction.w, 0.f,    float_err);

  auto distance = directional_light->get_distance(surface_point);
  EXPECT_NEAR(distance, infinity, float_err);

  auto intensity = directional_light->get_intensity(distance);
  EXPECT_NEAR(intensity, 10.f, float_err);
}

//==============================================================================
TEST(PointLight, directionDistanceAndIntensity) {
  float_t float_err = 0.001f;

  auto surface_point = glm::vec4(0.f, 0.f, 0.f, 1.f);

  auto point_light = new PointLight();
  point_light->set_color(glm::vec3(1.f)); // White.
  point_light->set_intensity(10.f);
  point_light->set_position(glm::vec4(7.f, 4.f, 0.f, 1.f));

  auto direction = point_light->get_direction(surface_point);
  EXPECT_NEAR(direction.x, 0.868f, float_err);
  EXPECT_NEAR(direction.y, 0.496f, float_err);
  EXPECT_NEAR(direction.z, 0.f,    float_err);
  EXPECT_NEAR(direction.w, 0.f,    float_err);

  auto distance = point_light->get_distance(surface_point);
  EXPECT_NEAR(distance, 8.062f, float_err);

  auto intensity = point_light->get_intensity(distance);
  EXPECT_NEAR(intensity, 0.0122f, float_err);
}

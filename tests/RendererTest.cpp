// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include "glm/ext.hpp"    // glm::to_string
#include <memory>

#include "../src/core/Renderer.h"
#include "../src/lights/DirectionalLight.h"
#include "../src/lights/PointLight.h"
#include "../src/objects/Sphere.h"
#include "../src/lights/Light.h"

//==============================================================================
TEST(Lambertian, testMutlipleDirections) {
  auto float_err        = 0.0001f;
  auto surface_normal   = glm::vec4(0.f,    1.f,    0.f,     0.f);
  auto ld_parallel      = glm::vec4(1.f,    0.f,     0.f,    0.f);
  auto ld_normal        = glm::vec4(0.f,    1.f,     0.f,    0.f);
  auto ld_arbitrary     = glm::vec4(0.483f, 0.782f, -0.391f, 0.f);

  auto renderer = new Renderer(nullptr, AABBox(), {}, {});

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

  auto renderer = new Renderer(nullptr, AABBox(), {}, {});

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

//==============================================================================
TEST(Phong, testEvaluation) {
  float_t float_err = 0.001f;

  auto white_color = glm::vec3(1.f);

  auto l1 = std::make_shared<DirectionalLight>(DirectionalLight());
  l1->set_direction(glm::vec4(-0.707f, -0.707f, 0.f, 0.f));
  l1->set_intensity(10.f);
  l1->set_color(white_color);

  auto l2 = std::make_shared<PointLight>(PointLight());
  l2->set_position(glm::vec4(-1.f, 1.f, 0.f, 1.f));
  l2->set_intensity(5.f);
  l2->set_color(white_color);

  auto l3 = std::make_shared<PointLight>(PointLight());
  l3->set_position(glm::vec4(-1.f, 0.f, 0.f, 1.f));
  l3->set_intensity(1.f);
  l3->set_color(white_color);

  std::vector<std::shared_ptr<Light>> lights;
  lights.push_back(l1);
  lights.push_back(l2);
  lights.push_back(l3);

  material m;
  m.mt = phong;
  m.ac = 0.f;
  m.dc = 0.8f;
  m.sc = 0.2f;
  m.se = 10.f;

  auto dummy = std::make_shared<Sphere>(Sphere());
  dummy->set_material(m);

  isect_info i;
  i.ip  = glm::vec4(0.f, 0.f, 0.f, 1.f);
  i.ipn = glm::vec4(-1.f, 0.f, 0.f, 0.f);
  i.ho  = dummy;

  auto ray_direction = glm::vec4(1.f, 0.f, 0.f, 0.f);

  Renderer renderer(nullptr, AABBox(), {}, lights);
  auto radiance = renderer.evaluate_phong(i, ray_direction);
  EXPECT_NEAR(radiance.r, 0.193f, float_err);
  EXPECT_NEAR(radiance.g, 0.193f, float_err);
  EXPECT_NEAR(radiance.b, 0.193f, float_err);
}
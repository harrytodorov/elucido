// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include "glm/ext.hpp"    // glm::to_string

#include "../src/objects/Sphere.h"
#include "../src/objects/Triangle.h"

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
// Ray's origin and center of the sphere lie on the same plane.
TEST(Sphere, intersectPlanar) {
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

//==============================================================================
TEST(Sphere, intersectParallel) {
  Ray ray;
  ray.set_orig({0.f, 1.f, 0.f, 1.f});
  ray.set_dir( {1.f, 0.f, 0.f, 0.f});

  auto s = new Sphere();
  s->set_radius(1.f);
  s->set_center({5.f, 0.f, 0.f, 1.f});

  auto ii = isect_info();
  auto intersected = s->intersect(ray, ii);

  EXPECT_TRUE(intersected);
  EXPECT_FLOAT_EQ(ii.tn, 5.f);

  EXPECT_FLOAT_EQ(ii.ip.x, 5.f);
  EXPECT_FLOAT_EQ(ii.ip.y, 1.f);
  EXPECT_FLOAT_EQ(ii.ip.z, 0.f);
  EXPECT_FLOAT_EQ(ii.ip.w, 1.f);

  EXPECT_FLOAT_EQ(ii.ipn.x,  0.f);
  EXPECT_FLOAT_EQ(ii.ipn.y,  1.f);
  EXPECT_FLOAT_EQ(ii.ipn.z,  0.f);
  EXPECT_FLOAT_EQ(ii.ipn.w,  0.f);
}

//==============================================================================
TEST(Sphere, intersectInside) {
  float_t float_err = 0.001;

  Ray ray;
  ray.set_orig({-1.f,    0.f,    0.f, 1.f});
  ray.set_dir( { 0.707f, 0.707f, 0.f, 0.f});

  auto s = new Sphere();
  s->set_radius(3.f);
  s->set_center({0.f, 0.f, 0.f, 1.f});

  auto ii = isect_info();
  auto intersected = s->intersect(ray, ii);

  EXPECT_TRUE(intersected);
  EXPECT_NEAR(ii.tn, 3.622f, float_err);

  EXPECT_NEAR(ii.ip.x, 1.561f, float_err);
  EXPECT_NEAR(ii.ip.y, 2.561f, float_err);
  EXPECT_NEAR(ii.ip.z, 0.f,    float_err);
  EXPECT_NEAR(ii.ip.w, 1.f,    float_err);

  EXPECT_NEAR(ii.ipn.x,  0.520f, float_err);
  EXPECT_NEAR(ii.ipn.y,  0.853f, float_err);
  EXPECT_NEAR(ii.ipn.z,  0.f,    float_err);
  EXPECT_NEAR(ii.ipn.w,  0.f,    float_err);
}

//==============================================================================
TEST(Sphere, intersectOutside) {
  Ray ray;
  ray.set_orig({0.f, 1.f, 0.f, 1.f});
  ray.set_dir( {1.f, 0.f, 0.f, 0.f});

  auto s = new Sphere();
  s->set_radius(0.8f);
  s->set_center({5.f, 0.f, 0.f, 1.f});

  auto ii = isect_info();
  auto intersected = s->intersect(ray, ii);

  EXPECT_TRUE(!intersected);
}

//==============================================================================
// Ray's origin and center of the sphere lie on the same plane.
TEST(Sphere, shadowIntersectPlanar) {
  Ray ray;
  ray.set_orig({0.f, 0.f, 0.f, 1.f});
  ray.set_dir( {1.f, 0.f, 0.f, 0.f});

  auto s = new Sphere();
  s->set_radius(1.f);
  s->set_center({5.f, 0.f, 0.f, 1.f});

  auto intersected = s->shadow_intersect(ray);

  EXPECT_TRUE(intersected);
}

//==============================================================================
TEST(Sphere, shadowIntersectParallel) {
  Ray ray;
  ray.set_orig({0.f, 1.f, 0.f, 1.f});
  ray.set_dir( {1.f, 0.f, 0.f, 0.f});

  auto s = new Sphere();
  s->set_radius(1.f);
  s->set_center({5.f, 0.f, 0.f, 1.f});

  auto intersected = s->shadow_intersect(ray);

  EXPECT_TRUE(intersected);
}

//==============================================================================
TEST(Sphere, shadowIntersectInside) {

  Ray ray;
  ray.set_orig({-1.f,    0.f,    0.f, 1.f});
  ray.set_dir( { 0.707f, 0.707f, 0.f, 0.f});

  auto s = new Sphere();
  s->set_radius(3.f);
  s->set_center({0.f, 0.f, 0.f, 1.f});

  auto intersected = s->shadow_intersect(ray);

  EXPECT_TRUE(intersected);
}

//==============================================================================
TEST(Sphere, shadowIntersectOutside) {
  Ray ray;
  ray.set_orig({0.f, 1.f, 0.f, 1.f});
  ray.set_dir( {1.f, 0.f, 0.f, 0.f});

  auto s = new Sphere();
  s->set_radius(0.8f);
  s->set_center({5.f, 0.f, 0.f, 1.f});

  auto intersected = s->shadow_intersect(ray);

  EXPECT_TRUE(!intersected);
}

//==============================================================================
TEST(Triangle, basicInitialization) {
  Triangle t;

  EXPECT_EQ(t.object_type(), triangle);

  EXPECT_FLOAT_EQ(t.vert0().x, -1.f);
  EXPECT_FLOAT_EQ(t.vert0().y, -1.f);
  EXPECT_FLOAT_EQ(t.vert0().z,  0.f);
  EXPECT_FLOAT_EQ(t.vert0().w,  1.f);

  EXPECT_FLOAT_EQ(t.vert1().x,  1.f);
  EXPECT_FLOAT_EQ(t.vert1().y, -1.f);
  EXPECT_FLOAT_EQ(t.vert1().z,  0.f);
  EXPECT_FLOAT_EQ(t.vert1().w,  1.f);

  EXPECT_FLOAT_EQ(t.vert2().x,  0.f);
  EXPECT_FLOAT_EQ(t.vert2().y,  1.f);
  EXPECT_FLOAT_EQ(t.vert2().z,  0.f);
  EXPECT_FLOAT_EQ(t.vert2().w,  1.f);

  EXPECT_FLOAT_EQ(t.normal().x, 0.f);
  EXPECT_FLOAT_EQ(t.normal().y, 0.f);
  EXPECT_FLOAT_EQ(t.normal().z, 1.f);
  EXPECT_FLOAT_EQ(t.normal().w, 0.f);
}

//==============================================================================
TEST(Triangle, intersectPlanar) {
  float_t float_err = 0.001;

  Ray ray;
  ray.set_orig({0.f, 0.f,  2.f, 1.f});
  ray.set_dir( {0.f, 0.f, -1.f, 0.f});

  auto v0 = glm::vec4(-1.f, -1.f, 0.f, 1.f);
  auto v1 = glm::vec4( 1.f, -1.f, 0.f, 1.f);
  auto v2 = glm::vec4( 0.f,  1.f, 0.f, 1.f);
  // Normal = {0, 0, 1, 0}

  Triangle t;
  t.set_vertices(v0, v1, v2);

  auto ii = isect_info();
  auto intersected = t.intersect(ray, ii);

  EXPECT_TRUE(intersected);
  EXPECT_NEAR(ii.tn, 2.f, float_err);

  EXPECT_NEAR(ii.ip.x, 0.f, float_err);
  EXPECT_NEAR(ii.ip.y, 0.f, float_err);
  EXPECT_NEAR(ii.ip.z, 0.f, float_err);
  EXPECT_NEAR(ii.ip.w, 1.f, float_err);

  EXPECT_NEAR(ii.ipn.x, 0.f, float_err);
  EXPECT_NEAR(ii.ipn.y, 0.f, float_err);
  EXPECT_NEAR(ii.ipn.z, 1.f, float_err);
  EXPECT_NEAR(ii.ipn.w, 0.f, float_err);
}

//==============================================================================
TEST(Triangle, intersectParallel) {
  Ray ray;
  ray.set_orig({0.f,  2.f,  0.f, 1.f});
  ray.set_dir( {0.f, -1.f,  0.f, 0.f});

  auto v0 = glm::vec4(-1.f, -1.f, 0.f, 1.f);
  auto v1 = glm::vec4( 1.f, -1.f, 0.f, 1.f);
  auto v2 = glm::vec4( 0.f,  1.f, 0.f, 1.f);
  // Normal = {0, 0, 1, 0}

  Triangle t;
  t.set_vertices(v0, v1, v2);

  auto ii = isect_info();
  auto intersected = t.intersect(ray, ii);

  EXPECT_TRUE(!intersected);
}

//==============================================================================
TEST(Triangle, intersectOutside) {
  Ray ray;
  ray.set_orig({-1.f,  0.f,  2.f, 1.f});
  ray.set_dir( { 0.f,  0.f, -1.f, 0.f});

  auto v0 = glm::vec4(-1.f, -1.f, 0.f, 1.f);
  auto v1 = glm::vec4( 1.f, -1.f, 0.f, 1.f);
  auto v2 = glm::vec4( 0.f,  1.f, 0.f, 1.f);
  // Normal = {0, 0, 1, 0}

  Triangle t;
  t.set_vertices(v0, v1, v2);

  auto ii = isect_info();
  auto intersected = t.intersect(ray, ii);

  EXPECT_TRUE(!intersected);
}

//==============================================================================
TEST(Triangle, shadowIntersectPlanar) {
  float_t float_err = 0.001;

  Ray ray;
  ray.set_orig({0.f, 0.f,  2.f, 1.f});
  ray.set_dir( {0.f, 0.f, -1.f, 0.f});

  auto v0 = glm::vec4(-1.f, -1.f, 0.f, 1.f);
  auto v1 = glm::vec4( 1.f, -1.f, 0.f, 1.f);
  auto v2 = glm::vec4( 0.f,  1.f, 0.f, 1.f);
  // Normal = {0, 0, 1, 0}

  Triangle t;
  t.set_vertices(v0, v1, v2);

  auto intersected = t.shadow_intersect(ray);

  EXPECT_TRUE(intersected);
}

//==============================================================================
TEST(Triangle, shadowIntersectParallel) {
  Ray ray;
  ray.set_orig({0.f,  2.f,  0.f, 1.f});
  ray.set_dir( {0.f, -1.f,  0.f, 0.f});

  auto v0 = glm::vec4(-1.f, -1.f, 0.f, 1.f);
  auto v1 = glm::vec4( 1.f, -1.f, 0.f, 1.f);
  auto v2 = glm::vec4( 0.f,  1.f, 0.f, 1.f);
  // Normal = {0, 0, 1, 0}

  Triangle t;
  t.set_vertices(v0, v1, v2);

  auto intersected = t.shadow_intersect(ray);

  EXPECT_TRUE(!intersected);
}

//==============================================================================
TEST(Triangle, shadowIntersectOutside) {
  Ray ray;
  ray.set_orig({-1.f,  0.f,  2.f, 1.f});
  ray.set_dir( { 0.f,  0.f, -1.f, 0.f});

  auto v0 = glm::vec4(-1.f, -1.f, 0.f, 1.f);
  auto v1 = glm::vec4( 1.f, -1.f, 0.f, 1.f);
  auto v2 = glm::vec4( 0.f,  1.f, 0.f, 1.f);
  // Normal = {0, 0, 1, 0}

  Triangle t;
  t.set_vertices(v0, v1, v2);

  auto intersected = t.shadow_intersect(ray);

  EXPECT_TRUE(!intersected);
}

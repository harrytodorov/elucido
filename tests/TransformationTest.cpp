// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include "glm/ext.hpp"    // glm::to_string

#include "../src/lights/PointLight.h"
#include "../src/lights/DirectionalLight.h"
#include "../src/objects/Sphere.h"
#include "../src/objects/Triangle.h"

//==============================================================================
TEST(PointLight, translation) {
  auto pl_pos = glm::vec4(0, 0, 0, 1);
  auto *pl = new PointLight();
  pl->p = pl_pos;

  pl->translate(3, uniform);
  pl->apply_transformations();

  EXPECT_FLOAT_EQ(pl->p.x, 3.f);
  EXPECT_FLOAT_EQ(pl->p.y, 3.f);
  EXPECT_FLOAT_EQ(pl->p.z, 3.f);
  EXPECT_FLOAT_EQ(pl->p.w, 1.f);
}

//==============================================================================
TEST(PointLight, successiveTranslation) {
  auto pl_pos = glm::vec4(0, 0, 0, 1);
  auto *pl = new PointLight();
  pl->p = pl_pos;

  pl->translate(3, uniform);
  pl->translate(2, X);
  pl->translate(2, Y);
  pl->apply_transformations();

  EXPECT_FLOAT_EQ(pl->p.x, 5.f);
  EXPECT_FLOAT_EQ(pl->p.y, 5.f);
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

  pl->translate(3, uniform);
  pl->rotate(90, Y);
  pl->translate(2, X);
  pl->translate(2, Y);
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
  im = glm::inverse(im);
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

  auto rv = glm::vec3(0, 1, 0);
  auto rm = glm::rotate(glm::mat4(1), glm::radians(90.f), rv);
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

//==============================================================================
TEST(DirectionalLight, translation) {
  auto float_err = 0.000001f;

  auto dl_dir = glm::vec4(1, 0, 0, 0);
  auto *dl = new DirectionalLight();
  dl->d = dl_dir;

  dl->translate(3.f, uniform);
  dl->apply_transformations();

  EXPECT_NEAR(dl->d.x, 1.f, float_err);
  EXPECT_NEAR(dl->d.y, 0.f, float_err);
  EXPECT_NEAR(dl->d.z, 0.f, float_err);
  EXPECT_NEAR(dl->d.w, 0.f, float_err);
}

//==============================================================================
TEST(DirectionalLight, rotation) {
  auto float_err = 0.00001f;

  auto dl_dir = glm::vec4(1, 0, 0, 0);
  auto *dl = new DirectionalLight();
  dl->d = dl_dir;

  dl->rotate(-90.f, Y);
  dl->apply_transformations();

  EXPECT_NEAR(dl->d.x, 0.f, float_err);
  EXPECT_NEAR(dl->d.y, 0.f, float_err);
  EXPECT_NEAR(dl->d.z, 1.f, float_err);
  EXPECT_NEAR(dl->d.w, 0.f, float_err);
}

//==============================================================================
TEST(DirectionalLight, successiveRotation) {
  auto float_err = 0.00001f;

  auto dl_dir = glm::vec4(1, 0, 0, 0);
  auto *dl = new DirectionalLight();
  dl->d = dl_dir;

  dl->rotate(-90.f, Y);
  dl->rotate( 90.f, X);
  dl->apply_transformations();

  EXPECT_NEAR(dl->d.x,  0.f, float_err);
  EXPECT_NEAR(dl->d.y, -1.f, float_err);
  EXPECT_NEAR(dl->d.z,  0.f, float_err);
  EXPECT_NEAR(dl->d.w,  0.f, float_err);
}

//==============================================================================
TEST(DirectionalLight, translationAndRotation) {
  auto float_err = 0.00001f;

  auto dl_dir = glm::vec4(1, 0, 0, 0);
  auto *dl = new DirectionalLight();
  dl->d = dl_dir;

  dl->rotate(90.f, Y);
  dl->translate(3, uniform);
  dl->apply_transformations();

  EXPECT_NEAR(dl->d.x,  0.f, float_err);
  EXPECT_NEAR(dl->d.y,  0.f, float_err);
  EXPECT_NEAR(dl->d.z, -1.f, float_err);
  EXPECT_NEAR(dl->d.w,  0.f, float_err);
}

//==============================================================================
TEST(DirectionalLight, cameraTransforamtionIdentity) {
  auto float_err = 0.00001f;

  auto dl_dir = glm::vec4(1, 0, 0, 0);
  auto *dl = new DirectionalLight();
  dl->d = dl_dir;

  glm::mat4 im(1);
  im = glm::inverse(im);
  dl->apply_camera_transformation(im);

  EXPECT_NEAR(dl->d.x, 1.f, float_err);
  EXPECT_NEAR(dl->d.y, 0.f, float_err);
  EXPECT_NEAR(dl->d.z, 0.f, float_err);
  EXPECT_NEAR(dl->d.w, 0.f, float_err);
}

//==============================================================================
TEST(DirectionalLight, cameraTransforamtionTranslation) {
  auto float_err = 0.00001f;

  auto dl_dir = glm::vec4(1, 0, 0, 0);
  auto *dl = new DirectionalLight();
  dl->d = dl_dir;

  auto tv = glm::vec3(3);
  auto mat = glm::translate(glm::mat4(1), tv);
  mat = glm::inverse(mat);
  dl->apply_camera_transformation(mat);

  EXPECT_NEAR(dl->d.x, 1.f, float_err);
  EXPECT_NEAR(dl->d.y, 0.f, float_err);
  EXPECT_NEAR(dl->d.z, 0.f, float_err);
  EXPECT_NEAR(dl->d.w, 0.f, float_err);
}

//==============================================================================
TEST(DirectionalLight, cameraTransforamtionRotation) {
  auto float_err = 0.00001f;

  auto dl_dir = glm::vec4(0, 0, 1, 0);
  auto *dl = new DirectionalLight();
  dl->d = dl_dir;

  auto rv = glm::vec3(0, 1, 0);
  auto mat = glm::rotate(glm::mat4(1), glm::radians(90.f), rv);
  mat = glm::inverse(mat);
  dl->apply_camera_transformation(mat);

  EXPECT_NEAR(dl->d.x, -1.f, float_err);
  EXPECT_NEAR(dl->d.y,  0.f, float_err);
  EXPECT_NEAR(dl->d.z,  0.f, float_err);
  EXPECT_NEAR(dl->d.w,  0.f, float_err);
}

//==============================================================================
TEST(DirectionalLight, cameraTransforamtionTranslationAndRotation) {
  auto float_err = 0.00001f;

  auto dl_dir = glm::vec4(0, 0, 1, 0);
  auto *dl = new DirectionalLight();
  dl->d = dl_dir;

  glm::mat4 mat(1);

  auto v = glm::vec3(0, 1, 0);
  auto rm = glm::rotate(glm::mat4(1), glm::radians(90.f), v);
  mat = rm * mat;

  v.x = 10; v.z = 10;
  auto tm = glm::translate(glm::mat4(1), v);
  mat = tm * mat;

  mat = glm::inverse(mat);
  dl->apply_camera_transformation(mat);

  EXPECT_NEAR(dl->d.x, -1.f, float_err);
  EXPECT_NEAR(dl->d.y,  0.f, float_err);
  EXPECT_NEAR(dl->d.z,  0.f, float_err);
  EXPECT_NEAR(dl->d.w,  0.f, float_err);
}

//==============================================================================
TEST(Sphere, translation) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(0, 0, 0, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  s->translate(3.f, uniform);
  s->apply_transformations();

  EXPECT_NEAR(s->center().x, 3.f, float_err);
  EXPECT_NEAR(s->center().y, 3.f, float_err);
  EXPECT_NEAR(s->center().z, 3.f, float_err);
  EXPECT_NEAR(s->center().w, 1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 1.f);
}

//==============================================================================
TEST(Sphere, successiveTranslation) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(0, 0, 0, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  s->translate(3.f, uniform);
  s->translate(2.f, Y);
  s->translate(2.f, Z);
  s->apply_transformations();

  EXPECT_NEAR(s->center().x, 3.f, float_err);
  EXPECT_NEAR(s->center().y, 5.f, float_err);
  EXPECT_NEAR(s->center().z, 5.f, float_err);
  EXPECT_NEAR(s->center().w, 1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 1.f);
}

//==============================================================================
TEST(Sphere, rotation) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(0, 0, 0, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  s->rotate(90.f, X);
  s->apply_transformations();

  EXPECT_NEAR(s->center().x, 0.f, float_err);
  EXPECT_NEAR(s->center().y, 0.f, float_err);
  EXPECT_NEAR(s->center().z, 0.f, float_err);
  EXPECT_NEAR(s->center().w, 1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 1.f);
}

//==============================================================================
TEST(Sphere, scaleNotUniform) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(0, 0, 0, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  s->scale(3.f, X);
  s->scale(3.f, Y);
  s->apply_transformations();

  EXPECT_NEAR(s->center().x, 0.f, float_err);
  EXPECT_NEAR(s->center().y, 0.f, float_err);
  EXPECT_NEAR(s->center().z, 0.f, float_err);
  EXPECT_NEAR(s->center().w, 1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 1.f);
}

//==============================================================================
TEST(Sphere, scaleUniform) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(0, 0, 0, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  s->scale(3.f, uniform);
  s->apply_transformations();

  EXPECT_NEAR(s->center().x, 0.f, float_err);
  EXPECT_NEAR(s->center().y, 0.f, float_err);
  EXPECT_NEAR(s->center().z, 0.f, float_err);
  EXPECT_NEAR(s->center().w, 1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 3.f);
}

//==============================================================================
TEST(Sphere, TranslationAndscaleUniform) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(0, 0, 0, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  s->scale(3.f, uniform);
  s->translate(4.f, X);
  s->translate(4.f, Z);
  s->apply_transformations();

  EXPECT_NEAR(s->center().x, 4.f, float_err);
  EXPECT_NEAR(s->center().y, 0.f, float_err);
  EXPECT_NEAR(s->center().z, 4.f, float_err);
  EXPECT_NEAR(s->center().w, 1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 3.f);
}

//==============================================================================
TEST(Sphere, cameraTransformationIdentity) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(0, 0, 0, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  glm::mat4 im(1);
  im = glm::inverse(im);

  s->apply_camera_transformation(im);

  EXPECT_NEAR(s->center().x, 0.f, float_err);
  EXPECT_NEAR(s->center().y, 0.f, float_err);
  EXPECT_NEAR(s->center().z, 0.f, float_err);
  EXPECT_NEAR(s->center().w, 1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 1.f);
}

//==============================================================================
TEST(Sphere, cameraTransformationTranslation) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(2, 0, -2, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  glm::mat4 mat(1);

  auto tv = glm::vec3(-1, 0, 1);
  auto tm = glm::translate(glm::mat4(1), tv);
  mat = tm * mat;

  mat = glm::inverse(mat);

  s->apply_camera_transformation(mat);

  EXPECT_NEAR(s->center().x,  3.f, float_err);
  EXPECT_NEAR(s->center().y,  0.f, float_err);
  EXPECT_NEAR(s->center().z, -3.f, float_err);
  EXPECT_NEAR(s->center().w,  1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 1.f);
}

//==============================================================================
TEST(Sphere, cameraTransformationRotation) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(0, 0, -2, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  glm::mat4 mat(1);

  auto rv = glm::vec3(0, 1, 0);
  auto rm = glm::rotate(glm::mat4(1), glm::radians(180.f), rv);
  mat = rm * mat;

  mat = glm::inverse(mat);

  s->apply_camera_transformation(mat);

  EXPECT_NEAR(s->center().x, 0.f, float_err);
  EXPECT_NEAR(s->center().y, 0.f, float_err);
  EXPECT_NEAR(s->center().z, 2.f, float_err);
  EXPECT_NEAR(s->center().w, 1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 1.f);
}

//==============================================================================
TEST(Sphere, cameraTransformationTranslationAndRotation) {
  auto float_err = 0.00001f;

  auto s_center = glm::vec4(0, 0, -2, 1);
  auto s_rad    = 1.f;
  auto *s = new Sphere();
  s->set_center(s_center);
  s->set_radius(s_rad);

  glm::mat4 mat(1);

  auto v = glm::vec3(0, 1, 0);
  auto rm = glm::rotate(glm::mat4(1), glm::radians(180.f), v);
  mat = rm * mat;

  v.x = 2; v.z = 3;
  auto tm = glm::translate(glm::mat4(1), v);
  mat = tm * mat;

  mat = glm::inverse(mat);
  s->apply_camera_transformation(mat);

  EXPECT_NEAR(s->center().x,  2.f, float_err);
  EXPECT_NEAR(s->center().y, -1.f, float_err);
  EXPECT_NEAR(s->center().z,  5.f, float_err);
  EXPECT_NEAR(s->center().w,  1.f, float_err);
  EXPECT_FLOAT_EQ(s->radius(), 1.f);
}

//==============================================================================
TEST(Triangle, translation) {
  auto float_err = 0.00001f;

  auto v0 = glm::vec4(-1, -1, 0, 1);
  auto v1 = glm::vec4( 1, -1, 0, 1);
  auto v2 = glm::vec4( 0,  1, 0, 1);
  auto *t = new Triangle(v0, v1, v2);

  t->translate(2.f, Z);
  t->apply_transformations();

  EXPECT_NEAR(t->v0.x, -1.f, float_err);
  EXPECT_NEAR(t->v0.y, -1.f, float_err);
  EXPECT_NEAR(t->v0.z,  2.f, float_err);
  EXPECT_NEAR(t->v0.w,  1.f, float_err);

  EXPECT_NEAR(t->v1.x,  1.f, float_err);
  EXPECT_NEAR(t->v1.y, -1.f, float_err);
  EXPECT_NEAR(t->v1.z,  2.f, float_err);
  EXPECT_NEAR(t->v1.w,  1.f, float_err);

  EXPECT_NEAR(t->v2.x,  0.f, float_err);
  EXPECT_NEAR(t->v2.y,  1.f, float_err);
  EXPECT_NEAR(t->v2.z,  2.f, float_err);
  EXPECT_NEAR(t->v2.w,  1.f, float_err);

  EXPECT_NEAR(t->n.x,  0.f, float_err);
  EXPECT_NEAR(t->n.y,  0.f, float_err);
  EXPECT_NEAR(t->n.z,  1.f, float_err);
  EXPECT_NEAR(t->n.w,  0.f, float_err);
}

//==============================================================================
TEST(Triangle, successiveTranslation) {
  auto float_err = 0.00001f;

  auto v0 = glm::vec4(-1, -1, 0, 1);
  auto v1 = glm::vec4( 1, -1, 0, 1);
  auto v2 = glm::vec4( 0,  1, 0, 1);
  auto *t = new Triangle(v0, v1, v2);

  t->translate(2.f, Z);
  t->translate(2.f, Y);
  t->apply_transformations();

  EXPECT_NEAR(t->v0.x, -1.f, float_err);
  EXPECT_NEAR(t->v0.y,  1.f, float_err);
  EXPECT_NEAR(t->v0.z,  2.f, float_err);
  EXPECT_NEAR(t->v0.w,  1.f, float_err);

  EXPECT_NEAR(t->v1.x,  1.f, float_err);
  EXPECT_NEAR(t->v1.y,  1.f, float_err);
  EXPECT_NEAR(t->v1.z,  2.f, float_err);
  EXPECT_NEAR(t->v1.w,  1.f, float_err);

  EXPECT_NEAR(t->v2.x,  0.f, float_err);
  EXPECT_NEAR(t->v2.y,  3.f, float_err);
  EXPECT_NEAR(t->v2.z,  2.f, float_err);
  EXPECT_NEAR(t->v2.w,  1.f, float_err);

  EXPECT_NEAR(t->n.x,  0.f, float_err);
  EXPECT_NEAR(t->n.y,  0.f, float_err);
  EXPECT_NEAR(t->n.z,  1.f, float_err);
  EXPECT_NEAR(t->n.w,  0.f, float_err);
}

//==============================================================================
TEST(Triangle, rotation) {
  auto float_err = 0.00001f;

  auto v0 = glm::vec4(-1, -1, 0, 1);
  auto v1 = glm::vec4( 1, -1, 0, 1);
  auto v2 = glm::vec4( 0,  1, 0, 1);
  auto *t = new Triangle(v0, v1, v2);

  t->rotate(90.f, X);
  t->apply_transformations();

  EXPECT_NEAR(t->v0.x, -1.f, float_err);
  EXPECT_NEAR(t->v0.y,  0.f, float_err);
  EXPECT_NEAR(t->v0.z, -1.f, float_err);
  EXPECT_NEAR(t->v0.w,  1.f, float_err);

  EXPECT_NEAR(t->v1.x,  1.f, float_err);
  EXPECT_NEAR(t->v1.y,  0.f, float_err);
  EXPECT_NEAR(t->v1.z, -1.f, float_err);
  EXPECT_NEAR(t->v1.w,  1.f, float_err);

  EXPECT_NEAR(t->v2.x,  0.f, float_err);
  EXPECT_NEAR(t->v2.y,  0.f, float_err);
  EXPECT_NEAR(t->v2.z,  1.f, float_err);
  EXPECT_NEAR(t->v2.w,  1.f, float_err);

  EXPECT_NEAR(t->n.x,   0.f, float_err);
  EXPECT_NEAR(t->n.y,  -1.f, float_err);
  EXPECT_NEAR(t->n.z,   0.f, float_err);
  EXPECT_NEAR(t->n.w,   0.f, float_err);
}

//==============================================================================
TEST(Triangle, successiveRotation) {
  auto float_err = 0.00001f;

  auto v0 = glm::vec4(-1, -1, 0, 1);
  auto v1 = glm::vec4( 1, -1, 0, 1);
  auto v2 = glm::vec4( 0,  1, 0, 1);
  auto *t = new Triangle(v0, v1, v2);

  t->rotate(90.f, X);
  t->rotate(-90.f, Z);
  t->apply_transformations();

  EXPECT_NEAR(t->v0.x,  0.f, float_err);
  EXPECT_NEAR(t->v0.y,  1.f, float_err);
  EXPECT_NEAR(t->v0.z, -1.f, float_err);
  EXPECT_NEAR(t->v0.w,  1.f, float_err);

  EXPECT_NEAR(t->v1.x,  0.f, float_err);
  EXPECT_NEAR(t->v1.y, -1.f, float_err);
  EXPECT_NEAR(t->v1.z, -1.f, float_err);
  EXPECT_NEAR(t->v1.w,  1.f, float_err);

  EXPECT_NEAR(t->v2.x,  0.f, float_err);
  EXPECT_NEAR(t->v2.y,  0.f, float_err);
  EXPECT_NEAR(t->v2.z,  1.f, float_err);
  EXPECT_NEAR(t->v2.w,  1.f, float_err);

  EXPECT_NEAR(t->n.x,  -1.f, float_err);
  EXPECT_NEAR(t->n.y,   0.f, float_err);
  EXPECT_NEAR(t->n.z,   0.f, float_err);
  EXPECT_NEAR(t->n.w,   0.f, float_err);
}

//==============================================================================
TEST(Triangle, scale) {
  auto float_err = 0.00001f;

  auto v0 = glm::vec4(-1, -1, 0, 1);
  auto v1 = glm::vec4( 1, -1, 0, 1);
  auto v2 = glm::vec4( 0,  1, 0, 1);
  auto *t = new Triangle(v0, v1, v2);

  t->scale(2.f, uniform);
  t->apply_transformations();

  EXPECT_NEAR(t->v0.x, -2.f, float_err);
  EXPECT_NEAR(t->v0.y, -2.f, float_err);
  EXPECT_NEAR(t->v0.z,  0.f, float_err);
  EXPECT_NEAR(t->v0.w,  1.f, float_err);

  EXPECT_NEAR(t->v1.x,  2.f, float_err);
  EXPECT_NEAR(t->v1.y, -2.f, float_err);
  EXPECT_NEAR(t->v1.z,  0.f, float_err);
  EXPECT_NEAR(t->v1.w,  1.f, float_err);

  EXPECT_NEAR(t->v2.x,  0.f, float_err);
  EXPECT_NEAR(t->v2.y,  2.f, float_err);
  EXPECT_NEAR(t->v2.z,  0.f, float_err);
  EXPECT_NEAR(t->v2.w,  1.f, float_err);

  EXPECT_NEAR(t->n.x,   0.f, float_err);
  EXPECT_NEAR(t->n.y,   0.f, float_err);
  EXPECT_NEAR(t->n.z,   1.f, float_err);
  EXPECT_NEAR(t->n.w,   0.f, float_err);
}

//==============================================================================
TEST(Triangle, successiveScale) {
  auto float_err = 0.00001f;

  auto v0 = glm::vec4(-1, -1, 0, 1);
  auto v1 = glm::vec4( 1, -1, 0, 1);
  auto v2 = glm::vec4( 0,  1, 0, 1);
  auto *t = new Triangle(v0, v1, v2);

  t->scale(2.f, uniform);
  t->scale(0.25f, uniform);
  t->apply_transformations();

  EXPECT_NEAR(t->v0.x, -0.5f, float_err);
  EXPECT_NEAR(t->v0.y, -0.5f, float_err);
  EXPECT_NEAR(t->v0.z,  0.f, float_err);
  EXPECT_NEAR(t->v0.w,  1.f, float_err);

  EXPECT_NEAR(t->v1.x,  0.5f, float_err);
  EXPECT_NEAR(t->v1.y, -0.5f, float_err);
  EXPECT_NEAR(t->v1.z,  0.f, float_err);
  EXPECT_NEAR(t->v1.w,  1.f, float_err);

  EXPECT_NEAR(t->v2.x,  0.f, float_err);
  EXPECT_NEAR(t->v2.y,  0.5f, float_err);
  EXPECT_NEAR(t->v2.z,  0.f, float_err);
  EXPECT_NEAR(t->v2.w,  1.f, float_err);

  EXPECT_NEAR(t->n.x,   0.f, float_err);
  EXPECT_NEAR(t->n.y,   0.f, float_err);
  EXPECT_NEAR(t->n.z,   1.f, float_err);
  EXPECT_NEAR(t->n.w,   0.f, float_err);
}


// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <memory>

#include "../src/accelerators/Grid.h"
#include "../src/accelerators/AABBox.h"
#include "../src/objects/TriangleMesh.h"
#include "../src/objects/Triangle.h"
#include "../src/objects/Sphere.h"

//==============================================================================
TEST(AccelerationStructure, convertToPrimitives) {
  const char *fp = "test_resources/cube.obj";
  std::shared_ptr<Object> cube = std::make_shared<TriangleMesh>(TriangleMesh(fp));

  AABBox box;
  Grid *grid_structure = new Grid();

  auto primitives = grid_structure->convert_to_primitive(cube);

  EXPECT_EQ(primitives.size(), 12);

  delete grid_structure;
}

//==============================================================================
TEST(Grid, intersectSingleSphere) {
  Ray ray;
  ray.set_orig({0.f, 0.f, 0.f, 1.f});
  ray.set_dir( {1.f, 0.f, 0.f, 0.f});

  std::shared_ptr<Object> s = std::make_shared<Sphere>(Sphere());
  std::static_pointer_cast<Sphere>(s)->set_radius(1.f);
  std::static_pointer_cast<Sphere>(s)->set_center({5.f, 0.f, 0.f, 1.f});

  std::vector<std::shared_ptr<Object>> objs;
  objs.push_back(s);

  auto ci = as_construct_info();
  auto ii = isect_info();

  Grid *g = new Grid();
  g->construct(std::static_pointer_cast<Sphere>(s)->bounding_box(),
               objs, 1, ci);

  auto intersected = g->traverse(ray, ii);

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
TEST(Grid, intersectSingleSphereParallel) {
  Ray ray;
  ray.set_orig({0.f, 1.f, 0.f, 1.f});
  ray.set_dir( {1.f, 0.f, 0.f, 0.f});

  std::shared_ptr<Object> s = std::make_shared<Sphere>(Sphere());
  std::static_pointer_cast<Sphere>(s)->set_radius(1.f);
  std::static_pointer_cast<Sphere>(s)->set_center({5.f, 0.f, 0.f, 1.f});

  auto ci = as_construct_info();
  auto ii = isect_info();

  std::vector<std::shared_ptr<Object>> objs;
  objs.push_back(s);

  Grid *g = new Grid();
  g->construct(std::static_pointer_cast<Sphere>(s)->bounding_box(),
               objs, 1, ci);

  auto intersected = g->traverse(ray, ii);

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
TEST(Grid, intersectSingleSphereInside) {
  float_t float_err = 0.001;

  Ray ray;
  ray.set_orig({-1.f,    0.f,    0.f, 1.f});
  ray.set_dir( { 0.707f, 0.707f, 0.f, 0.f});

  std::shared_ptr<Object> s = std::make_shared<Sphere>(Sphere());
  std::static_pointer_cast<Sphere>(s)->set_radius(3.f);
  std::static_pointer_cast<Sphere>(s)->set_center({0.f, 0.f, 0.f, 1.f});

  auto ci = as_construct_info();
  auto ii = isect_info();

  std::vector<std::shared_ptr<Object>> objs;
  objs.push_back(s);

  Grid *g = new Grid();
  g->construct(std::static_pointer_cast<Sphere>(s)->bounding_box(),
               objs, 1, ci);

  auto intersected = g->traverse(ray, ii);

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
TEST(Grid, intersectSingleSphereOutside) {
  Ray ray;
  ray.set_orig({0.f, 1.f, 0.f, 1.f});
  ray.set_dir( {1.f, 0.f, 0.f, 0.f});

  std::shared_ptr<Object> s = std::make_shared<Sphere>(Sphere());
  std::static_pointer_cast<Sphere>(s)->set_radius(0.8f);
  std::static_pointer_cast<Sphere>(s)->set_center({5.f, 0.f, 0.f, 1.f});

  auto ci = as_construct_info();
  auto ii = isect_info();

  std::vector<std::shared_ptr<Object>> objs;
  objs.push_back(s);

  Grid *g = new Grid();
  g->construct(std::static_pointer_cast<Sphere>(s)->bounding_box(),
               objs, 1, ci);

  auto intersected = g->traverse(ray, ii);

  EXPECT_TRUE(!intersected);
}

//==============================================================================
TEST(Grid, intersectSingleTriangle) {
  float_t float_err = 0.001;

  Ray ray;
  ray.set_orig({0.f, 0.f,  2.f, 1.f});
  ray.set_dir( {0.f, 0.f, -1.f, 0.f});

  auto v0 = glm::vec4(-1.f, -1.f, 0.f, 1.f);
  auto v1 = glm::vec4( 1.f, -1.f, 0.f, 1.f);
  auto v2 = glm::vec4( 0.f,  1.f, 0.f, 1.f);
  // Normal = {0, 0, 1, 0}

  std::shared_ptr<Object> t = std::make_shared<Triangle>(Triangle());
  std::static_pointer_cast<Triangle>(t)->set_vertices(v0, v1, v2);

  auto ci = as_construct_info();
  auto ii = isect_info();

  std::vector<std::shared_ptr<Object>> objs;
  objs.push_back(t);

  Grid *g = new Grid();
  g->construct(std::static_pointer_cast<Triangle>(t)->bounding_box(),
               objs, 1, ci);

  auto intersected = g->traverse(ray, ii);

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

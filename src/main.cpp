// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "glm/ext.hpp"

#include "accelerators/AccelerationStructure.h"
#include "accelerators/Grid.h"
#include "objects/Object.h"
#include "objects/TriangleMesh.h"
#include "lights/PointLight.h"
#include "cameras/PerspectiveCamera.h"
#include "objects/Triangle.h"
#include "objects/Sphere.h"
#include "lights/DirectionalLight.h"
#include "cameras/OrthographicCamera.h"
#include "extra/Scene.h"
#include "extra/Utilities.h"

void render_cornell_scene() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 4;
  char fn[100];
  loading_info li;
  render_info ri;

  /// material set-up
  material box_white;
  box_white.c = white;
  box_white.ac = 0.f;

  material box_red;
  box_red.c = red;
  box_red.ac = 0.f;

  material box_green;
  box_green.c = green;
  box_green.ac = 0.f;

  material mm;
  mm.mt = rrm;
  mm.ior = 1.5111f;

  material bm;
  bm.mt = rm;
  bm.c = rabit_color;

  material tm;
  tm.mt = rm;
  tm.c = gold;

  /// light set-up

  glm::vec4 lp2(-1.5f, 5.f, -4.5f, 1);
  glm::vec4 lp3(2.f, 5.f, -7.5f, 1);

  PointLight l2(lp2, orangish, 150);
  lights.push_back(&l2);

  PointLight l3(lp3, violet, 150);
  lights.push_back(&l3);

  /// object set-up

  glm::vec4 v0(-5, 0, -3, 1);
  glm::vec4 v1(5, 0, -3, 1);
  glm::vec4 v2(-5, 10, -3, 1);
  glm::vec4 v3(5, 10, -3, 1);
  glm::vec4 v4(-5, 0, -13, 1);
  glm::vec4 v5(5, 0, -13, 1);
  glm::vec4 v6(-5, 10, -13, 1);
  glm::vec4 v7(5, 10, -13, 1);

  // floor
  Triangle t1(v0, v1, v4, box_white);
  objects.push_back(&t1);
  Triangle t2(v1, v5, v4, box_white);
  objects.push_back(&t2);

  // left wall
  Triangle t3(v0, v4, v2, box_red);
//    objects.push_back(&t3);
  Triangle t4(v4, v6, v2, box_red);
//    objects.push_back(&t4);

  // right wall
  Triangle t5(v1, v3, v5, box_green);
//    objects.push_back(&t5);
  Triangle t6(v3, v7, v5, box_green);
//    objects.push_back(&t6);

  // back wall
  Triangle t7(v4, v5, v6, box_white);
//    objects.push_back(&t7);
  Triangle t8(v5, v7, v6, box_white);
//    objects.push_back(&t8);

  // ceiling
  Triangle t9(v2, v6, v3, box_white);
//    objects.push_back(&t9);
  Triangle t10(v6, v7, v3, box_white);
//    objects.push_back(&t10);

  // front wall
  Triangle t11(v0, v2, v1, box_white);
//    objects.push_back(&t11);
  Triangle t12(v1, v2, v3, box_white);
//    objects.push_back(&t12);


  // load teapot
  sprintf(fn, "../../object_files/wt_teapot.obj");
  TriangleMesh teapot(tm, true);

  // measure loading the triangulated mesh
  auto start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = teapot.load_mesh(fn);
  auto finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  teapot.rotate(-60.f, Axis::Y);
  teapot.scale(1.7f, Axis::uniform);
  teapot.translate(-7.f, Axis::Z);
  teapot.translate(2.3f, Axis::X);
  teapot.apply_transformations();
  objects.push_back(&teapot);

  // load monkey
  sprintf(fn, "../../object_files/monkey.obj");
  TriangleMesh monkey(mm, true);

  // measure loading the triangulated mesh
  start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = monkey.load_mesh(fn);
  finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  monkey.rotate(-30.f, Axis::X);
  monkey.rotate(25.f, Axis::Y);
  monkey.translate(-7.2f, Axis::Z);
  monkey.translate(-2.f, Axis::X);
  monkey.translate(0.6f, Axis::Y);
  monkey.apply_transformations();
  objects.push_back(&monkey);

  // load bunny
  sprintf(fn, "../../object_files/bunny.obj");
  TriangleMesh bunny(bm, true);

  // measure loading the triangulated mesh
  start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = bunny.load_mesh(fn);
  finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  bunny.rotate(45.f, Axis::Y);
  bunny.translate(-10.f, Axis::Z);
  bunny.translate(0.35f, Axis::Y);
  bunny.apply_transformations();
  objects.push_back(&bunny);

  /// camera transformations
  camera->rotate(-10.f, Axis::X);
  camera->translate(-4.f, Axis::Z);
  camera->translate(2.5f, Axis::Y);

  /// rendering

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;
  ip.save_to_png("scene_without_intersection_acceleration.png");
}

void render_simple_refl_scene() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 3;
  char fn[100];
  loading_info li;
  render_info ri;
  auto start_loading = std::chrono::high_resolution_clock::now()
  , finish_loading = std::chrono::high_resolution_clock::now();

  /// materials set-up
  material floor;
  floor.c = whitish;
  floor.ac = 0.f;

  material ball1;
  ball1.mt = rrm;
  ball1.ior = 1.458f;
  ball1.c = white;

  material ball2;
  ball2.ac = 0.f;
  ball2.dc = 0.8f;
  ball2.c = white;

  material ball3;
  ball3.mt = rm;

  material tm;
  tm.mt = rm;

  material mm;
  mm.mt = pm;

  /// objects set-up

  glm::vec4 v0(-3.f, -0.5f, 0.5f, 1);
  glm::vec4 v1(3.f, -0.5f, 0.5f, 1);
  glm::vec4 v2(-3.f, -0.5f, -5.5f, 1);
  glm::vec4 v3(3, -0.5f, -5.5f, 1);
  glm::vec4 sp1(1.f, 0.f, 0.f, 1);
  glm::vec4 sp2(1.f, 0.f, 0.f, 1);
  glm::vec4 sp3(0.f, 0.f, -1.f, 1);

  // create reflective plane
  Triangle t1(v0, v1, v2, floor);
  t1.apply_transformations();
  objects.push_back(&t1);

  Triangle t2(v1, v3, v2, floor);
  objects.push_back(&t2);

  // spheres
  Sphere s1(sp1, 0.5f, ball2);
//    objects.push_back(&s1);

  Sphere s2(sp2, 0.5f, ball1);
  objects.push_back(&s2);

  Sphere s3(sp3, 0.5f, ball3);
//    objects.push_back(&s3);

  // load monkey
  sprintf(fn, "./monkey.obj");
  TriangleMesh monkey(mm, true);

  // measure loading the triangulated mesh
  start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = monkey.load_mesh(fn);
  finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  monkey.rotate(-30.f, Axis::X);
  monkey.rotate(25.f, Axis::Y);
  monkey.translate(0.1f, Axis::Y);
  monkey.translate(-0.7f, Axis::Z);
  monkey.translate(-1.f, Axis::X);
  monkey.apply_transformations();
  objects.push_back(&monkey);

  // load teapot
  sprintf(fn, "./wt_teapot.obj");
  TriangleMesh teapot(tm, true);

  // measure loading the triangulated mesh
  start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = teapot.load_mesh(fn);
  finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  teapot.scale(1.3f, Axis::uniform);
  teapot.rotate(-135.f, Axis::Y);
  teapot.translate(-0.3f, Axis::Y);
  teapot.translate(-1.5f, Axis::Z);
  teapot.translate(1.3f, Axis::X);
  teapot.apply_transformations();
  objects.push_back(&teapot);

  /// illuminate scene

  glm::vec4 lp2(-1.5f, 2.5f, 0.f, 1);
  glm::vec4 lp3(1.5f, 2.5f, 1.f, 1);

  PointLight pl2(lp2, red, 55.f);
  lights.push_back(&pl2);

  PointLight pl3(lp3, violet, 50.f);
  lights.push_back(&pl3);

  /// adjust camera settings

  dynamic_cast<PerspectiveCamera *>(camera)->set_fov(90.f);

  // place the camera to look at the plane
  camera->rotate(-10.f, Axis::X);
  camera->translate(0.5f, Axis::Y);
  camera->translate(1.5f, Axis::Z);

  /// render scene
  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of refraction rays                  : " << ri.nrrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;
  ip.save_to_png("scene_without_gamma.png");

}

void test_refraction_scene() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 1;
  char fn[100];
  loading_info li;
  render_info ri;

  /// materials set-up

  material refr;
  refr.mt = rrm;
  refr.ior = 1.2f;
  refr.c = bluish;

  material phong;
  phong.c = orangish;

  /// objects set-up

  glm::vec4 sp1(-0.3f, 0, -1.3, 1);
  glm::vec4 sp2(0, 0, -10, 1);

  Sphere s1(sp1, 0.4f, refr);
  Sphere s2(sp2, 2.4f, phong);

//    objects.push_back(&s1);
  objects.push_back(&s2);

  // load cube
  sprintf(fn, "./cube.obj");
  TriangleMesh cube(refr, false);

  // measure loading the triangulated mesh
  auto start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = cube.load_mesh(fn);
  auto finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  cube.scale(0.5f, Axis::uniform);
  cube.translate(-3.f, Axis::Z);
  cube.apply_transformations();
//    objects.push_back(&cube);

  Triangle t1;
  t1.rotate(90.f, Axis::Z);
  t1.translate(-3.f, Axis::Z);
//    t1.translate(1.f, Axis::Y);
  t1.apply_transformations();
  objects.push_back(&t1);

  /// lights set-up

  glm::vec4 lp1(-1.f, 1.5f, -2.f, 1);

  PointLight l1(lp1, 70.f);

  lights.push_back(&l1);

  /// camera set-up
  camera->translate(3.f, Axis::Z);
  camera->rotate(20.f, Axis::X);
  camera->rotate(20, Axis::Y);
  camera->translate(-3.f, Axis::Z);

  /// render scene
  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;
  ip.save_to_png("test_refraction_scene.png");
}

void spheres() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 3;
  render_info ri;
  uint32_t nos = 10;  // number of spheres
  std::random_device
      rd;                                     // obtain a random number from hardware
  std::mt19937 eng(rd());                              // seed generator
  std::uniform_real_distribution<float_t>
      srzp(-13.f, -7.f);  // define range for sphere's z-position
  std::uniform_real_distribution<float_t>
      srxyp(-13.f, 13.f); // define range for sphere's xy-position
  std::uniform_real_distribution<float_t>
      srr(0.1f, 1.f);     // define range for sphere's radius
  std::uniform_real_distribution<float_t>
      cr(0.f, 1.f);       // define range for color values
  char fn[100];

  /// materials set-up
  material sm;
  sm.ac = 0.f;
  sm.dc = 0.7f;
  sm.sc = 0.2f;
  sm.se = 14.f;

  /// objects set-up
  Sphere *s;
  glm::vec4 sp(1, 1, -3, 1);    // we need w-component of the 4D vector to be 1
  float_t sr(1);
  float_t r, g, b;

  // place spheres randomly in the scene
  for (uint32_t i = 0; i < nos; i++) {
    s = new Sphere();
    // randomly choose and assign sphere's position
    sp.x = srxyp(eng);
    sp.y = srxyp(eng);
    sp.z = srzp(eng);
    (*s).set_center(sp);

    // randomly choose and assign sphere's radius
    sr = srr(eng);
    (*s).set_radius(sr);

    // randomly choose and assign color
    r = cr(eng);
    g = cr(eng);
    b = cr(eng);

    sm.c = glm::vec3(r, g, b);

    (*s).om = sm;

    // add sphere in the scene
    objects.push_back(&(*s));
  }

  /// illuminate the scene
  glm::vec4 lp1(-3.f, 0.0f, -4.5f, 1);
  glm::vec4 lp2(3.f, 0.0f, -4.5f, 1);

  PointLight l1(lp1, 200.f);
  PointLight l2(lp2, 200.f);

  lights.push_back(&l1);
  lights.push_back(&l2);

  /// render scene

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();

  // print out statistics
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;

  sprintf(fn, "%d_spheres_%d_samples.png", nos, ip.ns);
  ip.save_to_png(fn);
}

void boxes() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 1;
  render_info ri;
  loading_info li;
  char fn[100];    // file name
  char ob[50];     // object
  uint32_t noo = 1;    // number of objects
  float_t r, g, b;    // values for rand generated rgb
  std::random_device rd;         // obtain a random number from hardware
  std::mt19937 eng(rd());  // seed generator
  std::uniform_real_distribution<float_t>
      orzp(-10.f, -7.f);  // define range for object's z-position
  std::uniform_real_distribution<float_t>
      orxyp(-4.f, 3.f);   // define range for object's xy-position
  std::uniform_real_distribution<float_t>
      ors(1.f, 1.5f);     // define range for object's scale
  std::uniform_real_distribution<float_t>
      orc(0.f, 1.f);      // define range for color values

  /// materials set-up
  material cm;
  cm.ac = 0.f;
  cm.dc = 0.7f;
  cm.sc = 0.2f;
  cm.se = 14.f;

  /// objects set-up

  // load cube
  sprintf(ob, "monkey");
  sprintf(fn, "./%s.obj", ob);
  TriangleMesh bunny(cm, false);

  // measure loading of the cube
  auto start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = bunny.load_mesh(fn);
  auto finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  // place cubes randomly in the scene
  for (int i = 0; i < noo; i++) {
    auto *obj_copy = new TriangleMesh(bunny);

    // randomly choose cube's position
    (*obj_copy).translate(orxyp(eng), Axis::X);
    (*obj_copy).translate(orxyp(eng), Axis::Y);
    (*obj_copy).translate(orzp(eng), Axis::Z);

    // randomly choose cube's scale
    (*obj_copy).scale(ors(eng), Axis::uniform);

    // apply transformations for the cube
    (*obj_copy).apply_transformations();

    // randomly choose color
    r = orc(eng);
    g = orc(eng);
    b = orc(eng);

    cm.c = glm::vec3(r, g, b);

    (*obj_copy).om = cm;

    // add cube in the scene
    objects.push_back(&(*obj_copy));
  }

  /// illuminate the scene
  glm::vec4 ld1(-4, 2, -1, 0);
  glm::vec4 lp1(0, 0, -2, 1);

  DirectionalLight l1(ld1, 0.8f);
  PointLight l2(lp1, 50.f);

  lights.push_back(&l1);
  lights.push_back(&l2);

  /// render scene

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();

  // print out statistics
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;

  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;

  // save image to disc
  sprintf(fn, "%d_%ss_%d_samples.png", noo, ob, ip.ns);
  ip.save_to_png(fn);
}

void test_scene() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 1;
  render_info ri;
  loading_info li;
  char fn[100];    // file name
  char ob[50];     // object

  /// materials set-up
  material cm;
  cm.ac = 0.f;
  cm.dc = 0.7f;
  cm.sc = 0.2f;
  cm.se = 14.f;
  cm.mt = rm;
  cm.c = deadgold;

  /// objects set-up

  // load cube
  sprintf(ob, "wt_teapot");
  sprintf(fn, "./%s.obj", ob);
  TriangleMesh bunny(cm, false);

  // measure loading of the cube
  auto start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = bunny.load_mesh(fn);
  auto finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  bunny.rotate(30.f, Axis::Y);
  bunny.translate(-4.f, Axis::Z);
  bunny.translate(-2.f, Axis::Y);
  bunny.apply_transformations();

  objects.push_back(&bunny);

  /// illuminate the scene
  glm::vec4 lp1(-3.f, 0, 0, 1);
  PointLight l1(lp1, 50.f);
  l1.color = orangish;

  lights.push_back(&l1);

  /// render scene

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();

  // print out statistics
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;

  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;

  // save image to disc
  sprintf(fn, "%s_%d_samples.png", ob, ip.ns);
  ip.save_to_png(fn);
}

void triangles() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 4;
  render_info ri;
  uint32_t
      notr = 10;                                         // number of triangles
  std::random_device
      rd;                                     // obtain a random number from hardware
  std::mt19937 eng(rd());                              // seed generator
  std::uniform_real_distribution<float_t>
      trzp(-15.f, -3.f);  // define range for triangle's z-position
  std::uniform_real_distribution<float_t>
      trxyp(-6.f, 6.f);   // define range for triangle's xy-position
  std::uniform_real_distribution<float_t>
      trr(0.f, 45.f);     // define range for triangle's rotation
  std::uniform_real_distribution<float_t>
      trs(0.3f, 1.f);     // define range for triangle's scale
  std::uniform_real_distribution<float_t>
      cr(0.f, 1.f);       // define range for color values
  char fn[100];

  /// materials set-up
  material tm;
  tm.ac = 0.f;
  tm.dc = 0.7f;
  tm.sc = 0.2f;
  tm.se = 14.f;

  /// objects set-up
  Triangle *t;
  float_t rot;
  float_t r, g, b;

  for (uint32_t i = 0; i < notr; i++) {
    t = new Triangle();
    // scale triangle randomly
    (*t).scale(trs(eng), Axis::uniform);
    // place triangle randomly
    rot = trr(eng);
    (rot >= 30.f) ? (*t).rotate(rot, Axis::X) : ((rot <= 15.f) ? (*t).rotate(rot, Axis::Z)
                                                         : (*t).rotate(rot, Axis::Y));
    (*t).translate(trzp(eng), Axis::Z);
    (*t).translate(trxyp(eng), Axis::X);
    (*t).translate(trxyp(eng), Axis::Y);

    // randomly choose and assign color
    r = cr(eng);
    g = cr(eng);
    b = cr(eng);
    tm.c = glm::vec3(r, g, b);
    (*t).om = tm;

    // add triangle in the scene
    (*t).apply_transformations();
    objects.push_back(&(*t));
  }

  /// illuminate the scene
  glm::vec4 lp1(-3.f, 0.0f, -0.5f, 1);
  glm::vec4 lp2(3.f, 0.0f, -0.5f, 1);

  PointLight l1(lp1, 200.f);
  PointLight l2(lp2, 200.f);

  lights.push_back(&l1);
  lights.push_back(&l2);

  /// render scene

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();

  // print out statistics
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;

  sprintf(fn, "%d_triangles_%d_samples.png", notr, ip.ns);
  ip.save_to_png(fn);
}

void triangle_meshes() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 4;
  render_info ri;
  loading_info li;
  uint32_t notm =
      4;                                          // number of triangle meshes
  std::random_device
      rd;                                     // obtain a random number from hardware
  std::mt19937 eng(rd());                              // seed generator
  std::uniform_real_distribution<float_t>
      tmzp(-6.f, -3.f);   // define range for mesh's z-position
  std::uniform_real_distribution<float_t>
      tmxyp(-4.5f, 4.5f);   // define range for mesh's xy-position
  std::uniform_real_distribution<float_t>
      tmr(0.f, 45.f);     // define range for mesh's rotation
  std::uniform_real_distribution<float_t>
      tms(0.7f, 1.4f);     // define range for mesh's scale
  std::uniform_real_distribution<float_t>
      cr(0.f, 1.f);       // define range for color values
  char fn[100];

  /// materials set-up
  material mm;
  mm.ac = 0.f;

  /// objects set-up
  TriangleMesh *tm;
  float_t rot;
  float_t r, g, b;

  // load monkey
  sprintf(fn, "../../object_files/monkey.obj");
  TriangleMesh monkey(mm, true);

  // measure loading monkey
  auto start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = monkey.load_mesh(fn);
  auto finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  // load teapot
  sprintf(fn, "../../object_files/wt_teapot.obj");
  TriangleMesh teapot(mm, true);

  // measure loading teapot
  start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = teapot.load_mesh(fn);
  finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  for (uint32_t i = 0; i < notm; i++) {

    // add monkey

    tm = new TriangleMesh(monkey);
    // scale monkey randomly
    (*tm).scale(tms(eng), Axis::uniform);
    // place monkey randomly
    rot = tmr(eng);
    (rot >= 30.f) ? (*tm).rotate(rot, Axis::X) : ((rot <= 15.f) ? (*tm).rotate(rot, Axis::Z)
                                                          : (*tm).rotate(rot,
                                                                         Axis::Y));
    (*tm).translate(tmzp(eng), Axis::Z);
    (*tm).translate(tmxyp(eng), Axis::X);
    (*tm).translate(tmxyp(eng), Axis::Y);
    // randomly choose and assign color
    r = cr(eng);
    g = cr(eng);
    b = cr(eng);
    mm.c = glm::vec3(r, g, b);
    (*tm).om = mm;
    // add monkey in the scene
    (*tm).apply_transformations();
    objects.push_back(&(*tm));

    // add teapot

    tm = new TriangleMesh(teapot);
    // scale teapot randomly
    (*tm).scale(tms(eng), Axis::uniform);
    // place teapot randomly
    rot = tmr(eng);
    (rot >= 30.f) ? (*tm).rotate(rot, Axis::X) : ((rot <= 15.f) ? (*tm).rotate(rot, Axis::Z)
                                                          : (*tm).rotate(rot,
                                                                         Axis::Y));
    (*tm).translate(tmzp(eng), Axis::Z);
    (*tm).translate(tmxyp(eng), Axis::X);
    (*tm).translate(tmxyp(eng), Axis::Y);
    // randomly choose and assign color
    r = cr(eng);
    g = cr(eng);
    b = cr(eng);
    mm.c = glm::vec3(r, g, b);
    (*tm).om = mm;
    // add teapot in the scene
    (*tm).apply_transformations();
    objects.push_back(&(*tm));
  }

  /// illuminate the scene
  glm::vec4 lp1(-3.f, 0.0f, -0.5f, 1);
  glm::vec4 lp2(3.f, 0.0f, -0.5f, 1);

  PointLight l1(lp1, 200.f);
  PointLight l2(lp2, 200.f);

  lights.push_back(&l1);
  lights.push_back(&l2);

  /// render scene

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-primitive intersections      : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;

  sprintf(fn, "%d_triangle_meshes_%d_samples.png", notm, ip.ns);
  ip.save_to_png(fn);
}

void balls_scene() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 1;
  char fn[100];
  loading_info li;
  render_info ri;

  /// materials set-up

  // floor
  material f;
  f.c = whitish;
  f.ac = 0.f;

  // reflective pink ball
  material rpb;
  rpb.mt = rm;
  rpb.ri = 0.8f;
  rpb.sc = 0.3f;
  rpb.se = 16.f;
  rpb.c = pink;

  /// objects set-up

  glm::vec4 v0(-30.f, -0.5f, 50.5f, 1);
  glm::vec4 v1(30.f, -0.5f, 50.5f, 1);
  glm::vec4 v2(-30.f, -0.5f, -50.5f, 1);
  glm::vec4 v3(30, -0.5f, -50.5f, 1);
  glm::vec4 sp1(0.f, 0.5f, -2.f, 1);

  // reflective plane
  Triangle t1(v0, v1, v2, f);
  objects.push_back(&t1);

  Triangle t2(v1, v3, v2, f);
  objects.push_back(&t2);

  // reflective pink ball
  Sphere s1(sp1, 1.f, rpb);
  objects.push_back(&s1);

  /// lights set-up
  glm::vec4 lp2(-1.5f, 2.5f, 0.f, 1);
  glm::vec4 lp3(1.5f, 2.5f, 1.f, 1);

  PointLight pl2(lp2, white, 55.f);
  lights.push_back(&pl2);

  PointLight pl3(lp3, violet, 50.f);
//    lights.push_back(&pl3);


  /// render scene
  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of refraction rays                  : " << ri.nrrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;
  ip.save_to_png("ball_scene.png");

}

void monkey() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 4;
  render_info ri;
  loading_info li;
  char fn[100];

  /// materials set-up

  // monkey
  material mm;
  mm.c = orangish;
  mm.ac = 0.f;

  // floor
  material f;
  f.c = whitish;
  f.ac = 0.f;

  /// objects set-up

  glm::vec4 v0(-2.5f, -1.f, -1.5f, 1);
  glm::vec4 v1(2.5f, -1.f, -1.5f, 1);
  glm::vec4 v2(-2.5f, -1.f, -4.f, 1);
  glm::vec4 v3(2.5f, -1.f, -4.f, 1);

  // floor
  Triangle t1(v0, v1, v2, f);
  t1.translate(0.5f, Axis::Z);
  t1.apply_transformations();
  objects.push_back(&t1);

  Triangle t2(v1, v3, v2, f);
  t2.translate(0.5f, Axis::Z);
  t2.apply_transformations();
  objects.push_back(&t2);


  // load monkey
  sprintf(fn, "../../object_files/monkey.obj");
  TriangleMesh monkey(mm, true);

  // measure loading monkey
  auto start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = monkey.load_mesh(fn);
  auto finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  monkey.scale(0.8f, Axis::uniform);
  monkey.rotate(-13.f, Axis::X);
  monkey.translate(-2.f, Axis::Z);
  monkey.translate(-0.3f, Axis::Y);
  monkey.apply_transformations();
  objects.push_back(&monkey);

  /// lights set-up

  glm::vec4 lp(-1.f, 1.5f, -0.5f, 1);

  PointLight pl1(lp, white, 35.f);
  lights.push_back(&pl1);

  /// render scene

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-primitive intersections      : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;

  sprintf(fn, "monkey_scene.png");
  ip.save_to_png(fn);
}

void teapot() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(550, 550);
  ip.ns = 3;
  render_info ri;
  loading_info li;
  char fn[100];

  /// materials set-up

  // teapot
  material mm;
  mm.c = orangish;
  mm.ac = 0.f;

  // floor
  material f;
  f.c = whitish;
  f.ac = 0.f;

  /// objects set-up

  glm::vec4 v0(-2.5f, -1.f, -1.5f, 1);
  glm::vec4 v1(2.5f, -1.f, -1.5f, 1);
  glm::vec4 v2(-2.5f, -1.f, -4.f, 1);
  glm::vec4 v3(2.5f, -1.f, -4.f, 1);

  // floor
  Triangle t1(v0, v1, v2, f);
  t1.translate(0.5f, Axis::Z);
  t1.apply_transformations();
//    objects.push_back(&t1);

  Triangle t2(v1, v3, v2, f);
  t2.translate(0.5f, Axis::Z);
  t2.apply_transformations();
//    objects.push_back(&t2);

  // load teapot
  sprintf(fn, "../../object_files/wt_teapot.obj");
  TriangleMesh teapot(mm, true);

  // measure loading teapot
  auto start_loading = std::chrono::high_resolution_clock::now();
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  li = teapot.load_mesh(fn);
  auto finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  teapot.scale(0.9f, Axis::uniform);
  teapot.translate(-1.7f, Axis::Z);
  teapot.translate(-0.5f, Axis::Y);
  teapot.apply_transformations();
  objects.push_back(&teapot);

  /// lights set-up

  glm::vec4 lp(-1.f, 0.f, 0.f, 1);

  PointLight pl1(lp, white, 30.f);
  lights.push_back(&pl1);

  /// render scene

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-primitive intersections      : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;

  sprintf(fn, "teapot_orthographic.png");
  ip.save_to_png(fn);
}

void diffuse_scene() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(500, 500);
  ip.ns = 5;
  render_info ri;
  char fn[100];

  /// materials set-up
  material diffuse;
  diffuse.ac = 0.f;
  diffuse.sc = 0.f;
  diffuse.dc = 100.f;

  /// objects set-up

  // a sphere
  glm::vec4 sp1(0.f, 0.f, -2.f, 1);
  Sphere s1(sp1, 0.5f, diffuse);

  objects.push_back(&s1);

  /// illuminate scene

  glm::vec4 lp(-1.f, 0.5f, 0.f, 1);

  PointLight pl(lp, red, 55.f);
  lights.push_back(&pl);

  /// render scene
  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of refraction rays                  : " << ri.nrrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;
  ip.save_to_png("diffuse_100.png");
}

void specular() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(500, 500);
  ip.ns = 5;
  render_info ri;
  char fn[100];

  /// materials set-up
  material diffuse;
  diffuse.ac = 0.f;
  diffuse.dc = 0.f;
  diffuse.sc = 0.2f;
  diffuse.se = 1024.f;

  /// objects set-up

  // a sphere
  glm::vec4 sp1(0.f, 0.f, -2.f, 1);
  Sphere s1(sp1, 0.5f, diffuse);

  objects.push_back(&s1);

  /// illuminate scene

  glm::vec4 lp(-1.f, 0.5f, 0.f, 1);

  PointLight pl(lp, red, 100.f);
  lights.push_back(&pl);

  /// render scene
  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of refraction rays                  : " << ri.nrrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;
  ip.save_to_png("specular_sc_0_2_se_1024.png");
}

void grid_tests() {
  AABBox box;

  std::vector<Object *> objects;
//  char fn[100];
//  sprintf(fn, "../../object_files/dragon.obj");
//  TriangleMesh *dragon = new TriangleMesh();
//  dragon->load_mesh(fn);
//  dragon->translate(-20, Axis::Z);
//  dragon->apply_transformations();
//  objects.emplace_back(dragon);
//
//  dragon->translate(-10, Axis::Z);
//  dragon->apply_transformations();
//  objects.emplace_back(dragon);
//
//  dragon->translate(10, Axis::Y);
//  dragon->apply_transformations();
//  objects.emplace_back(dragon);
//
//  dragon->translate(10, Axis::X);
//  dragon->apply_transformations();
//  objects.emplace_back(dragon);

//  Sphere *s = new Sphere(glm::vec4(0,0,0,1), 0.5f);
//  objects.emplace_back(s);

//  s->translate(-1, Axis::Z);
//  objects.emplace_back(s);

  Triangle *t = new Triangle();
  t->rotate(15.f, Axis::X);
  t->apply_transformations();
  objects.emplace_back(t);

  for (auto object : objects) {
    box.extend_by(object->bb.bounds[0]);
    box.extend_by(object->bb.bounds[1]);
  }

  std::cout << "Grid's bounding box min extent: (" << box.bounds[0].x << ", " <<
            box.bounds[0].y << ", " << box.bounds[0].z << ")" << std::endl;
  std::cout << "Grid's bounding box max extent: (" << box.bounds[1].x << ", " <<
            box.bounds[1].y << ", " << box.bounds[1].z << ")" << std::endl
      << std::endl;

  Grid *grid = new Grid(box, objects);
  grid->setAlpha(300.f);
  grid_info i;

  auto startConstruction = std::chrono::high_resolution_clock::now();
  i = grid->constructGrid();
  auto finishConstruction = std::chrono::high_resolution_clock::now();
  std::cout << "Grid's construction time: " <<
            std::chrono::duration_cast<std::chrono::milliseconds>
            (finishConstruction - startConstruction).count() << "ms"
            << std::endl;
  std::cout << "Grid's alpha: " << grid->getAlpha() << std::endl;
  std::cout << "Grid's resoultion: " << i.r[0] << 'x' << i.r[1] << 'x' << i.r[2]
            << std::endl;
  std::cout << "Number of cells: " << i.r[0]*i.r[1]*i.r[2] << std::endl;
  std::cout << "Number of  primitives: " << i.np << std::endl;
  std::cout << "Number of non-empty cells: " << i.nfc << std::endl;
  std::cout << "Average number of primitives per cell: " << i.nppc << std::endl;
  std::cout << std::endl;
//
//  glm::vec4 o(0, 0, 0, 1);
//  glm::vec4 d(0, 0, -1, 1);
//  Ray r(o, d);
//
//  isect_info ii;
//  grid->intersect(r, ii);
//  std::cout << "Intersection point from grid: \t(" << ii.ip.x << ", " << ii.ip.y
//            << ", " << ii.ip.z << ")" << std::endl;
//
//  render_info ri;
//  ii = isect_info();
//  r.trace(objects, ii, ri);
//  std::cout << "Intersection point basic: \t\t(" << ii.ip.x << ", " << ii.ip.y
//            << ", " << ii.ip.z << ")" << std::endl;
}

void dragon() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 3;
  char fn[100];
  loading_info li;
  render_info ri;

  /// material set-up

  material box_white;
  box_white.c = white;
  box_white.ac = 0.f;

  material dm;
  dm.mt = rrm;
  dm.ior = 1.5111f;

  /// light set-up

  glm::vec4 lp1(0.0f, 2.5f, -9.5f, 1);
  glm::vec4 lp2(3.5f, 2.5f, -9.5f, 1);
  glm::vec4 lp3(-3.5f, 2.5f, -9.5f, 1);

  PointLight l1(lp3, white, 60);
  lights.push_back(&l1);

  PointLight l2(lp1, bg_green, 60);
  lights.push_back(&l2);

  PointLight l3(lp2, red, 60);
  lights.push_back(&l3);

  PointLight l4(lp1, white, 90);
  l4.translate(6, Axis::Z);
  l4.translate(-0.5f, Axis::X);
  l4.apply_transformations();
  lights.push_back(&l4);


  /// object set-up

  glm::vec4 v0(-5, 0, -3, 1);
  glm::vec4 v1(5, 0, -3, 1);
  glm::vec4 v2(-5, 10, -3, 1);
  glm::vec4 v3(5, 10, -3, 1);
  glm::vec4 v4(-5, 0, -13, 1);
  glm::vec4 v5(5, 0, -13, 1);
  glm::vec4 v6(-5, 10, -13, 1);
  glm::vec4 v7(5, 10, -13, 1);

  // floor
  Triangle t1(v0, v1, v4, box_white);
  objects.push_back(&t1);
  Triangle t2(v1, v5, v4, box_white);
  objects.push_back(&t2);

  // dragon
  sprintf(fn, "../../object_files/dragon.obj");
  TriangleMesh dragon(dm, true);

  // measure loading the triangulated mesh
  std::cout << std::endl;
  std::cout << "Start loading..." << std::endl;
  auto start_loading = std::chrono::high_resolution_clock::now();
  li = dragon.load_mesh(fn);
  auto finish_loading = std::chrono::high_resolution_clock::now();
  std::cout << "Done loading '" << fn << "'." << std::endl;
  std::cout << "Loading time                          : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(
                finish_loading - start_loading).count() << " milliseconds"
            << std::endl;
  std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh       : " << li.nvn
            << std::endl;
  std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
  std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
  std::cout << std::endl;

  dragon.rotate(98.f, Axis::Y);
  dragon.translate(0.84887f, Axis::Y);
  dragon.translate(-8.f, Axis::Z);
  dragon.apply_transformations();
  objects.push_back(&dragon);

  /// camera transformations

  camera->rotate(-10.f, Axis::X);
  camera->translate(-4.f, Axis::Z);
  camera->translate(2.5f, Axis::Y);

  std::cout << "Dragons bounding box min: (" << dragon.bb.bounds[0].x << ", " <<
            dragon.bb.bounds[0].y << ", " << dragon.bb.bounds[0].z << ")" <<
            std::endl;
  std::cout << "Dragons bounding box max: (" << dragon.bb.bounds[1].x << ", "
            << dragon.bb.bounds[1].y << ", " << dragon.bb.bounds[1].z << ")" <<
            std::endl << std::endl;

  /// rendering

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();

  // print some rendering statistics
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;
  ip.save_to_png("dragon_scene_bg_flag_full_size_01.png");
}

void spheres_grid() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 3;
  char fn[100];
  loading_info li;
  render_info ri;

  /// light set-up
  glm::vec4 lp1(0.0f, 1.5f, 0.5f, 1);

  PointLight l1(lp1, white, 50);
  lights.push_back(&l1);

  /// object set-up
  // first row spheres
  Sphere *s1 = new Sphere(glm::vec4(0, 0, -1, 1), 0.5f);
  objects.push_back(&(*s1));
  Sphere *s2 = new Sphere(glm::vec4(0, 0, -1.5f, 1), 0.5f);
  objects.push_back(&(*s2));
  Sphere *s3 = new Sphere(glm::vec4(0, 0, -2, 1), 0.5f);
  objects.push_back(&(*s3));
  Sphere *s4 = new Sphere(glm::vec4(0, 0, -2.5, 1), 0.5f);
  objects.push_back(&(*s4));

  // second row spheres
  Sphere *s5 = new Sphere(glm::vec4(1.f, 0, -1, 1), 0.5f);
  objects.push_back(&(*s5));
  Sphere *s6 = new Sphere(glm::vec4(1.f, 0, -1.5f, 1), 0.5f);
  objects.push_back(&(*s6));
  Sphere *s7 = new Sphere(glm::vec4(1.f, 0, -2, 1), 0.5f);
  objects.push_back(&(*s7));
  Sphere *s8 = new Sphere(glm::vec4(1.f, 0, -2.5, 1), 0.5f);
  objects.push_back(&(*s8));

  // third row spheres
  Sphere *s9 = new Sphere(glm::vec4(-1.f, 0, -1, 1), 0.5f);
  objects.push_back(&(*s9));
  Sphere *s10 = new Sphere(glm::vec4(-1.f, 0, -1.5f, 1), 0.5f);
  objects.push_back(&(*s10));
  Sphere *s11 = new Sphere(glm::vec4(-1.f, 0, -2, 1), 0.5f);
  objects.push_back(&(*s11));
  Sphere *s12 = new Sphere(glm::vec4(-1.f, 0, -2.5, 1), 0.5f);
  objects.push_back(&(*s12));

  // first row spheres, second column
  Sphere *s13 = new Sphere(glm::vec4(0, 1.2f, -1, 1), 0.5f);
  objects.push_back(&(*s13));
  Sphere *s14 = new Sphere(glm::vec4(0, 1.2f, -1.5f, 1), 0.5f);
  objects.push_back(&(*s14));
  Sphere *s15 = new Sphere(glm::vec4(0, 1.2f, -2, 1), 0.5f);
  objects.push_back(&(*s15));
  Sphere *s16 = new Sphere(glm::vec4(0, 1.2f, -2.5, 1), 0.5f);
  objects.push_back(&(*s16));

  // second row spheres
  Sphere *s17 = new Sphere(glm::vec4(1.f, 1.2f, -1, 1), 0.5f);
  objects.push_back(&(*s17));
  Sphere *s18 = new Sphere(glm::vec4(1.f, 1.2f, -1.5f, 1), 0.5f);
  objects.push_back(&(*s18));
  Sphere *s19 = new Sphere(glm::vec4(1.f, 1.2f, -2, 1), 0.5f);
  objects.push_back(&(*s19));
  Sphere *s20 = new Sphere(glm::vec4(1.f, 1.2f, -2.5, 1), 0.5f);
  objects.push_back(&(*s20));

  // third row spheres
  Sphere *s21 = new Sphere(glm::vec4(-1.f, 1.2f, -1, 1), 0.5f);
  objects.push_back(&(*s21));
  Sphere *s22 = new Sphere(glm::vec4(-1.f, 1.2f, -1.5f, 1), 0.5f);
  objects.push_back(&(*s22));
  Sphere *s23 = new Sphere(glm::vec4(-1.f, 1.2f, -2, 1), 0.5f);
  objects.push_back(&(*s23));
  Sphere *s24 = new Sphere(glm::vec4(-1.f, 1.2f, -2.5, 1), 0.5f);
  objects.push_back(&(*s24));


//  /// camera transformations
  camera->rotate(-30.f, Axis::X);
  camera->translate(2.5, Axis::Y);
  camera->translate(0.5f, Axis::Z);

  /// rendering


  // Turn on/off accelerators structure.
  camera->use_acceleration(true);

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();

  // print some rendering statistics
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-object intersections         : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;
  ip.save_to_png("grid_test_spheres.png");
}

void triangles_grid() {
  std::vector<Object *> objects;
  std::vector<Light *> lights;
  Camera *camera = new PerspectiveCamera();
  ImagePlane ip = ImagePlane(1280, 720);
  ip.ns = 3;
  char fn[100];
  loading_info li;
  render_info ri;

  /// Light set-up.
  glm::vec4 lp1(0.5f, 1.5f, 0.f, 1);

  PointLight l1(lp1, white, 50);
  lights.push_back(&l1);

  /// Material set-up.
  // teapot
  material mm;
  mm.c = orangish;
  mm.ac = 0.f;

  // floor
  material f;
  f.c = whitish;
  f.ac = 0.f;

  /// Object set-up.
  // Load cube.
  sprintf(fn, "../../object_files/cube.obj");
  TriangleMesh cube(f, false);
  li = cube.load_mesh(fn);
  cube.translate(-2, Axis::Z);
  cube.translate(-1, Axis::Y);
//  cube.translate(1.5f, Axis::X);
  cube.apply_transformations();
  objects.push_back(&cube);

  /// render scene
  // Turn on/off accelerators structure.
  camera->use_acceleration(true, 1);

  // measure rendering time
  std::cout << "Start rendering..." << std::endl;
  auto start_rendering = std::chrono::high_resolution_clock::now();
  ri = camera->render_scene(objects, lights, ip);
  auto finish_rendering = std::chrono::high_resolution_clock::now();
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time                        : "
            << std::chrono::duration_cast<std::chrono::seconds>(
                finish_rendering - start_rendering).count() << " seconds"
            << std::endl;
  std::cout << "# of primary rays                     : " << ri.npr
            << std::endl;
  std::cout << "# of shadow rays                      : " << ri.nsr
            << std::endl;
  std::cout << "# of reflection rays                  : " << ri.nrr
            << std::endl;
  std::cout << "# of objects in the scene             : " << ri.no << std::endl;
  std::cout << "# of light sources in the scene       : " << ri.nls
            << std::endl;
  std::cout << "# of ray-primitive intersection tests : " << ri.nrpt
            << std::endl;
  std::cout << "# of ray-primitive intersections      : " << ri.nroi
            << std::endl;
  std::cout << "ratio (isect tests / isect)           : "
            << (1.f * ri.nrpt) / ri.nroi << std::endl;

  sprintf(fn, "cube.png");
  ip.save_to_png(fn);

}

int main(int argc, char **argv) {

//  if (argc != 2) {
//    std::cout << "Usage: " << argv[0] << " <scene file>" << std::endl;
//    exit(1);
//  }
//  std::string filename = argv[1];
//  auto result = read_scene_from_file(filename);
//
//  return 0;
}

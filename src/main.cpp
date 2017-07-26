#include <iostream>
#include <fstream>
#include "objects/Object.h"
#include "objects/TriangleMesh.h"
#include "lights/PointLight.h"
#include "cameras/PerspectiveCamera.h"
#include "objects/Triangle.h"
#include "Utilities.h"
#include "objects/Sphere.h"

void render_cornell_scene() {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(1920, 1080);
    char fn[100];
    loading_info li;
    render_info ri;

    /// material set-up
    material box_white;
    box_white.c = white;

    material refl = box_white;
    refl.mt = rm;

    material box_red;
    box_red.c = red;

    material box_green;
    box_green.c = green;

    material dg;
    dg.c = deadgold;
    dg.mt = rm;

    material lg;
    lg.c = lightslategray;
    lg.mt = rm;

    /// light set-up

    glm::vec4 l1_p(0.f, 0.f, 0.f, 1);

    PointLight l2(l1_p, violet, 55);
    l2.translate(-4.5f, Z);
    l2.translate(7.f, Y);
    l2.translate(-1.5f, X);
    l2.apply_transformations();
    lights.push_back(&l2);

    PointLight l3(l1_p, orangish, 60);
    l3.translate(2, X);
    l3.translate(-4.5f, Z);
    l3.translate(4, Y);
    l3.apply_transformations();
    lights.push_back(&l3);

    /// object set-up

    glm::vec4 v0(-5,  0,  -3, 1);
    glm::vec4 v1( 5,  0,  -3, 1);
    glm::vec4 v2(-5, 10,  -3, 1);
    glm::vec4 v3( 5, 10,  -3, 1);
    glm::vec4 v4(-5,  0, -13, 1);
    glm::vec4 v5( 5,  0, -13, 1);
    glm::vec4 v6(-5, 10, -13, 1);
    glm::vec4 v7( 5, 10, -13, 1);

    // refl
    Triangle t1(v0, v1, v4, box_white);
    objects.push_back(&t1);
    Triangle t2(v1, v5, v4, box_white);
    objects.push_back(&t2);

    // left wall
    Triangle t3(v0, v4, v2, box_red);
    objects.push_back(&t3);
    Triangle t4(v4, v6, v2, box_red);
    objects.push_back(&t4);

    // right wall
    Triangle t5(v1, v3, v5, box_green);
    objects.push_back(&t5);
    Triangle t6(v3, v7, v5, box_green);
    objects.push_back(&t6);

    // back wall
    Triangle t7(v4, v5, v6, box_white);
    objects.push_back(&t7);
    Triangle t8(v5, v7, v6, box_white);
    objects.push_back(&t8);

    // ceiling
    Triangle t9(v2, v6, v3, box_white);
    objects.push_back(&t9);
    Triangle t10(v6, v7, v3, box_white);
    objects.push_back(&t10);

    // front wall
    Triangle t11(v0, v2, v1, box_white);
    objects.push_back(&t11);
    Triangle t12(v1, v2, v3, box_white);
    objects.push_back(&t12);


    // load teapot
    sprintf(fn, "./wt_teapot.obj");
    TriangleMesh teapot(lg, true);

    // measure loading the triangulated mesh
    auto start_loading = std::chrono::high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << "Start loading..." << std::endl;
    li = teapot.load_mesh(fn);
    auto finish_loading = std::chrono::high_resolution_clock::now();
    std::cout << "Done loading '" <<  fn << "'." << std::endl;
    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
    std::cout << std::endl;

    teapot.scale(1.7f, XYZ);
    teapot.translate(1.f, Y);
    teapot.translate(-7.5f, Z);
    teapot.translate(2.f, X);
    teapot.apply_transformations();
    objects.push_back(&teapot);

    // load monkey
    sprintf(fn, "./monkey.obj");
    TriangleMesh monkey(dg, true);

    // measure loading the triangulated mesh
    start_loading = std::chrono::high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << "Start loading..." << std::endl;
    li = monkey.load_mesh(fn);
    finish_loading = std::chrono::high_resolution_clock::now();
    std::cout << "Done loading '" <<  fn << "'." << std::endl;
    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
    std::cout << std::endl;

    monkey.rotate(-20.f, X);
    monkey.rotate(15.f, Y);
    monkey.translate(-6.3f, Z);
    monkey.translate(-1.f, X);
    monkey.translate(1.2f, Y);
    monkey.apply_transformations();
    objects.push_back(&monkey);

    /// camera transformations
    camera->rotate(-15.f, X);
    camera->translate(3.f, Y);
    camera->translate(-3.1f, Z);
    dynamic_cast<PerspectiveCamera *>(camera)->set_fov(90.44f);

    /// rendering

    // measure rendering time
    std::cout << "Start rendering..." << std::endl;
    auto start_rendering = std::chrono::high_resolution_clock::now();
    ri = camera->render_scene(objects, lights, ip);
    auto finish_rendering = std::chrono::high_resolution_clock::now();
    std::cout << "Done rendering." << std::endl;
    std::cout << "Rendering time                        : " << std::chrono::duration_cast<std::chrono::seconds>(finish_rendering - start_rendering).count() << " seconds" << std::endl;
    std::cout << "# of primary rays                     : " << ri.npr << std::endl;
    std::cout << "# of shadow rays                      : " << ri.nsr << std::endl;
    std::cout << "# of reflection rays                  : " << ri.nrr << std::endl;
    std::cout << "# of objects in the scene             : " << ri.no << std::endl;
    std::cout << "# of light sources in the scene       : " << ri.nls << std::endl;
    std::cout << "# of ray-object intersection tests    : " << ri.nrot << std::endl;
    std::cout << "# of ray-object intersections         : " << ri.nroi << std::endl;
    std::cout << "ratio (isect tests / isect)           : " << (1.f * ri.nrot) / ri.nroi << std::endl;
    ip.save_to_png("cornell_box3.png");
}

void render_simple_refl_scene() {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(1280, 720);
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
    glm::vec4 sp2(-1.f, 0.f, 0.f, 1);
    glm::vec4 sp3(0.f, 0.f, -1.f, 1);

    // create reflective plane
    Triangle t1(v0, v1, v2, floor);
    t1.apply_transformations();
    objects.push_back(&t1);

    Triangle t2(v1, v3, v2, floor);
    objects.push_back(&t2);

    // spheres
    Sphere s1(sp1, 0.5f, ball2);
    objects.push_back(&s1);

    Sphere s2(sp2, 0.5f, ball1);
    objects.push_back(&s2);

    Sphere s3(sp3, 0.5f, ball3);
    objects.push_back(&s3);

    // load monkey
    sprintf(fn, "./monkey.obj");
    TriangleMesh monkey(mm, true);

    // measure loading the triangulated mesh
    start_loading = std::chrono::high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << "Start loading..." << std::endl;
    li = monkey.load_mesh(fn);
    finish_loading = std::chrono::high_resolution_clock::now();
    std::cout << "Done loading '" <<  fn << "'." << std::endl;
    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
    std::cout << std::endl;

    monkey.rotate(-30.f, X);
    monkey.rotate(25.f, Y);
    monkey.translate(0.1f, Y);
    monkey.translate(-1.5f, Z);
    monkey.translate(-1.f, X);
    monkey.apply_transformations();
//    objects.push_back(&monkey);

    // load teapot
    sprintf(fn, "./wt_teapot.obj");
    TriangleMesh teapot(tm, true);

    // measure loading the triangulated mesh
    start_loading = std::chrono::high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << "Start loading..." << std::endl;
    li = teapot.load_mesh(fn);
    finish_loading = std::chrono::high_resolution_clock::now();
    std::cout << "Done loading '" <<  fn << "'." << std::endl;
    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
    std::cout << std::endl;

    teapot.scale(1.3f, XYZ);
    teapot.rotate(-135.f, Y);
    teapot.translate(-0.3f, Y);
    teapot.translate(-2.5f, Z);
    teapot.translate(1.f, X);
    teapot.apply_transformations();
//    objects.push_back(&teapot);

    /// illuminate scene

    glm::vec4 lp2(-1.5f, 2.5f, 0.f, 1);
    glm::vec4 lp3(1.5f, 2.5f, 1.f, 1);

    PointLight pl2(lp2, white, 75.f);
    lights.push_back(&pl2);

    PointLight pl3(lp3, orangish, 70.f);
    lights.push_back(&pl3);

    /// adjust camera settings

    dynamic_cast<PerspectiveCamera *>(camera)->set_fov(90.f);

    // place the camera to look at the plane
    camera->rotate(-10.f, X);
    camera->translate(0.5f, Y);
    camera->translate(1.5f, Z);

    /// render scene
    // measure rendering time
    std::cout << "Start rendering..." << std::endl;
    auto start_rendering = std::chrono::high_resolution_clock::now();
    ri = camera->render_scene(objects, lights, ip);
    auto finish_rendering = std::chrono::high_resolution_clock::now();
    std::cout << "Done rendering." << std::endl;
    std::cout << "Rendering time                        : " << std::chrono::duration_cast<std::chrono::seconds>(finish_rendering - start_rendering).count() << " seconds" << std::endl;
    std::cout << "# of primary rays                     : " << ri.npr << std::endl;
    std::cout << "# of shadow rays                      : " << ri.nsr << std::endl;
    std::cout << "# of reflection rays                  : " << ri.nrr << std::endl;
    std::cout << "# of refraction rays                  : " << ri.nrrr << std::endl;
    std::cout << "# of objects in the scene             : " << ri.no << std::endl;
    std::cout << "# of light sources in the scene       : " << ri.nls << std::endl;
    std::cout << "# of ray-object intersection tests    : " << ri.nrot << std::endl;
    std::cout << "# of ray-object intersections         : " << ri.nroi << std::endl;
    std::cout << "ratio (isect tests / isect)           : " << (1.f * ri.nrot) / ri.nroi << std::endl;
    ip.save_to_png("simple_reflective_ball_scene2.png");

}

void test_refraction_scene() {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(500, 500);
    char fn[100];
    loading_info li;
    render_info ri;

    /// materials set-up

    material refr;
    refr.mt = rrm;
    refr.ior = 1.f;
    refr.ac = 0.f;
    refr.dc = 0.6f;
    refr.sc = 0.1f;
    refr.se = 16.f;
    refr.c = bluish;

    material phong;
    phong.c = orangish;

    /// objects set-up

    glm::vec4 sp1(0, 0, -1.3, 1);
    glm::vec4 sp2(0, 0, -10, 1);

    Sphere s1(sp1, 0.4f, refr);
    Sphere s2(sp2, 2.4f, phong);

    objects.push_back(&s1);
    objects.push_back(&s2);

//    // load cube
//    sprintf(fn, "./cube.obj");
//    TriangleMesh cube(refr, false);
//
//    // measure loading the triangulated mesh
//    auto start_loading = std::chrono::high_resolution_clock::now();
//    std::cout << std::endl;
//    std::cout << "Start loading..." << std::endl;
//    li = cube.load_mesh(fn);
//    auto finish_loading = std::chrono::high_resolution_clock::now();
//    std::cout << "Done loading '" <<  fn << "'." << std::endl;
//    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
//    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
//    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
//    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
//    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
//    std::cout << std::endl;
//
//    cube.scale(0.5f, XYZ);
//    cube.translate(-3.f, Z);
//    cube.apply_transformations();
//    objects.push_back(&cube);
//
    /// lights set-up

    glm::vec4 lp1(-1.f, 1.5f, -2.f, 1);

    PointLight l1(lp1, 70.f);

    lights.push_back(&l1);

    /// render scene
    // measure rendering time
    std::cout << "Start rendering..." << std::endl;
    auto start_rendering = std::chrono::high_resolution_clock::now();
    ri = camera->render_scene(objects, lights, ip);
    auto finish_rendering = std::chrono::high_resolution_clock::now();
    std::cout << "Done rendering." << std::endl;
    std::cout << "Rendering time                        : " << std::chrono::duration_cast<std::chrono::seconds>(finish_rendering - start_rendering).count() << " seconds" << std::endl;
    std::cout << "# of primary rays                     : " << ri.npr << std::endl;
    std::cout << "# of shadow rays                      : " << ri.nsr << std::endl;
    std::cout << "# of reflection rays                  : " << ri.nrr << std::endl;
    std::cout << "# of objects in the scene             : " << ri.no << std::endl;
    std::cout << "# of light sources in the scene       : " << ri.nls << std::endl;
    std::cout << "# of ray-object intersection tests    : " << ri.nrot << std::endl;
    std::cout << "# of ray-object intersections         : " << ri.nroi << std::endl;
    std::cout << "ratio (isect tests / isect)           : " << (1.f * ri.nrot) / ri.nroi << std::endl;
    ip.save_to_png("test_refraction_scene.png");
}

int main(int argc, char **argv) {
//    render_simple_refl_scene();

    glm::vec3 g(0.5f, 1.8f, 100.f);
    g = glm::clamp(g, 0.f, 1.f);
    return 0;
}
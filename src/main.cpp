#include <iostream>
#include "objects/Object.h"
#include "objects/TriangleMesh.h"
#include "lights/PointLight.h"
#include "cameras/PerspectiveCamera.h"
#include "objects/Triangle.h"
#include "objects/Sphere.h"
#include "lights/DirectionalLight.h"

void render_cornell_scene() {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
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

    material dg;
    dg.c = deadgold;
    dg.mt = rm;

    material lg;
    lg.c = lightslategray;
    lg.mt = rm;

    /// light set-up

    glm::vec4 lp2(-1.5f, 4.f, -4.5f, 1);
    glm::vec4 lp3(2.f, 4.f, -4.5f, 1);

    PointLight l2(lp2, orangish, 50);
    lights.push_back(&l2);

    PointLight l3(lp3, violet, 50);
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
    teapot.translate(-7.f, Z);
    teapot.translate(2.f, X);
    teapot.apply_transformations();
//    objects.push_back(&teapot);

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

    monkey.rotate(-30.f, X);
    monkey.rotate(25.f, Y);
    monkey.translate(-6.f, Z);
    monkey.translate(-1.1f, X);
    monkey.translate(0.6f, Y);
    monkey.apply_transformations();
//    objects.push_back(&monkey);

    // load bunny
    sprintf(fn, "./bunny.obj");
    TriangleMesh bunny(dg, true);

    // measure loading the triangulated mesh
    start_loading = std::chrono::high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << "Start loading..." << std::endl;
    li = bunny.load_mesh(fn);
    finish_loading = std::chrono::high_resolution_clock::now();
    std::cout << "Done loading '" <<  fn << "'." << std::endl;
    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
    std::cout << std::endl;

    bunny.rotate(45.f, Y);
    bunny.translate(-6.5f, Z);
    bunny.translate(0.35f, Y);
    bunny.apply_transformations();
    objects.push_back(&bunny);


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
    ip.save_to_png("cornell_box.png");
}

void render_simple_refl_scene() {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(1280, 720);
    ip.ns = 5;
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
    monkey.translate(-0.7f, Z);
    monkey.translate(-1.f, X);
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
    teapot.translate(-1.5f, Z);
    teapot.translate(1.3f, X);
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
    ImagePlane ip = ImagePlane(1280, 720);
    ip.ns = 2;
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

    objects.push_back(&s1);
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
    std::cout << "Done loading '" <<  fn << "'." << std::endl;
    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
    std::cout << std::endl;

    cube.scale(0.5f, XYZ);
    cube.translate(-3.f, Z);
    cube.apply_transformations();
    objects.push_back(&cube);

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

void balls_scene() {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(1280, 720);
    ip.ns = 2;
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
    ip.save_to_png("ball_scene.png");

}

void spheres() {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(1280, 720);
    ip.ns = 4;
    render_info ri;
    uint32_t nos = 100;  // number of spheres
    std::random_device  rd;                                     // obtain a random number from hardware
    std::mt19937        eng(rd());                              // seed generator
    std::uniform_real_distribution<float_t> srzp(-13.f, -7.f);  // define range for sphere's z-position
    std::uniform_real_distribution<float_t> srxyp(-13.f, 13.f); // define range for sphere's xy-position
    std::uniform_real_distribution<float_t> srr(0.1f, 1.f);     // define range for sphere's radius
    std::uniform_real_distribution<float_t> cr(0.f, 1.f);       // define range for color values

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
    float_t r,g,b;

    // place sphere's randomly in the scene
    for (uint32_t i = 0; i < nos; i++) {
        s = new Sphere();
        // randomly choose and assign sphere's position
        sp.x = srxyp(eng);
        sp.y = srxyp(eng);
        sp.z = srzp(eng);
        (*s).set_center_p(sp);

        // randomly choose and assign sphere's radius
        sr = srr(eng);
        (*s).set_radius(sr);

        // randomly choose and assign color
        r = cr(eng);
        g = cr(eng);
        b = cr(eng);

        sm.c = glm::vec3(r,g,b);

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
    std::cout << "Rendering time                        : " << std::chrono::duration_cast<std::chrono::seconds>(finish_rendering - start_rendering).count() << " seconds" << std::endl;
    std::cout << "# of primary rays                     : " << ri.npr << std::endl;
    std::cout << "# of shadow rays                      : " << ri.nsr << std::endl;
    std::cout << "# of reflection rays                  : " << ri.nrr << std::endl;
    std::cout << "# of objects in the scene             : " << ri.no << std::endl;
    std::cout << "# of light sources in the scene       : " << ri.nls << std::endl;
    std::cout << "# of ray-object intersection tests    : " << ri.nrot << std::endl;
    std::cout << "# of ray-object intersections         : " << ri.nroi << std::endl;
    std::cout << "ratio (isect tests / isect)           : " << (1.f * ri.nrot) / ri.nroi << std::endl;
    ip.save_to_png("spheres.png");
}

void boxes() {
    std::vector<Object*>    objects;
    std::vector<Light*>     lights;
    Camera*                 camera = new PerspectiveCamera();
    ImagePlane              ip = ImagePlane(1280, 720);
    ip.ns                   = 1;
    render_info             ri;
    loading_info            li;
    char                    fn[100];    // file name
    uint32_t                noc = 5;  // number of cubes
    float_t                 r, g, b;    // values for rand generated rgb
    std::random_device      rd;         // obtain a random number from hardware
    std::mt19937            eng(rd());  // seed generator
    std::uniform_real_distribution<float_t> crzp(-10.f, -7.f);  // define range for cubes's z-position
    std::uniform_real_distribution<float_t> crxyp(-4.f, 3.f);   // define range for cubes's xy-position
    std::uniform_real_distribution<float_t> crs(1.f, 1.5f);     // define range for cube's scale
    std::uniform_real_distribution<float_t> cr(0.f, 1.f);       // define range for color values

    /// materials set-up
    material cm;
    cm.ac = 0.f;
    cm.dc = 0.7f;
    cm.sc = 0.2f;
    cm.se = 14.f;

    /// objects set-up

    // load cube
    sprintf(fn, "./bunny.obj");
    TriangleMesh cube(cm, false);

    // measure loading of the cube
    auto start_loading = std::chrono::high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << "Start loading..." << std::endl;
    li = cube.load_mesh(fn);
    auto finish_loading = std::chrono::high_resolution_clock::now();
    std::cout << "Done loading '" <<  fn << "'." << std::endl;
    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
    std::cout << std::endl;

    // place cubes randomly in the scene
    for (int i = 0; i < noc; i++) {
        auto *cube_copy = new TriangleMesh(cube);

        // randomly choose cube's position
        (*cube_copy).translate(crxyp(eng), X);
        (*cube_copy).translate(crxyp(eng), Y);
        (*cube_copy).translate(crzp(eng), Z);

        // randomly choose cube's scale
        (*cube_copy).scale(crs(eng), XYZ);

        // apply transformations for the cube
        (*cube_copy).apply_transformations();

        // randomly choose color
        r = cr(eng);
        g = cr(eng);
        b = cr(eng);

        cm.c = glm::vec3(r, g, b);

        (*cube_copy).om = cm;

        // add cube in the scene
        objects.push_back(&(*cube_copy));
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
    std::cout << "Rendering time                        : " << std::chrono::duration_cast<std::chrono::seconds>(finish_rendering - start_rendering).count() << " seconds" << std::endl;

    std::cout << "# of primary rays                     : " << ri.npr << std::endl;
    std::cout << "# of shadow rays                      : " << ri.nsr << std::endl;
    std::cout << "# of reflection rays                  : " << ri.nrr << std::endl;
    std::cout << "# of objects in the scene             : " << ri.no << std::endl;
    std::cout << "# of light sources in the scene       : " << ri.nls << std::endl;
    std::cout << "# of ray-object intersection tests    : " << ri.nrot << std::endl;
    std::cout << "# of ray-object intersections         : " << ri.nroi << std::endl;
    std::cout << "ratio (isect tests / isect)           : " << (1.f * ri.nrot) / ri.nroi << std::endl;

    // save image to disc
    ip.save_to_png("bunny.png");
}

int main(int argc, char **argv) {
    boxes();
    return 0;
}
#include <iostream>
#include <fstream>
#include "png++/png.hpp"
#include "objects/Object.h"
#include "objects/TriangleMesh.h"
#include "lights/PointLight.h"
#include "cameras/PerspectiveCamera.h"
#include "objects/Triangle.h"
#include "Utilities.h"
#include "objects/Sphere.h"

void save_to_ppm(uint32_t width, uint32_t height, glm::vec3 fb[], const char fn[50]) {
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs(fn, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (uint32_t i = 0; i < height * width; ++i) {
        char r = (char)(255 * glm::clamp(fb[i].r, 0.f, 1.f));
        char g = (char)(255 * glm::clamp(fb[i].g, 0.f, 1.f));
        char b = (char)(255 * glm::clamp(fb[i].b, 0.f, 1.f));
        ofs << r << g << b;
    }
    ofs.close();
}

void save_to_ppm(ImagePlane &ip, const char fn[50]) {
    save_to_ppm(ip.hres, ip.vres, ip.fb, fn);
}

void save_to_png(ImagePlane &ip, const char fn[50]) {
    png::image<png::rgb_pixel> ri(ip.hres, ip.vres);

    for (size_t y = 0; y < ip.vres; ++y) {
        for (size_t x = 0; x < ip.hres; ++x) {

            // color at current pixel in the framebuffer
            glm::vec3 cacp = ip.fb[y*ip.hres + x];

            // convert float value of pixel in a png::byte [0, 255]
            png::byte r = (unsigned char) (255 * glm::clamp(cacp.r, 0.f, 1.f));
            png::byte g = (unsigned char) (255 * glm::clamp(cacp.g, 0.f, 1.f));
            png::byte b = (unsigned char) (255 * glm::clamp(cacp.b, 0.f, 1.f));

            // assign color values to image
            ri[y][x] = png::rgb_pixel(r, g, b);
        }
    }

    // save image to disk
    ri.write(fn);
}

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

    material lg;
    lg.c = lightslategray;

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


//    Sphere s1(refl);
//    s1.translate(-6.5f, Z);
//    s1.translate(3.f, Y);
//    s1.apply_transformations();
//    objects.push_back(&s1);

    // load cube
    sprintf(fn, "./cube.obj");
    TriangleMesh cube(lg);

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

    cube.translate(1.f, Y);
    cube.translate(-7.5f, Z);
    cube.translate(2.f, X);
    cube.apply_transformations();
    objects.push_back(&cube);

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
    monkey.translate(-5.3f, Z);
    monkey.translate(-1.f, X);
    monkey.translate(1.2f, Y);
    monkey.apply_transformations();
    objects.push_back(&monkey);

    /// camera transformations
    camera->rotate(-15.f, X);
    camera->translate(4.f, Y);
    camera->translate(-2.7f, Z);
    dynamic_cast<PerspectiveCamera *>(camera)->set_fov(102.44f);

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
    save_to_png(ip, "cornell_box2.png");
}

void render_simple_refl_scene() {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(1920, 1080);
    char fn[100];
    loading_info li;
    render_info ri;

    /// materials set-up
    material floor;
    floor.ac = 0.f;
    floor.mt = prm;
    floor.c = orangish;

    material ball1;
    ball1.ac = 0.f;
    ball1.dc = 0.6f;
    ball1.sc = 0.2f;
    ball1.se = 12.f;
    ball1.c = bluish;
    ball1.mt = rm;

    material ball2 = ball1;
    ball2.c = violet;

    /// objects set-up

    glm::vec4 v0(-3.f, -0.5f, 0.5f, 1);
    glm::vec4 v1(3.f, -0.5f, 0.5f, 1);
    glm::vec4 v2(-3.f, -0.5f, -5.5f, 1);
    glm::vec4 v3(3, -0.5f, -5.5f, 1);
    glm::vec4 sp1(-1.5f, 0.5f, -2.5f, 1);
    glm::vec4 sp2(1.5f, 0.5f, -2.5f, 1);

    // create reflective plane
    Triangle t1(v0, v1, v2, floor);
    objects.push_back(&t1);

    Triangle t2(v1, v3, v2, floor);
    objects.push_back(&t2);

    // spheres
    Sphere s1(sp1, 1.f, ball1);
    objects.push_back(&s1);

    Sphere s2(sp2, 1.f, ball2);
    objects.push_back(&s2);

    /// illuminate scene

    glm::vec4 lp(-0.5f, 3.f, 1.f, 1);

    PointLight pl(lp, 200.f);
    lights.push_back(&pl);

    /// adjust camera settings

    dynamic_cast<PerspectiveCamera *>(camera)->set_fov(90.f);

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
    save_to_png(ip, "simple_reflective_ball_scene.png");
}

void render_monkey_mirror_scene() {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(1920, 1080);
    char fn[100];
    loading_info li;
    render_info ri;
}

int main(int argc, char **argv) {

    render_simple_refl_scene();
    return 0;
}
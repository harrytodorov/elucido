#include <iostream>
#include <fstream>
#include "objects/Object.h"
#include "objects/TriangleMesh.h"
#include "lights/PointLight.h"
#include "cameras/PerspectiveCamera.h"
#include "objects/Triangle.h"
#include "Utilities.h"

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

int main(int argc, char **argv) {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(1280, 720);
    char fn[100];

    /// material set-up
    float_t box_ac = 0.2f;
    float_t box_dc = 0.7f;
    float_t box_sc = 0.1f;
    float_t box_se = 10.f;

    material box_white;
    box_white.c = white;

    material box_red;
    box_red.c = red;

    material box_green;
    box_green.c = green;

    /// light set-up

    glm::vec4 l1_p(0.f, 0.f, 0.f, 1);

    PointLight l2(l1_p, white, 70);
    l2.translate(-3.f, Z);
    l2.translate(7.f, Y);
    l2.apply_transformations();
    lights.push_back(&l2);
//
//    PointLight l3(l1_p, orangish, 40);
//    l3.translate(2, X);
//    l3.translate(1, Y);
//    l3.apply_transformations();
//    lights.push_back(&l3);

    /// object set-up

    glm::vec4 v0(-5,  0,  -3, 1);
    glm::vec4 v1( 5,  0,  -3, 1);
    glm::vec4 v2(-5, 10,  -3, 1);
    glm::vec4 v3( 5, 10,  -3, 1);
    glm::vec4 v4(-5,  0, -13, 1);
    glm::vec4 v5( 5,  0, -13, 1);
    glm::vec4 v6(-5, 10, -13, 1);
    glm::vec4 v7( 5, 10, -13, 1);

    // floor
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

//    Sphere s1(box_white);
//    s1.translate(-6.5f, Z);
//    s1.translate(3.f, Y);
//    s1.apply_transformations();
//    objects.push_back(&s1);

    // load cube
    sprintf(fn, "./cube.obj");
    TriangleMesh tm1(box_red);

    // measure loading the triangulated mesh
    auto start_loading = std::chrono::high_resolution_clock::now();
    loading_info li;
    std::cout << std::endl;
    std::cout << "Start loading..." << std::endl;
    li = tm1.load_mesh(fn);
    auto finish_loading = std::chrono::high_resolution_clock::now();
    std::cout << "Done loading '" <<  fn << "'." << std::endl;
    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
    std::cout << std::endl;

    tm1.translate(1.f, Y);
    tm1.translate(-6.5f, Z);
    tm1.apply_transformations();
    objects.push_back(&tm1);

    // load teapot
    sprintf(fn, "./wt_teapot.obj");
    TriangleMesh tm2(box_white, true);

    // measure loading the triangulated mesh
    start_loading = std::chrono::high_resolution_clock::now();
    li = loading_info();
    std::cout << std::endl;
    std::cout << "Start loading..." << std::endl;
    li = tm2.load_mesh(fn);
    finish_loading = std::chrono::high_resolution_clock::now();
    std::cout << "Done loading '" <<  fn << "'." << std::endl;
    std::cout << "Loading time                          : " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh             : " << li.nv << std::endl;
    std::cout << "# of vertex normals in the mesh       : " << li.nvn << std::endl;
    std::cout << "# of triangles in the mesh            : " << li.nt << std::endl;
    std::cout << "# of faces in the mesh                : " << li.nf << std::endl;
    std::cout << std::endl;

    tm2.translate(-4.f, Z);
    tm2.scale(1.7f, XYZ);
    tm2.translate(2.f, Y);
    tm2.apply_transformations();
    objects.push_back(&tm2);

    /// camera transformations
    camera->translate(4.f, Y);
    camera->translate(1.f, Z);

    /// rendering

    // measure rendering time
    std::cout << "Start rendering..." << std::endl;
    render_info ri;
    auto start_rendering = std::chrono::high_resolution_clock::now();
    ri = camera->render_scene(objects, lights, ip);
    auto finish_rendering = std::chrono::high_resolution_clock::now();
    std::cout << "Done rendering." << std::endl;
    std::cout << "Rendering time                        : " << std::chrono::duration_cast<std::chrono::seconds>(finish_rendering - start_rendering).count() << " seconds" << std::endl;
    std::cout << "# of primary rays                     : " << ri.npr << std::endl;
    std::cout << "# of shadow rays                      : " << ri.nsr << std::endl;
    std::cout << "# of objects in the scene             : " << ri.no << std::endl;
    std::cout << "# of light sources in the scene       : " << ri.nls << std::endl;
    std::cout << "# of ray-object intersection tests    : " << ri.nrot << std::endl;
    std::cout << "# of ray-object intersections         : " << ri.nroi << std::endl;
    save_to_ppm(ip, "cornell_box.ppm");

    return 0;
}
#include <iostream>
#include <fstream>
#include "objects/Object.h"
#include "cameras/OrthographicCamera.h"
#include "cameras/PerspectiveCamera.h"
#include "objects/TriangleMesh.h"
#include "lights/PointLight.h"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

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
    ImagePlane ip = ImagePlane(1920, 1080);
    ip.bc = 0.7f * white;
    char fn[100];

//    {
//        /// scene 01
//
//        /// material set-up
//        float_t ac = 0.f;
//        float_t dc = 0.6f;
//        float_t sc = 0.3f;
//        float_t se = 10.f;
//        PhongMaterial mat1(ac, dc, sc, se, red);
//        PhongMaterial mat2(ac, dc, sc, se, blue);
//        PhongMaterial mat3(ac, dc, sc, se, green);
//        PhongMaterial mat4(ac, dc, sc, se, red);
//
//        /// object set-up
//        glm::vec4 v0(-7, 0, 0, 1);
//        glm::vec4 v1(7, 0, 0, 1);
//        glm::vec4 v2(-7, 0, -14, 1);
//        glm::vec4 v3(7, 0, -14, 1);
//
//        Triangle t1(v0, v1, v2, &mat1);
//        objects.push_back(&t1);
//        t1.translate(-2.f, Y);
//        t1.translate(-2.5f, Z);
//        t1.apply_transformations();
//
//        Triangle t2(v1, v3, v2, &mat1);
//        objects.push_back(&t2);
//        t2.translate(-2.f, Y);
//        t2.translate(-2.5f, Z);
//        t2.apply_transformations();
//
//
//        glm::vec4 s1_p(0.f, -1.f, -4.f, 1);
//        float_t s1_r(1.f);
//        Sphere s1(s1_p, s1_r, &mat2);
//        objects.push_back(&s1);
//
////        glm::vec4 s2_p(2.5f, -1.f, -7.f, 1);
////        float_t s2_r(1.f);
////        Sphere s2(s2_p, s2_r, &mat3);
////        objects.push_back(&s2);
////
////        glm::vec4 s3_p(-2.5f, -1.f, -7.f, 1);
////        float_t s3_r(1.f);
////        Sphere s3(s3_p, s3_r, &mat4);
////        objects.push_back(&s3);
//
//        /// light set-up
//
//        glm::vec4 l1_p(0.f, 0.f, 0.0f, 1);
//        Light *l1 = new PointLight(l1_p, white, 120);
//        l1->translate(1.7, Y);
//        l1->translate(-6.f, Z);
//        l1->apply_transformations();
//        lights.push_back(l1);
//
//        /// transformations
//
//        // rotate sphere around light with diffuse
//        for(int i = 0; i < 73; i++) {
//            s1.translate(6, Z);
//            s1.rotate(5, Y);
//            s1.translate(-6, Z);
//            s1.apply_transformations();
//            sprintf(fn, "sphere_rotating_around_light_y_axis_%03d.ppm", i);
//            camera->render_scene(objects, lights, ip);
//            save_to_ppm(ip, fn);
//        }
//
//        camera->render_scene(objects, lights, ip);
//        save_to_ppm(ip, "sphere.ppm");
//    }

    /// material set-up

    float_t ac = 0.2f;
    float_t dc = 0.8f;
    float_t sc = 0.3f;
    float_t se = 16.f;
    PhongMaterial mat1(ac, dc, sc, se, whitish);
    PhongMaterial mat2(ac, dc, sc, se, deadgold);

    /// light set-up

    glm::vec4 l1_p(0.f, 0.f, 0.0f, 1);

    PointLight l1(l1_p, white, 30);
    l1.translate(2, Y);
    l1.translate(1.5f, X);
    l1.apply_transformations();
    lights.push_back(&l1);

    PointLight l2(l1_p, violet, 35);
    l2.translate(-2, X);
    l2.translate(2, Y);
    l2.translate(-2.4f, Z);
    l2.apply_transformations();
    lights.push_back(&l2);

//    PointLight l3(l1_p, orangish, 40);
//    l3.translate(2, X);
//    l3.translate(1, Y);
//    l3.translate(-4.f, Z);
//    l3.apply_transformations();
//    lights.push_back(&l3);

    /// object set-up

    glm::vec4 v0(-7, 0, 7, 1);
    glm::vec4 v1(7, 0, 7, 1);
    glm::vec4 v2(-7, 0, -7, 1);
    glm::vec4 v3(7, 0, -7, 1);

    Triangle t1(v0, v1, v2, &mat2);
    objects.push_back(&t1);
    t1.translate(-1.f, Y);
    t1.translate(-2.5f, Z);
    t1.apply_transformations();

    Triangle t2(v1, v3, v2, &mat2);
    objects.push_back(&t2);
    t2.translate(-1.f, Y);
    t2.translate(-2.5f, Z);
    t2.apply_transformations();

    sprintf(fn, "./cube.obj");
    TriangleMesh tm1(&mat1);

    // measure loading the triangulated mesh
    auto start_loading = std::chrono::high_resolution_clock::now();
    loading_info li;
    std::cout << "Start loading the triangulated mesh..." << std::endl;
    li = tm1.load_mesh(fn);
    auto finish_loading = std::chrono::high_resolution_clock::now();
    std::cout << "Done loading the triangulated mesh: " << std::endl;
    std::cout << "Elapsed time for loading the mesh: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_loading - start_loading).count() << " milliseconds" << std::endl;
    std::cout << "# of vertices in the mesh: " << li.num_vertices << std::endl;
    std::cout << "# of vertex normals in the mesh: " << li.num_of_vn << std::endl;
    std::cout << "# of triangles in the mesh: " << li.num_of_triangles << std::endl;
    std::cout << "# of faces in the mesh: " << li.num_of_faces << std::endl;
    std::cout << std::endl;

//    tm1.rotate(-45, XY);
    tm1.translate(-4.5f, Z);
    tm1.translate(0.5f, Y);
    tm1.apply_transformations();
    objects.push_back(&tm1);

    /// camera transformations
    camera->rotate(-15, X);
    camera->translate(1.5, Y);

    for (int t = 0; t < 19; ++t) {
        tm1.translate(4.5f, Z);
        tm1.translate(-0.5f, Y);
        tm1.rotate(20, XY);
        tm1.translate(-4.5f, Z);
        tm1.translate(0.5f, Y);
        tm1.apply_transformations();
        sprintf(fn, "cube_rotating/cube_rotating_around_xy_axis_%03d.ppm", t);
        camera->render_scene(objects, lights, ip);
        save_to_ppm(ip, fn);
    }

//    // measure rendering time
//    std::cout << "Start rendering scene..." << std::endl;
//    render_info ri;
//    auto start_rendering = std::chrono::high_resolution_clock::now();
//    ri = camera->render_scene(objects, lights, ip);
//    auto finish_rendering = std::chrono::high_resolution_clock::now();
//    std::cout << "Done rendering the scene: " << std::endl;
//    std::cout << "Elapsed time for rendering the scene: " << std::chrono::duration_cast<std::chrono::milliseconds>(finish_rendering - start_rendering).count() << " milliseconds" << std::endl;
//    std::cout << "# of primary rays: " << ri.primary_rays << std::endl;
//    std::cout << "# of shadow rays: " << ri.shadow_rays << std::endl;
//    std::cout << "# of objects in the scene: " << ri.num_of_objects << std::endl;
//    std::cout << "# of light sources in the scene: " << ri.num_of_light_sources<< std::endl;
//    save_to_ppm(ip, "cube.ppm");

    return 0;
}
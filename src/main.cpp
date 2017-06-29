#include <iostream>
#include <fstream>
#include "objects/Object.h"
#include "cameras/OrthographicCamera.h"
#include "objects/Sphere.h"
#include "cameras/PerspectiveCamera.h"
#include "lights/PointLight.h"
#include "objects/Triangle.h"
#include <glm/gtc/matrix_transform.hpp>

inline float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }

void save_to_ppm(uint32_t width, uint32_t height, glm::vec3 fb[], const char fn[50]) {
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs(fn, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (uint32_t i = 0; i < height * width; ++i) {
        char r = (char)(255 * clamp(0, 1, fb[i].x));
        char g = (char)(255 * clamp(0, 1, fb[i].y));
        char b = (char)(255 * clamp(0, 1, fb[i].z));
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

    {
        /// scene 01

        /// material set-up
        float_t ac = 0.f;
        float_t dc = 0.6f;
        float_t sc = 0.3f;
        float_t se = 10.f;
        PhongMaterial mat1(ac, dc, sc, se, red);
        PhongMaterial mat2(ac, dc, sc, se, blue);
        PhongMaterial mat3(ac, dc, sc, se, green);
        PhongMaterial mat4(ac, dc, sc, se, red);

        /// object set-up
        glm::vec4 v0(-7, 0, 0, 1);
        glm::vec4 v1(7, 0, 0, 1);
        glm::vec4 v2(-7, 0, -14, 1);
        glm::vec4 v3(7, 0, -14, 1);

        Triangle t1(v0, v1, v2, &mat1);
        objects.push_back(&t1);
        t1.translate(-2.f, Y);
        t1.translate(-2.5f, Z);
        t1.apply_transformations();

        Triangle t2(v1, v3, v2, &mat1);
        objects.push_back(&t2);
        t2.translate(-2.f, Y);
        t2.translate(-2.5f, Z);
        t2.apply_transformations();


        glm::vec4 s1_p(0.f, -1.f, -4.f, 1);
        float_t s1_r(1.f);
        Sphere s1(s1_p, s1_r, &mat2);
        objects.push_back(&s1);

//        glm::vec4 s2_p(2.5f, -1.f, -7.f, 1);
//        float_t s2_r(1.f);
//        Sphere s2(s2_p, s2_r, &mat3);
//        objects.push_back(&s2);
//
//        glm::vec4 s3_p(-2.5f, -1.f, -7.f, 1);
//        float_t s3_r(1.f);
//        Sphere s3(s3_p, s3_r, &mat4);
//        objects.push_back(&s3);

        /// light set-up

        glm::vec4 l1_p(0.f, 0.f, 0.0f, 1);
        Light *l1 = new PointLight(l1_p, white, 120);
        l1->translate(1.7, Y);
        l1->translate(-6.f, Z);
        l1->apply_transformations();
        lights.push_back(l1);

        /// transformations

        // rotate sphere around light with diffuse
        for(int i = 0; i < 73; i++) {
            s1.translate(6, Z);
            s1.rotate(5, Y);
            s1.translate(-6, Z);
            s1.apply_transformations();
            sprintf(fn, "sphere_rotating_around_light_y_axis_%03d.ppm", i);
            camera->render_scene(objects, lights, ip);
            save_to_ppm(ip, fn);
        }

//        camera->render_scene(objects, lights, ip);
//        save_to_ppm(ip, "sphere.ppm");
    }

    return 0;
}
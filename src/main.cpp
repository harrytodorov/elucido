#include <iostream>
#include <fstream>
#include "objects/Object.h"
#include "objects/Sphere.h"
#include "cameras/OrthographicCamera.h"
#include "lights/PointLight.h"
#include "cameras/PerspectiveCamera.h"

void save_to_ppm(uint32_t width, uint32_t height, glm::vec3 *(&fb), const char fn[50]) {
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs(fn, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (uint32_t i = 0; i < height * width; ++i) {
        char r = (char) fb[i].x;
        char g = (char) fb[i].y;
        char b = (char) fb[i].z;
        ofs << r << g << b;
    }
    ofs.close();
}

void save_to_ppm(ImagePlane &ip, const char fn[50]) {
    save_to_ppm(ip.hres, ip.vres, ip.fb, fn);
}

int main(int argc,char **argv) {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(640, 480);



    // set up with two spheres diff colors
//    {
//        // sphere 1
//        glm::vec3 s1_pos = glm::vec3(0, 0, 0);
//        glm::vec3 s1_col = glm::vec3(255, 0, 0);
//        float_t   s1_rad = 85.0;
//        Object *s1 = new Sphere(s1_pos, s1_rad);
//        s1->color = s1_col;
//        objects.push_back(s1);
//
//        // sphere 2
//        glm::vec3 s2_pos = glm::vec3(0, 0, -185);
//        glm::vec3 s2_col = glm::vec3(0, 255, 0);
//        float_t   s2_rad = 100.0;
//        Object *s2 = new Sphere(s2_pos, s2_rad);
//        s2->color = s2_col;
//        objects.push_back(s2);
//
//        camera->render_scene(objects, lights, ip);
//        save_to_ppm(ip, "2_spheres_overlap_ortho_cam_ambient_only.ppm");
//    }

    // set up a basic scene with sphere and plane and one light (only diffuse)
    {
        // sphere set up
        glm::vec3 s_pos = glm::vec3(0, 0, -41); // position (0, 0, 0) world space
        glm::vec3 s_col = glm::vec3(255, 0, 0); // red color
        float_t   s_rad = 30.0;                 // radius

        Object *s1 = new Sphere(s_pos, s_rad);
        s1->color = s_col;
        objects.push_back(s1);

        // light set up
        glm::vec3 l_pos = glm::vec3(20, 0, -5);
        glm::vec3 l_col = glm::vec3(255, 0, 0);
        float_t   l_int = 150.0;

        Light *l1 = new PointLight(l_pos, l_col, l_int);
        lights.push_back(l1);

        camera->render_scene(objects, lights, ip);
        save_to_ppm(ip, "sphere_pers.ppm");
    }

    return 0;
}


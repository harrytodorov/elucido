#include <iostream>
#include <fstream>
#include "objects/Object.h"
#include "objects/Sphere.h"
#include "cameras/OrthographicCamera.h"

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

void save_to_ppm(Camera &c, const char fn[50]) {
    save_to_ppm(c.ip_x, c.ip_y, c.fb, fn);
}

int main(int argc,char **argv) {
    std::vector<Object*> objects;

    Object *s1 = new Sphere(glm::vec3(0, 0, -80), 15.0);
    s1->color = glm::vec3(255,152,0);   // orangish

    Object *s2 = new Sphere(glm::vec3(0, 0, -100), 30);
    s2->color = glm::vec3(156,39,176);  // violet


    objects.push_back(s1);
    objects.push_back(s2);

    glm::vec3 camera_pos = glm::vec3(0);
    char *fn = new char[50];                // file name
    glm::vec3 bc = glm::vec3(250, 250, 250);

    // create the camera
    Camera* camera = new OrthographicCamera(bc, 0.5);

    camera->render_scene(objects);
    save_to_ppm(*camera, "red_sphere.ppm");

//    // render sphere with different pixel sizes
//    for (int pix_s = 0; pix_s < 40; pix_s++) {
//        camera->ps = (float_t) 0.1 * pix_s;
//        camera->render_scene(objects);
//        sprintf(fn, "./basic_sphere_diff_pix_s/basic_sphere_(%0.1f).ppm", camera->ps);
//        save_to_ppm(camera->ip_x, camera->ip_y, camera->fb, fn);
//    }

    return 0;
}
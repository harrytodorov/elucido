#include <iostream>
#include <fstream>
#include "objects/Object.h"
#include "objects/Sphere.h"
#include "cameras/OrthographicCamera.h"

inline float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }

void save_to_ppm(int width, int height, glm::vec3 *(&framebuffer)) {
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs("./basic_sphere.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (uint32_t i = 0; i < height * width; ++i) {
        char r = (char)(255 * clamp(0, 1, framebuffer[i].x));
        char g = (char)(255 * clamp(0, 1, framebuffer[i].y));
        char b = (char)(255 * clamp(0, 1, framebuffer[i].z));
        ofs << r << g << b;
    }

    ofs.close();
}

int main(int argc,char **argv) {
    Object* s1 = new Sphere(glm::vec3(0, 0, 0), 85.0);

    // create the camera
    Camera* camera = new OrthographicCamera();

    // render the scene;
    camera->render_scene1(*s1);

    save_to_ppm(camera->ip_x, camera->ip_y, camera->fb);
    return 0;
}
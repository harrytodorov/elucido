#include <iostream>
#include <fstream>
#include "objects/Object.h"
#include "cameras/OrthographicCamera.h"
#include "objects/Sphere.h"
#include "cameras/PerspectiveCamera.h"
#include "lights/PointLight.h"
#include <glm/gtc/matrix_transform.hpp>

inline float clamp(const float &lo, const float &hi, const float &v)
{ return std::max(lo, std::min(hi, v)); }

void save_to_ppm(uint32_t width, uint32_t height, glm::vec3 *(&fb), const char fn[50]) {
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

int main(int argc,char **argv) {
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Camera* camera = new PerspectiveCamera();
    ImagePlane ip = ImagePlane(640, 480);
    ip.bc = 0.7f * white;
    char fn[100];

//    // cube set-up
//    {
//        // front face of the cube
//        glm::vec4 ff_bl(-30, -30, -5, 1);  // front face bottom left
//        glm::vec4 ff_br( 30, -30, -5, 1);  // front face bottom right
//        glm::vec4 ff_tl(-30,  30, -5, 1);  // front face top left
//        glm::vec4 ff_tr( 30,  30, -5, 1);  // front face top right
//
//        // back face of the cube
//        glm::vec4 bf_bl(-30, -30, -70, 1);  // back face bottom left
//        glm::vec4 bf_br( 30, -30, -70, 1);  // back face bottom right
//        glm::vec4 bf_tl(-30,  30, -70, 1);  // back face top left
//        glm::vec4 bf_tr( 30,  30, -10, 1);  // back face top right
//
//        // face normals of the cube
//        glm::vec4 ff_n( 0,  0,  1, 0);      // front face normal
//        glm::vec4 bf_n( 0,  0, -1, 0);      // back face normal
//        glm::vec4 lf_n(-1,  0,  0, 0);      // left face normal
//        glm::vec4 rf_n( 1,  0,  0, 0);      // right face normal
//        glm::vec4 df_n( 0, -1,  0, 0);      // bottom(down) face normal
//        glm::vec4 tf_n( 0,  1,  0, 0);      // top face normal
//
//        // the cube is build from 12 triangles
//
//        glm::vec3 t_col(0, 0, 255);
//
//        // triangle 1; front face bottom left
//        Object *t1 = new Triangle(ff_bl, ff_br, ff_tl, ff_n);
//        t1->color = t_col;
//        objects.push_back(t1);
//
//        // triangle 2; front face top right
//        Object *t2 = new Triangle(ff_tr, ff_tl, ff_br, ff_n);
//        t2->color = t_col;
//        objects.push_back(t2);
//
//        // triangle 3; back face bottom left
//        Object *t3 = new Triangle(bf_bl, bf_br, bf_tl, bf_n);
//        t3->color = t_col;
//        objects.push_back(t3);
//
////        // triangle 4; back face top right
////        Object *t4 = new Triangle(bf_tl, bf_tr, bf_br, bf_n);
////        t4->color = t_col;
////        objects.push_back(t4);
////
////        // triangle 5; bottom face front left
////        Object *t5 = new Triangle(bf_bl, ff_bl, ff_br, df_n);
////        t5->color = t_col;
////        objects.push_back(t5);
////
////        // triangle 6; bottom face back right
////        Object *t6 = new Triangle(bf_bl, bf_br, ff_br, df_n);
////        t6->color = t_col;
////        objects.push_back(t6);
////
////        // triangle 7; top face front left
////        Object *t7 = new Triangle(bf_tl, ff_tl, ff_tr, tf_n);
////        t7->color = t_col;
////        objects.push_back(t7);
////
////        // triangle 8; top face bottom right
////        Object *t8 = new Triangle(bf_tl, bf_tr, ff_tr, tf_n);
////        t8->color = t_col;
////        objects.push_back(t8);
////
////        // triangle 9; left face front bottom
////        Object *t9 = new Triangle(ff_tl, ff_bl, bf_bl, lf_n);
////        t9->color = t_col;
////        objects.push_back(t9);
////
////        // triangle 10; left face back top
////        Object *t10 = new Triangle(ff_tl, bf_tl, bf_bl, lf_n);
////        t10->color = t_col;
////        objects.push_back(t10);
////
////        // triangle 11; right face front bottom
////        Object *t11 = new Triangle(ff_tr, ff_br, bf_br, rf_n);
////        t11->color = t_col;
////        objects.push_back(t11);
////
////        // triangle 12; right face back top
////        Object *t12 = new Triangle(ff_tr, bf_tr, bf_br, rf_n);
////        t12->color = t_col;
////        objects.push_back(t12);
////
//        // light set up
//        glm::vec4 l_pos(0, 0, 15, 1);
//        glm::vec3 l_col(255);
//        float_t   l_int(3000);
//
//        Light *l1 = new PointLight(l_pos, l_col, l_int);
//        lights.push_back(l1);
//
//
//        // set up translation
//        glm::mat4 translate_obj = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
//
//        // apply translation
//        for (auto& object : objects) {
//            object->apply_camera_transformation(translate_obj);
//        }
//
//        camera->render_scene(objects, lights, ip);
//        save_to_ppm(ip, "cube.ppm");
//
//    }
//

//    {
//        /// scene 01
//
////        // triangle 01
////        glm::vec4 t1_p();
////        Object *t1 = new Triangle();
//
//        // sphere 01
//        glm::vec4 s1_p(0.f, 0, -1.5f, 1);
//        float_t   s1_r(0.5f);
//        Object *s1 = new Sphere(s1_p, s1_r);
//        s1->om.c = green;
//        objects.push_back(s1);
//
////        // sphere 02
////        glm::vec4 s2_p(-0.2f, 0, -1.2f, 1);
////        float_t   s2_r(0.5f);
////        Object *s2 = new Sphere(s2_p, s2_r);
////        s2->om.c = red;
////        objects.push_back(s2);
////
////        // sphere 03
////        glm::vec4 s3_p(-0.6f, 0, -1.8f, 1);
////        float_t   s3_r(0.5f);
////        Object *s3 = new Sphere(s3_p, s3_r);
////        s3->om.c = white;
////        objects.push_back(s3);
//
////        // sphere 04
////        glm::vec4 s4_p(0.7f, 0, -0.7f, 1);
////        float_t   s4_r(0.2f);
////        Object *s4 = new Sphere(s4_p, s4_r);
////        s4->om.c = blue;
////        objects.push_back(s4);
//
////        // sphere 05
////        glm::vec4 s5_p(0.7f, 0, -1.7f, 1);
////        float_t s5_r(0.3f);
////        Object *s5 = new Sphere(s5_p, s5_r);
////        s5->om.c = white;
////        objects.push_back(s5);
////
////        // sphere 06
////        glm::vec4 s6_p(0.7f, 0, -2.7f, 1);
////        float_t s6_r(0.5f);
////        Object *s6 = new Sphere(s6_p, s6_r);
////        s6->om.c = green;
////        objects.push_back(s6);
////
////        // sphere 07
////        glm::vec4 s7_p(0.7f, 0, -4.3f, 1);
////        float_t s7_r(0.8f);
////        Object *s7 = new Sphere(s7_p, s7_r);
////        s7->om.c = sienna;
////        objects.push_back(s7);
//
//
//        // light 01
//        glm::vec4 l1_p(-1.f, 0, 0, 1);
//        float_t l1_i(1);
//        Light *l01 = new PointLight(l1_p, white, l1_i);
//        lights.push_back(l01);
////
////        // light 02
////        glm::vec4 l2_p(0, 0, -0.5f, 1);
////        float_t l2_i(10);
////        Light *l02 = new PointLight(l2_p, white, l2_i);
////        lights.push_back(l02);
//
////        // light 03
////        glm::vec4 l3_p(0, 0, -4.f, 1);
////        float_t l3_i(10);
////        Light *l03 = new PointLight(l3_p, white, l3_i);
////        lights.push_back(l03);
//
//
//
////        /// transformations
////        camera->rotate(-45.f, Y);
////
//        /// rendering
//
//        camera->render_scene(objects, lights, ip);
//        save_to_ppm(ip, "sphere.ppm");
//    }

    {
        /// scene 02

        /// object set-up
        glm::vec4 s2_p(0, 0, -3, 1);
        float_t s2_r(1.0f);

        Sphere s2;
        s2.set_center_p(s2_p);
        s2.set_radius(s2_r);
        s2.om.c = red;
        s2.om.dc = 0.8f;
        s2.om.sc = 0.7f;
        s2.om.se = 10.f;
        s2.om.ac = 0.f;
        objects.push_back(&s2);


        /// light set-up
        glm::vec4 l1_p(0.f, 0.f, -1.f, 1);

        Light *l1 = new PointLight(l1_p, white, 10);
        lights.push_back(l1);

        /// transformations

        // light is positioned initially on the left side of the sphere

        // translate the light so its relative to the object position
        l1->translate(1.f, Z);

        // rotate it 90 deg
        l1->rotate(10, Y);

        // translate it back rotated
        l1->translate(-1.f, Z);

        l1->apply_transformations();

//        // light rotates around a sphere with constant diffuse
//        for (int i = 0; i < 37; i++) {
//            sprintf(fn, "light_rotation_ar_sphere_diffuse_0_7_%03d.ppm", i);
//            camera->render_scene(objects, lights, ip);
//            save_to_ppm(ip, fn);
//            l1->rotate(5.f, Y);
//        }

        camera->render_scene(objects, lights, ip);
        save_to_ppm(ip, "sphere.ppm");
    }

    return 0;
}
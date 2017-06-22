#include <iostream>
#include <fstream>
#include "objects/Object.h"
#include "cameras/OrthographicCamera.h"
#include "objects/Triangle.h"
#include <glm/gtc/matrix_transform.hpp>

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
    Camera* camera = new OrthographicCamera();
    ImagePlane ip = ImagePlane(640, 480);

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
//            object->apply_transformation(translate_obj);
//        }
//
//        camera->render_scene(objects, lights, ip);
//        save_to_ppm(ip, "cube.ppm");
//
//    }
//
    {
        /// scene 01

//        // sphere 01
//        glm::vec4 s1_p(-0.5f, 0, -1.5f, 1);
//        float_t   s1_r(0.5f);
//        Object *s1 = new Sphere(s1_p, s1_r);
//        s1->om.c = green;
//        objects.push_back(s1);
//
//        // sphere 02
//        glm::vec4 s2_p(0, 0, -2.8f, 1);
//        float_t   s2_r(0.6f);
//        Object *s2 = new Sphere(s2_p, s2_r);
//        s2->om.c = blue;
//        objects.push_back(s2);

        // triangle 01
        Object *t1 = new Triangle();
        objects.push_back(t1);

        /// transformations
        // translate t1 along negative-z so it's visible for the camera
        t1->translate(-2.f, Z);
        // scale it, so it does not take so much space
        t1->scale(0.5f, XYZ);
        // now rotate it, because we can :D
        t1->rotate(30.f, Z);
        t1->rotate(15.f, X);

        /// rendering

        camera->render_scene(objects, lights, ip);
        save_to_ppm(ip, "scene01.ppm");
    }

    return 0;
}
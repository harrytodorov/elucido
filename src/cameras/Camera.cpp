//
// Created by Haralambi Todorov on 18/05/2017.
//

#include <vector>
#include <iostream>
#include "Camera.h"
#include "glm/glm.hpp"
#include "../Ray.h"
#include "../objects/Object.h"

//void Camera::compute_uvw(void) {
//    w = eye_point - look_at;
//    // normalize up vector
//    glm::normalize(up);
//    glm::normalize(w);
//    u = glm::cross(up, w);
//    glm::normalize(u);
//    v = glm::cross(w, u);
//}
//
//void Camera::render_scene(std::vector<std::unique_ptr<Object>> &objects, Magick::Image &image_plane) {
//    Ray ray;
//    double x, y;
//    ray.orig = eye_point;
//    double scale = tan(deg2rad(fov * 0.5));
//    double aspect_ratio = x_res / y_res;
//    glm::mat4x4 cam_to_world = look_At(glm::dvec3(0, 0, 0), glm::dvec3(0, 0, -10));
//    double infinity = std::numeric_limits<double>::max();
//    double tNear = infinity;
//
//    // image plane dimensions as a char array
//    // needed to construct an Image object (ImageMagick)
//    char   image_plane_d[100];
//    sprintf(image_plane_d, "%dx%d", x_res, y_res);
//
//    image_plane = Magick::Image::Image(image_plane_d, "black");
//
//    // transform the ray origin to world-space
//    ray.orig = cam_to_world * glm::dvec4(0, 0, 0, 1);
//
//    for (int c = 0; c < y_res; c++) {
//        for (int r = 0; r < x_res; r++) {
//            x = (2 * (r + 0.5) / x_res - 1) * scale;
//            y = (1 - 2 * (c + 0.5) / y_res) * scale * 1 / aspect_ratio;
//
//            // transform the ray direction
//            ray.dir = cam_to_world * glm::dvec4(x, y, -1, 0);
//            glm::normalize(ray.dir);
//
//            // iterate through objects
//            std::vector<std::unique_ptr<Object>>::const_iterator iter = objects.begin();
//            for (; iter != objects.end(); ++iter) {
//                double t = infinity;
//                if ((*iter)->intersect(ray.orig, ray.dir, t) && t < tNear) {
//                    std::cout << "here" << std::endl;
//                    image_plane.pixelColor(r, c, Magick::Color("snow"));
//                }
//            }
//        }
//    }
//}
//
//glm::mat4x4 Camera::look_At(const glm::dvec3 &from, const glm::dvec3 &to) {
//    const glm::dvec3 tmp = glm::dvec3(0, 1, 0);
//    glm::dvec3 forward = glm::normalize(from - to);
//    glm::dvec3 right = glm::cross(normalize(tmp), forward);
//    glm::dvec3 up = glm::cross(forward, right);
//
//    glm::mat4x4 cam_to_world;
//
//    cam_to_world[0][0] = right.x;
//    cam_to_world[0][1] = right.y;
//    cam_to_world[0][2] = right.z;
//    cam_to_world[1][0] = up.x;
//    cam_to_world[1][1] = up.y;
//    cam_to_world[1][2] = up.z;
//    cam_to_world[2][0] = forward.x;
//    cam_to_world[2][1] = forward.y;
//    cam_to_world[2][2] = forward.z;
//
//    cam_to_world[3][0] = from.x;
//    cam_to_world[3][1] = from.y;
//    cam_to_world[3][2] = from.z;
//
//    return cam_to_world;
//}

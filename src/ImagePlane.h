//
// Created by Haralambi Todorov on 26/05/2017.
//

#ifndef ELUCIDO_IMAGEPLANE_H
#define ELUCIDO_IMAGEPLANE_H


#include <cstdint>
#include <cmath>
#include <glm/vec3.hpp>

class ImagePlane {
public:
    uint32_t        hres;      // horizontal image resolution
    uint32_t        vres;      // vertical image resolution
    const float_t   s = 1.0;   // pixel size; this project will have a fixed pixel size
                               // for getting effects as zooming camera properties / object transformations
                               // should be applied
    glm::vec3*      fb;        // frame buffer
    glm::vec3       bc;        // background color

    // constructors & destructors
    // - default resolution of the image plane if not specified is 640x480
    // - default background color is black (0, 0, 0)
    ImagePlane(const uint32_t &ip_x, const uint32_t &ip_y) {
        hres = ip_x;
        vres = ip_y;
        bc   = glm::vec3(0);
        fb   = new glm::vec3[ip_x * ip_y];
    }
    ImagePlane() : ImagePlane(640, 480) {}
    ~ImagePlane() {}

};


#endif //ELUCIDO_IMAGEPLANE_H

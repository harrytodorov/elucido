//
// Created by Haralambi Todorov on 27/06/2017.
//

#ifndef ELUCIDO_PHONGMATERIAL_H
#define ELUCIDO_PHONGMATERIAL_H


#include <cmath>
#include "Material.h"

class PhongMaterial : public Material {
    float_t   ac;       // ambient constant
    float_t   dc;       // diffuse constant
    float_t   sc;       // specular constant
    float_t   se;       // specular exponent

public:
    // default constructor
    // - default ambient constant is 0.2
    // - default diffuse constant is 0.6
    // - default specular constant is 0.2
    // - default specular exponent is 10
    PhongMaterial() : Material(phong_mat), ac(0.2f), dc(0.6f), sc(0.2f), se(10.f) {}
    PhongMaterial(const glm::vec3 &c) : Material(c, phong_mat), ac(0.2f), dc(0.6f), sc(0.2f), se(10.f) {}
    PhongMaterial(const float_t &a, const float_t &d, const float_t &sc, const float_t &se) :
            Material(phong_mat),
            ac(a),
            dc(d),
            sc(sc),
            se(se)
    {}
    PhongMaterial(const float_t &a, const float_t &d, const float_t &sc, const float_t &se, const glm::vec3 &c) :
            Material(c, phong_mat),
            ac(a),
            dc(d),
            sc(sc),
            se(se)
    {}
    ~PhongMaterial() {}

    inline void set_ambient(const float_t &a) {ac = a;}
    inline void set_diffuse(const float_t &d) {dc = d;}
    inline void set_specular(const float_t &sc, const float_t &se) {this->sc = sc; this->se = se;}
    inline float_t get_ambient() { return ac;}
    inline float_t get_diffuse() { return dc;}
    inline float_t get_specular() { return sc;}
    inline float_t get_specular_exp() { return se;}
};


#endif //ELUCIDO_PHONGMATERIAL_H

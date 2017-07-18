//
// Created by Haralambi Todorov on 18/05/2017.
//
#include "Camera.h"

void Camera::rotate(float_t rot_angle, uint32_t axes_of_rotation) {
    // create 3d vector to determine the axis of rotation
    glm::vec3 rv(0);

    switch (axes_of_rotation) {
        case X :
            rv.x = 1;
            break;
        case Y :
            rv.y = 1;
            break;
        case Z :
            rv.z = 1;
            break;
        case XY :
            rv.x = 1;
            rv.y = 1;
            break;
        case XZ :
            rv.x = 1;
            rv.z = 1;
            break;
        case YZ :
            rv.y = 1;
            rv.z = 1;
            break;
        case XYZ :
            rv = glm::vec3(1);
            break;
        default:
            printf("You're using an undefined axis of rotation.");
            break;
    }

    // get the rotation matrix
    glm::mat4 rm = glm::rotate(glm::mat4(1), glm::radians(rot_angle), rv);

    // apply the rotation matrix to the camera's transformation matrix
    ctm = rm * ctm;

    // update the transpose of the inverse of the camera's transformation matrix
    tictm = glm::transpose(glm::inverse(rm)) * tictm;
}

void Camera::translate(const float_t &translation, const uint32_t &axes_of_translation) {
    // create 3d vector to determine the axes of translation
    glm::vec3 tv(0);

    switch (axes_of_translation) {
        case X :
            tv.x = translation;
            break;
        case Y :
            tv.y = translation;
            break;
        case Z :
            tv.z = translation;
            break;
        case XY :
            tv.x = translation;
            tv.y = translation;
            break;
        case XZ :
            tv.x = translation;
            tv.z = translation;
            break;
        case YZ :
            tv.y = translation;
            tv.z = translation;
            break;
        case XYZ :
            tv = glm::vec3(translation);
            break;
        default:
            printf("You're using an undefined axis of translation.");
            break;
    }
    // get the translation matrix
    glm::mat4 tm = glm::translate(glm::mat4(1), tv);

    // assign the translation matrix to the camera's transformation matrix
    ctm = tm * ctm;
}

void Camera::compute_color_at_surface(const std::vector<Light *> &lights, const std::vector<Object *> &objects,
                                      const glm::vec4 view_direction, const isect_info &ii, glm::vec3 &color, render_info &ri) {

    switch (ii.ho->om->mt) {
        case Material::phong: {
            Ray shadow_ray;
            PhongMaterial *material = (PhongMaterial *) ii.ho->om;
            isect_info dummy;
            float_t visibility(1.f);

            // set the hit color to black before adding the ambient, defuse and specular components
            // in case the default background color is not black
            color = black;

            // holders for diffuse & specular values;
            glm::vec3 diffuse(0), specular(0);
            float_t lambertian_refl(0);

            // iterate through all light sources and calculate specular and defuse components
            for (auto &light : lights) {
                glm::vec4 light_direction(0);
                glm::vec3 light_intensity(0);
                float_t light_dist = infinity;

                light->illuminate(ii.ip, light_direction, light_intensity, light_dist);

                // TODO: check where is the bug in the calculation of shadows

                // compute if the surface point is in shadow
                shadow_ray.rt = shadow;

                // hit_normal * shadow_bias is used to translate the origin point by a slightly bit
                // set_orig one could avoid self-shadows, because of float number precision
                shadow_ray.set_orig(ii.ip + ii.ipn*shadow_bias);

                // for the direction of the shadow ray we take the opposite of the light direction
                shadow_ray.set_dir(-light_direction);

                if (shadow_ray.trace(objects, dummy, <#initializer#>) && dummy.tn < light_dist) {
                    visibility = 0.f;
                }

                // dot product based on Lambert's cosine law for Lambertian reflectance;
                lambertian_refl = glm::dot(ii.ipn, -light_direction);

                // calculate diffuse component
                diffuse += visibility * (material->c * light_intensity * glm::max(0.f, lambertian_refl));

                // calculate specular component
                glm::vec4 light_reflection = glm::normalize(2.f * lambertian_refl * ii.ipn + light_direction);
                float_t max_lf_vd = glm::max(0.f, glm::dot(light_reflection, view_direction));
                float_t pow_max_se = glm::pow(max_lf_vd, material->get_specular_exp());

                specular += visibility * (light_intensity * pow_max_se);
            }

            // add ambient, diffuse and specular to the the hit color
            color += material->get_ambient() * material->c + material->get_diffuse() * diffuse + material->get_specular() * specular;
            break;
        }

    }

}
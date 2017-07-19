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

glm::vec4 Camera::reflect(const glm::vec4 &view, const glm::vec4 &surface_normal) {
    return view - 2.f * glm::dot(view, surface_normal) * surface_normal;
}

glm::vec3 Camera::cast_ray(const Ray &ray, const std::vector<Light *> &lights, const std::vector<Object *> &objects, const uint32_t &depth, render_info &ri) {
    // if we've reached the bottom of the recursion tree, we return the background color
    if (depth > max_depth) return bgc;

    // hit color
    glm::vec3 hc(bgc);

    // intersection information
    isect_info ii;

    // trace a ray through the scene
    if (ray.trace(objects, ii, ri)) {

        // if we have an intersection we set the background color to 0 for all components; black
        hc = black;

        // get the surface properties of the intersection
        ii.ho->get_surface_properties(ii);

        // material of the intersected object
        material mat = ii.ho->om;

        switch (mat.mt) {
            case pm : {
                Ray shadow_ray;
                isect_info dummy;
                float_t visibility(1.f);

                // holders for diffuse & specular values;
                glm::vec3 diffuse(0), specular(0);
                float_t lamb_refl(0);

                // iterate through all light sources and calculate specular and defuse components
                for (auto &light : lights) {
                    glm::vec4 light_direction(0);
                    glm::vec3 light_intensity(0);
                    float_t light_dist = infinity;

                    light->illuminate(ii.ip, light_direction, light_intensity, light_dist);

                    // compute if the surface point is in shadow
                    shadow_ray.rt = shadow;

                    // hit_normal * bias is used to translate the origin point by a slightly bit
                    // set_orig one could avoid self-shadows, because of float number precision
                    shadow_ray.set_orig(ii.ip + ii.ipn*bias);

                    // for the direction of the shadow ray we take the opposite of the light direction
                    shadow_ray.set_dir(-light_direction);

                    if (shadow_ray.trace(objects, dummy, ri) && dummy.tn < light_dist) {
                        visibility = 0.f;
                    }

                    // dot product based on Lambert's cosine law for Lambertian reflectance;
                    lamb_refl = glm::dot(ii.ipn, -light_direction);

                    // calculate diffuse component
                    diffuse += visibility * (mat.c * light_intensity * glm::max(0.f, lamb_refl));

                    // calculate specular component
                    glm::vec4 light_reflection = glm::normalize(2.f * lamb_refl * ii.ipn + light_direction);
                    float_t max_lf_vd = glm::max(0.f, glm::dot(light_reflection, -ray.dir()));
                    float_t pow_max_se = glm::pow(max_lf_vd, mat.se);

                    specular += visibility * (light_intensity * pow_max_se);
                }

                // add ambient, diffuse and specular to the the hit color
                hc += mat.ac * mat.c + mat.dc * diffuse + mat.sc * specular;
                break;
            }

            case rm : {
                glm::vec4 rv = reflect(ray.dir(), ii.ipn);

                // create a reflection ray
                Ray rr;
                rr.rt = reflection;
                rr.set_dir(rv);
                rr.set_orig(ii.ip + ii.ipn*bias);

                hc += mat.ri * cast_ray(rr, lights, objects, depth+1, ri);
                break;
            }
        }
    }

    return hc;
}


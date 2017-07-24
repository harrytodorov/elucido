//
// Created by Haralambi Todorov on 18/05/2017.
//
#include "Camera.h"

glm::vec3 Camera::cast_ray(const Ray &ray, const std::vector<Light *> &lights, const std::vector<Object *> &objects, const uint32_t &depth, render_info &ri) {
    // if we've reached the bottom of the recursion tree, we return the background color
    if (depth > max_depth) return bgc;

    // hit color
    glm::vec3 hc(bgc);

    // intersection information
    isect_info ii;

    // trace a ray through the scene
    if (ray.trace(objects, ii, ri)) {

        // get the surface properties of the intersection
        ii.ho->get_surface_properties(ii);

        // material of the intersected object
        material mat = ii.ho->om;

        // if we have an intersection we set the background color to 0 for all components; black
        hc = black;

        //  for reflective materials
        if (mat.mt == prm || mat.mt == rm) {
            glm::vec4 reflection_vec = glm::normalize(reflect(ray.dir(), ii.ipn));

            // create a reflection ray
            Ray rr;
            rr.rt = reflection;
            rr.set_orig(ii.ip + ii.ipn*bias);
            rr.set_dir(reflection_vec);

            hc += mat.ri * cast_ray(rr, lights, objects, depth+1, ri);
        }

        // for refractive materials
        if (mat.mt == rrm || mat.mt == prrm) {
            glm::vec4 sn{ii.ipn}, refrcation_vec;
            bool outside;

            // cos theta_incident; needed to project incident direction vector on the surface normal
            float_t cosi = glm::clamp(glm::dot(ray.dir(), sn), -1.f, 1.f);
            float_t ior1{1.f}, ior2{mat.ior};

            outside = cosi < 0.f;
            if (outside) {cosi = -cosi;} else {std::swap(ior1, ior2); sn = -sn;}

            // ratio of the indices of refraction of the 2 medias
            float_t iorr{ior1 / ior2};

            // sin^2 theta_incidence
            float_t sin2t = iorr*iorr * (1.f - cosi*cosi);

            // if sin^2 theta_transmission > 1, we have total internal reflection
            if (sin2t > 1.f) std::cout << "bad!" << std::endl;

            float_t cost = glm::sqrt(1.f - sin2t);

            refrcation_vec = glm::normalize(iorr * ray.dir() + sn*(iorr*cosi - cost));

            // create a refraction ray
            Ray rr;
            rr.rt = refraction;
            rr.set_orig(outside ? ii.ip - ii.ipn*bias : ii.ip + ii.ipn*bias);
            rr.set_dir(refrcation_vec);

            hc += cast_ray(rr, lights, objects, depth+1, ri);
        }

        // for materials with Phong Illumination model
        if (mat.mt == pm || mat.mt == prm || mat.mt == prrm) {
            Ray shadow_ray;
            isect_info dummy;

            // holders for diffuse & specular values;
            glm::vec3 diffuse(0), specular(0);
            float_t lamb_refl(0);

            // iterate through all light sources and calculate specular and defuse components
            for (auto &light : lights) {
                glm::vec4 light_direction(0);
                glm::vec3 light_intensity(0);
                float_t light_dist = infinity;
                float_t visibility(1.f);

                light->illuminate(ii.ip, light_direction, light_intensity, light_dist);

                // compute if the surface point is in shadow
                shadow_ray.rt = shadow;

                // hit_normal * bias is used to translate the origin point by a slightly bit
                // set_orig one could avoid self-shadows, because of float number precision
                shadow_ray.set_orig(ii.ip + ii.ipn*bias);

                // for the direction of the shadow ray we take the opposite of the light direction
                shadow_ray.set_dir(-light_direction);

                // refractive materials do not cast shadows.. later they'll cast caustic effects
                if ((shadow_ray.trace(objects, dummy, ri) && dummy.ho != ii.ho) && dummy.tn < light_dist && dummy.ho->om.mt != rrm) {
                    visibility = 0.f;
                    continue;
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
        }
    }

    return hc;
}

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

void Camera::apply_inverse_view_transform(const std::vector<Object *> &objects, const std::vector<Light *> &lights) {
    // get the inverse camera transformation matrix
    glm::mat4 ictm = inverse_ctm();

    // apply the inverse camera's transformation matrix to all objects
    // and light sources in the scene
    for (auto& object : objects) {
        object->apply_camera_transformation(ictm, tictm);
    }
    for (auto& light : lights) {
        light->apply_camera_transformation(ictm, tictm);
    }
}

void Camera::reverse_inverse_view_transform(const std::vector<Object *> &objects, const std::vector<Light *> &lights) {
    glm::mat4 itictm = inverse_tictm();

    // after rendering reverse all objects and light sources to
    // their original positions;
    // use the camera transformation matrix to
    // bring them to their original positions
    for (auto& object : objects) {
        object->apply_camera_transformation(ctm, itictm);
    }
    for (auto& light : lights) {
        light->apply_camera_transformation(ctm, itictm);
    }
}
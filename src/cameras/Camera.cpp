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

        // for materials with Phong Illumination model
        if (mat.mt == pm) {
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
                // ignore self-shadows; those would be handled correctly later
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
            hc += mat.ac*mat.c + mat.dc*diffuse + mat.sc*specular;
        }

        //  for reflective materials
        if (mat.mt == rm) {
            glm::vec4 reflection_vec = glm::normalize(reflect(ray.dir(), ii.ipn));

            // create a reflection ray
            Ray rr;
            rr.rt = reflection;
            rr.set_orig(ii.ip + ii.ipn*bias);
            rr.set_dir(reflection_vec);

            hc += mat.ri * cast_ray(rr, lights, objects, depth+1, ri);

            // compute a specular reflection for reflective materials
            glm::vec3 specular(0);

            // iterate through all light sources and calculate specular and defuse components
            for (auto &light : lights) {
                glm::vec4 light_direction(0);
                glm::vec3 light_intensity(0);
                float_t light_dist = infinity, lamb_refl(0);

                light->illuminate(ii.ip, light_direction, light_intensity, light_dist);

                // dot product based on Lambert's cosine law for Lambertian reflectance;
                lamb_refl = glm::dot(ii.ipn, -light_direction);

                // calculate specular component
                glm::vec4 light_reflection = glm::normalize(2.f * lamb_refl * ii.ipn + light_direction);
                float_t max_lf_vd = glm::max(0.f, glm::dot(light_reflection, -ray.dir()));
                float_t pow_max_se = glm::pow(max_lf_vd, mat.se);

                specular += light_intensity * pow_max_se;
            }

            hc += mat.sc*specular;
        }

        // for refractive materials
        if (mat.mt == rrm) {
            Ray rr;
            glm::vec3 reflection_col(0), refraction_col(0);
            glm::vec4 direction;
            float_t reflectance;

            // evaluate the reflectance-refraction ratio using Fresnel equations
            compute_fresnel(ray.dir(), ii.ipn, mat.ior, reflectance);

            // is the ray hitting the object from the inside or the outside
            bool outside = glm::dot(ray.dir(), ii.ipn) < 0.f;

            // compute refraction if it's not the case of total internal reflection
            if (reflectance < 1.f) {
                rr.rt = refraction;
                rr.set_orig(outside ? ii.ip - bias*ii.ipn : ii.ip + bias*ii.ipn);

                // calculate and set direction for the refraction
                direction = refract(ray.dir(), ii.ipn, mat.ior);
                rr.set_dir(direction);

                refraction_col = cast_ray(rr, lights, objects, depth+1, ri);
            }

            // compute the reflection
            rr.rt = reflection;
            rr.set_orig(outside ? ii.ip + bias*ii.ipn : ii.ip - bias*ii.ipn);

            // calculate and set direction for the reflection
            direction = reflect(ray.dir(), ii.ipn);
            rr.set_dir(direction);

            reflection_col = cast_ray(rr, lights, objects, depth+1, ri);

            // mix reflection & refraction according to Fresnel
            hc += reflectance*reflection_col + (1.f-reflectance)*refraction_col;
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

glm::vec4 Camera::refract(const glm::vec4 &incident_direction, const glm::vec4 &surface_normal, const float_t &ior) {
    glm::vec4 sn{surface_normal};

    // cos theta_incident; needed to project incident direction vector on the surface normal
    float_t cosi = glm::clamp(glm::dot(incident_direction, sn), -1.f, 1.f);

    // we suppose that rays travel in air at standard temperature and pressure (STP)
    float_t ior1{1.00029f}, ior2{ior};

    // if we have negative cos theta_incident (ray travels from medium with ior1 to medium with ior2 coming), we take it absolute value otherwise
    // ray travels from medium with ior2 to medium with ior1, we swap the two iors and reverse the sign of the surface normal
    if (cosi < 0.f) {cosi = -cosi;} else {std::swap(ior1, ior2); sn = -sn;}

    // evaluate ratio of the indices of refraction of the 2 medias
    float_t iorr{ior1 / ior2};

    // evaluate sin^2 theta_transmission
    float_t sin2t = iorr*iorr * (1.f - cosi*cosi);

    // evaluate cos theta_transmission
    float_t cost = glm::sqrt(1.f - sin2t);

    // return 0 vector in case of total internal reflection
    return sin2t < 0 ? glm::vec4(0) : glm::normalize(iorr * incident_direction + sn*(iorr*cosi - cost));
}

void Camera::compute_fresnel(const glm::vec4 &incident_direction, const glm::vec4 &surface_normal, const float_t &ior,
                             float_t &reflectance) {
    // cos theta_incident; needed to project incident direction vector on the surface normal
    float_t cosi = glm::clamp(glm::dot(incident_direction, surface_normal), -1.f, 1.f);

    // we suppose that rays travel in air at standard temperature and pressure (STP)
    float_t ior1{1.00029f}, ior2{ior};

    // ray travels from medium with ior2 to medium with ior1, we swap the two iors
    if (cosi > 0.f) {std::swap(ior1, ior2);}

    // evaluate sin theta_transmission
    float_t sint = (ior1/ior2) * glm::sqrt(glm::max(0.f, 1.f - cosi*cosi));

    // in case of total internal reflection (TIR) we care about just the reflection
    if (sint >= 1.f) {
        reflectance = 1.f;
    } else {
        // compute the reflectance in case we don't have TIR

        // evaluate cos theta_transmission
        float_t cost = glm::sqrt(glm::max(0.f, 1.f - sint*sint));

        // we need positive values for cos theta_incidence
        cosi < 0.f ? cosi = -cosi : cosi;

        // compute the orthogonal part of Fresnel equations
        float_t fo = (ior1*cosi - ior2*cost) / (ior1*cosi + ior2*cost);

        // computer parallel part of the Fresnel equations
        float_t fp = (ior2*cosi - ior1*cost) / (ior2*cosi + ior1*cost);

        reflectance = (fo*fo + fp*fp) / 2.f;
    }
}

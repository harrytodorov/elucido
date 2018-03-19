// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Camera.h"

//==============================================================================
glm::vec3 Camera::cast_ray(const Ray &ray,
                           const std::vector<std::shared_ptr<Light>> &lights,
                           const std::vector<std::shared_ptr<Object>> &objects,
                           const uint32_t &depth,
                           render_info &ri) {
  // if we've reached the bottom of the recursion tree, we return the background col
  if (depth > max_depth) return bgc;

  // hit col
  glm::vec3 hc(bgc);

  // intersection information
  isect_info ii;

  // increment number of primary rays
  if (ray.rt == primary) __sync_fetch_and_add(&ri.npr, 1);

  // check if a ray intersects the scene's bounding box and if so
  // trace it through the scene
  if (scene_bb.intersect(ray) && ray.trace(objects, ii, ri)) {

    // material of the intersected object
    material mat = ii.ho->material();

    // if we have an intersection we set the background col to 0 for all components; black
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
        glm::vec4 light_direction = light->get_direction(ii.ip);
        float_t light_distance = light->get_distance(ii.ip);
        glm::vec3 light_intensity = light->color() * light->get_intensity(light_distance);

        float_t visibility(1.f);

        // compute if the surface point is in shadow
        shadow_ray.rt = shadow;

        // hit_normal * bias is used to translate the origin point by a slightly bit
        // set_orig one could avoid self-shadows, because of float number precision
        shadow_ray.set_orig(ii.ip + ii.ipn * bias);

        // for the direction of the shadow ray we take the opposite of the light direction
        shadow_ray.set_dir(light_direction);

        // ignore self-shadows; those would be handled correctly later
        if (shadow_ray.trace(objects, dummy, ri)
            && dummy.tn < light_distance && dummy.ho != ii.ho) {
          visibility = 0.f;
          continue;
        }

        // dot product based on Lambert's cosine law for Lambertian reflectance;
        lamb_refl = glm::dot(ii.ipn, light_direction);

        // calculate diffuse component
        diffuse = mat.c * light_intensity * glm::max(0.f, lamb_refl);

        // calculate specular component
        glm::vec4 light_reflection =
            glm::normalize(2.f * lamb_refl * ii.ipn - light_direction);
        float_t
            max_lf_vd = glm::max(0.f, glm::dot(light_reflection, -ray.dir()));
        float_t pow_max_se = glm::pow(max_lf_vd, mat.se);

        specular = light_intensity * pow_max_se;

        // add ambient, diffuse and specular to the the hit col
        hc += visibility
            * (mat.ac * mat.c + mat.dc * diffuse + mat.sc * specular);
      }
    }

    //  for reflective materials
    if (mat.mt == rm) {
      glm::vec4 reflection_vec = glm::normalize(reflect(ray.dir(), ii.ipn));

      // create a reflection ray
      Ray rr;
      rr.rt = reflection;
      rr.set_orig(ii.ip + ii.ipn * bias);
      rr.set_dir(reflection_vec);

      hc += mat.ri * cast_ray(rr, lights, objects, depth + 1, ri);

      // compute a specular reflection for reflective materials
      glm::vec3 specular(0);

      // iterate through all light sources and calculate specular
      for (auto &light : lights) {
        glm::vec4 light_direction = light->get_direction(ii.ip);
        float_t light_distance = light->get_distance(ii.ip);
        glm::vec3 light_intensity = light->color() * light->get_intensity(light_distance);
        float_t lamb_refl(0);

        // dot product based on Lambert's cosine law for Lambertian reflectance;
        lamb_refl = glm::dot(ii.ipn, light_direction);

        // calculate specular component
        glm::vec4 light_reflection =
            glm::normalize(2.f * lamb_refl * ii.ipn - light_direction);
        float_t
            max_lf_vd = glm::max(0.f, glm::dot(light_reflection, -ray.dir()));
        float_t pow_max_se = glm::pow(max_lf_vd, mat.se);

        specular += light_intensity * pow_max_se;
      }

      // add specular highlight to the material
      hc += mat.sc * specular;

      // add col to the reflective material
      hc = hc * mat.c;
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
        rr.set_orig(outside ? ii.ip - bias * ii.ipn : ii.ip + bias * ii.ipn);

        // calculate and set direction for the refraction
        direction = refract(ray.dir(), ii.ipn, mat.ior);
        rr.set_dir(direction);

        refraction_col = cast_ray(rr, lights, objects, depth + 1, ri);
      }

      // compute the reflection
      rr.rt = reflection;
      rr.set_orig(outside ? ii.ip + bias * ii.ipn : ii.ip - bias * ii.ipn);

      // calculate and set direction for the reflection
      direction = reflect(ray.dir(), ii.ipn);
      rr.set_dir(direction);

      reflection_col = cast_ray(rr, lights, objects, depth + 1, ri);

      // mix reflection & refraction according to Fresnel + add col of the object
      hc +=
          (reflectance * reflection_col + (1.f - reflectance) * refraction_col)
              * mat.c;
    }
  }

  return hc;
}

//==============================================================================
glm::vec3 Camera::cast_ray(const Ray &ray,
                           const std::vector<std::shared_ptr<Light>> &lights,
                           const std::vector<std::shared_ptr<Object>> &objects,
                   const uint32_t &depth,
                   const AccelerationStructure *structure,
                   render_info &ri) {
  if (depth > max_depth) return bgc;

  glm::vec3 hc(bgc);

  // intersection information
  isect_info ii;

  // increment number of primary rays
  if (ray.rt == primary) __sync_fetch_and_add(&ri.npr, 1);

  // check if a ray intersects the scene's bounding box and if so
  // trace it through the scene
  if (structure->intersect(ray, ii)) {

    // material of the intersected object
    material mat = ii.ho->material();

    // if we have an intersection we set the background col to 0 for all components; black
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
        glm::vec4 light_direction = light->get_direction(ii.ip);
        float_t light_distance = light->get_distance(ii.ip);
        glm::vec3 light_intensity = light->color() * light->get_intensity(light_distance);
        float_t visibility(1.f);

        // compute if the surface point is in shadow
        shadow_ray.rt = shadow;

        // hit_normal * bias is used to translate the origin point by a slightly bit
        // set_orig one could avoid self-shadows, because of float number precision
        shadow_ray.set_orig(ii.ip + ii.ipn * bias);

        // for the direction of the shadow ray we take the opposite of the light direction
        shadow_ray.set_dir(light_direction);

        // ignore self-shadows; those would be handled correctly later
        if (structure->intersect(shadow_ray, dummy) &&
            dummy.tn < light_distance && dummy.ho != ii.ho) {
          visibility = 0.f;
          continue;
        }

        // dot product based on Lambert's cosine law for Lambertian reflectance;
        lamb_refl = glm::dot(ii.ipn, light_direction);

        // calculate diffuse component
        diffuse = mat.c * light_intensity * glm::max(0.f, lamb_refl);

        // calculate specular component
        glm::vec4 light_reflection =
            glm::normalize(2.f * lamb_refl * ii.ipn - light_direction);
        float_t
            max_lf_vd = glm::max(0.f, glm::dot(light_reflection, -ray.dir()));
        float_t pow_max_se = glm::pow(max_lf_vd, mat.se);

        specular = light_intensity * pow_max_se;

        // add ambient, diffuse and specular to the the hit col
        hc += visibility
            * (mat.ac * mat.c + mat.dc * diffuse + mat.sc * specular);
      }
    }

    //  for reflective materials
    if (mat.mt == rm) {
      glm::vec4 reflection_vec = glm::normalize(reflect(ray.dir(), ii.ipn));

      // create a reflection ray
      Ray rr;
      rr.rt = reflection;
      rr.set_orig(ii.ip + ii.ipn * bias);
      rr.set_dir(reflection_vec);

      hc += mat.ri * cast_ray(rr, lights, objects, depth + 1, structure, ri);

      // compute a specular reflection for reflective materials
      glm::vec3 specular(0);

      // iterate through all light sources and calculate specular
      for (auto &light : lights) {
        glm::vec4 light_direction = light->get_direction(ii.ip);
        float_t light_distance = light->get_distance(ii.ip);
        glm::vec3 light_intensity = light->color() * light->get_intensity(light_distance);
        float_t lamb_refl(0);

        // dot product based on Lambert's cosine law for Lambertian reflectance;
        lamb_refl = glm::dot(ii.ipn, light_direction);

        // calculate specular component
        glm::vec4 light_reflection =
            glm::normalize(2.f * lamb_refl * ii.ipn - light_direction);
        float_t
            max_lf_vd = glm::max(0.f, glm::dot(light_reflection, -ray.dir()));
        float_t pow_max_se = glm::pow(max_lf_vd, mat.se);

        specular += light_intensity * pow_max_se;
      }

      // add specular highlight to the material
      hc += mat.sc * specular;

      // add col to the reflective material
      hc = hc * mat.c;
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
        rr.set_orig(outside ? ii.ip - bias * ii.ipn : ii.ip + bias * ii.ipn);

        // calculate and set direction for the refraction
        direction = refract(ray.dir(), ii.ipn, mat.ior);
        rr.set_dir(direction);

        refraction_col = cast_ray(rr, lights, objects, depth + 1, structure, ri);
      }

      // compute the reflection
      rr.rt = reflection;
      rr.set_orig(outside ? ii.ip + bias * ii.ipn : ii.ip - bias * ii.ipn);

      // calculate and set direction for the reflection
      direction = reflect(ray.dir(), ii.ipn);
      rr.set_dir(direction);

      reflection_col = cast_ray(rr, lights, objects, depth + 1, structure, ri);

      // mix reflection & refraction according to Fresnel + add col of the object
      hc +=
          (reflectance * reflection_col + (1.f - reflectance) * refraction_col)
              * mat.c;
    }
  }

  return hc;
}

//==============================================================================
void Camera::rotate(const float_t &rot_angle, const Axis &rotation_axis) {
  apply_rotation(rotation_axis, rot_angle, vm);
}

//==============================================================================
void Camera::translate(const float_t &translation,
                       const Axis &translation_axis) {
  apply_translation(translation_axis, translation, vm);
}

//==============================================================================
void Camera::apply_inverse_view_transform(const std::vector<std::shared_ptr<Object>> &objects,
                                          const std::vector<std::shared_ptr<Light>> &lights) {
  glm::mat4 ivm = glm::inverse(vm);
  for (auto const &object : objects) {
    object->apply_camera_transformation(ivm);
  }
  for (auto const &light : lights) {
    light->apply_camera_transformation(ivm);
  }
}

//==============================================================================
void Camera::reverse_inverse_view_transform(const std::vector<std::shared_ptr<Object>> &objects,
                                            const std::vector<std::shared_ptr<Light>> &lights) {
  for (auto const &object : objects) {
    object->apply_camera_transformation(vm);
  }
  for (auto const &light : lights) {
    light->apply_camera_transformation(vm);
  }
}

//==============================================================================
glm::vec4 Camera::refract(const glm::vec4 &incident_direction,
                          const glm::vec4 &surface_normal,
                          const float_t &ior) {
  glm::vec4 sn{surface_normal};

  // cos theta_incident; needed to project incident direction vector on the surface normal
  float_t cosi = glm::clamp(glm::dot(incident_direction, sn), -1.f, 1.f);

  // we suppose that rays travel in air at standard temperature and pressure (STP)
  float_t ior1{1.00029f}, ior2{ior};

  // if we have negative cos theta_incident (ray travels from medium with ior1 to medium with ior2 coming), we take it absolute value otherwise
  // ray travels from medium with ior2 to medium with ior1, we swap the two iors and reverse the sign of the surface normal
  if (cosi < 0.f) { cosi = -cosi; }
  else {
    std::swap(ior1, ior2);
    sn = -sn;
  }

  // evaluate ratio of the indices of refraction of the 2 medias
  float_t iorr{ior1 / ior2};

  // evaluate sin^2 theta_transmission
  float_t sin2t = iorr * iorr * (1.f - cosi * cosi);

  // evaluate cos theta_transmission
  float_t cost = glm::sqrt(1.f - sin2t);

  // return 0 vector in case of total internal reflection
  return sin2t < 0 ? glm::vec4(0) : glm::normalize(
      iorr * incident_direction + sn * (iorr * cosi - cost));
}

//==============================================================================
void Camera::compute_fresnel(const glm::vec4 &incident_direction,
                             const glm::vec4 &surface_normal,
                             const float_t &ior,
                             float_t &reflectance) {
  // cos theta_incident; needed to project incident direction vector on the surface normal
  float_t cosi =
      glm::clamp(glm::dot(incident_direction, surface_normal), -1.f, 1.f);

  // we suppose that rays travel in air at standard temperature and pressure (STP)
  float_t ior1{1.00029f}, ior2{ior};

  // ray travels from medium with ior2 to medium with ior1, we swap the two iors
  if (cosi > 0.f) { std::swap(ior1, ior2); }

  // evaluate sin theta_transmission
  float_t sint = (ior1 / ior2) * glm::sqrt(glm::max(0.f, 1.f - cosi * cosi));

  // in case of total internal reflection (TIR) we care about just the reflection
  if (sint >= 1.f) {
    reflectance = 1.f;
  } else {
    // compute the reflectance in case we don't have TIR

    // evaluate cos theta_transmission
    float_t cost = glm::sqrt(glm::max(0.f, 1.f - sint * sint));

    // we need positive values for cos theta_incidence
    cosi < 0.f ? cosi = -cosi : cosi;

    // compute the orthogonal part of Fresnel equations
    float_t fo = (ior1 * cosi - ior2 * cost) / (ior1 * cosi + ior2 * cost);

    // computer parallel part of the Fresnel equations
    float_t fp = (ior2 * cosi - ior1 * cost) / (ior2 * cosi + ior1 * cost);

    reflectance = (fo * fo + fp * fp) / 2.f;
  }
}

//==============================================================================
void Camera::extend_scene_bb(const std::vector<std::shared_ptr<Object>> &objects) {
  for (auto &object : objects) {
    scene_bb.extend_by(object->bounding_box().bounds[0]);
    scene_bb.extend_by(object->bounding_box().bounds[1]);
  }
}

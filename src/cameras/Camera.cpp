// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Camera.h"
#include "../core/Common.h"


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

// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "PointLight.h"

//=============================================================================
void PointLight::illuminate(const glm::vec4 &hit_point,
                            glm::vec4 &light_dir,
                            glm::vec3 &light_intensity,
                            float_t &distance) {
  float_t sq_dist;

  // compute the light direction
  light_dir = hit_point - p;

  // compute the distance between the light source and the hit point
  sq_dist = glm::dot(light_dir, light_dir);
  distance = glm::sqrt(sq_dist);

  // now we can safely normalize the light direction vector
  light_dir = glm::normalize(light_dir);

  // for computing the light intensity use the inverse square law
  light_intensity = intensity * color / (float_t) (4.f * M_PI * sq_dist);
}

//=============================================================================
void PointLight::apply_camera_transformation(const glm::mat4 &ictm,
                                             const glm::mat4 &itctm) {
  p = ictm * p;
}

//=============================================================================
void PointLight::translate(const float_t &translation,
                           const Axis &axes_of_translation) {
  glm::vec3 tv = create_transformation_vector(axes_of_translation, translation);

  // assign the translation matrix to object's model transform
  glm::mat4 tm = glm::translate(glm::mat4(1), tv);
  mt = tm * mt;
}

//=============================================================================
void PointLight::rotate(const float_t &angle_of_rotation,
                        const Axis &axes_of_rotation) {
  glm::vec3 rv = create_transformation_vector(axes_of_rotation,
                                              angle_of_rotation);

  // assign the rotation matrix to object's model transform
  glm::mat4 rm = glm::rotate(glm::mat4(1), glm::radians(angle_of_rotation), rv);
  mt = rm * mt;
}

//=============================================================================
void PointLight::apply_transformations() {
  // apply the transformations stored in the light's model transorm matrix to its position
  p = mt * p;

  // after applying the transformations to a point light; its model transform matrix is set back to the identity matrix
  mt = glm::mat4(1);
}

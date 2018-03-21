// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_COMMON_H
#define ELUCIDO_COMMON_H

#include <cmath>
#include "glm/vec3.hpp"

#include "Utilities.h"
#include "Ray.h"

/**
 * Create a transformation vector.
 * @param transformation_axes:   Axes along which a transformation would be
  *                              applied.
 * @param transformation_amount: The amount to be transformed along these axes.
 * @return:                      The transformation vector.
 *                               In case an invalid axes of transformation is
 *                               provided, a vector of 1s is returned.
 */
glm::vec3 create_transformation_vector(const Axis &transformation_axes,
                                       const float_t &transformation_amount);

/**
 * Apply a rotation around a given axis by an angle on the model transform
 * matrix.
 * @param axis:             Axis around which it would be rotated.
 * @param rotation_angle:   The angle of the rotation.
 * @param model_transform:  The model transform matrix onto which the
 *                          rotation would be applied.
 */
void apply_rotation(const Axis &axis,
                    const float_t &rotation_angle,
                    glm::mat4 &model_transform);

/**
 * Apply a translation around a given axis by a translation amount on the
 * model transform matrix.
 * @param axis:                 Axis in which the translation would be
 *                              applied.
 * @param translation_amount:   The amount of the translation.
 * @param model_transform:      The model transform matrix onto which the
 *                              translation would be applied.
 */
void apply_translation(const Axis &axis,
                       const float_t &translation_amount,
                       glm::mat4 &model_transform);

/**
 * Apply scaling on a given axis by a scale amount on the model transform
 * matrix.
 * @param axis:             Axis in which the scale would be applied.
 * @param scale_amount:     The amount of the scale.
 * @param model_transform:  The model transform matrix onto which the scale
 *                          would be applied.
 */
void apply_scale(const Axis &axis,
                 const float_t &scale_amount,
                 glm::mat4 &model_transform);

/**
 * Determine if a ray intersects a triangle using the Moeller-Trumbore
 * ray/triangle intersection algorithm for double faced triangles.
 * The function returns the distance (t) from the ray's origin to the
 * intersection point, the barycentric coordinates (u, v) and whether
 * the normal should be flipped or not (in case the found intersection point
 * was on the back of the triangle).
 * @param r:  The ray with which the triangle would be intersected.
 * @param v0: Vertex 0.
 * @param v1: Vertex 1.
 * @param v2: Vertex 2.
 * @param t:  Distance from the intersection point to the ray's origin.
 * @param u:  Barycentric u-parameter.
 * @param v:  Barycentric v-parameter.
 * @return:   True in case of an intersection, false otherwise.
 *            In case the ray runs parallel to the surface of the
 *            triangle, false is returned.
 */
bool triangle_intersect(const Ray &r,
                        const glm::vec4 &v0,
                        const glm::vec4 &v1,
                        const glm::vec4 &v2,
                        float_t &t,
                        float_t &u,
                        float_t &v,
                        bool &flip_normal);

/**
 * Same algorithm as for the normal intersection is used for the shadow
 * intersection routine.
 * @param r:  The ray with which the triangle would be intersected.
 * @param v0: Vertex 0.
 * @param v1: Vertex 1.
 * @param v2: Vertex 2.
 * @return:   True in case of an intersection, false otherwise.
 *            In case the ray runs parallel to the surface of the
 *            triangle, false is returned.
 */
bool triangle_shadow_intersect(const Ray &r,
                               const glm::vec4 &v0,
                               const glm::vec4 &v1,
                               const glm::vec4 &v2,
                               const float_t &light_distance);
#endif //ELUCIDO_COMMON_H

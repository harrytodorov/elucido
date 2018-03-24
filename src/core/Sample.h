// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_SAMPLE_H
#define ELUCIDO_SAMPLE_H

#include <vector>
#include <cstdint>
#include <glm/vec2.hpp>

#include "Utilities.h"

/**
 * Finds the nearest perfect square of a number n.
 * The square is rounded down.
 * E.g. The result for 7 would be 2 (4 = 2^2).
 * @param n:    A positive integer.
 * @return:     The nearest perfect square (floored) of the number n.
 */
uint32_t find_nearest_perfect_sqr(const uint32_t &n);

/**
 * Appends n (number_of_samples) 2D random sample positions in the range
 * [0, 1)x[0,1) to the provided sample_positions vector.
 * The standard C++ "minstd_rand" random number generator is used to generate
 * uniformly distributed real (float_t) numbers.
 * All elements of the vector are cleared before.
 * @param number_of_samples:    Number of samples to be generated.
 * @param sample_positions:     Vector of sampled positions to which the
 *                              newly generated samples would be appended.
 */
void generate_random_samples(const uint32_t &number_of_samples,
                             std::vector<glm::vec2> &sample_positions);

/**
 * Appends floor(sqrt(n))^2 2D stratified jittered sample positions to
 * the provided sample_positions vector.
 * Sample positions are within the unit square [0,1)x[0,1).
 * @param number_of_samples:    Number of samples to be generated.
 * @param sample_positions:     Vector of sampled positions to which the newly
 *                              generated samples would be appended.
 */
void generate_half_jittered_samples(const uint32_t &number_of_samples,
                                    std::vector<glm::vec2> &sample_positions);

/**
 * Appends n (number_of_samples) n-Rook (shuffled along x-direction) 2D sample
 * positions to the provided sample_positions vector.
 * Sample positions are within the unit square [0,1)x[0,1).
 * @param number_of_samples:    Number of samples to be generated.
 * @param sample_positions:     Vector of sample positions to which the newly
 *                              generated samples would be appended.
 */
void generate_nrooks_samples(const uint32_t &number_of_samples,
                             std::vector<glm::vec2> &sample_positions);

/**
 * Appends floor(sqrt(n))^2 2D multi-jittered sample positions to the
 * provided sample_positions vector.
 * Sample positions are within the unit square [0,1)x[0,1).
 * @param number_of_samples:    Number of samples to be generated.
 * @param sample_positions:     Vector of sample positions to which the
 *                              newly generated samples would be appended.
 */
void generate_multi_jittered_samples(const uint32_t &number_of_samples,
                                     std::vector<glm::vec2> &sample_positions);

/**
 * Calculate the weighted average radiance for a given pixel and a set of
 * samples (radiance and position) using the Box filter over the defined extent.
 *
 * pixel_radiance = 1/num_samples * sum_samples(sample.radiance)
 *  ~ for samples within the given extent.
 *
 * @param samples:  Vector containing a set of evaluated samples.
 * @param x:        The pixel's x position.
 * @param y:        The pixel's y position.
 * @param extent:   The extent of the Box filter.
 * @return:         The weighted average radiance at the pixel.
 */
glm::vec3 evaluate_box_filter(const std::vector<ip_sample> &samples,
                              const uint32_t &x,
                              const uint32_t &y,
                              const float_t &extent);

/**
 * Calculate the weighted average radiance for a given pixel and a set of samples
 * (radiance and position) using the Triangle filter over the defined extent.
 *
 * pixel_radiance = 1/sum_samples(sample.weight) *  sample.weight *
 *                                                  sum_samples(sample.radiance)
 *  ~ for samples within the given extent.
 *
 * @param samples:  Vector containing a set of evaluated samples.
 * @param x:        The pixel's x position.
 * @param y:        The pixel's y position.
 * @param extent:   The extent of the Triangle filter.
 * @return:         TThe weighted average radiance at the pixel.
 */
glm::vec3 evaluate_triangle_filter(const std::vector<ip_sample> &samples,
                                   const uint32_t &x,
                                   const uint32_t &y,
                                   const float_t &extent);

/**
 * TODO: find a good default alpha parameter for the Gaussian.
 * Calculate the weighted average radiance for a given pixel and a set of samples
 * (radiance and position) using the Gaussian filter over the defined extent.
 *
 * pixel_radiance = 1/sum_samples(sample.weight) *  sample.weight *
 *                                                  sum_samples(sample.radiance)
 *  ~ for samples within the given extent.
 *
 * @param samples:  Vector containing a set of evaluated samples.
 * @param x:        The pixel's x position.
 * @param y:        The pixel's y position.
 * @param extent:   The extent of the Gaussian filter.
 * @param alpha:    The alpha parameter of the Gaussian filter.
 * @return:         The weighted average radiance at the pixel.
 */
glm::vec3 evaluate_gaussian_filter(const std::vector<ip_sample> &samples,
                                   const uint32_t &x,
                                   const uint32_t &y,
                                   const float_t &extent,
                                   const float_t &alpha);

/**
 * Calculate the weighted average radiance for a given pixel and a set of samples
 * (radiance and position) using the Mitchell filter.
 * The filter function is defined over the extent (-2,2)x(-2,2). Samples
 * outside the given extent would be given a weight of 0.
 * B and C are filter parameters, which according to authors should satisfy
 * following property B + 2C = 1.
 * The authors also proposed B=1/3 and C=1/3 as values giving satisfactory
 * results.
 * @param samples:  Vector containing a set of evaluated samples.
 * @param x:        The pixel's x position.
 * @param y:        The pixel's y position.
 * @param extent:   The extent of the Mitchell filter. Is not needed (see above)
 * @param b:        The B parameter of the Mitchell filter.
 * @param c:        The C parameter of the Mitchell filter.
 * @return:         The weighted average radiance at the pixel.
 */
glm::vec3 evaluate_mitchell_filter(const std::vector<ip_sample> &samples,
                                   const uint32_t &x,
                                   const uint32_t &y,
                                   const float_t &extent,
                                   const float_t &b = 1.f / 3.f,
                                   const float_t &c = 1.f / 3.f);

/**
 * ТОДО: find good default tau parameter.
 *
 * Calculate the weighted average radiance for a given pixel and a set of samples
 * (radiance and position) using a filter based on the Sinc function with
 * a Lanczos window.
 * The Sinc function is defined over the given extent. Samples outside of the
 * extent would be given weight of 0.
 * @param samples:  Vector containing a set of evaluated samples.
 * @param x:        The pixel's x position.
 * @param y:        The pixel's y position.
 * @param extent:   The extent of the windowed Sinc filter.
 * @param tau:      The tau parameter. The number of cycles the Sinc function
 *                  passes before its clamped to 0.
 * @return:         The weighted average radiance at the pixel.
 */
glm::vec3 evaluate_sinc_filter(const std::vector<ip_sample> &samples,
                               const uint32_t &x,
                               const uint32_t &y,
                               const float_t &extent,
                               const float_t &tau);

#endif //ELUCIDO_SAMPLE_H

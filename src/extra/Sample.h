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

// Sampling strategies.
/**
 * Appends n (number_of_samples) 2D positions in the range [0, 1) to the
 * provided sample_positions vector.
 * All elements of the vector are cleared before.
 * @param number_of_samples:    Number of samples to be generated.
 * @param sample_positions:     Vector of sampled positions to which the
 *                              newly generated samples would be appended.
 */
void generate_random_samples(const uint32_t &number_of_samples,
                             std::vector<glm::vec2> &sample_positions);

/**
 *
 * @param number_of_samples
 * @param sample_positions
 */
void generate_half_jittered_samples(const uint32_t &number_of_samples,
                                    std::vector<glm::vec2> &sample_positions);

void generate_nrooks_samples(const uint32_t &number_of_samples,
                             std::vector<glm::vec2> &sample_positions);

// Filtering strategies.
glm::vec3 box_filter(const std::vector<std::shared_ptr<ip_sample>> &samples);
glm::vec3 gaussian_filter(const std::vector<std::shared_ptr<ip_sample>> &samples);
glm::vec3 tent_filter(const std::vector<std::shared_ptr<ip_sample>> &samples);
glm::vec3 cubic_spline_filter(const std::vector<std::shared_ptr<ip_sample>> &samples);

#endif //ELUCIDO_SAMPLE_H

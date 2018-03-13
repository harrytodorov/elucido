// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <random>
#include "Sample.h"

//==============================================================================
uint32_t find_nearest_perfect_sqr(const uint32_t &n) {
  auto new_n = static_cast<uint32_t>(glm::sqrt(static_cast<float_t>(n)));
  return new_n;
}

//==============================================================================
void generate_random_samples(const uint32_t &number_of_samples,
                             std::vector<glm::vec2> &sample_positions) {
  std::random_device rd;
  std::minstd_rand engine(rd());
  std::uniform_real_distribution<float_t> random_x(0.f, 1.f);
  std::uniform_real_distribution<float_t> random_y(0.f, 1.f);

  for (size_t i = 0; i < number_of_samples; i++) {
    sample_positions.emplace_back(random_x(engine), random_y(engine));
  }
}

//==============================================================================
void generate_half_jittered_samples(const uint32_t &number_of_samples,
                                    std::vector<glm::vec2> &sample_positions) {
  std::random_device rd;
  std::minstd_rand engine(rd());
  std::uniform_real_distribution<float_t> random_x(0.25f, 0.75f);
  std::uniform_real_distribution<float_t> random_y(0.25f, 0.75f);

  auto n = find_nearest_perfect_sqr(number_of_samples);
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      sample_positions.emplace_back((i + random_x(engine)) / n,
                                    (j + random_y(engine)) / n);
    }
  }
}

//==============================================================================
void generate_nrooks_samples(const uint32_t &number_of_samples,
                             std::vector<glm::vec2> &sample_positions) {

}
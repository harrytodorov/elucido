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
  std::random_device rd;
  std::minstd_rand engine(rd());
  std::uniform_real_distribution<float_t> random_x(0.f, 1.f);
  std::uniform_real_distribution<float_t> random_y(0.f, 1.f);
  std::uniform_int_distribution<uint32_t> shuffle_x(0, number_of_samples - 1);

  for (size_t i = 0; i < number_of_samples; i++) {
    auto x_pos = shuffle_x(engine);
    sample_positions.emplace_back((x_pos + random_x(engine)) / number_of_samples,
                                  (i + random_y(engine)) / number_of_samples);
  }
}

//==============================================================================
void generate_multi_jittered_samples(const uint32_t &number_of_samples,
                                     std::vector<glm::vec2> &sample_positions) {
  auto n = find_nearest_perfect_sqr(number_of_samples);
  float_t subcell_width = 1.f / (n*n);

  std::random_device rd;
  std::minstd_rand engine(rd());
  std::uniform_real_distribution<float_t> random_x(0.f, subcell_width);
  std::uniform_real_distribution<float_t> random_y(0.f, subcell_width);
  std::uniform_real_distribution<float_t> shuffle(0.f, 1.f);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      auto x = i*n*subcell_width + j*subcell_width + random_x(engine);
      auto y = j*n*subcell_width + i*subcell_width + random_y(engine);

      sample_positions.emplace_back(x, y);
    }
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      // Shuffle x-coordinates within each column.
      size_t  k = j + static_cast<size_t>(shuffle(engine) * (n - j - 1));
      float_t t = sample_positions[i*n + j].x;
      sample_positions[i*n + j].x = sample_positions[i*n + k].x;
      sample_positions[i*n + k].x = t;

      // Shuffle y-coordinates within each row.
      k = j + static_cast<size_t>(shuffle(engine) * (n - j - 1));
      t = sample_positions[j*n + i].y;
      sample_positions[j*n + i].y = sample_positions[k*n + i].y;
      sample_positions[k*n + i].y = t;
    }
  }
}

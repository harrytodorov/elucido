// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <random>
#include <cmath>  // PI
#include "Sample.h"

//==============================================================================
uint32_t find_nearest_perfect_sqr(const uint32_t &n) {
  auto new_n = static_cast<uint32_t>(glm::sqrt(static_cast<float_t>(n)));
  return new_n;
}

//==============================================================================
float_t gaussian1D(const float_t &d,
                   const float_t &exp_width,
                   const float_t &alpha) {
  return glm::max(0.f, glm::exp(-alpha * d*d) - exp_width);
}

//==============================================================================
float_t mitchell1D(const float_t &x, const float_t &b, const float_t &c) {
  auto abs_x = glm::abs(x);
  if (abs_x >= 2.f) return 0.f;

  auto x3         = abs_x*abs_x*abs_x;
  auto x2         = abs_x*abs_x;
  auto inv_factor = 1.f / 6.f;
  // Case |x| < 1.
  if (abs_x < 1.f) {
    return ((12.f - 9.f*b - 6.f*c)*x3 +
           (-18.f + 12.f*b + 6.f*c)*x2 + (6.f - 2.f*b)) * inv_factor;
  // Case 1 <= |x| < 2.
  } else {
    return ((-b - 6.f*c)*x3 + (6.f*b + 30.f*c)*x2 +
           (-12.f*b - 48.f*c)*abs_x + (8.f*b + 24.f*c)) * inv_factor;
  }
}

//==============================================================================
float_t sinc1D(const float_t &x, const float_t &tau, const float_t &extent) {
  auto abs_x = glm::abs(x);
  if (abs_x < 1e-4)   return 1.f;
  if (abs_x > extent) return 0.f;
  abs_x *= M_PI;
  // Sinc function.
  float_t sinc    = glm::sin(abs_x * tau) / (abs_x * tau);
  // Lanczos window over the Sinc function.
  float_t lanczos = glm::sin(abs_x) / abs_x;
  return sinc * lanczos;
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

//==============================================================================
glm::vec3 evaluate_box_filter(const std::vector<ip_sample> &samples,
                              const uint32_t &x,
                              const uint32_t &y,
                              const float_t &extent) {
  auto weighted_radiance  = glm::vec3(0);
  float_t weights_sum = 0.f;
  float_t shifted_x   = x + 0.5f;
  float_t shifted_y   = y + 0.5f;
  float_t width       = extent / 2.f;

  for (auto const &sample : samples) {
    auto diff_x = glm::abs(sample.position.x - shifted_x);
    auto diff_y = glm::abs(sample.position.y - shifted_y);

    if (diff_x <= width && diff_y <= width) {
      weighted_radiance += sample.radiance;
      weights_sum       += 1.f;
    }
  }

  return weighted_radiance / weights_sum;
}

//==============================================================================
glm::vec3 evaluate_triangle_filter(const std::vector<ip_sample> &samples,
                                   const uint32_t &x,
                                   const uint32_t &y,
                                   const float_t &extent) {
  auto weighted_radiance = glm::vec3(0);
  float_t weights_sum = 0.f;
  float_t shifted_x   = x + 0.5f;
  float_t shifted_y   = y + 0.5f;
  float_t width       = extent / 2.f;

  for (auto const &sample : samples) {
    auto diff_x = glm::abs(sample.position.x - shifted_x);
    auto diff_y = glm::abs(sample.position.y - shifted_y);

    if (diff_x <= width && diff_y <= width) {
      auto weight_x = extent - 2*diff_x;
      auto weight_y = extent - 2*diff_y;
      auto weight   = weight_x*weight_y;

      weighted_radiance += weight*sample.radiance;
      weights_sum += weight;
    }
  }

  return weighted_radiance / weights_sum;
}

//==============================================================================
glm::vec3 evaluate_gaussian_filter(const std::vector<ip_sample> &samples,
                                   const uint32_t &x,
                                   const uint32_t &y,
                                   const float_t &extent,
                                   const float_t &alpha) {
  auto weighted_radiance = glm::vec3(0);
  float_t weights_sum = 0.f;
  float_t shifted_x   = x + 0.5f;
  float_t shifted_y   = y + 0.5f;
  float_t width       = extent / 2.f;
  // This value sets the value of the gaussian to 0, when
  // it's outside the extent.
  float_t exp_width   = glm::exp(-alpha * width*width);

  for (auto const &sample : samples) {
    auto diff_x = glm::abs(sample.position.x - shifted_x);
    auto diff_y = glm::abs(sample.position.y - shifted_y);

    auto weight_x = gaussian1D(diff_x, exp_width, alpha);
    auto weight_y = gaussian1D(diff_y, exp_width, alpha);
    auto weight   = weight_x*weight_y;

    weighted_radiance += weight*sample.radiance;
    weights_sum       += weight;
  }

  return weighted_radiance / weights_sum;
}

//==============================================================================
glm::vec3 evaluate_mitchell_filter(const std::vector<ip_sample> &samples,
                                   const uint32_t &x,
                                   const uint32_t &y,
                                   const float_t &extent,
                                   const float_t &b,
                                   const float_t &c) {
  auto weighted_radiance = glm::vec3(0);
  float_t weights_sum = 0.f;
  float_t shifted_x   = x + 0.5f;
  float_t shifted_y   = y + 0.5f;

  for (auto const &sample : samples) {
    auto diff_x = glm::abs(sample.position.x  - shifted_x);
    auto diff_y = glm::abs(sample.position.y  - shifted_y);

    auto weight_x = mitchell1D(diff_x, b, c);
    auto weight_y = mitchell1D(diff_y, b, c);
    auto weight   = weight_x*weight_y;

    weighted_radiance += weight*sample.radiance;
    weights_sum       += weight;
  }

  return weighted_radiance / weights_sum;
}

//==============================================================================
glm::vec3 evaluate_sinc_filter(const std::vector<ip_sample> &samples,
                               const uint32_t &x,
                               const uint32_t &y,
                               const float_t &extent,
                               const float_t &tau) {
  auto weighted_radiance = glm::vec3(0);
  float_t weights_sum = 0.f;
  float_t shifted_x   = x + 0.5f;
  float_t shifted_y   = y + 0.5f;

  for (auto const &sample : samples) {
    auto diff_x = glm::abs(sample.position.x  - shifted_x);
    auto diff_y = glm::abs(sample.position.y  - shifted_y);

    auto weight_x = sinc1D(diff_x, tau, extent);
    auto weight_y = sinc1D(diff_y, tau, extent);
    auto weight   = weight_x*weight_y;

    weighted_radiance += weight*sample.radiance;
    weights_sum       += weight;
  }

  return weighted_radiance / weights_sum;
}

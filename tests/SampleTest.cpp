// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include <glm/vec2.hpp>
#include "glm/ext.hpp"    // glm::to_string

#include "../src/core/Sample.h"

//==============================================================================
TEST(Sample, nearestPerfectSquare) {
  EXPECT_EQ(find_nearest_perfect_sqr(3), 1);
  EXPECT_EQ(find_nearest_perfect_sqr(150), 12);
  EXPECT_EQ(find_nearest_perfect_sqr(2049), 45);
}

//==============================================================================
TEST(Sample, RandomSamples) {
  uint32_t n = 10;
  std::vector<glm::vec2> samples;
  generate_random_samples(n, samples);

  EXPECT_EQ(samples.size(), n);
  for (auto const &sample : samples) {
    EXPECT_TRUE((sample.x >= 0.f) && (sample.x <= 1.f));
    EXPECT_TRUE((sample.y >= 0.f) && (sample.y <= 1.f));
  }
}

//==============================================================================
TEST(Sample, HalfJitteredSamplesPerfectSquare) {
  uint32_t n = 16;
  std::vector<glm::vec2> samples;
  generate_half_jittered_samples(n, samples);

  EXPECT_EQ(samples.size(), n);
  auto n2 = static_cast<uint32_t>(glm::sqrt(n));
  for (size_t i = 0; i < n2; i++) {
    for (size_t j = 0; j < n2; j++) {
      EXPECT_TRUE((samples[i*n2 + j].x >= (i+0.25f) / n2) &&
                  (samples[i*n2 + j].x <= (i+0.75f) / n2));
      EXPECT_TRUE((samples[i*n2 + j].y >= (j+0.25f) / n2) &&
                  (samples[i*n2 + j].y <= (j+0.75f) / n2));
    }
  }
}

//==============================================================================
TEST(Sample, HalfJitteredSamplesNotPerfectSquare) {
  uint32_t n = 15;
  std::vector<glm::vec2> samples;
  generate_half_jittered_samples(n, samples);

  auto sqr_n = static_cast<uint32_t>(glm::sqrt(n));
  EXPECT_EQ(samples.size(), sqr_n*sqr_n);
  for (size_t i = 0; i < sqr_n; i++) {
    for (size_t j = 0; j < sqr_n; j++) {
      EXPECT_TRUE((samples[i*sqr_n + j].x >= (i+0.25f) / sqr_n) &&
          (samples[i*sqr_n + j].x <= (i+0.75f) / sqr_n));
      EXPECT_TRUE((samples[i*sqr_n + j].y >= (j+0.25f) / sqr_n) &&
          (samples[i*sqr_n + j].y <= (j+0.75f) / sqr_n));
    }
  }
}

//==============================================================================
TEST(Sample, NRooks) {
  uint32_t n = 10;
  std::vector<glm::vec2> samples;
  generate_nrooks_samples(n, samples);

  EXPECT_EQ(samples.size(), n);
  auto subcell_width = 1.f / n;
  for (size_t i = 0; i < samples.size(); i++) {
    EXPECT_TRUE((samples[i].y >= i*subcell_width) &&
                (samples[i].y <= (i+1)*subcell_width));
    EXPECT_TRUE((samples[i].x >= 0.f) && (samples[i].x <= 1.f));
  }
}

//==============================================================================
TEST(Sample, MultiJittered) {
  uint32_t n = 9;
  std::vector<glm::vec2> samples;
  generate_multi_jittered_samples(n, samples);

  EXPECT_EQ(samples.size(), n);
  auto sqr_n = static_cast<uint32_t>(glm::sqrt(n));
  auto subcell_width = 1.f / (sqr_n*sqr_n);
  for (auto const &sample : samples) {
    EXPECT_TRUE((sample.x >= 0.f) && (sample.x <= 1.f));
    EXPECT_TRUE((sample.y >= 0.f) && (sample.y <= 1.f));
  }
}

//==============================================================================
TEST(Filter, Box) {
  ip_sample s1(glm::vec3(0.8f), glm::vec2(1.5f,  5.5f));
  ip_sample s2(glm::vec3(0.7f), glm::vec2(1.25f, 5.75f));
  ip_sample s3(glm::vec3(1.f),  glm::vec2(1.75f, 5.75f));
  ip_sample s4(glm::vec3(0.9f), glm::vec2(1.25f, 5.25f));
  ip_sample s5(glm::vec3(1.f),  glm::vec2(1.5f,  6.05f));

  std::vector<ip_sample> samples;
  samples.push_back(s1);
  samples.push_back(s2);
  samples.push_back(s3);
  samples.push_back(s4);
  samples.push_back(s5);

  float_t extent = 1.f;
  uint32_t x(1), y(5);
  glm::vec3 radiance = evaluate_box_filter(samples, x, y, extent);
  EXPECT_FLOAT_EQ(radiance.x, 0.85);
  EXPECT_FLOAT_EQ(radiance.y, 0.85);
  EXPECT_FLOAT_EQ(radiance.z, 0.85);
}

//==============================================================================
TEST(Filter, Triangle) {
  auto float_err = 0.001;
  ip_sample s1(glm::vec3(0.8f), glm::vec2(1.5f,  5.5f));
  ip_sample s2(glm::vec3(0.7f), glm::vec2(1.25f, 5.75f));
  ip_sample s3(glm::vec3(1.f),  glm::vec2(1.75f, 5.75f));
  ip_sample s4(glm::vec3(0.9f), glm::vec2(1.25f, 5.25f));
  ip_sample s5(glm::vec3(1.f),  glm::vec2(1.5f,  6.05f));

  std::vector<ip_sample> samples;
  samples.push_back(s1);
  samples.push_back(s2);
  samples.push_back(s3);
  samples.push_back(s4);
  samples.push_back(s5);

  float_t extent = 1.f;
  uint32_t x(1), y(5);
  glm::vec3 radiance = evaluate_triangle_filter(samples, x, y, extent);
  EXPECT_NEAR(radiance.x, 0.8285, float_err);
  EXPECT_NEAR(radiance.y, 0.8285, float_err);
  EXPECT_NEAR(radiance.z, 0.8285, float_err);
}

//==============================================================================
TEST(Filter, Gaussian) {
  auto float_err = 0.001;
  ip_sample s1(glm::vec3(0.8f), glm::vec2(1.5f,  5.5f));
  ip_sample s2(glm::vec3(0.7f), glm::vec2(1.25f, 5.75f));
  ip_sample s3(glm::vec3(1.f),  glm::vec2(1.75f, 5.75f));
  ip_sample s4(glm::vec3(0.9f), glm::vec2(1.25f, 5.25f));
  ip_sample s5(glm::vec3(1.f),  glm::vec2(1.5f,  6.05f));

  std::vector<ip_sample> samples;
  samples.push_back(s1);
  samples.push_back(s2);
  samples.push_back(s3);
  samples.push_back(s4);
  samples.push_back(s5);

  float_t extent  = 1.f;
  float_t alpha   = 2.f;
  uint32_t x(1), y(5);
  glm::vec3 radiance = evaluate_gaussian_filter(samples, x, y, extent, alpha);
  EXPECT_NEAR(radiance.x, 0.8397f, float_err);
  EXPECT_NEAR(radiance.y, 0.8397f, float_err);
  EXPECT_NEAR(radiance.z, 0.8397f, float_err);
}

//==============================================================================
TEST(Filter, Mitchell) {
  auto float_err = 0.001;
  ip_sample s1(glm::vec3(0.8f), glm::vec2(1.5f,  5.5f));
  ip_sample s2(glm::vec3(0.7f), glm::vec2(1.25f, 5.75f));
  ip_sample s3(glm::vec3(1.f),  glm::vec2(1.75f, 5.75f));
  ip_sample s4(glm::vec3(0.9f), glm::vec2(1.25f, 5.25f));
  ip_sample s5(glm::vec3(1.f),  glm::vec2(1.5f,  6.05f));

  std::vector<ip_sample> samples;
  samples.push_back(s1);
  samples.push_back(s2);
  samples.push_back(s3);
  samples.push_back(s4);
  samples.push_back(s5);

  float_t extent  = 1.f;
  float_t b       = 1.f / 3.f;
  float_t c       = 1.f / 3.f;
  uint32_t x(1), y(5);
  glm::vec3 radiance = evaluate_mitchell_filter(samples, x, y, extent, b, c);
  EXPECT_NEAR(radiance.x, 0.8679f, float_err);
  EXPECT_NEAR(radiance.y, 0.8679f, float_err);
  EXPECT_NEAR(radiance.z, 0.8679f, float_err);
}

//==============================================================================
TEST(Filter, Sinc) {
  auto float_err = 0.001;
  ip_sample s1(glm::vec3(0.8f), glm::vec2(1.5f,  5.5f));
  ip_sample s2(glm::vec3(0.7f), glm::vec2(1.25f, 5.75f));
  ip_sample s3(glm::vec3(1.f),  glm::vec2(1.75f, 5.75f));
  ip_sample s4(glm::vec3(0.9f), glm::vec2(1.25f, 5.25f));
  ip_sample s5(glm::vec3(1.f),  glm::vec2(1.5f,  6.05f));

  std::vector<ip_sample> samples;
  samples.push_back(s1);
  samples.push_back(s2);
  samples.push_back(s3);
  samples.push_back(s4);
  samples.push_back(s5);

  float_t extent  = 1.f;
  float_t tau     = 3.f;
  uint32_t x(1), y(5);
  glm::vec3 radiance = evaluate_sinc_filter(samples, x, y, extent, tau);
  EXPECT_NEAR(radiance.x, 0.7954f, float_err);
  EXPECT_NEAR(radiance.y, 0.7954f, float_err);
  EXPECT_NEAR(radiance.z, 0.7954f, float_err);
}
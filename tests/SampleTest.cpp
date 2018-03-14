// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include <glm/vec2.hpp>

#include "../src/extra/Sample.h"

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
  EXPECT_EQ(samples.size(), sqr_n);
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
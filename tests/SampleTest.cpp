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

  EXPECT_EQ(samples.size(), 9);
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

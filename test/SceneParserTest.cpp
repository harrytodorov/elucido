// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "../src/extra/UtilityFunctions.cpp"
#include "../include/elucido/Utilities.h"

//==============================================================================
TEST(SceneParser, checkForInvalidFile) {
  std::string filename =
      "blabla.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, file_problem);
  EXPECT_EQ(result.first.second, 0);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForInvalidStatement) {
  std::string filename =
    "/Users/harry/dev/elucido/test_resources/test_checkForInvalidStatement.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_statement);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForInvalidCreateStatement) {
  std::string filename =
      "/Users/harry/dev/elucido/test_resources/test_checkForInvalidCreateStatement.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_syntax);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForDuplicate) {
  std::string filename =
      "/Users/harry/dev/elucido/test_resources/test_checkForDuplicate.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, duplicate);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForInvalidSetStatement) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForInvalidSetStatement.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_syntax);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForNotExistingThingToSet) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForNotExistingThingToSet.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, thing_not_created);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForInvalidSetProperty) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForInvalidSetProperty.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForInvalidSetPropertyValue) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForInvalidSetPropertyValue.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfCameraTypeIsSet) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfCameraTypeIsSet.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfCameraTypeParameterMatchOrtho) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfCameraTypeParameterMatchOrtho.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfCameraTypeParameterMatchPersp) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfCameraTypeParameterMatchPersp.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfColorParamerterValueHasThreeDigits) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfColorParamerterValueHasThreeDigits.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfColorParamerterValueIsInRange) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfColorParamerterValueIsInRange.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfMaterialTypeIsSupported) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfMaterialTypeIsSupported.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfMaterialColorIsAlreadyDefined) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfMaterialColorIsAlreadyDefined.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfLightTypeIsDefined) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfLighTypeIsDefined.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 9);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfLightTypeAndPropertyMatch) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfLighTypeAndPropertyMatch.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 11);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForObjectIfMaterialIsDefinedBeforeSettingIt) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForObjectIfMaterialIsDefinedBeforeSettingIt.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForObjectIfTypeAndPropertyMatch) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForObjectIfTypeAndPropertyMatch.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 10);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForObjectIfVectorIsDefinedBeforeSettingIt) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForObjectIfVectorIsDefinedBeforeSettingIt.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 8);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForObjectIfVectorIsDefinedBeforeSettingIt2) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForObjectIfVectorIsDefinedBeforeSettingIt2.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 8);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForObjectIfVectorCountMatchForProperty) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForObjectIfVectorCountMatchForProperty.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 10);
  EXPECT_EQ(result.second.size(), 0);
}

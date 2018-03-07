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

//==============================================================================
TEST(SceneParser, checkForImagePlaneIfOutputTypeIsAvailable) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForImagePlaneIfOutputTypeIsAvailable.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForImagePlaneIfGammaIsTrueOrFalse) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForImagePlaneIfGammaIsTrueOrFalse.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForASIfTypeIsDefinedBeforeSettingParameter) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForASIfTypeIsDefinedBeforeSettingParameter.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 7);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForSceneIfCameraExistBeforeSetting) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForSceneIfCameraExistBeforeSetting.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForSceneIfImagePlaneExistBeforeSetting) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForSceneIfImagePlaneExistBeforeSetting.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForSceneIfASExistBeforeSetting) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForSceneIfASExistBeforeSetting.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForSceneIfObjectExistBeforeSetting) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForSceneIfObjectExistBeforeSetting.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForSceneIfLightExistBeforeSetting) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForSceneIfLightExistBeforeSetting.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForSceneIfAnimationExistBeforeSetting) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForSceneIfAnimationExistBeforeSetting.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_set_property_value);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfTransformationTypeIsValid) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfTransformationTypeIsValid.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_transformation_type);
  EXPECT_EQ(result.first.second, 7);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfTransformationAxisIsValid) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfTransformationAxisIsValid.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_transformation_axix);
  EXPECT_EQ(result.first.second, 7);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfThingSupportTransformation) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfThingSupportTransformation.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_thing_to_transform);
  EXPECT_EQ(result.first.second, 7);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkTransformThingWhichIsCreated) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkTransformThingWhichIsCreated.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, thing_not_created);
  EXPECT_EQ(result.first.second, 3);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfTransformationTypeIsSupported) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfTransformationTypeIsSupported.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_transformation_type);
  EXPECT_EQ(result.first.second, 7);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfTransformationHasValidSyntax) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfTransformationHasValidSyntax.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_syntax);
  EXPECT_EQ(result.first.second, 8);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfAnimationHasValidSyntax) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfAnimationHasValidSyntax.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_syntax);
  EXPECT_EQ(result.first.second, 10);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkIfAnimationExist) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkIfAnimationExist.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, thing_not_created);
  EXPECT_EQ(result.first.second, 6);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForAnimationIfThingSupportAnimating) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForAnimationIfThingSupportAnimating.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_thing_to_transform);
  EXPECT_EQ(result.first.second, 10);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForAnimationIfThingSupportTransformationType) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForAnimationIfThingSupportTransformationType.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_transformation_type);
  EXPECT_EQ(result.first.second, 10);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForAnimationIfTransformationTypeIsValid) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForAnimationIfTransformationTypeIsValid.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_transformation_type);
  EXPECT_EQ(result.first.second, 10);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForAnimationIfTransformationAxisIsValid) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForAnimationIfTransformationAxisIsValid.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_transformation_axix);
  EXPECT_EQ(result.first.second, 10);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForAnimationIfThingToBeTransformedExist) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForAnimationIfThingToBeTransformedExist.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, thing_not_created);
  EXPECT_EQ(result.first.second, 7);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, checkForAnimationAnimatingMultipleCameras) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_checkForAnimationAnimatingMultipleCameras.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, invalid_animation_camera);
  EXPECT_EQ(result.first.second, 16);
  EXPECT_EQ(result.second.size(), 0);
}

//==============================================================================
TEST(SceneParser, exhaustiveSceneCreation) {
  std::string filename = "/Users/harry/dev/elucido/test_resources/test_exhaustiveSceneCreation.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, success);
  EXPECT_EQ(result.first.second, 136);

  // Number of scenes
  EXPECT_EQ(result.second.size(), 2);

  // FIRST SCENE
  auto scene1 = result.second[0];

  // Basic scene information.
  EXPECT_STREQ(scene1.name.c_str(),   "scene1");
  EXPECT_EQ(scene1.lights.size(),     1);
  EXPECT_EQ(scene1.objects.size(),    2);
  EXPECT_EQ(scene1.animations.size(), 1);

  // Camera information.
  auto cam_s1 = scene1.camera;
  EXPECT_STREQ(cam_s1->name.c_str(),       "c1");
  EXPECT_EQ(cam_s1->type,                  orthographic);
  EXPECT_EQ(cam_s1->property.first,        zoom_factor);
  EXPECT_FLOAT_EQ(cam_s1->property.second, 1.5f);

  // Camera transformations.
  auto cam1_trans = cam_s1->transformations;
  ASSERT_EQ(cam_s1->transformations.size(), 2);

  EXPECT_EQ(cam1_trans[0].type, translation);
  EXPECT_EQ(cam1_trans[0].axis, XY);
  EXPECT_FLOAT_EQ(cam1_trans[0].amount, 15.f);

  EXPECT_EQ(cam1_trans[1].type, rotation);
  EXPECT_EQ(cam1_trans[1].axis, Y);
  EXPECT_FLOAT_EQ(cam1_trans[1].amount, 45.f);
//
//  // Image plane information
//  auto ip_s1 = scene1.image_plane;
//  EXPECT_EQ(ip_s1->name, "ip1");
//  EXPECT_EQ(ip_s1->output_type, png_o);
//  EXPECT_EQ(ip_s1->horizontal, 1920);
//  EXPECT_EQ(ip_s1->vertical, 1080);
//  EXPECT_EQ(ip_s1->use_gamma, 1);
//  EXPECT_EQ(ip_s1->number_samples, 25);
}

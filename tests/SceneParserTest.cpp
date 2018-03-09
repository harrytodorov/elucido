// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <gtest/gtest.h>
#include <vector>
#include <string>

#include "../src/extra/Utilities.h"

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
  std::string filename =
      "/Users/harry/dev/elucido/test_resources/test_exhaustiveSceneCreation.txt";
  auto result = read_scene_from_file(filename);
  EXPECT_EQ(result.first.first, success);
  EXPECT_EQ(result.first.second, 136);

  // Number of scenes
  EXPECT_EQ(result.second.size(), 2);

  // ---------------------------------------------------------------------------
  // FIRST SCENE
  auto scene1 = result.second[0];

  // Basic scene information.
  EXPECT_STREQ(scene1.name.c_str(), "scene1");
  EXPECT_EQ(scene1.lights.size(), 1);
  EXPECT_EQ(scene1.objects.size(), 2);
  EXPECT_EQ(scene1.animations.size(), 1);

  // Camera information.
  auto cam_s1 = scene1.camera;
  EXPECT_STREQ(cam_s1->name.c_str(), "c1");
  EXPECT_EQ(cam_s1->type, orthographic);
  EXPECT_EQ(cam_s1->property.first, zoom_factor);
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

  // Image plane information.
  auto ip_s1 = scene1.image_plane;
  EXPECT_STREQ(ip_s1->name.c_str(), "ip1");
  EXPECT_EQ(ip_s1->output_type, png_o);
  EXPECT_EQ(ip_s1->horizontal, 1920);
  EXPECT_EQ(ip_s1->vertical, 1080);
  EXPECT_EQ(ip_s1->use_gamma, 1);
  EXPECT_EQ(ip_s1->number_samples, 25);

  // Acceleration structure information.
  auto ac_s1 = scene1.acceleration_structure;
  EXPECT_STREQ(ac_s1->name.c_str(), "ac1");
  EXPECT_EQ(ac_s1->type, grid);
  EXPECT_FLOAT_EQ(ac_s1->alpha, 3.5f);
  EXPECT_EQ(ac_s1->max_resolution, 64);

  // Objects information.
  auto obj_s1 = scene1.objects;
  EXPECT_EQ(obj_s1.size(), 2);

  // SPHERE.
  EXPECT_STREQ(obj_s1[0].name.c_str(), "sphere1");
  EXPECT_EQ(obj_s1[0].type, sphere);

  // Sphere center; Vector p1.
  EXPECT_STREQ(obj_s1[0].center->name.c_str(), "p1");
  EXPECT_FLOAT_EQ(obj_s1[0].center->x, 1.f);
  EXPECT_FLOAT_EQ(obj_s1[0].center->y, 1.f);
  EXPECT_FLOAT_EQ(obj_s1[0].center->z, 1.f);

  EXPECT_FLOAT_EQ(obj_s1[0].radius, 2.5f);

  // Sphere material; Material phong_basic.
  auto m_prop = obj_s1[0].material->properties;
  EXPECT_STREQ(obj_s1[0].material->name.c_str(), "phong_basic");
  EXPECT_EQ(obj_s1[0].material->type, pm);
  EXPECT_EQ(m_prop.size(), 4);
  EXPECT_FLOAT_EQ(m_prop[ambient], 0.5f);
  EXPECT_FLOAT_EQ(m_prop[diffuse], 0.2f);
  EXPECT_FLOAT_EQ(m_prop[spec_exp], 1.5f);
  EXPECT_FLOAT_EQ(m_prop[spec_const], 0.15f);

  // Material color; Color green.
  auto col_g = obj_s1[0].material->color;
  EXPECT_STREQ(col_g->name.c_str(), "green");
  EXPECT_EQ(col_g->r, 0);
  EXPECT_EQ(col_g->g, 255);
  EXPECT_EQ(col_g->b, 0);

  // Sphere transformations.
  auto s_trans = obj_s1[0].transformations;
  EXPECT_EQ(s_trans.size(), 3);

  EXPECT_EQ(s_trans[0].type, translation);
  EXPECT_EQ(s_trans[0].axis, Y);
  EXPECT_FLOAT_EQ(s_trans[0].amount, 5.f);

  EXPECT_EQ(s_trans[1].type, rotation);
  EXPECT_EQ(s_trans[1].axis, XY);
  EXPECT_FLOAT_EQ(s_trans[1].amount, 15.f);

  EXPECT_EQ(s_trans[2].type, scale);
  EXPECT_EQ(s_trans[2].axis, XYZ);
  EXPECT_FLOAT_EQ(s_trans[2].amount, 5.f);

  // TRIANGLE
  EXPECT_STREQ(obj_s1[1].name.c_str(), "triangle1");
  EXPECT_EQ(obj_s1[1].type, triangle);

  // Triangle vertices.
  EXPECT_EQ(obj_s1[1].vertices.size(), 3);

  // Vector p2.
  EXPECT_STREQ(obj_s1[1].vertices[1]->name.c_str(), "p2");
  EXPECT_EQ(obj_s1[1].vertices[1]->x, 2);
  EXPECT_EQ(obj_s1[1].vertices[1]->y, 2);
  EXPECT_EQ(obj_s1[1].vertices[1]->z, 2);

  // Vector p3.
  EXPECT_STREQ(obj_s1[1].vertices[2]->name.c_str(), "p3");
  EXPECT_EQ(obj_s1[1].vertices[2]->x, 3);
  EXPECT_EQ(obj_s1[1].vertices[2]->y, 3);
  EXPECT_EQ(obj_s1[1].vertices[2]->z, 3);

  // Triangle transformations.
  auto t_trans = obj_s1[1].transformations;
  EXPECT_EQ(t_trans.size(), 3);

  EXPECT_EQ(t_trans[0].type, translation);
  EXPECT_EQ(t_trans[0].axis, X);
  EXPECT_FLOAT_EQ(t_trans[0].amount, 1.f);

  EXPECT_EQ(t_trans[1].type, rotation);
  EXPECT_EQ(t_trans[1].axis, XY);
  EXPECT_FLOAT_EQ(t_trans[1].amount, 15.f);

  EXPECT_EQ(t_trans[2].type, scale);
  EXPECT_EQ(t_trans[2].axis, Z);
  EXPECT_FLOAT_EQ(t_trans[2].amount, 5.f);

  // LIGHTS
  auto l_s1 = scene1.lights;
  EXPECT_EQ(l_s1.size(), 1);

  EXPECT_STREQ(l_s1[0].name.c_str(), "l1");
  EXPECT_EQ(l_s1[0].type, point);
  EXPECT_FLOAT_EQ(l_s1[0].intensity, 100.f);

  // Light transformations.
  auto l_s1_trans = l_s1[0].transformations;
  EXPECT_EQ(l_s1_trans.size(), 1);

  EXPECT_EQ(l_s1_trans[0].type, translation);
  EXPECT_EQ(l_s1_trans[0].axis, Z);
  EXPECT_FLOAT_EQ(l_s1_trans[0].amount, -10.f);

  // ANIMATIONS
  auto anim_s1 = scene1.animations;
  EXPECT_EQ(anim_s1.size(), 1);

  EXPECT_STREQ(anim_s1[0].name.c_str(), "anim_scene1");
  EXPECT_EQ(anim_s1[0].num_of_images_in_sequence, 15);

  EXPECT_EQ(anim_s1[0].objects.size(), 1);
  EXPECT_EQ(anim_s1[0].objects.at("triangle1").size(), 1);
  EXPECT_EQ(anim_s1[0].objects.at("triangle1")[0].type, rotation);
  EXPECT_EQ(anim_s1[0].objects.at("triangle1")[0].axis, X);
  EXPECT_FLOAT_EQ(anim_s1[0].objects.at("triangle1")[0].amount, 15.f);

  EXPECT_EQ(anim_s1[0].lights.size(), 1);
  EXPECT_EQ(anim_s1[0].lights.at("l1")[0].type, translation);
  EXPECT_EQ(anim_s1[0].lights.at("l1")[0].axis, Z);
  EXPECT_FLOAT_EQ(anim_s1[0].lights.at("l1")[0].amount, -5.f);

  EXPECT_STREQ(anim_s1[0].camera.first.c_str(), "c1");
  EXPECT_EQ(anim_s1[0].camera.second.size(), 1);
  EXPECT_EQ(anim_s1[0].camera.second[0].type, translation);
  EXPECT_EQ(anim_s1[0].camera.second[0].axis, XY);
  EXPECT_FLOAT_EQ(anim_s1[0].camera.second[0].amount, 15.f);
  // ---------------------------------------------------------------------------

  // ---------------------------------------------------------------------------
  // SECOND SCENE
  auto scene2 = result.second[1];

  // Basic scene information.
  EXPECT_STREQ(scene2.name.c_str(), "scene2");
  EXPECT_EQ(scene2.lights.size(), 1);
  EXPECT_EQ(scene2.objects.size(), 1);
  EXPECT_EQ(scene2.animations.size(), 1);

  // Camera information.
  auto cam_s2 = scene2.camera;
  EXPECT_STREQ(cam_s2->name.c_str(), "c2");
  EXPECT_EQ(cam_s2->type, perspective);
  EXPECT_EQ(cam_s2->property.first, field_of_view);
  EXPECT_FLOAT_EQ(cam_s2->property.second, 2.5f);

  // Camera transformations.
  auto cam_s2_trans = cam_s2->transformations;
  ASSERT_EQ(cam_s2_trans.size(), 2);

  EXPECT_EQ(cam_s2_trans[0].type, translation);
  EXPECT_EQ(cam_s2_trans[0].axis, X);
  EXPECT_FLOAT_EQ(cam_s2_trans[0].amount, -15.f);

  EXPECT_EQ(cam_s2_trans[1].type, rotation);
  EXPECT_EQ(cam_s2_trans[1].axis, Z);
  EXPECT_FLOAT_EQ(cam_s2_trans[1].amount, 90.f);

  // Image plane information.
  auto ip_s2 = scene2.image_plane;
  EXPECT_STREQ(ip_s2->name.c_str(), "ip1");
  EXPECT_EQ(ip_s2->output_type, png_o);
  EXPECT_EQ(ip_s2->horizontal, 1920);
  EXPECT_EQ(ip_s2->vertical, 1080);
  EXPECT_EQ(ip_s2->use_gamma, 1);
  EXPECT_EQ(ip_s2->number_samples, 25);

  // Acceleration structure information.
  auto ac_s2 = scene2.acceleration_structure;
  EXPECT_STREQ(ac_s2->name.c_str(), "ac1");
  EXPECT_EQ(ac_s2->type, grid);
  EXPECT_FLOAT_EQ(ac_s2->alpha, 3.5f);
  EXPECT_EQ(ac_s2->max_resolution, 64);

  // Objects information.
  auto obj_s2 = scene2.objects;
  EXPECT_EQ(obj_s2.size(), 1);

  // Triangle mesh.
  EXPECT_STREQ(obj_s2[0].name.c_str(), "tm1");
  EXPECT_EQ(obj_s2[0].type, triangle_mesh);
  EXPECT_STREQ(obj_s2[0].file_name.c_str(), "bunny.obj");
  EXPECT_EQ(obj_s2[0].interpolation, 1);

  // Triangle mesh transformations.
  auto tm_trans = obj_s2[0].transformations;
  EXPECT_EQ(tm_trans.size(), 3);

  EXPECT_EQ(tm_trans[0].type, translation);
  EXPECT_EQ(tm_trans[0].axis, X);
  EXPECT_FLOAT_EQ(tm_trans[0].amount, 5.f);

  EXPECT_EQ(tm_trans[1].type, rotation);
  EXPECT_EQ(tm_trans[1].axis, Y);
  EXPECT_FLOAT_EQ(tm_trans[1].amount, 45.f);

  EXPECT_EQ(tm_trans[2].type, scale);
  EXPECT_EQ(tm_trans[2].axis, XYZ);
  EXPECT_FLOAT_EQ(tm_trans[2].amount, 0.5f);

  // LIGHTS
  auto l_s2 = scene2.lights;
  EXPECT_EQ(l_s2.size(), 1);

  EXPECT_STREQ(l_s2[0].name.c_str(), "l2");
  EXPECT_EQ(l_s2[0].type, directional);
  EXPECT_FLOAT_EQ(l_s2[0].intensity, 50.f);

  // Light transformations.
  auto l_s2_trans = l_s2[0].transformations;
  EXPECT_EQ(l_s2_trans.size(), 1);

  EXPECT_EQ(l_s2_trans[0].type, rotation);
  EXPECT_EQ(l_s2_trans[0].axis, Y);
  EXPECT_FLOAT_EQ(l_s2_trans[0].amount, 45.f);

  // ANIMATIONS
  auto anim_s2 = scene2.animations;
  EXPECT_EQ(anim_s2.size(), 1);

  EXPECT_STREQ(anim_s2[0].name.c_str(), "anim_scene2");
  EXPECT_EQ(anim_s2[0].num_of_images_in_sequence, 100);

  EXPECT_EQ(anim_s2[0].objects.size(), 1);
  EXPECT_EQ(anim_s2[0].objects.at("tm1").size(), 1);
  EXPECT_EQ(anim_s2[0].objects.at("tm1")[0].type, scale);
  EXPECT_EQ(anim_s2[0].objects.at("tm1")[0].axis, XYZ);
  EXPECT_FLOAT_EQ(anim_s2[0].objects.at("tm1")[0].amount, 1.5f);

  EXPECT_EQ(anim_s2[0].lights.size(), 1);
  EXPECT_EQ(anim_s2[0].lights.at("l2")[0].type, rotation);
  EXPECT_EQ(anim_s2[0].lights.at("l2")[0].axis, Y);
  EXPECT_FLOAT_EQ(anim_s2[0].lights.at("l2")[0].amount, 35.f);

  EXPECT_STREQ(anim_s2[0].camera.first.c_str(), "c2");
  EXPECT_EQ(anim_s2[0].camera.second.size(), 1);
  EXPECT_EQ(anim_s2[0].camera.second[0].type, rotation);
  EXPECT_EQ(anim_s2[0].camera.second[0].axis, Z);
  EXPECT_FLOAT_EQ(anim_s2[0].camera.second[0].amount, 2.f);
  // ---------------------------------------------------------------------------
}

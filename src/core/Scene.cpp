// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Scene.h"
#include "../objects/Sphere.h"
#include "../objects/Triangle.h"
#include "../objects/TriangleMesh.h"
#include "../lights/PointLight.h"
#include "../lights/DirectionalLight.h"
#include "../cameras/OrthographicCamera.h"
#include "../cameras/PerspectiveCamera.h"
#include "../accelerators/Grid.h"
#include "Renderer.h"

//==============================================================================
void Scene::convert_color01_range(glm::vec3 &color) {
  auto inv = 1.f / 255.f;
  color *= inv;
}

//==============================================================================
bool Scene::generate_as(const acceleration_structure_description &asd) {
    std::shared_ptr<AccelerationStructure> as = nullptr;

  switch (asd.type) {
    // Grid.
    case AccelerationStructureType::grid : {
      as = std::make_shared<Grid>(Grid(scene_bb, objects));

      // Alpha.
      if (asd.alpha != 0.f) {
        std::static_pointer_cast<Grid>(as)->set_alpha(asd.alpha);
      }

      // Maximum grid resolution per axis.
      if (asd.max_resolution != 0) {
        std::static_pointer_cast<Grid>(as)->set_max_res(asd.max_resolution);
      }
    } break;

    default: break;
  }

  set_as(as);
  return true;
}

//==============================================================================
bool Scene::generate_camera(const camera_description &cd,
                            const uint32_t &image_width,
                            const uint32_t &image_height) {
  std::shared_ptr<Camera> c = nullptr;

  switch (cd.type) {
    // Orthographic camera.
    case CameraType::orthographic : {
      c = std::make_shared<OrthographicCamera>(OrthographicCamera());

      if (cd.property.first == zoom_factor) {
        std::static_pointer_cast<OrthographicCamera>(c)->set_zoom_factor(cd.property.second);
      }
    } break;

    // Perspective camera.
    case CameraType::perspective : {
      c = std::make_shared<PerspectiveCamera>(PerspectiveCamera());

      if (cd.property.first == field_of_view) {
        std::static_pointer_cast<PerspectiveCamera>(c)->set_fov(cd.property.second);
      }
    } break;

    default: return false;
  }

  // Image width & height.
  c->set_image_width(image_width);
  c->set_image_height(image_height);

  // Transformations.
  if (cd.transformations.size() > 0) {
    for (auto const &t : cd.transformations) {
      switch (t.type) {
        case TransformationType::translation: {
          c->translate(t.amount, t.axis);
        } break;
        case TransformationType::rotation: {
          c->rotate(t.amount, t.axis);
        } break;
        default: return false;
      }
    }
  }

  set_camera(c);
  return true;
}

//==============================================================================
bool Scene::generate_light(const light_description &light) {
  if (light.type == not_set_lt) return false;

  std::shared_ptr<Light> l = nullptr;

  switch (light.type) {
    // Point light.
    case LightType::point : {
      l = std::make_shared<PointLight>(PointLight());

      if (light.property.first == position) {
        glm::vec4 position(light.property.second->x,
                           light.property.second->y,
                           light.property.second->z,
                           1.f);
        std::static_pointer_cast<PointLight>(l)->set_position(position);
      }
    } break;

    // Directional light.
    case LightType::directional : {
      l = std::make_shared<DirectionalLight>(DirectionalLight());

      if (light.property.first == direction) {
        glm::vec4 direction(light.property.second->x,
                            light.property.second->y,
                            light.property.second->z,
                            0.f);
        std::static_pointer_cast<DirectionalLight>(l)->set_direction(direction);
      }
    } break;

    default: return false;
  }

  // Transformations.
  if (light.transformations.size() > 0) {
    for (auto const &t : light.transformations) {
      switch (t.type) {
        case TransformationType::translation: {
          l->translate(t.amount, t.axis);
        } break;
        case TransformationType::rotation: {
          l->rotate(t.amount, t.axis);
        } break;
        default: return false;
      }
    }
    l->apply_transformations();
  }

  // Intensity.
  l->set_intensity(light.intensity);

  // Color.
  if (light.color != nullptr) {
    glm::vec3 col(light.color->r,
                  light.color->g,
                  light.color->b);
    convert_color01_range(col);
    l->set_color(col);
  }

  add_light(l);
  return true;
}

//==============================================================================
bool Scene::generate_object(const object_description &object) {
  if (object.type == not_set_ot) return false;

  std::shared_ptr<Object> obj = nullptr;

  switch (object.type) {
    // Sphere.
    case ObjectType::sphere: {
      obj = std::make_shared<Sphere>(Sphere());

      // Center.
      if (object.center != nullptr) {
        glm::vec4 center(object.center->x,
                         object.center->y,
                         object.center->z,
                         1.f);
        std::static_pointer_cast<Sphere>(obj)->set_center(center);
      }

      // Radius.
      if (object.radius > 0.f) {
        float_t radius(object.radius);
        std::static_pointer_cast<Sphere>(obj)->set_radius(radius);
      }
    } break;

    // Triangle.
    case ObjectType::triangle: {
      obj = std::make_shared<Triangle>(Triangle());

      // Vertices.
      if (object.vertices.size() != 0) {
        glm::vec4 v0(object.vertices[0]->x,
                     object.vertices[0]->y,
                     object.vertices[0]->z,
                     1.f);

        glm::vec4 v1(object.vertices[1]->x,
                     object.vertices[1]->y,
                     object.vertices[1]->z,
                     1.f);

        glm::vec4 v2(object.vertices[2]->x,
                     object.vertices[2]->y,
                     object.vertices[2]->z,
                     1.f);
        std::static_pointer_cast<Triangle>(obj)->set_vertices(v0, v1, v2);
      }
    } break;

    // Triangle mesh.
    case ObjectType::triangle_mesh: {
      if (object.file_name.size() == 0) return false;
      obj = std::make_shared<TriangleMesh>(TriangleMesh());

      auto li = std::static_pointer_cast<TriangleMesh>(obj)->load_mesh(object.file_name.c_str());
      if (li.nt == 0) return false;

      // TODO: does not make sense to have 3 integers for interpolation.
      if (object.interpolation == 1)
        std::static_pointer_cast<TriangleMesh>(obj)->in = true;
    } break;

    default: return false;
  }

  // Transformations.
  if (object.transformations.size() > 0) {
    for (auto const &t : object.transformations) {
      switch (t.type) {
        case TransformationType::translation: {
          obj->translate(t.amount, t.axis);
        } break;
        case TransformationType::rotation: {
          obj->rotate(t.amount, t.axis);
        } break;
        case TransformationType::scale: {
          obj->scale(t.amount, t.axis);
        } break;

        default: break;
      }
    }
    obj->apply_transformations();
  }

  // Material.
  material m = material();
  if (object.material != nullptr) {
    m.mt = object.material->type;
    m.ac = object.material->properties[ambient];
    m.dc = object.material->properties[diffuse];
    m.sc = object.material->properties[spec_const];
    m.se = object.material->properties[spec_exp];
    m.ior = object.material->properties[ior];
    m.ri = object.material->properties[refl_ind];
    if (object.material->color != nullptr) {
      glm::vec3 m_col(object.material->color->r,
                      object.material->color->g,
                      object.material->color->b);
      convert_color01_range(m_col);
      m.c = m_col;
    }
  }
  obj->set_material(m);

  add_object(obj);
  return true;
}

//==============================================================================
bool Scene::load_scene(const scene_description &description) {

  // Validate scene description.
  // For a scene to be valid, it should have a camera and
  // an image plane.

  // Check if camera and image plane are set.
  if (description.camera == nullptr || description.image_plane == nullptr)
    return false;

  // Validate Camera.
  if (description.camera->type == not_set_ct)
    return false;

  // Validate image plane.
  if (description.image_plane->vertical == 0 ||
      description.image_plane->horizontal == 0)
    return false;

  // Initialize the scene.
  this->name = description.name;

  // Generate image plane.
  auto ipd  = description.image_plane;
  auto ip   = std::make_shared<ImagePlane>(ipd->horizontal, ipd->vertical);
  ip->ns    = ipd->number_samples;
  set_image_plane(ip);

  // Generate camera.
  if (!generate_camera(*(description.camera), ip->hres, ip->vres)) {
    std::cout << "There was a problem while generating '"
              << description.camera->name << "'." << std::endl;
    return false;
  }

  // Generate objects.
  for (auto const &object : description.objects) {
    if (!generate_object(object)) {
      std::cout << "There was a problem while generating '" << object.name
                << "'." << std::endl;
    }
  }

  // Generate lights.
  for (auto const &light : description.lights) {
    if (!generate_light(light)) {
      std::cout << "There was a problem while generating '" << light.name
                << "'." << std::endl;
    }
  }

  // Extend scene's bounding box.
  extend_scene_bb();

  // Generate acceleration structure.
  if (description.acceleration_structure != nullptr) {
    generate_as(*(description.acceleration_structure));
    // TODO: print Grid creation information.
    std::static_pointer_cast<Grid>(acceleration_structure)->constructGrid();
  }

  // TODO: Animations.

  return true;
}

//==============================================================================
void Scene::extend_scene_bb() {
  for (auto const &object : objects) {
    scene_bb.extend_by(object->bounding_box().bounds[0]);
    scene_bb.extend_by(object->bounding_box().bounds[1]);
  }
}

//==============================================================================
void Scene::add_object(const std::shared_ptr<Object> object) {
  objects.push_back(object);
}

//==============================================================================
void Scene::add_light(const std::shared_ptr<Light> light) {
  lights.push_back(light);
}

//==============================================================================
void Scene::set_camera(const std::shared_ptr<Camera> _camera) {
  camera = _camera;
}

//==============================================================================
void Scene::set_image_plane(const std::shared_ptr<ImagePlane> _ip) {
  image_plane = _ip;
}

void Scene::set_as(const std::shared_ptr<AccelerationStructure> _ac) {
  acceleration_structure = _ac;
}

//==============================================================================
void Scene::render_image() {
  Ray     primary_ray;

  std::unique_ptr<Renderer> renderer(new Renderer(acceleration_structure,
                                                  scene_bb,
                                                  objects,
                                                  lights));

  // Apply inverse view transform on objects and light sources.
  camera->apply_inverse_view_transform(objects, lights);

  for (uint32_t row = 0; row < image_plane->vres; row++) {
    for (uint32_t col = 0; col < image_plane->hres; col++) {
      primary_ray = camera->get_ray(col, row, 0.5f, 0.5f);
      auto pix_radiance = renderer->cast_ray(primary_ray, 1);
      if (pix_radiance.x == 0.f &&
          pix_radiance.y == 0.f &&
          pix_radiance.z == 0.f) {
        std::cout << col << ", " << row << std::endl;
      }
      image_plane->fb[row*image_plane->hres + col] = pix_radiance;
    }
  }

  // Reverse inverse view transform.
  camera->reverse_inverse_view_transform(objects, lights);

  auto rendered_scene_name = name + ".png";
  image_plane->save_to_png(rendered_scene_name);
}

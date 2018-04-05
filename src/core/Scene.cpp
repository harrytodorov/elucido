// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <iostream>
#include "glm/ext.hpp"    // glm::to_string

#include "Scene.h"

//==============================================================================
void Scene::convert_color01_range(glm::vec3 &color) {
  auto inv = 1.f / 255.f;
  color *= inv;
}

//==============================================================================
bool Scene::generate_as(const std::shared_ptr<acceleration_structure_description> &asd) {
  std::shared_ptr<AccelerationStructure> as = nullptr;

  switch (asd->type) {
    // Dynamic grid.
    case AccelerationStructureType::grid : {
      as = std::make_shared<DynamicGrid>(DynamicGrid());

      // Alpha.
      if (asd->alpha != 0.f) {
        std::static_pointer_cast<DynamicGrid>(as)->set_alpha(asd->alpha);
      }

      // Maximum grid resolution per axis.
      if (asd->max_resolution != 0) {
        std::static_pointer_cast<DynamicGrid>(as)->set_max_res(asd->max_resolution);
      }
    } break;

    // Compact grid.
    case AccelerationStructureType::compact_grid : {
      as = std::make_shared<CompactGrid>(CompactGrid());

      // Alpha.
      if (asd->alpha != 0.f) {
        std::static_pointer_cast<CompactGrid>(as)->set_alpha(asd->alpha);
      }

      // Maximum grid resolution per axis.
      if (asd->max_resolution != 0) {
        std::static_pointer_cast<CompactGrid>(as)->set_max_res(asd->max_resolution);
      }
    } break;

    default: break;
  }

  set_as(as);
  return true;
}

//==============================================================================
bool Scene::generate_camera(const std::shared_ptr<camera_description> &cd,
                            const uint32_t &image_width,
                            const uint32_t &image_height) {
  std::shared_ptr<Camera> c = nullptr;

  switch (cd->type) {
    // Orthographic camera.
    case CameraType::orthographic : {
      c = std::make_shared<OrthographicCamera>(OrthographicCamera());

      if (cd->property.first == zoom_factor) {
        std::static_pointer_cast<OrthographicCamera>(c)->set_zoom_factor(cd->property.second);
      }
    } break;

    // Perspective camera.
    case CameraType::perspective : {
      c = std::make_shared<PerspectiveCamera>(PerspectiveCamera());

      if (cd->property.first == field_of_view) {
        std::static_pointer_cast<PerspectiveCamera>(c)->set_fov(cd->property.second);
      }
    } break;

    default: return false;
  }

  // Image width & height.
  c->set_image_width(image_width);
  c->set_image_height(image_height);

  // Transformations.
  if (!cd->transformations.empty()) {
    for (auto const &t : cd->transformations) {
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
  if (!light.transformations.empty()) {
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
      si.np++;

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
      si.np++;

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
      if (object.file_name.empty()) return false;
      obj = std::make_shared<TriangleMesh>(TriangleMesh());

      // Print triangle mesh's loading information.
      auto sl = std::chrono::high_resolution_clock::now();
      auto li = std::static_pointer_cast<TriangleMesh>(obj)->load_mesh(object.file_name.c_str());
      auto fl = std::chrono::high_resolution_clock::now();
      auto ld = std::chrono::duration_cast<std::chrono::milliseconds>(fl - sl).count();
      if (!li.l) return false;
      print_tm_loading_info(li, ld, object.file_name);
      si.np += li.nt;

      // TODO: does not make sense to have 3 integers for interpolation.
      if (object.interpolation == 1)
        std::static_pointer_cast<TriangleMesh>(obj)->in = true;
    } break;

    default: return false;
  }

  // Transformations.
  if (!object.transformations.empty()) {
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
  ip->set_number_of_samples(ipd->number_samples);
  ip->set_sampling_strategy(ipd->sampling_strategy);
  ip->set_pixel_filter(ipd->pixel_filter);
  ip->generate_unit_samples();
  set_image_plane(ip);

  // Generate camera.
  if (!generate_camera(description.camera, ip->hres, ip->vres)) {
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
  si.no = objects.size();

  // Generate lights.
  for (auto const &light : description.lights) {
    if (!generate_light(light)) {
      std::cout << "There was a problem while generating '" << light.name
                << "'." << std::endl;
    }
  }
  si.nl = lights.size();

  // Generate acceleration structure.
  if (description.acceleration_structure != nullptr) {
    generate_as(description.acceleration_structure);
  }

  // TODO: Animations.

  print_scene_info(si);
  return true;
}

//==============================================================================
void Scene::extend_scene_bb() {
  scene_bb.reset();
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
void Scene::print_as_construction_info(const as_construct_info &i,
                                       const AccelerationStructureType &type) {
  std::cout << "Construction time:\t\t\t\t\t\t"
            << i.d << "ms"
            << std::endl;
  std::cout << "Type:\t\t\t\t\t\t\t\t\t";

  if (type == grid) {
    std::cout << "dynamic grid" << std::endl;
    std::cout << "Resolution:\t\t\t\t\t\t\t\t"
              << i.r[0] << 'x' << i.r[1] << 'x' << i.r[2]
              << std::endl;
    std::cout << "# of cells:\t\t\t\t\t\t\t\t"
              << i.r[0] * i.r[1] * i.r[2]
              << std::endl;
    std::cout << "# of non-empty cells:\t\t\t\t\t"
              << i.nfc
              << std::endl;
    std::cout << "Average number of primitives per cell:\t"
              << i.npnc
              << std::endl;
  } else if (type == compact_grid) {
    std::cout << "compact grid" << std::endl;
    std::cout << "Resolution:\t\t\t\t\t\t\t\t"
              << i.r[0] << 'x' << i.r[1] << 'x' << i.r[2]
              << std::endl;
    std::cout << "# of cells:\t\t\t\t\t\t\t\t"
              << i.r[0] * i.r[1] * i.r[2]
              << std::endl;
    std::cout << "# of non-empty cells:\t\t\t\t\t"
              << i.nfc
              << std::endl;
    std::cout << "Average number of primitives per cell:\t"
              << i.npnc
              << std::endl;
  }

  std::cout << "----------" << std::endl;
  std::cout << std::endl;
}

//==============================================================================
void Scene::print_tm_loading_info(const mesh_loading_info &li,
                           const size_t &loading_time,
                           const std::string &fn) {
  std::cout << "Done loading '"
            << fn << "'." << std::endl;
  std::cout << "Loading time:\t\t\t\t\t\t\t"
            << loading_time << "ms" << std::endl;
  std::cout << "# of vertices in the mesh:\t\t\t\t"
            << li.nv << std::endl;
  std::cout << "# of vertex normals in the mesh:\t\t"
            << li.nvn << std::endl;
  std::cout << "# of triangles in the mesh:\t\t\t\t"
            << li.nt << std::endl;
  std::cout << "# of faces in the mesh:\t\t\t\t\t"
            << li.nf << std::endl;
  std::cout << "----------" << std::endl;
  std::cout << std::endl;
}

//==============================================================================
void Scene::print_render_info(const render_info &ri,
                              const size_t &render_time) {
  std::cout << "Done rendering." << std::endl;
  std::cout << "Rendering time:\t\t\t\t\t\t\t"
            << render_time << "sec" << std::endl;
  std::cout << "# of primary rays:\t\t\t\t\t\t"
            << ri.npr << std::endl;
  std::cout << "# of shadow rays:\t\t\t\t\t\t"
            << ri.nsr << std::endl;
  std::cout << "# of reflection rays:\t\t\t\t\t"
            << ri.nrr << std::endl;
  std::cout << "# of refraction rays:\t\t\t\t\t"
            << ri.nrrr << std::endl;
  std::cout << "# of ray-primitive intersection tests:\t"
            << ri.nrpt << std::endl;
  std::cout << "# of ray-object intersections:\t\t\t"
            << ri.nroi << std::endl;
  std::cout << "ratio (isect tests / isect):\t\t\t"
            << (1.f * ri.nrpt) / ri.nroi << std::endl;
  std::cout << "----------" << std::endl;
  std::cout << std::endl;
}

//==============================================================================
void Scene::print_scene_info(const scene_info &i) {
  std::cout << "Scene consists of:" << std::endl;
  std::cout << "# of primitives:\t\t\t\t\t\t"
            << i.np << std::endl;
  std::cout << "# of objects:\t\t\t\t\t\t\t"
            << i.no << std::endl;
  std::cout << "# of light sources:\t\t\t\t\t\t"
            << i.nl << std::endl;
  std::cout << "----------" << std::endl;
  std::cout << std::endl;
}

//==============================================================================
void Scene::prepare_scene() {
  // Compute scene's bounding box.
  extend_scene_bb();

  if (acceleration_structure != nullptr) {
    // Construct acceleration structure.
    as_construct_info info;
    auto sc   = std::chrono::high_resolution_clock::now();
    acceleration_structure->construct(scene_bb, objects, si.np, info);
    auto fc   = std::chrono::high_resolution_clock::now();
    info.d = std::chrono::duration_cast<std::chrono::milliseconds>(fc - sc).count();
    print_as_construction_info(info, acceleration_structure->get_type());
  }
}

//==============================================================================
void Scene::render_image() {
  Ray     primary_ray;
  std::vector<ip_sample> ip_samples;

  // Apply inverse view transform on objects and light sources.
  camera->apply_inverse_view_transform(objects, lights);

  prepare_scene();

  std::unique_ptr<Renderer> renderer(new Renderer(acceleration_structure,
                                                  scene_bb,
                                                  objects,
                                                  lights));

  auto sr = std::chrono::high_resolution_clock::now();
  // Iterate through the image plane's pixels starting from the top left
  // corner.
  for (uint32_t row = 0; row < image_plane->vres; row++) {
    for (uint32_t col = 0; col < image_plane->hres; col++) {
      ip_samples.clear();

      for (auto const &sample : image_plane->us) {
        primary_ray = camera->get_ray(col, row, sample.x, sample.y);

        auto sample_radiance = renderer->cast_ray(primary_ray, max_depth);
        auto sample_position = glm::vec2(col + sample.x, row + sample.y);

        ip_samples.emplace_back(sample_radiance, sample_position);
      }

      auto pixel_radiance = evaluate_sinc_filter(ip_samples, col, row, 1.f, 1.f);
      image_plane->fb[row*image_plane->hres + col] = pixel_radiance;
    }
  }

  // Print render time and statistics.
  auto fr = std::chrono::high_resolution_clock::now();
  auto rd = std::chrono::duration_cast<std::chrono::seconds>(fr - sr).count();
  auto ri = renderer->finished();
  print_render_info(ri, rd);

  // Reverse inverse view transform.
  camera->reverse_inverse_view_transform(objects, lights);

  auto rendered_scene_name = name + ".png";
  image_plane->save_to_png(rendered_scene_name);
}

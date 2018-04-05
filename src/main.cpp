// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include <iostream>
#include <memory>
#include "glm/ext.hpp"    // glm::to_string

#include "objects/Object.h"
#include "objects/Sphere.h"
#include "accelerators/CompactGrid.h"
#include "core/Scene.h"

int main(int argc, char **argv) {
//  std::shared_ptr<Object> s1 = std::make_shared<Sphere>(Sphere());
//  std::static_pointer_cast<Sphere>(s1)->set_radius(1.f);
//  std::static_pointer_cast<Sphere>(s1)->set_center({-1.1f, 0.f, 0.f, 1.f});
//
//  std::shared_ptr<Object> s2 = std::make_shared<Sphere>(Sphere());
//  std::static_pointer_cast<Sphere>(s2)->set_radius(1.f);
//  std::static_pointer_cast<Sphere>(s2)->set_center({1.1f, 0.f, 0.f, 1.f});
//
//  AABBox gridBounds;
//  gridBounds.extend_by(std::static_pointer_cast<Sphere>(s1)->bounding_box().bounds[0]);
//  gridBounds.extend_by(std::static_pointer_cast<Sphere>(s1)->bounding_box().bounds[1]);
//  gridBounds.extend_by(std::static_pointer_cast<Sphere>(s2)->bounding_box().bounds[0]);
//  gridBounds.extend_by(std::static_pointer_cast<Sphere>(s2)->bounding_box().bounds[1]);
//
//  std::cout << "Min bound: " << glm::to_string(gridBounds.bounds[0]) << std::endl;
//  std::cout << "Max bound: " << glm::to_string(gridBounds.bounds[1]) << std::endl;
//
//  std::vector<std::shared_ptr<Object>> objs;
//  objs.push_back(s1);
//  objs.push_back(s2);
//
//  auto i = as_construct_info();
//  CompactGrid *cg = new CompactGrid();
//  cg->construct(gridBounds, objs, 4, i);
//
//  std::cout << "DynamicGrid's resolution:\t\t\t\t\t\t"
//            << i.r[0] << 'x' << i.r[1] << 'x' << i.r[2]
//            << std::endl;
//  std::cout << "# of cells:\t\t\t\t\t\t\t\t"
//            << i.r[0] * i.r[1] * i.r[2]
//            << std::endl;
//  std::cout << "# of non-empty cells:\t\t\t\t\t"
//            << i.nfc
//            << std::endl;
//  std::cout << "Average number of primitives per cell:\t"
//            << i.npnc
//            << std::endl;

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <scene file>" << std::endl;
    exit(1);
  }

  std::string filename = argv[1];
  auto result = read_scene_from_file(filename);
  if (result.first.first == success) {
    auto scene = new Scene();
    auto ret = scene->load_scene(result.second[0]);
    if (ret)
      scene->render_image();
  } else {
    std::cout << "Line " << result.first.second << ": "
              << STATUS_CODES_MAP.at(result.first.first) << std::endl;
  }
  return 0;
}

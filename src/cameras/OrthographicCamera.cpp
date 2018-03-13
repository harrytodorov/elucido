// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "OrthographicCamera.h"

//=============================================================================
render_info OrthographicCamera::render_scene(const std::vector<Object *> &objects,
                                             const std::vector<Light *> &lights,
                                             ImagePlane &ip) {
  float_t curr_x;
  float_t curr_y;
  float_t ar;                                     // image plane's aspect ratio
  Ray ray;
  glm::vec3 pc;                                     // pixel color
  render_info ri;                                     // rendering information
  std::random_device
      rd;                                     // obtain a random number from hardware
  std::mt19937 eng(rd());                              // seed generator
  std::uniform_real_distribution<float_t>
      pr(0.05f, 0.95f);   // define ranges for pixel x/y
  AccelerationStructure *as;

  // position all objects in the scene relative to the camera's position at the origin; inverse view transformation
  apply_inverse_view_transform(objects, lights);

  // reshape scene's bounding box
  extend_scene_bb(objects);

  if (use_as) {
    // Initialize the accelerators structure.
    as = new Grid(scene_bb, objects);
    grid_info i;

    // Set grid's alpha parameter.
    (static_cast<Grid*>(as))->setAlpha(grid_alpha);

    // Print some useful information regarding grid's construction.
    auto startConstruction = std::chrono::high_resolution_clock::now();
    i = (static_cast<Grid*>(as))->constructGrid();
    auto finishConstruction = std::chrono::high_resolution_clock::now();
    std::cout << std::endl;
    std::cout << "Grid's construction time: " <<
              std::chrono::duration_cast<std::chrono::milliseconds>
                  (finishConstruction - startConstruction).count() << "ms"
              << std::endl;
    std::cout << "Grid's alpha: " << (static_cast<Grid*>(as))->getAlpha() << std::endl;
    std::cout << "Grid's resoultion: " << i.r[0] << 'x' << i.r[1] << 'x' << i.r[2]
              << std::endl;
    std::cout << "Number of cells: " << i.r[0]*i.r[1]*i.r[2] << std::endl;
    std::cout << "Number of  primitives: " << i.np << std::endl;
    std::cout << "Number of non-empty cells: " << i.nfc << std::endl;
    std::cout << "Average number of primitives per cell: " << i.nppc << std::endl;
    std::cout << std::endl;
  }

  // Set the ray direction same as the direction of the camera
  // and normalize it, shouldn't be necessary, but just in case
  ray.set_dir(glm::normalize(lookat - eye));

  // get pointer to the frame buffer
  glm::vec3 *pixels = ip.fb;

  // calculate the image plane's aspect ratio
  // force float division
  ar = (ip.hres * 1.f) / ip.vres;

  for (int r = 0; r < ip.vres; r++) {
    for (int c = 0; c < ip.hres; c++) {

      // set color for pixel black before collecting color at sample points
      pc = black;

      // use half-jittered sampling to reduce aliasing artifacts
      for (uint32_t ny = 0; ny < ip.ns; ny++) {
        for (uint32_t nx = 0; nx < ip.ns; nx++) {

          // calculate the x/y coordinates for each sample per pixel;
          // the image plane is positioned orthogonal to the z-plane and is placed at (0, 0, -1)
          // we also incorporate a scaling factor for the orthographic camera, which scales the
          // the pixel cell in world coordinate space; this would be used for zooming with orthographic camera
          curr_x =
              (2.f * ((c * ip.ns + nx + pr(eng)) / (ip.hres * ip.ns)) - 1.f)
                  * ar * zf;
          curr_y =
              (1.f - 2.f * ((r * ip.ns + ny + pr(eng)) / (ip.vres * ip.ns)))
                  * zf;

          // set the ray origin for each sample position
          ray.set_orig(glm::vec4(curr_x, curr_y, 0.f, 1.f));

          if (use_as) {
            // cast a ray into the scene and get the color value for it
            // with accelerators structure
            pc += cast_ray(ray, lights, objects, 0, as, ri);
          } else {
            // cast a ray into the scene and get the color value for it
            // without accelerators structure
            pc += cast_ray(ray, lights, objects, 0, ri);
          }
        }
      }

      // use simple box filter to get the average of the samples per pixel
      pc /= ip.ns * ip.ns;

      *(pixels++) = glm::clamp(pc, 0.f, 1.f);
    }
//        printf("row %d of %d is computed.\n", r+1, ip.vres);
  }

  // reverse the inverse view transform; bring objects to their original positions
  reverse_inverse_view_transform(objects, lights);

  // get rendering information
  ri.nls = (uint32_t) lights.size();
  ri.no = (uint32_t) objects.size();

  return ri;
}

Ray OrthographicCamera::get_ray(const uint32_t &pixel_x,
                                const uint32_t &pixel_y,
                                const float_t &sample_x,
                                const float_t &sample_y,
                                const uint32_t &width,
                                const uint32_t &height) {
  auto ar = (width * 1.f) / height;
  auto x = 2.f * ((pixel_x + sample_x) / width) * ar * zf;
  auto y = (1.f - 2.f * ((pixel_y + sample_y) / height)) * zf;
  auto o = glm::vec4(x, y, eye.z, 1.f);

  Ray r;
  r.rt = primary;
  r.set_orig(o);
  r.set_dir(glm::normalize(lookat - eye));

  return r;
}

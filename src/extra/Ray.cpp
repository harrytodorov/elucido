// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Ray.h"
#include "../objects/TriangleMesh.h"

//==============================================================================
bool Ray::trace(const std::vector<Object *> &objects,
                isect_info &ii,
                render_info &ri) const {
  ii = isect_info();

  // increment number of shadow rays
  if (this->rt == shadow) __sync_fetch_and_add(&ri.nsr, 1);

  // increment number of reflection rays
  if (this->rt == reflection) __sync_fetch_and_add(&ri.nrr, 1);

  // increment number of refraction rays
  if (this->rt == refraction) __sync_fetch_and_add(&ri.nrrr, 1);

  // iterate through objects and find the closest intersection
  for (auto &object : objects) {
    // information we got from the intersection with the current object
    isect_info co;

    // increment the number of ray-object tests; bounding box
    __sync_fetch_and_add(&ri.nrpt, 1);

    // first intersect with object's bounding box
    // we only intersect triangulated meshes with their bounding boxes,
    // because tests for spheres and triangles are already cheap enough
    if (object->object_type() == triangle_mesh && !object->bounding_box().intersect(*this))
      continue;

    // for a triangulated mesh, the number of ray-primitive intersections
    // is equal to the number of triangles in the mesh
    if (object->object_type() == triangle_mesh)
      // increment the number of ray-primitive tests; object itself
      __sync_fetch_and_add(&ri.nrpt,
                           dynamic_cast<TriangleMesh *>(object)->nt);
    else
      // increment the number of ray-primitive tests; object itself
      __sync_fetch_and_add(&ri.nrpt, 1);

    // if we have an intersection with the object's bounding box,
    // then we try to intersect with the object itself
    if (!object->intersect(*this, co) || co.tn > ii.tn) continue;

    // in case there is an intersection and the nearest distance
    // is closer to the nearest distance we already have, we
    // update our intersection
    ii = co;
    ii.ho = object;

    // increment the number of ray-object intersections
    __sync_fetch_and_add(&ri.nroi, 1);
  }

  return (ii.ho != nullptr);
}

//==============================================================================
void Ray::set_dir(const glm::vec4 &_d) {
  this->d = _d;
  id = 1.f / d;
  s[0] = (uint32_t) (id.x < 0);
  s[1] = (uint32_t) (id.y < 0);
  s[2] = (uint32_t) (id.z < 0);
}

//
// Created by Haralambi Todorov on 18/05/2017.
//

#include "Ray.h"

bool Ray::trace(const std::vector<Object *> &objects, isect_info &ii, render_info &ri) {
    // reset the information stored in the intersection structure
    ii = isect_info();

    // increment number of shadow rays
    if (this->rt == shadow) ri.shadow_rays++;

    // iterate through objects and find the closest intersection
    for (auto& object : objects) {
        // information we got from the intersection with the current object
        isect_info co;

        // increment the number of ray-object tests; bounding box
        __sync_fetch_and_add(&ri.num_of_ray_object_tests, 1);

        // first intersect with object's bounding box
        if (object->bb.intersect(*this)) {

        // increment the number of ray-object tests; object itself
        __sync_fetch_and_add(&ri.num_of_ray_object_tests, 1);

            // if we have an intersection with the object's bounding box, then we try to intersect
            // with the object itself
            if (object->intersect(*this, co) && co.tn < ii.tn) {
                // in case there is an intersection and the nearest distance is closer to the nearest
                // distance we already have, we update our intersection
                ii = co;
                ii.ho = object;

                // increment the number of ray-object intersections
                __sync_fetch_and_add(&ri.num_of_ray_object_intersections, 1);

            }
        }
    }

    return (ii.ho != nullptr);
}

//
// Created by Haralambi Todorov on 18/05/2017.
//

#include "Ray.h"

bool Ray::trace(const std::vector<Object *> &objects, isect_info &ii) {
    // reset the information stored in the intersection structure
    ii = isect_info();

    // iterate through objects and find the closest intersection
    for (auto& object : objects) {
        isect_info co;      // information we got from the intersection with the current object

        // first intersect with object's bounding box
        if (object->bb.intersect(*this)) {

            // if we have an intersection with the object's bounding box, then we try to intersect
            // with the object itself
            if (object->intersect(*this, co) && co.tn < ii.tn) {
                // in case there is an intersection and the nearest distance is closer to the nearest
                // distance we already have, we update our intersection
                ii = co;
                ii.ho = object;
            }
        }
    }

    return (ii.ho != nullptr);
}

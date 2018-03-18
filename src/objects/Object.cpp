// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Object.h"

//==============================================================================
Object::Object(const Object &o) {
  this->om = o.om;
  this->mt = o.mt;
  this->bb = o.bb;
  this->ot = o.ot;
}

//==============================================================================
void Object::translate(const float_t &translation,
                       const Axis &translation_axis) {
  apply_translation(translation_axis, translation, mt);
}

//==============================================================================
void Object::rotate(const float_t &angle_of_rotation,
                    const Axis &rotation_axis) {
  apply_rotation(rotation_axis, angle_of_rotation, mt);
}

//==============================================================================
void Object::scale(const float_t &scaling_factor,
                   const Axis &scale_axis) {
  apply_scale(scale_axis, scaling_factor, mt);
}
void Object::get_surface_properties(isect_info &i) const {
  // TODO: To be removed.
}

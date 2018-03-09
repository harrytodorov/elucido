// Copyright 2017, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "Object.h"

//=============================================================================
Object::Object(const Object &o) {
  this->om = o.om;
  this->mt = o.mt;
  this->bb = o.bb;
  this->ot = o.ot;
}

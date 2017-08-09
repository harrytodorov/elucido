//
// Created by Haralambi Todorov on 19/05/2017.
//

#include "Object.h"

Object::Object(const Object &o) {
    this->om = o.om;
    this->mt = o.mt;
    this->nmt = o.nmt;
    this->bb = o.bb;
    this->ot = o.ot;
}

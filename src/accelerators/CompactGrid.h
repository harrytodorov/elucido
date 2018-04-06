// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_COMPACTGRID_H
#define ELUCIDO_COMPACTGRID_H

#include "AccelerationStructure.h"
#include "Grid.h"

class CompactGrid : public AccelerationStructure, public Grid {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  CompactGrid() :
      AccelerationStructure(),
      Grid()
  {
    as_type = compact_grid;
  }

  ~CompactGrid() {
    delete[] cells;
    delete[] object_lists;
  };

  //==============================================================================
// Function declarations
//==============================================================================
  void            construct(const AABBox &box,
                            const std::vector<std::shared_ptr<Object>> &objects,
                            const uint32_t &number_primitives,
                            as_construct_info &info);
  bool            traverse(const Ray &r, isect_info &ii) const;

//==============================================================================
// Data members
//==============================================================================
 private:
  uint32_t  *object_lists;
  uint32_t  *cells;
};

#endif //ELUCIDO_COMPACTGRID_H

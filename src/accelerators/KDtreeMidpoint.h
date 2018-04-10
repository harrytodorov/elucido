// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#ifndef ELUCIDO_KDTREE_H
#define ELUCIDO_KDTREE_H

#include <vector>
#include <utility>

#include "AccelerationStructure.h"
#include "AABBox.h"

struct KDNode {
//==============================================================================
// Constructors & destructors
//==============================================================================
  KDNode() {}

  ~KDNode() {}

//==============================================================================
// Function declarations
//==============================================================================
  inline void initialize_leaf(const std::vector<Primitive> &_overlapping_primitives) {
    is_leaf                 = true;
    overlapping_primitives  = _overlapping_primitives;
  }

  inline void initialize_interior(const AABBox &overlapping_box) {
    is_leaf = false;
    bbox    = overlapping_box;
  }

  inline void set_left_child(const uint32_t &leftnode_index) {
    left_child_index = leftnode_index;
  }
  inline const uint32_t & left_child() const { return left_child_index; }

  inline void set_right_child(const uint32_t &rightnode_index) {
    right_child_index = rightnode_index;
  }
  inline const uint32_t & right_child() const { return right_child_index; }

  inline bool leaf() { return is_leaf; }

  inline const AABBox& box() const { return bbox; }
  inline bool intersect(const Ray &r, isect_info &i) const {
    for (auto const& p : overlapping_primitives) {
      isect_info cp;

      if (p.intersect(r, cp) && cp.tn < i.tn) {
        i = cp;
        i.ho = p.obj;
        if (i.ho->object_type() == triangle_mesh) {
          i.ti = p.tri_ind;
        }
      }
    }

    return (i.ho != nullptr);
  }
  inline uint32_t primitives_size() const {
    return overlapping_primitives.size();
  }

//==============================================================================
// Data members
//==============================================================================
 private:
  bool                    is_leaf{false};
  std::vector<Primitive>  overlapping_primitives{};
  AABBox                  bbox{};
  uint32_t                left_child_index{0};
  uint32_t                right_child_index{0};
};

class KDtreeMidpoint : public AccelerationStructure {
//==============================================================================
// Constructors & destructors
//==============================================================================
 public:
  KDtreeMidpoint() :
    AccelerationStructure(),
    nodes(std::vector<std::unique_ptr<KDNode>>())
  {
    as_type = kdtree_midpoint;
  }

  ~KDtreeMidpoint() {}
//==============================================================================
// Function declarations
//==============================================================================
  void            construct(const AABBox &box,
                            const std::vector<std::shared_ptr<Object>> &objects,
                            const uint32_t &number_primitives,
                            as_construct_info &info);
  void build_node(const std::vector<Primitive> &overlapping_primitives,
                    const AABBox &overlapping_box,
                    const uint32_t &depth);

  inline void set_max_depth(const uint32_t &d) { max_tree_depth = d; }
  inline void set_max_primitives(const uint32_t &mp) { max_primitves = mp; }

  bool            traverse(const Ray &r, isect_info &ii) const;

//==============================================================================
// Data members
//==============================================================================
 private:
  std::vector<std::unique_ptr<KDNode>>  nodes;
  uint32_t                              next_free_position{0};
  uint32_t                              max_tree_depth{0};
  uint32_t                              max_primitves{5};
};

#endif //ELUCIDO_KDTREE_H

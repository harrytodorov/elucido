// Copyright (c) 2018, University of Freiburg.
// Author: Haralambi Todorov <harrytodorov@gmail.com>

#include "KDtreeMidpoint.h"
#include <deque>

//==============================================================================
bool KDtreeMidpoint::traverse(const Ray &r, isect_info &ii) const {
  // Scalar distance from the ray's origin to the nearest hit point of
  // the ray with the tree.
  float_t   tBoundingBox;
  uint64_t  intersected_primitives{0};

  // Check if the ray intersect's the tree at all.
  if (!bbox.intersect(r, tBoundingBox))
    return false;

  // Reset the intersection information.
  ii = isect_info();

  // Initially put just the root node of the tree.
  std::deque<uint32_t> nodes_to_intersect = {0};

  bool intersected = false;

  while (!nodes_to_intersect.empty()) {
    uint32_t cn = nodes_to_intersect.front();
    nodes_to_intersect.pop_front();

    // Process leaf node.
    if (nodes[cn]->leaf()) {
      if (nodes[cn]->intersect(r, ii)) {
        intersected = true;
      }
      intersected_primitives += nodes[cn]->primitives_size();
    // Process interior node.
    } else {
      if (nodes[cn]->box().intersect(r)) {
        nodes_to_intersect.push_back(nodes[cn]->left_child());
        nodes_to_intersect.push_back(nodes[cn]->right_child());
      }
    }
  }

  ii.nrpt = intersected_primitives;
  return intersected;
}

//==============================================================================
void KDtreeMidpoint::construct(const AABBox &box,
                       const std::vector<std::shared_ptr<Object>> &objects,
                       const uint32_t &number_primitives,
                       as_construct_info &info) {

  // Compute primitives.
  compute_primitives(number_primitives, objects);

  bbox.bounds[0] = box.bounds[0];
  bbox.bounds[1] = box.bounds[1];

  // Compute maximum tree depth according to formula:
  //  8 + 1.3 * log(N), where N is # of primitives.
  if (max_tree_depth <= 0) {
    max_tree_depth = static_cast<uint32_t>(8.f +
        1.3f * glm::log(number_primitives));
  }

  // Reserve space for nodes.
  auto max_number_nodes = static_cast<size_t>(glm::pow(2, max_tree_depth + 1));
  nodes.reserve(max_number_nodes - 1);

  // Construct tree recursively.
  build_node(primitives, bbox, 0);

  // Free unused space for nodes.
  nodes.shrink_to_fit();
}

//==============================================================================
void KDtreeMidpoint::build_node(const std::vector<Primitive> &overlapping_primitives,
                                const AABBox &overlapping_box,
                                const uint32_t &depth) {
  // Initialize leaf node, if termination criteria are met.
  if (overlapping_primitives.size() <= max_primitves ||
      depth == max_tree_depth) {
    std::unique_ptr<KDNode> node(new KDNode());
    node->initialize_leaf(overlapping_primitives);
    nodes.push_back(std::move(node));
    next_free_position++;
    return;
  }

  // Compute midpoint of overlapping primitives.
  glm::vec4 midpoint{0.f};
  for (auto const &primitive : overlapping_primitives) {
    midpoint += primitive.centroid();
  }
  midpoint /= static_cast<float_t>(overlapping_primitives.size());

  std::vector<Primitive> opl;
  std::vector<Primitive> opr;
  Axis split_axis = overlapping_box.longestAxis();

  // Classify primitives with respect to split axis.
  for (auto const &p : overlapping_primitives) {
    switch (split_axis) {
      case Axis::X :
        (midpoint.x >= p.centroid().x) ? opr.push_back(p) : opl.push_back(p);
        break;

      case Axis::Y :
        (midpoint.y >= p.centroid().y) ? opr.push_back(p) : opl.push_back(p);
        break;

      case Axis::Z :
        (midpoint.z >= p.centroid().z) ? opr.push_back(p) : opl.push_back(p);
        break;

      default: break;
    }
  }

  // Initialize interior node.
  std::unique_ptr<KDNode> node(new KDNode());
  node->initialize_interior(overlapping_box);
  nodes.push_back(std::move(node));
  uint32_t parent_node_position = next_free_position;
  next_free_position++;

  // Compute bounding box for left child and recursively initialize the node.
  AABBox lbb;
  for (auto const &p : opl) {
    lbb.extend_by(p.getBB().bounds[0]);
    lbb.extend_by(p.getBB().bounds[1]);
  }
  nodes[parent_node_position]->set_left_child(next_free_position);
  build_node(opl, lbb, depth + 1);

  // Compute bounding box for right child and recursively initialize the node.
  AABBox rbb;
  for (auto const &p : opr) {
    rbb.extend_by(p.getBB().bounds[0]);
    rbb.extend_by(p.getBB().bounds[1]);
  }
  nodes[parent_node_position]->set_right_child(next_free_position);
  build_node(opr, rbb, depth + 1);
}

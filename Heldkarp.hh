#ifndef HELDKARP_HH_INCLUDED
#define HELDKARP_HH_INCLUDED

#include <set>

#include "Laby.hh"

namespace held_karp {

typedef uint64_t Cost;
typedef uint8_t NodeId;
typedef uint32_t NodeBitSet;

/**
 * Number of nodes
 */
const NodeId NODE_COUNT = MUSTPASSNB + 1;

const NodeBitSet EMPTY_BIT_SET = 0;
const NodeBitSet FULL_BIT_SET = (1 << NODE_COUNT) - 1;

/**
 * Infinite cost value
 */
const Cost INFINITY = 1000000000;

/**
 * Dist[src_node_id][dest_node_id] = cost
 */
typedef std::array<std::array<Cost, NODE_COUNT>, NODE_COUNT> Dist;

struct MemoItem {
  Cost cost;
  NodeId parent;
};

/**
 * Memo[visited_bitset][last_node_id] = {cost, parent}
 */
typedef std::array<std::array<MemoItem, NODE_COUNT>, 1 << NODE_COUNT> Memo;

struct State {
  NodeBitSet visited;
  NodeId last;
  const State delete_visited(NodeId node_id) noexcept;
  bool is_visited(NodeId node_id) noexcept;
};
}

class Heldkarp {
 private:
  held_karp::Dist dist;
  held_karp::Memo memo;

  /**
   * Returns the cost of the provided state.
   */
  held_karp::Cost cost(held_karp::State state);
 public:
  explicit Heldkarp(held_karp::Dist dist);
  held_karp::Cost get(held_karp::NodeId end);
};

#endif // HELDKARP_HH_INCLUDED

#include "Heldkarp.hh"

namespace held_karp {
const State State::delete_visited(NodeId node_id) noexcept {
  return State{this->visited & ~(1 << node_id), this->last};
}
bool State::is_visited(NodeId node_id) noexcept {
  return (this->visited & (1 << node_id)) != 0;
}
}

Heldkarp::Heldkarp(held_karp::Dist dist) : dist(dist) {}

held_karp::Cost Heldkarp::get(held_karp::NodeId end) {
  held_karp::State state = held_karp::State{held_karp::FULL_BIT_SET, end};
  return this->cost(state);
}

held_karp::Cost Heldkarp::cost(held_karp::State state) {
  if (this->memo[state.visited][state.last].cost != 0) {
    return this->memo[state.visited][state.last].cost;
  }
  if (state.visited == held_karp::EMPTY_BIT_SET) {
    held_karp::Cost cost = 0;
    this->memo[state.visited][state.last] = {cost, state.last};
    return cost;
  }
  held_karp::Cost best_cost = held_karp::INFINITY;
  held_karp::NodeId best_parent = 0;
  for (held_karp::NodeId node_id = 0; node_id < held_karp::NODE_COUNT; node_id++) {
    if (state.is_visited(node_id)) {
      held_karp::Cost last_component_cost = this->dist[node_id][state.last];
      if (last_component_cost >= held_karp::INFINITY) {
        continue;
      }
      held_karp::Cost cost = this->cost(state.delete_visited(node_id)) + last_component_cost;
      if (cost < best_cost) {
        best_cost = cost;
        best_parent = node_id;
      }
    }
  }
  this->memo[state.visited][state.last] = {best_cost, best_parent};
  return best_cost;
}

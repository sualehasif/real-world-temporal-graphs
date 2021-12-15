#pragma once

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"

#include <algorithm>
#include <cstdint>

template <class node_t = uint32_t, bool triangle_count = true>
class AppendOnlyGraph {
private:
  absl::flat_hash_map<node_t, absl::flat_hash_set<node_t>> graph;
  uint64_t num_edges_ = 0;
  uint64_t max_degree_ = 0;
  uint64_t triangle_count_ = 0;

public:
  // returns true if the edge was added, false if it was already in the graph
  bool add_edge(node_t source, node_t dest) {
    bool inserted = graph[source].insert(dest).second;
    graph[dest].insert(source);
    if (inserted) {
      num_edges_ += 1;
      max_degree_ = std::max(graph[source].size(), max_degree_);
      if constexpr (triangle_count) {
        for (const auto &it : graph[source]) {
          if (graph[dest].contains(it)) {
            triangle_count_ += 1;
          }
        }
      }
    }
    return inserted;
  }
  uint64_t degree(node_t i) {
    auto it = graph.find(i);
    if (it == nullptr) {
      return 0;
    } else {
      return it->size();
    }
  }
  uint64_t num_nodes() { return graph.size(); }
  uint64_t num_edges() { return num_edges_; }
  double average_degree() {
    return static_cast<double>(num_edges()) / num_nodes();
  }
  uint64_t max_degree() { return max_degree_; }
  uint64_t num_triangles() { return triangle_count_; }
};
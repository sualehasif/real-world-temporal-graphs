#pragma once
#include "graph_generators.hpp"
#include "io_util.hpp"

#include "absl/container/flat_hash_map.h"
#include "absl/container/flat_hash_set.h"

#include <algorithm>
#include <cstdint>
#include <vector>

template <class node_t = uint32_t, bool triangle_count = true>
class AppendOnlyGraph {
private:
  std::vector<absl::flat_hash_set<node_t>> graph;
  uint64_t num_edges_ = 0;
  uint64_t num_nodes_ = 0;
  uint64_t max_degree_ = 0;
  uint64_t triangle_count_ = 0;

public:
  // returns true if the edge was added, false if it was already in the graph
  bool add_edge(node_t source, node_t dest) {
    auto max_node = std::max(source, dest);
    if (max_node >= graph.size()) {
      graph.resize(max_node + 1);
    }
    if (graph[source].size() == 0) {
      num_nodes_ += 1;
    }
    if (graph[dest].size() == 0) {
      num_nodes_ += 1;
    }
    bool inserted = graph[source].insert(dest).second;
    graph[dest].insert(source);
    if (inserted) {
      num_edges_ += 1;
      max_degree_ = std::max((uint64_t)graph[source].size(), max_degree_);
      if constexpr (triangle_count) {
        if (graph[source].size() <= graph[dest].size()) {
          for (const auto &it : graph[source]) {
            if (graph[dest].contains(it)) {
              triangle_count_ += 1;
            }
          }
        } else {
          for (const auto &it : graph[dest]) {
            if (graph[source].contains(it)) {
              triangle_count_ += 1;
            }
          }
        }
      }
    }
    return inserted;
  }
  uint64_t degree(node_t i) {
    if (i >= graph.size()) {
      return 0;
    }
    return graph[i].size();
  }
  uint64_t num_nodes() { return num_nodes_; }
  uint64_t num_edges() { return num_edges_; }
  double average_degree() {
    return static_cast<double>(num_edges()) / num_nodes();
  }
  uint64_t max_degree() { return max_degree_; }
  uint64_t num_triangles() { return triangle_count_; }

  template <class timestamp_t = uint32_t>
  static std::pair<std::vector<std::tuple<node_t, node_t, timestamp_t>>, node_t>
  get_graph_edges(const std::string &command, node_t num_nodes,
                  uint64_t rmat_num_edges, double rmat_a, double rmat_b,
                  double rmat_c, double er_p, uint64_t ws_K, double ws_beta,
                  const std::string &input_filename, bool shuffle) {
    std::vector<std::tuple<node_t, node_t, timestamp_t>> edges;
    if (command == "rmat") {
      edges = generate_rmat(num_nodes, rmat_num_edges, rmat_a, rmat_b, rmat_c);
    } else if (command == "er") {
      edges = generate_er(num_nodes, er_p);
    } else if (command == "ws") {
      edges = generate_watts_strogatz(num_nodes, ws_K, ws_beta);
    } else if (command == "adj") {
      uint64_t num_edges;
      edges =
          get_edges_from_file_adj_sym(input_filename, &num_nodes, &num_edges);
    } else if (command == "adj_rmat") {
      uint64_t num_edges;
      edges =
          get_edges_from_file_adj_sym(input_filename, &num_nodes, &num_edges);
      auto edges2 =
          generate_rmat(num_nodes, rmat_num_edges, rmat_a, rmat_b, rmat_c);
      edges.insert(edges.end(), edges2.begin(), edges2.end());
    } else if (command == "adj_er") {
      uint64_t num_edges;
      edges =
          get_edges_from_file_adj_sym(input_filename, &num_nodes, &num_edges);
      auto edges2 = generate_er(
          num_nodes, static_cast<double>(num_edges) /
                         (static_cast<uint64_t>(num_nodes) * num_nodes));
      edges.insert(edges.end(), edges2.begin(), edges2.end());
    } else if (command == "adj_ws") {
      uint64_t num_edges;
      edges =
          get_edges_from_file_adj_sym(input_filename, &num_nodes, &num_edges);
      auto edges2 =
          generate_watts_strogatz(num_nodes, num_edges / num_nodes, ws_beta);
      edges.insert(edges.end(), edges2.begin(), edges2.end());
    } else if (command == "edges") {
      edges = get_edges_from_file_edges<node_t>(input_filename, shuffle);
      for (const auto &e : edges) {
        num_nodes = std::max(num_nodes, std::get<0>(e));
        num_nodes = std::max(num_nodes, std::get<1>(e));
      }
    } else if (command == "edges_bin") {
      edges = get_binary_edges_from_file_edges(input_filename, shuffle);
      for (const auto &e : edges) {
        num_nodes = std::max(num_nodes, std::get<0>(e));
        num_nodes = std::max(num_nodes, std::get<1>(e));
      }
    } else {
      std::cout << "command not found was: " << command << std::endl;
      num_nodes = 0;
    }

    return {std::move(edges), num_nodes + 1};
  }
  template <class timestamp_t = uint32_t>
  static std::pair<std::vector<std::tuple<node_t, node_t, timestamp_t>>, node_t>
  get_graph_edges(const std::string &input_filename) {
    if (ends_with(input_filename, "bin")) {
      return get_graph_edges("edges_bin", 0, 0, 0, 0, 0, 0, 0, 0,
                             input_filename, false);
    }
    return get_graph_edges("edges", 0, 0, 0, 0, 0, 0, 0, 0, input_filename,
                           false);
  }
};

template <class node_t = uint32_t, bool triangle_count = true> class Graph {
private:
  std::vector<absl::flat_hash_set<node_t>> graph;
  uint64_t num_edges_ = 0;
  uint64_t num_nodes_ = 0;
  // degree, id
  std::set<std::pair<node_t, node_t>> degree_heap;
  uint64_t triangle_count_ = 0;

public:
  // returns true if the edge was added, false if it was already in the graph
  bool add_edge(node_t source, node_t dest) {
    auto max_node = std::max(source, dest);
    if (max_node >= graph.size()) {
      graph.resize(max_node + 1);
    }
    if (graph[source].size() == 0) {
      num_nodes_ += 1;
    }
    if (graph[dest].size() == 0) {
      num_nodes_ += 1;
    }
    bool inserted = graph[source].insert(dest).second;
    graph[dest].insert(source);
    if (inserted) {
      num_edges_ += 1;
      degree_heap.erase({graph[source].size() - 1, source});
      degree_heap.erase({graph[dest].size() - 1, dest});
      degree_heap.insert({graph[source].size(), source});
      degree_heap.insert({graph[dest].size(), dest});
      if constexpr (triangle_count) {
        if (graph[source].size() <= graph[dest].size()) {
          for (const auto &it : graph[source]) {
            if (graph[dest].contains(it)) {
              triangle_count_ += 1;
            }
          }
        } else {
          for (const auto &it : graph[dest]) {
            if (graph[source].contains(it)) {
              triangle_count_ += 1;
            }
          }
        }
      }
    }
    return inserted;
  }
  bool remove_edge(node_t source, node_t dest) {
    auto max_node = std::max(source, dest);
    if (max_node >= graph.size()) {
      graph.resize(max_node + 1);
    }
    if (graph[source].size() == 0) {
      num_nodes_ += 1;
    }
    if (graph[dest].size() == 0) {
      num_nodes_ += 1;
    }
    bool removed = graph[source].erase(dest) > 0;
    graph[dest].erase(source);
    if (removed) {
      num_edges_ -= 1;
      degree_heap.erase({graph[source].size() + 1, source});
      degree_heap.erase({graph[dest].size() + 1, dest});
      degree_heap.insert({graph[source].size(), source});
      degree_heap.insert({graph[dest].size(), dest});
      if constexpr (triangle_count) {
        if (graph[source].size() <= graph[dest].size()) {
          for (const auto &it : graph[source]) {
            if (graph[dest].contains(it)) {
              triangle_count_ -= 1;
            }
          }
        } else {
          for (const auto &it : graph[dest]) {
            if (graph[source].contains(it)) {
              triangle_count_ -= 1;
            }
          }
        }
      }
    }
    return removed;
  }
  uint64_t degree(node_t i) {
    if (i >= graph.size()) {
      return 0;
    }
    return graph[i].size();
  }
  uint64_t num_nodes() { return num_nodes_; }
  uint64_t num_edges() { return num_edges_; }
  double average_degree() {
    return static_cast<double>(num_edges()) / num_nodes();
  }
  uint64_t max_degree() { return degree_heap.rbegin()->first; }
  uint64_t num_triangles() { return triangle_count_; }

  template <class timestamp_t = uint32_t>
  static std::pair<std::vector<std::tuple<bool, node_t, node_t, timestamp_t>>,
                   node_t>
  get_graph_edges(const std::string &command, node_t num_nodes,
                  uint64_t rmat_num_edges, double rmat_a, double rmat_b,
                  double rmat_c, double er_p, uint64_t ws_K, double ws_beta,
                  const std::string &input_filename, bool shuffle) {
    std::vector<std::tuple<bool, node_t, node_t, timestamp_t>> edges;
    if (command == "edges_bin") {
      edges =
          get_binary_edges_from_file_edges_with_remove(input_filename, shuffle);
      for (const auto &e : edges) {
        num_nodes = std::max(num_nodes, std::get<1>(e));
        num_nodes = std::max(num_nodes, std::get<2>(e));
      }
    } else {
      std::cout << "command not found was: " << command << std::endl;
      num_nodes = 0;
    }

    return {std::move(edges), num_nodes + 1};
  }
};
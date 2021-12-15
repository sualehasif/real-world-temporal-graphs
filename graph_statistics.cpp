#include "graph.hpp"
#include "graph_generators.hpp"
#include <cstdlib>

template <class node_t = uint32_t, class timestamp_t = uint32_t>
void graph_statistics(
    std::vector<std::tuple<node_t, node_t, timestamp_t>> edges,
    uint64_t print_freq) {
  AppendOnlyGraph<> g;
  printf("timestep, num_nodes, num_edges, average_degree, max_degree, "
         "num_triangles, "
         "new_edges\n");

  uint64_t i = 0;
  for (; i < edges.size(); i += print_freq) {
    uint64_t count_new = 0;
    for (uint64_t j = i; j < i + print_freq; j++) {
      count_new += g.add_edge(std::get<0>(edges[j]), std::get<1>(edges[j]));
    }
    printf("%lu, %lu, %lu, %f, %lu, %lu, %lu\n", i, g.num_nodes(),
           g.num_edges(), g.average_degree(), g.max_degree(), g.num_triangles(),
           count_new);
  }
}

void get_rmat_statistics(uint64_t num_nodes, uint64_t num_edges,
                         uint64_t print_freq) {
  auto edges = generate_rmat(num_nodes, num_edges);
  graph_statistics(edges, print_freq);
}

void get_er_statistics(uint64_t num_nodes, double p, uint64_t print_freq) {
  auto edges = generate_er(num_nodes, p);
  graph_statistics(edges, print_freq);
}

void get_ws_statistics(uint64_t num_nodes, uint64_t K, double beta,
                       uint64_t print_freq) {
  auto edges = generate_watts_strogatz(num_nodes, K, beta);
  graph_statistics(edges, print_freq);
}

int main(int32_t argc, char *argv[]) {
  if (std::string("rmat") == argv[1]) {
    get_rmat_statistics(std::strtol(argv[2], nullptr, 10),
                        std::strtol(argv[3], nullptr, 10),
                        std::strtol(argv[4], nullptr, 10));
  }
  if (std::string("er") == argv[1]) {
    get_er_statistics(std::strtol(argv[2], nullptr, 10),
                      std::strtod(argv[3], nullptr),
                      std::strtol(argv[4], nullptr, 10));
  }

  if (std::string("ws") == argv[1]) {
    get_ws_statistics(
        std::strtol(argv[2], nullptr, 10), std::strtol(argv[3], nullptr, 10),
        std::strtod(argv[4], nullptr), std::strtol(argv[5], nullptr, 10));
  }
  return 0;
}
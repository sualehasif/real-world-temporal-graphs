#include "graph.hpp"
#include "graph_generators.hpp"

void get_rmat_statistics(uint64_t num_nodes, uint64_t num_edges,
                         uint64_t print_freq) {
  auto edges = generate_rmat(num_nodes, num_edges);
  AppendOnlyGraph<> g;
  printf("num_nodes, num_edges, average_degree, max_degree, num_triangles, "
         "new_edges\n");

  uint64_t i = 0;
  for (; i < edges.size(); i += print_freq) {
    uint64_t count_new = 0;
    for (uint64_t j = i; j < i + print_freq; j++) {
      count_new += g.add_edge(std::get<0>(edges[j]), std::get<1>(edges[j]));
    }
    printf("%lu, %lu, %f, %lu, %lu, %lu\n", g.num_nodes(), g.num_edges(),
           g.average_degree(), g.max_degree(), g.num_triangles(), count_new);
  }
}

int main() {
  get_rmat_statistics(10000, 1000000, 10000);
  return 0;
}
#include "graph.hpp"
#include "graph_generators.hpp"
#include "io_util.hpp"
#include <cstdlib>
#include <iostream>

template <class node_t = uint32_t, class timestamp_t = uint32_t>
void graph_statistics(
    std::vector<std::tuple<node_t, node_t, timestamp_t>> edges,
    uint64_t print_freq, char *output_filename) {
  std::ofstream myfile;
  myfile.open(output_filename);
  AppendOnlyGraph<> g;
  myfile << "timestep, num_nodes, num_edges, average_degree, max_degree, "
            "num_triangles, "
            "new_edges"
         << std::endl;

  uint64_t i = 0;
  for (; i < edges.size(); i += print_freq) {
    uint64_t count_new = 0;
    for (uint64_t j = i; j < i + print_freq; j++) {
      count_new += g.add_edge(std::get<0>(edges[j]), std::get<1>(edges[j]));
    }
    myfile << i << "," << g.num_nodes() << "," << g.num_edges() << ","
           << g.average_degree() << "," << g.max_degree() << ","
           << g.num_triangles() << "," << count_new << std::endl;
  }
  myfile.close();
}

void get_rmat_statistics(uint64_t num_nodes, uint64_t num_edges,
                         uint64_t print_freq, char *output_filename) {
  auto edges = generate_rmat(num_nodes, num_edges);
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_er_statistics(uint64_t num_nodes, double p, uint64_t print_freq,
                       char *output_filename) {
  auto edges = generate_er(num_nodes, p);
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_ws_statistics(uint64_t num_nodes, uint64_t K, double beta,
                       uint64_t print_freq, char *output_filename) {
  auto edges = generate_watts_strogatz(num_nodes, K, beta);
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_adj_statistics(const std::string &filename, uint64_t print_freq,
                        char *output_filename) {

  auto edges = get_edges_from_file_adj_sym(filename);
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_edges_statistics(const std::string &filename, bool shuffle,
                          uint64_t print_freq, char *output_filename) {

  auto edges = get_edges_from_file_edges(filename, shuffle);
  std::cout << "done loading the graph" << std::endl;
  std::cout << "have " << edges.size() << " edges printing every " << print_freq << std::endl;
  printf("output filename is %s\n", output_filename);
  graph_statistics(edges, print_freq, output_filename);
}

int main(int32_t argc, char *argv[]) {
  if (std::string("rmat") == argv[1]) {
    get_rmat_statistics(std::strtol(argv[2], nullptr, 10),
                        std::strtol(argv[3], nullptr, 10),
                        std::strtol(argv[4], nullptr, 10), argv[5]);
  }
  if (std::string("er") == argv[1]) {
    get_er_statistics(std::strtol(argv[2], nullptr, 10),
                      std::strtod(argv[3], nullptr),
                      std::strtol(argv[4], nullptr, 10), argv[5]);
  }

  if (std::string("ws") == argv[1]) {
    get_ws_statistics(std::strtol(argv[2], nullptr, 10),
                      std::strtol(argv[3], nullptr, 10),
                      std::strtod(argv[4], nullptr),
                      std::strtol(argv[5], nullptr, 10), argv[6]);
  }
  if (std::string("adj") == argv[1]) {
    get_adj_statistics(argv[2], std::strtol(argv[3], nullptr, 10), argv[4]);
  }

  if (std::string("edges") == argv[1]) {
    get_edges_statistics(argv[2], std::strtol(argv[3], nullptr, 10),
                         std::strtol(argv[4], nullptr, 10), argv[5]);
  }
  return 0;
}

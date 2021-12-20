#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "graph.hpp"
#include "graph_generators.hpp"
#include "io_util.hpp"
#include <cstdlib>
#include <iostream>

template <class node_t = uint32_t, class timestamp_t = uint32_t>
void graph_statistics(
    std::vector<std::tuple<node_t, node_t, timestamp_t>> edges,
    uint64_t print_freq, std::string &output_filename) {
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
    uint64_t end = i + print_freq;
    if (end > edges.size()) {
      end = edges.size();
    }
    for (uint64_t j = i; j < end; j++) {
      count_new += g.add_edge(std::get<0>(edges[j]), std::get<1>(edges[j]));
    }
    myfile << end << "," << g.num_nodes() << "," << g.num_edges() << ","
           << g.average_degree() << "," << g.max_degree() << ","
           << g.num_triangles() << "," << count_new << std::endl;
  }
  myfile.close();
}

void get_rmat_statistics(uint64_t num_nodes, uint64_t num_edges, double a,
                         double b, double c, uint64_t print_freq,
                         std::string &output_filename) {
  auto edges = generate_rmat(num_nodes, num_edges, a, b, c);
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_er_statistics(uint64_t num_nodes, double p, uint64_t print_freq,
                       std::string &output_filename) {
  auto edges = generate_er(num_nodes, p);
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_ws_statistics(uint64_t num_nodes, uint64_t K, double beta,
                       uint64_t print_freq, std::string &output_filename) {
  auto edges = generate_watts_strogatz(num_nodes, K, beta);
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_adj_statistics(const std::string &filename, uint64_t print_freq,
                        std::string &output_filename) {

  uint32_t num_nodes;
  uint64_t num_edges;
  auto edges = get_edges_from_file_adj_sym(filename, &num_nodes, &num_edges);
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}
void get_adj_rmat_statistics(const std::string &filename, double a, double b,
                             double c, uint64_t print_freq,
                             std::string &output_filename) {

  uint32_t num_nodes;
  uint64_t num_edges;
  auto edges = get_edges_from_file_adj_sym(filename, &num_nodes, &num_edges);
  auto edges2 = generate_rmat(num_nodes, num_edges);
  edges.insert(edges.end(), edges2.begin(), edges2.end());
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_adj_er_statistics(const std::string &filename, uint64_t print_freq,
                           std::string &output_filename) {

  uint32_t num_nodes;
  uint64_t num_edges;
  auto edges = get_edges_from_file_adj_sym(filename, &num_nodes, &num_edges);
  auto edges2 = generate_er(num_nodes,
                            static_cast<double>(num_edges) /
                                (static_cast<uint64_t>(num_nodes) * num_nodes));
  edges.insert(edges.end(), edges2.begin(), edges2.end());
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_adj_ws_statistics(const std::string &filename, double beta,
                           uint64_t print_freq, std::string &output_filename) {

  uint32_t num_nodes;
  uint64_t num_edges;
  auto edges = get_edges_from_file_adj_sym(filename, &num_nodes, &num_edges);
  auto edges2 = generate_watts_strogatz(num_nodes, num_edges / num_nodes, beta);
  edges.insert(edges.end(), edges2.begin(), edges2.end());
  std::cout << "done loading the graph" << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

void get_edges_statistics(const std::string &filename, bool shuffle,
                          uint64_t print_freq, std::string &output_filename) {

  auto edges = get_edges_from_file_edges(filename, shuffle);
  std::cout << "done loading the graph" << std::endl;
  std::cout << "have " << edges.size() << " edges printing every " << print_freq
            << std::endl;
  std::cout << "output filename is " << output_filename << std::endl;
  graph_statistics(edges, print_freq, output_filename);
}

ABSL_FLAG(std::string, command, "", "Which test to run");
ABSL_FLAG(std::string, output, "del.csv", "output filename");
ABSL_FLAG(uint64_t, print_freq, 1000, "How often to dup stats to file");
ABSL_FLAG(uint64_t, num_nodes, 0, "Number of Nodes, used in generated graphs");
ABSL_FLAG(uint64_t, rmat_num_edges, 0, "Number of edges to generate for rmat");
ABSL_FLAG(double, rmat_a, .5, "Value of a for rmat");
ABSL_FLAG(double, rmat_b, .1, "Value of b for rmat");
ABSL_FLAG(double, rmat_c, .1, "Value of c for rmat");
ABSL_FLAG(double, er_p, .01, "Value of p for er");
ABSL_FLAG(uint64_t, ws_k, 40, "Value of K for ws");
ABSL_FLAG(double, ws_beta, .1, "Value of beta for ws");
ABSL_FLAG(std::string, input, "", "input filename for real graphs");
ABSL_FLAG(bool, edges_shuffle, false,
          "Shuffle the order for edges in a .edges file");

int main(int32_t argc, char *argv[]) {
  absl::SetProgramUsageMessage(
      "Generate Statistics for a variaty of graph stypes");
  absl::ParseCommandLine(argc, argv);
  std::string command = absl::GetFlag(FLAGS_command);
  std::string output_filename = absl::GetFlag(FLAGS_output);
  std::string input_filename = absl::GetFlag(FLAGS_input);
  uint64_t print_freq = absl::GetFlag(FLAGS_print_freq);
  if (command == "rmat") {
    get_rmat_statistics(
        absl::GetFlag(FLAGS_num_nodes), absl::GetFlag(FLAGS_rmat_num_edges),
        absl::GetFlag(FLAGS_rmat_a), absl::GetFlag(FLAGS_rmat_b),
        absl::GetFlag(FLAGS_rmat_c), print_freq, output_filename);
  } else if (command == "er") {
    get_er_statistics(absl::GetFlag(FLAGS_num_nodes), absl::GetFlag(FLAGS_er_p),
                      print_freq, output_filename);
  } else if (command == "ws") {
    get_ws_statistics(absl::GetFlag(FLAGS_num_nodes), absl::GetFlag(FLAGS_ws_k),
                      absl::GetFlag(FLAGS_ws_beta), print_freq,
                      output_filename);
  } else if (command == "adj") {
    get_adj_statistics(input_filename, print_freq, output_filename);
  } else if (command == "adj_rmat") {
    get_adj_rmat_statistics(input_filename, absl::GetFlag(FLAGS_rmat_a),
                            absl::GetFlag(FLAGS_rmat_b),
                            absl::GetFlag(FLAGS_rmat_c), print_freq,
                            output_filename);
  } else if (command == "adj_er") {
    get_adj_er_statistics(input_filename, print_freq, output_filename);
  } else if (command == "adj_ws") {
    get_adj_ws_statistics(input_filename, absl::GetFlag(FLAGS_ws_beta),
                          print_freq, output_filename);
  } else if (command == "edges") {
    get_edges_statistics(input_filename, absl::GetFlag(FLAGS_edges_shuffle),
                         print_freq, output_filename);
  } else {
    std::cout << "command not found was: " << command << std::endl;
  }
  return 0;
}

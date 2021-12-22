#pragma once
#include <limits>
#define newA(__E, __n) (__E *)malloc((__n) * sizeof(__E))
#include "parallel.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>

// A structure that keeps a sequence of strings all allocated from
// the same block of memory
struct words {
  char *Chars;    // array storing all strings
  long n;         // total number of characters
  char **Strings; // pointers to strings (all should be null terminated)
  long m;         // number of substrings
  words() {}
  words(char *C, long nn, char **S, long mm)
      : Chars(C), n(nn), Strings(S), m(mm) {}
  void del() {
    free(Chars);
    free(Strings);
  }
};

inline bool isSpace(char c) {
  switch (c) {
  case '\r':
  case '\t':
  case '\n':
  case 0:
  case ' ':
    return true;
  default:
    return false;
  }
}
// parallel code for converting a string to words
words stringToWords(char *Str, uint64_t n) {
  p_for(uint64_t i = 0; i < n; i++) if (isSpace(Str[i])) Str[i] = 0;

  // mark start of words
  bool *FL = newA(bool, n);
  FL[0] = Str[0];
  p_for(uint64_t i = 1; i < n; i++) FL[i] = Str[i] && !Str[i - 1];

  uint32_t worker_count = getWorkers();
  std::vector<uint64_t> sub_counts(worker_count, 0);
  uint64_t section_count = (n / worker_count) + 1;
  p_for_1(uint64_t i = 0; i < worker_count; i++) {
    uint64_t start = i * section_count;
    uint64_t end = std::min((i + 1) * section_count, n);
    uint64_t local_count = 0;
    for (uint64_t j = start; j < end; j++) {
      if (FL[j]) {
        local_count += 1;
      }
    }
    sub_counts[i] = local_count;
  }
  // count and prefix sum
  for (uint32_t i = 1; i < worker_count; i++) {
    sub_counts[i] += sub_counts[i - 1];
  }
  uint64_t m = sub_counts[worker_count - 1];
  /*
  uint64_t *offsets = newA(uint64_t, m);
  uint64_t j = 0;
  for (uint64_t i = 0; i < m; i++) {
    while (FL[j] != 1) {
      j++;
    }
    offsets[i] = j;
    j++;
  }
  */
  uint64_t *offsets = newA(uint64_t, m);
  p_for_1(uint64_t i = 0; i < worker_count; i++) {
    uint64_t start = i * section_count;
    uint64_t end = std::min((i + 1) * section_count, n);
    uint64_t offset;
    if (i == 0)
      offset = 0;
    else
      offset = sub_counts[i - 1];
    for (uint64_t j = start; j < end; j++) {
      if (FL[j] == 1) {
        offsets[offset++] = j;
      }
    }
  }

  // pointer to each start of word
  char **SA = newA(char *, m);
  p_for(uint64_t j = 0; j < m; j++) SA[j] = Str + offsets[j];

  free(offsets);
  free(FL);
  return words(Str, n, SA, m);
}
char *readStringFromFile(const char *fileName, long *length) {
  std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    std::cout << "Unable to open file: " << fileName << std::endl;
    abort();
  }
  long end = file.tellg();
  file.seekg(0, std::ios::beg);
  long n = end - file.tellg();
  char *bytes = newA(char, n + 1);
  file.read(bytes, n);
  file.close();
  *length = n;
  return bytes;
}

template <class node_t = uint32_t, class timestamp_t = uint32_t>
std::vector<std::tuple<node_t, node_t, timestamp_t>>
get_edges_from_file_adj_sym(const std::string &filename, node_t *num_nodes,
                            uint64_t *num_edges) {
  int64_t length = 0;
  char *S = readStringFromFile(filename.c_str(), &length);
  if (length == 0) {
    printf("file has 0 length, exiting\n");
    exit(-1);
  }
  words W = stringToWords(S, length);
  if (strcmp(W.Strings[0], "AdjacencyGraph")) {
    std::cout << "Bad input file: missing header, got " << W.Strings[0]
              << std::endl;
    exit(-1);
  }
  uint64_t len = W.m - 1;
  if (len == 0) {
    printf("the file appears to have no data, exiting\n");
    exit(-1);
  }
  uint64_t *In = newA(uint64_t, len);
  p_for(uint64_t i = 0; i < len; i++) {
    In[i] = strtoul(W.Strings[i + 1], nullptr, 10);
  }
  W.del();
  uint64_t n = In[0];
  uint64_t m = In[1];
  *num_nodes = n;
  *num_edges = m;
  if (n == 0 || m == 0) {
    printf("the file says we have no edges or vertices, exiting\n");
    free(In);
    exit(-1);
  }

  if (len != n + m + 2 && len != n + 2 * m + 2) {
    std::cout << "n = " << n << " m = " << m << std::endl;
    std::cout << "Bad input file: length = " << len << " n+m+2 = " << n + m + 2
              << std::endl;
    std::cout << "or: length = " << len << " n+2*m+2 = " << n + 2 * m + 2
              << std::endl;
    free(In);
    exit(-1);
  }
  uint64_t *offsets = In + 2;
  uint64_t *edges = In + 2 + n;
  std::vector<std::tuple<node_t, node_t, timestamp_t>> edges_array(m);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint64_t> dis_time(0, m);
  p_for(uint64_t i = 0; i < n; i++) {
    uint64_t o = offsets[i];
    uint64_t l = ((i == n - 1) ? m : offsets[i + 1]) - offsets[i];
    for (uint64_t j = o; j < o + l; j++) {
      edges_array[j] = {i, edges[j], dis_time(gen)};
    }
  }
  free(In);

  std::sort(edges_array.begin(), edges_array.end(),
            [](auto const &t1, auto const &t2) {
              return std::get<2>(t1) < std::get<2>(t2);
            });
  return edges_array;
}

template <class node_t = uint32_t, class timestamp_t = uint32_t>
std::vector<std::tuple<node_t, node_t, timestamp_t>>
get_edges_from_file_edges(const std::string &filename, bool shuffle) {
  std::ifstream infile(filename);

  std::vector<std::tuple<node_t, node_t, timestamp_t>> edges_array;
  node_t a;
  node_t b;
  timestamp_t c;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint64_t> dis_time(
      0, std::numeric_limits<uint32_t>::max());
  while (infile >> a >> b >> c) {
    if (!shuffle) {
      edges_array.emplace_back(a, b, c);
    } else {
      edges_array.emplace_back(a, b, dis_time(gen));
    }
  }
  std::sort(edges_array.begin(), edges_array.end(),
            [](auto const &t1, auto const &t2) {
              return std::get<2>(t1) < std::get<2>(t2);
            });
  return edges_array;
}
#!/bin/bash

# Run with LLsub ./adj_graphs.sh [1,40,1] -J adj_graph

GRAPH_PATH=/home/gridsan/groups/dynamic-graphs/graphs/soc-LiveJournal1_sym.adj
PRINT_FREQ=1000
OUTPUT_FILENAME=adj/adj_stats.csv
mkdir -p adj
./run adj $GRAPH_PATH $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK

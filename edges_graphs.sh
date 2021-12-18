#!/bin/bash

# Run with LLsub ./edges_graphs.sh [1,20,1] -J adj_graph

GRAPH_PATH=/home/gridsan/groups/dynamic-graphs/graphs/soc-bitcoin/soc-bitcoin.edges
PRINT_FREQ=1000
OUTPUT_FILENAME=edges/edges_stats.csv
mkdir -p edges
if [$LLSUB_RANK -eq 0 ]
then
./run edges $GRAPH_PATH 0 $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK
else
./run edges $GRAPH_PATH 1 $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK
fi


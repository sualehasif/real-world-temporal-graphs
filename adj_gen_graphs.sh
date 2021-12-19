#!/bin/bash

# Run with LLsub ./adj_gen_graphs.sh [1,40,1] -J adj_gen_graph  -- <graph_name> <generator>

NAME=$1
GEN=$2

GRAPH_PATH=/home/gridsan/groups/dynamic-graphs/graphs/$NAME
PRINT_FREQ=1000
OUTPUT_FILENAME=output/${NAME}_${GEN}/${NAME}_${GEN}_stats.csv
mkdir -p output/${NAME}_${GEN}
./run adj_$GEN $GRAPH_PATH $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK
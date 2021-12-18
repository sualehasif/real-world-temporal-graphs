#!/bin/bash

# Run with LLsub ./adj_graphs.sh [1,40,1] -J adj_graph  -- <graph_name>

NAME=$1

GRAPH_PATH=/home/gridsan/groups/dynamic-graphs/graphs/$NAME
PRINT_FREQ=1000
OUTPUT_FILENAME=output/$NAME/${NAME}_stats.csv
mkdir -p output/$NAME
./run adj $GRAPH_PATH $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK

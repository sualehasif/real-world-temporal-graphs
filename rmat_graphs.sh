#!/bin/bash

# Run with LLsub ./rmat_graphs.sh [1,40,1] -J rmat_graph

NUM_NODES=1000000
NUM_EDGES=100000000
PRINT_FREQ=1000
OUTPUT_FILENAME=output/rmat/rmat_stats.csv
mkdir -p output/rmat
./run rmat $NUM_NODES $NUM_EDGES $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK

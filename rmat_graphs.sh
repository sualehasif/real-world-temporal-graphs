#!/bin/bash

# Run with LLsub ./rmat_graphs.sh [1,40,1] -J rmat_graph

NUM_NODES=100000
NUM_EDGES=10000000
PRINT_FREQ=1000
OUTPUT_FILENAME=rmat/rmat_stats.csv
mkdir -p rmat
./run rmat $NUM_NODES $NUM_EDGES $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK

#!/bin/bash

# Run with LLsub ./rmat_graphs.sh [1,40,1] -J ws_graph

NUM_NODES=100000
K=100
BETA=.1
PRINT_FREQ=1000
OUTPUT_FILENAME=ws/ws_stats.csv
mkdir -p ws
./run ws $NUM_NODES $K $BETA $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK

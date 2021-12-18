#!/bin/bash

# Run with LLsub ./ws_graphs.sh [1,40,1] -J ws_graph

NUM_NODES=1000000
K=100
BETA=.1
PRINT_FREQ=1000
OUTPUT_FILENAME=output/ws/ws_stats.csv
mkdir -p output/ws
./run ws $NUM_NODES $K $BETA $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK

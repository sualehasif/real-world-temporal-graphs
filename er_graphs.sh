#!/bin/bash

# Run with LLsub ./er_graphs.sh [1,40,1] -J er_graph

NUM_NODES=100000
P=.01
PRINT_FREQ=1000
OUTPUT_FILENAME=output/er/er_stats.csv
mkdir -p output/er
./run er $NUM_NODES $P $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK

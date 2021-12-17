#!/bin/bash

# Run with LLsub ./er_graphs.sh [1,40,1] -J er_graph

NUM_NODES=100000
P=.01
PRINT_FREQ=1000
OUTPUT_FILENAME=er/er_stats.csv
mkdir -p er
./run er $NUM_NODES $P $PRINT_FREQ $OUTPUT_FILENAME.$LLSUB_RANK

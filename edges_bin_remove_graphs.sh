#!/bin/bash

# Run with LLsub ./edges_bin_graphs.sh [20,1,1] -J edges_bin_graph -- <graph_name>

NAME=$1
GRAPH_PATH=/home/gridsan/groups/dynamic-graphs/graphs/$NAME/$NAME.bin
PRINT_FREQ=1000
OUTPUT_FILENAME=output/$NAME/${NAME}_stats.csv
mkdir -p output/$NAME
if [[ $LLSUB_RANK -eq 0 ]]
then
./run --command edges_bin --input $GRAPH_PATH --print_freq $PRINT_FREQ --output $OUTPUT_FILENAME.$LLSUB_RANK --remove
else
./run --command edges_bin --input $GRAPH_PATH --edges_shuffle --print_freq $PRINT_FREQ --output $OUTPUT_FILENAME.$LLSUB_RANK --remove
fi


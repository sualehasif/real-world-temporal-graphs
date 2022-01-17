#!/bin/bash

# Run with LLsub ./edges_graphs.sh [1,20,1] -J edges_graph -- <graph_name>

NAME=$1
GRAPH_PATH=/home/gridsan/groups/dynamic-graphs/graphs/$NAME/$NAME.edges
PRINT_FREQ=1000
OUTPUT_FILENAME=output/$NAME/${NAME}_stats.csv
mkdir -p output/$NAME
if [[ $LLSUB_RANK -eq 0 ]]
then
./run --command edges --input $GRAPH_PATH --print_freq $PRINT_FREQ --output $OUTPUT_FILENAME.$LLSUB_RANK
else
./run --command edges --input $GRAPH_PATH --edges_shuffle --print_freq $PRINT_FREQ --output $OUTPUT_FILENAME.$LLSUB_RANK
fi


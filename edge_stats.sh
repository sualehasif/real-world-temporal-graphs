#!/bin/bash


NAME=$1
if [ "$NAME" != "wikipedia" ]
then
GRAPH_PATH=/home/gridsan/groups/dynamic-graphs/graphs/$NAME/$NAME.edges
else 
GRAPH_PATH=/home/gridsan/groups/dynamic-graphs/graphs/$NAME/$NAME.bin
fi
OUTPUT_PATH=output/$NAME/
mkdir -p output/$NAME



./run_edges --output $OUTPUT_PATH/edge_histogram.csv --input $GRAPH_PATH 
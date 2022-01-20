#!/bin/bash

# Run with LLsub ./degree_graphs.sh [1,1,1] -J degree_graph -- <graph_name>
module load anaconda/2021a


NAME=$1
GRAPH_PATH=/home/gridsan/groups/dynamic-graphs/graphs/$NAME/$NAME.edges
PRINT_FREQ=1000
OUTPUT_PATH=output/$NAME/
mkdir -p output/$NAME


./run_nodes --command random --output $OUTPUT_PATH/nodes_random.csv --num_nodes 10 --input $GRAPH_PATH 

python plot_nodes.py $OUTPUT_PATH/nodes_random.csv $OUTPUT_PATH/nodes_random.png ${NAME}_random




./run_nodes --command top_update --output $OUTPUT_PATH/nodes_top_update.csv --num_nodes 10 --input $GRAPH_PATH 

python plot_nodes.py $OUTPUT_PATH/nodes_top_update.csv $OUTPUT_PATH/nodes_top_update.png ${NAME}_top_update



./run_nodes --command top_degree --output $OUTPUT_PATH/nodes_top_degree.csv --num_nodes 10 --input $GRAPH_PATH 

python plot_nodes.py $OUTPUT_PATH/nodes_top_degree.csv $OUTPUT_PATH/nodes_top_degree.png ${NAME}_top_degree
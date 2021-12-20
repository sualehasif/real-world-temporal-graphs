#!/bin/bash

# Run with LLsub ./get_graphs.sh [1,40,1] -J gen_graph -- <gen>

GEN=$1

PRINT_FREQ=1000
if [[ $GEN -eq er ]]
then
NUM_NODES=100000
else
NUM_NODES=1000000
fi
P=.01
NUM_EDGES=100000000
K=100
BETA=.1
A=.5
B=.1
C=.1
OUTPUT_FILENAME=output/$GEN/${GEN}_stats.csv
mkdir -p output/$GEN
./run  --command $GEN --output $OUTPUT_FILENAME.$LLSUB_RANK --print_freq $PRINT_FREQ  --num_nodes $NUM_NODES --rmat_num_edges $NUM_EDGES --rmat_a $A --rmat_b $B --rmat_c $C --er_p $P --ws_k $K --ws_beta $BETA


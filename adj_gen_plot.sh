#!/bin/bash

# Run with LLsub ./adj_plot.sh [1,1,1] -w # -- <graph_name> <generator>
module load anaconda/2021a
NAME=$1
GEN=$2
INPUT_FILENAME=output/${NAME}_${GEN}/${NAME}_${GEN}_stats.csv

python plot.py output/${NAME}_${GEN}/${NAME}_${GEN} $INPUT_FILENAME.*
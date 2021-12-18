#!/bin/bash

# Run with LLsub ./edges_plot.sh [1,1,1] -w # -- <graph_name>
module load anaconda/2021a
NAME=$1
INPUT_FILENAME=$NAME/${NAME}_stats.csv

python plot.py $NAME/$NAME $INPUT_FILENAME.*

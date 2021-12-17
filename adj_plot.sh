#!/bin/bash

# Run with LLsub ./adj_plot.sh [1,1,1]
module load anaconda/2021a
INPUT_FILENAME=adj/adj_stats.csv

python plot.py adj/adj $INPUT_FILENAME.*

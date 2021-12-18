#!/bin/bash

# Run with LLsub ./edges_plot.sh [1,1,1]
module load anaconda/2021a
INPUT_FILENAME=edges/edges_stats.csv

python plot.py edges/edges $INPUT_FILENAME.*

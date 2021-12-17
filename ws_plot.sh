#!/bin/bash

# Run with LLsub ./rmat_plot.sh [1,1,1]
module load anaconda/2021a
INPUT_FILENAME=ws/ws_stats.csv

python plot.py ws/ws $INPUT_FILENAME.*

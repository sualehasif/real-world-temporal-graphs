#!/bin/bash

# Run with LLsub ./rmat_plot.sh [1,1,1] -w #
module load anaconda/2021a
INPUT_FILENAME=output/ws/ws_stats.csv

python plot.py output/ws/ws $INPUT_FILENAME.*

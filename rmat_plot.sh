#!/bin/bash

# Run with LLsub ./rmat_plot.sh [1,1,1]
module load anaconda/2021a
INPUT_FILENAME=rmat/rmat_stats.csv
python plot.py rmat/rmat $INPUT_FILENAME.*

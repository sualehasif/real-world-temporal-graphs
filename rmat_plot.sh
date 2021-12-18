#!/bin/bash

# Run with LLsub ./rmat_plot.sh [1,1,1] -w #
module load anaconda/2021a
INPUT_FILENAME=output/rmat/rmat_stats.csv
python plot.py output/rmat/rmat $INPUT_FILENAME.*

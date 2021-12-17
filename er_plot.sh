#!/bin/bash

# Run with LLsub ./er_plot.sh [1,1,1]
module load anaconda/2021a
INPUT_FILENAME=er/er_stats.csv

python plot.py er/er $INPUT_FILENAME.*

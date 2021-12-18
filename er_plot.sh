#!/bin/bash

# Run with LLsub ./er_plot.sh [1,1,1] -w # 
module load anaconda/2021a
INPUT_FILENAME=output/er/er_stats.csv

python plot.py output/er/er $INPUT_FILENAME.*

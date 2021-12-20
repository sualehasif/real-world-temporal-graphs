#!/bin/bash

# Run with LLsub ./gen_plot.sh [1,1,1] -w # -- <gen>

GEN=$1

module load anaconda/2021a
INPUT_FILENAME=output/$GEN/${GEN}_stats.csv

python plot.py output/$GEN/$GEN $INPUT_FILENAME.*
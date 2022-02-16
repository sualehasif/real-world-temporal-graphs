#!/bin/bash

echo "graph, one update, multiple updates, total updates, ratio"

for graph in copresence-Thiers13 ia-online-ads-criteo ia-online-ads-criteo-attrs rec-amazon-ratings rec-amz-Books rec-amz-CDs-and-Vinyl rec-amz-Cell-Phones-and-Acc rec-amz-Clothing-Shoes-Jewelry rec-amz-Electronics rec-amz-Home-and-Kitchen rec-amz-Movies-and-TV rec-amz-Sports-and-Outdoors rec-epinions-user-ratings rec-movielens-ratings soc-bitcoin soc-flickr-growth soc-youtube-growth sx-stackoverflow wikipedia
do

one=$(head -2 output/$graph/edge_histogram.csv | cut -d "," -f 2 | paste -sd+ | bc)
multiple=$(tail -n +3  output/$graph/edge_histogram.csv | cut -d "," -f 2 | paste -sd+ | bc)
total=$(echo "scale=2 ; $one + $multiple" | bc)
ratio=$(echo "scale=2 ; $one / $total" | bc)

echo "$graph, $one, $multiple, $total, $ratio"
done


#!/bin/bash

for graph in copresence-Thiers13 ia-online-ads-criteo ia-online-ads-criteo-attrs rec-amazon-ratings rec-amz-Books rec-amz-CDs-and-Vinyl rec-amz-Cell-Phones-and-Acc rec-amz-Clothing-Shoes-Jewelry rec-amz-Electronics rec-amz-Home-and-Kitchen rec-amz-Movies-and-TV rec-amz-Sports-and-Outdoors rec-epinions-user-ratings rec-movielens-ratings soc-bitcoin soc-flickr-growth soc-youtube-growth sx-stackoverflow
do
result=$(LLsub ./edges_graphs.sh [1,20,1] -J ${graph}_graph -- $graph | head -n 1 | cut -d " " -f 4)
LLsub ./edges_plot.sh [1,1,1] -J ${graph}_plot -w $result -- $graph
done

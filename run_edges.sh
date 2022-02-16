#!/bin/bash

for graph in copresence-Thiers13 ia-online-ads-criteo ia-online-ads-criteo-attrs rec-amazon-ratings rec-amz-Books rec-amz-CDs-and-Vinyl rec-amz-Cell-Phones-and-Acc rec-amz-Clothing-Shoes-Jewelry rec-amz-Electronics rec-amz-Home-and-Kitchen rec-amz-Movies-and-TV rec-amz-Sports-and-Outdoors rec-epinions-user-ratings rec-movielens-ratings soc-bitcoin soc-flickr-growth soc-youtube-growth sx-stackoverflow reddit wikipedia
do
LLsub ./edge_stats.sh [1,1,1] -J nodes_${graph} -- $graph 
done

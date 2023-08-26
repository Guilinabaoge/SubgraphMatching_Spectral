#!/bin/bash

#make a query by randomwalk, and then introduce wildcard label for it.

datasets=("dblp" "eu2005" "hprd" "patents" "yeast" "youtube" "wordnet" "human")

for dataset in "${datasets[@]}"
  do
    for query_number in $(seq 1 50)
      do
        python3 generate_wildcard_query.py -s 64 -prop dense -d "$dataset" -p 0.5 -n "$query_number"
      done
  done

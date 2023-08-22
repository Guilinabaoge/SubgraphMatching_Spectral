#!/bin/bash

#make a query by randomwalk, and then introduce wildcard label for it.

datasets=("dblp" "eu2005" "hprd" "patents" "yeast" "youtube" "wordnet" "human")

for dataset in "${datasets[@]}"
  do
    for query_number in $(seq 31 200)
      do
        python3 query_generator.py -s 64 -p dense -d "$dataset"
        wait
        python3 generate_wildcard_query.py -s 64 -prop dense -d "$dataset" -p 0.75 -n "$query_number"
      done
  done

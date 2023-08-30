#!/bin/bash
#prop=("dense" "sparse")
prop=("dense")
datasets=("dblp" "eu2005" "hprd" "patents" "yeast" "youtube" "wordnet" "human")
for dataset in "${datasets[@]}"
do
  for p in "${prop[@]}"
    do
    for i in $(seq "$1" "$2")
    do
      for j in 64;
      do
      (cd ../cmake-build-debug/matching;
      timeout 600s ./SubgraphMatching.out -dataset "$dataset"  -qsize "$j" -qnumber "$i" -qprop "dense" -wildcard "$4" -filter "$5" -eigen "$6" -order "$8")
      echo "${filter}"
      done
    done
  done
done


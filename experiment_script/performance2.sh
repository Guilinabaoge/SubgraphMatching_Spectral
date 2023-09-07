#!/bin/bash

prop=("dense")

if [ "$7" -eq "32" ]; then
  size=("16" "24" "32")
  datasets=("dblp" "eu2005" "hprd" "patents" "yeast" "youtube")
else
  size=("12" "16" "20")
  datasets=( "wordnet" "human")
fi

for dataset in "${datasets[@]}"
do
  for p in "${prop[@]}"
    do
    for i in $(seq "$1" "$2")
    do
          for j in "${size[@]}";
          do
          (cd ../cmake-build-debug/matching;
          timeout 600s ./SubgraphMatching.out -dataset "$dataset"  -qsize "$j" -qnumber "$i" -qprop "dense" -wildcard "$4" -filter "$5" -eigen "$6" -order "$8")
          done
    done
  done
done


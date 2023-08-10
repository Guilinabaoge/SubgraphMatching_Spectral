#!/bin/bash

ordering=("GQL" "RI" "TSO" "DPiso" "CFL")
percent=("0.25" "0.5" "0.75" "1.0")
dataset=("hprd" "yeast" "eu2005" "dblp" "patents" "youtube" "human" "wordnet")

for o in "${ordering[@]}";
do
  mkdir "${o}"
  for p in "${percent[@]}";
  do
    mkdir "${o}"/"${p}"
    for data in "${dataset[@]}";
    do
      mkdir "${o}"/"${p}"/"${data}"
    done
  done
done

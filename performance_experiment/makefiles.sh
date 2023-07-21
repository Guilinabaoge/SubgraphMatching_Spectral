#!/bin/bash

percent=("0.25" "0.5" "0.75" "1.0")
dataset=("hprd" "yeast" "eu2005" "dblp" "patents" "youtube" "human" "wordnet")

for p in "${percent[@]}";
do
  mkdir "${p}"
  for data in "${dataset[@]}";
  do
    mkdir "${p}"/"${data}"
  done
done

#!/bin/bash

dataset32=("hprd" "yeast" "eu2005" "dblp" "patents" "youtube")
dataset20=("human" "wordnet")

for data in "${dataset32[@]}";
do
  ./singleDataSet.sh ${data} 0.25 32
done

for data in "${dataset20[@]}";
do
  ./singleDataSet.sh ${data} 0.25 32
done
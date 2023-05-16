#!/bin/bash
for i in $(seq $1 $2)
do
  for j in 16 24 32;
  do
  (cd ../../performance_build/matching ;timeout 300s ./SubgraphMatching.out -dataset $3  -qsize $j -qnumber $i -qprop $4 -filter $5)
  done
done

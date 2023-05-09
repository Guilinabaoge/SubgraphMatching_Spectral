#!/bin/bash
for i in $(seq $1 $2)
do
  for j in 12 16 20;
  do
  (cd ../../performance_build/matching ;timeout 300s ./SubgraphMatching.out -dataset wordnet -qsize $j -qnumber $i -qprop $3 -filter $4)
  done
done

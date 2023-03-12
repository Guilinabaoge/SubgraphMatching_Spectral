#!/bin/bash
for i in 4 12 18 21 22 24 39 44 48 ;
do
  for j in {1..5}
  do
    (cd ../.. ;timeout 1000s ./SubgraphMatching.out -dataset wordnet -qsize 20 -qnumber $i -qprop sparse)
  done
done
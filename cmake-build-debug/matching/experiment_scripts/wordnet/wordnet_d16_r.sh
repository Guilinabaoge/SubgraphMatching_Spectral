#!/bin/bash
for i in 8 16 17 23 28 29 31 48 64 69 73 79 85 90 103 104;
do
  for j in {1..5}
  do
    (cd ../.. ;timeout 1000s ./SubgraphMatching.out -dataset wordnet -qsize 16 -qnumber $i -qprop dense)
  done
done








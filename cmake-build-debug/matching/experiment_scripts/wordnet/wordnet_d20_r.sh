#!/bin/bash
for i in 1 4 13 ;
do
  for j in {1..5}
  do
    (cd ../.. ;timeout 1000s ./SubgraphMatching.out -dataset wordnet -qsize 20 -qnumber $i -qprop dense)
  done
done
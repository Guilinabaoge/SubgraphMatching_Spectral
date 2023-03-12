#!/bin/bash
for i in 14 19 21 26 30 32 33 34 37 41 43 53 54 56 62 68 91 94 104 116 145 167 172 183 189 194 ;
do
  for j in {1..5}
  do
    (cd ../.. ;timeout 1000s ./SubgraphMatching.out -dataset wordnet -qsize 16 -qnumber $i -qprop sparse)
  done
done
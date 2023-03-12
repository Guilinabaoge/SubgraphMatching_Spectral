#!/bin/bash
for i in {7..200}
do
(cd ..; timeout 3900s ./SubgraphMatching.out -dataset wordnet -qsize 20 -qnumber $i -qprop sparse)
done
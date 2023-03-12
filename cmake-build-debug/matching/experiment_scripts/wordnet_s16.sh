#!/bin/bash
for i in {79..200}
do
(cd .. ;timeout 3900s ./SubgraphMatching.out -dataset wordnet -qsize 16 -qnumber $i -qprop sparse)
done
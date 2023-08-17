#!/bin/bash

cd ../cmake-build-debug/matching;

#for x in {1}
#do
#./SubgraphMatching.out -dataset patents -qsize 32 -qnumber 5 -qprop dense -wildcard 0.75 -filter CFL -eigen 0 -order GQL
#done

for x in {1}
do
./SubgraphMatching.out -dataset patents -qsize 32 -qnumber 5 -qprop dense -wildcard 0.75 -filter CFL -eigen 1 -order GQL
done




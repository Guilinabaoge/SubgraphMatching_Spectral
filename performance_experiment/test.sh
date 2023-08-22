#!/bin/bash

cd ../cmake-build-debug/matching;

for x in {1}
do
./SubgraphMatching.out -dataset patents -qsize 128 -qnumber 1 -qprop dense -wildcard 0.75 -filter TSO -eigen 0 -order GQL
done

for x in {1}
do
./SubgraphMatching.out -dataset patents -qsize 128 -qnumber 1 -qprop dense -wildcard 0.75 -filter TSO -eigen 1 -order GQL
done




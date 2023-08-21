#!/bin/bash

cd ../cmake-build-debug/matching;

for x in {1}
do
./SubgraphMatching.out -dataset pantents -qsize 32 -qnumber 1 -qprop dense -wildcard 0.75 -filter GQL -eigen 0 -order GQL
done

for x in {1}
do
./SubgraphMatching.out -dataset pantents -qsize 32 -qnumber 1 -qprop dense -wildcard 0.75 -filter GQL -eigen 1 -order GQL
done




#!/bin/bash
#prop=("dense" "sparse")
prop=("dense")
if [ "$7" = "32" ]; then
  for p in "${prop[@]}"
    do
    for i in $(seq "$1" "$2")
    do
      for j in 64;
      do
      (cd ../cmake-build-debug/matching;
      timeout 600s ./SubgraphMatching.out -dataset "$3"  -qsize "$j" -qnumber "$i" -qprop "$p" -wildcard "$4" -filter "$5" -eigen "$6" -order "$8")
      echo "${filter}"
      done
    done
  done
else
  for p in "${prop[@]}"
    do
    for i in $(seq "$1" "$2")
    do
#      for j in 12 16 20;
      for j in 64;
      do
      (cd ../cmake-build-debug/matching;
      timeout 600s ./SubgraphMatching.out -dataset "$3"  -qsize "$j" -qnumber "$i" -qprop "$p" -wildcard "$4" -filter "$5" -eigen "$6" -order "$8")
      done
    done
  done
fi

#!/bin/bash
prop=("dense" "sparse")
if [ "$7" = "32" ]; then
  for p in "${prop[@]}"
    do
    for i in $(seq "$1" "$2")
    do
      for j in 16 24 32;
      do
      (cd ../cmake-build-debug/matching ;timeout 300s ./SubgraphMatching.out -dataset "$3"  -qsize "$j" -qnumber "$i" -qprop "$p" -wildcard "$4" -filter "$5" -eigen "$6")
      echo "${filter}"
      done
    done
  done
else
  for p in "${prop[@]}"
    do
    for i in $(seq "$1" "$2")
    do
      for j in 12 16 20;
      do
      (cd ../cmake-build-debug/matching ;timeout 300s ./SubgraphMatching.out -dataset "$3"  -qsize "$j" -qnumber "$i" -qprop "$p" -wildcard "$4" -filter "$5" -eigen "$6")
      done
    done
  done
fi
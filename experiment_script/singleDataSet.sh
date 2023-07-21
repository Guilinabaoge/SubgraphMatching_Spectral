#!/bin/bash 

if byobu list-sessions | grep -q "${1}"; then
  byobu kill-session -t "${1}"
  echo "Session wildcard killed."
else
  echo "Session ${1} does not exist, now create one"
fi

byobu new-session -d -s "${1}"

byobu select-pane -t 0
byobu split-window -v
byobu select-pane -t 0
byobu split-window -h
byobu select-pane -t 0
byobu split-window -h
byobu select-pane -t 0
byobu split-window -v
byobu select-pane -t 0
byobu split-window -v

byobu select-pane -t 2
byobu split-window -v
byobu select-pane -t 2
byobu split-window -h
byobu select-pane -t 5
byobu split-window -h
byobu select-pane -t 6
byobu split-window -v
byobu select-pane -t 5
byobu split-window -v
byobu select-pane -t 7
byobu split-window -v

#parameters: 1:datasetname, 2:wildcard percentage 3: 20/32
#e.g ./singleDataSet.sh wordnet 0.25 20
#e.g ./singleDataSet.sh youtube 0.25 32

filters=("LDF" "NLF" "GQL" "TSO" "CFL" "DPiso")
counter=0 
for element in "${filters[@]}"
do
	byobu select-pane -t "$counter"
	byobu send-keys "./performance.sh 1 200 $1 $2 $element 0 $3" Enter
	((counter++))
done

for element in "${filters[@]}"
do
	byobu select-pane -t "$counter"
	byobu send-keys "./performance.sh 1 200 $1 $2 $element 1 $3" Enter
	((counter++))
done



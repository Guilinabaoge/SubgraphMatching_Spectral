#!/bin/bash 

if byobu list-sessions | grep -q "$2"; then
  byobu kill-session -t "$2"
  echo "Session 'dense' killed."
else
  echo "Session 'dense' does not exist, now create one"
fi

byobu new-session -d -s "$2"

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

#parameters: 1:datasetname, 2:query property(dense/sparse) 3: 20/32
#e.g ./DenseOrSparse.sh wordnet dense 20
#e.g ./DenseOrSparse.sh youtube dense 32

arr=("LDF" "NLF" "GQL" "TSO" "CFL" "DPiso")
counter=0 
for element in "${arr[@]}"
do
	byobu select-pane -t "$counter"
	byobu send-keys "./performance.sh 1 200 $1 $2 $element 0 $3" Enter
	((counter++))
done

for element in "${arr[@]}"
do
	byobu select-pane -t "$counter"
	byobu send-keys "./performance.sh 1 200 $1 $2 $element 1 $3" Enter
	((counter++))
done



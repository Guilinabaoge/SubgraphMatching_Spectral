#!/bin/bash 

byobu kill-session -t dense 
byobu new-session -d -s dense
byobu select-pane -t 0
byobu split-window -v
byobu select-pane -t 1
byobu split-window -h
byobu select-pane -t 0
byobu split-window -h
byobu select-pane -t 0
byobu split-window -h
byobu select-pane -t 0
byobu split-window -v

arr=("LDF" "NLF" "GQL" "TSO" "CFL" "DPiso")
counter=0 
for element in ${arr[@]}
do
	byobu select-pane -t $counter
	byobu send-keys "./performance.sh 1 200 eu2005 dense $element" Enter
	((counter++))
done	



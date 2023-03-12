#!/bin/bash

#Create new session. I named this LeftMonitor for obvious reasons
byobu new-session -d -s wordnet_r

#Select default pane. Probably an unnecessary line of code
byobu select-pane -t 0

#Split pane 0 into two vertically stacked panes
byobu split-window -v

#Select the newly created pane 1. Again, probably unnecessary as the new pane gets selected after a split
byobu select-pane -t 1

#Split pane 1 horizontally to create two side-by-side panes
byobu split-window -h

#Repeat the selection and splitting process with the top half
byobu select-pane -t 0
byobu split-window -h
#At this point, four equally sized panes have been created.

#Select pane to interact with
byobu select-pane -t 1

#Pass a command to the selected pane. I'm using top as the example here.
#Note that you need to type Enter for byobu to simulate pressing the enter key.
#byobu send-keys "top" Enter

byobu select-pane -t 0
byobu send-keys "./wordnet_d16_r.sh" Enter

byobu select-pane -t 1
byobu send-keys "./wordnet_d20_r.sh" Enter

byobu select-pane -t 2
byobu send-keys "./wordnet_s16_r.sh" Enter

byobu select-pane -t 3
byobu send-keys "./wordnet_s20_r.sh" Enter

byobu

#gnome-terminal --full-screen -- byobu attach -t LeftMonitor
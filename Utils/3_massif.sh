#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
valgrind --tool=massif --massif-out-file=outputfile --pages-as-heap=yes ./eseguibile </home/nico/Documents/GitHub/API-project-service-stations/Tests/input.txt
massif-visualizer outputfile
$SHELL

#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
valgrind --tool=callgrind --callgrind-out-file=outputfile ./eseguibile </home/nico/Documents/GitHub/API-project-service-stations/Tests/input.txt
kcachegrind outputfile
$SHELL

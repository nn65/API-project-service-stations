#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./eseguibile </home/nico/Documents/GitHub/API-project-service-stations/Tests/input.txt
$SHELL

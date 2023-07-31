#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./eseguibile </home/nico/OneDrive/API_progetto/Tests/archivio_test_aperti/open_101.txt
$SHELL

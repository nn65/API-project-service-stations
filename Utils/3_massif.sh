#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
valgrind --tool=massif --massif-out-file=outputfile --pages-as-heap=yes ./eseguibile </home/nico/OneDrive/API_progetto/Tests/archivio_test_aperti/open_101.txt
massif-visualizer outputfile
$SHELL

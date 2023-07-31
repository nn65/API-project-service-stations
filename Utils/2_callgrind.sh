#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
valgrind --tool=callgrind --callgrind-out-file=outputfile ./eseguibile </home/nico/OneDrive/API_progetto/Tests/archivio_test_aperti/open_101.txt
kcachegrind outputfile
$SHELL

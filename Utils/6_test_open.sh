#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
gcc -Wall -Werror -std=gnu11 -O2 -lm -g3 main.c -o eseguibile
./eseguibile < /home/nico/OneDrive/API_progetto/Tests/archivio_test_aperti/open_101.txt > Tests/output.txt
diff -Z ./Tests/output.txt /home/nico/OneDrive/API_progetto/Tests/archivio_test_aperti/open_101.output.txt
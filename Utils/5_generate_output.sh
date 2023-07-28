#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
gcc -Wall -Werror -std=gnu11 -O2 -lm -g3 main.c -o eseguibile
./eseguibile < Tests/input.txt > Tests/output.txt
diff -Z ./Tests/output.txt ./Tests/expected_output.txt
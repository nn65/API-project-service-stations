#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
gcc main.c -o eseguibile_asan -fsanitize=address

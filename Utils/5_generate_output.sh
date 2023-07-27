#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
./eseguibile < Tests/input.txt > Tests/output.txt
diff ./Tests/output.txt ./Tests/expected_output.txt
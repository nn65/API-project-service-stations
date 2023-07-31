#!/bin/bash

cd ~/Documents/GitHub/API-project-service-stations/
gcc -Wall -Werror -std=gnu11 -O2 -lm -g3 main.c -o eseguibile

for i in {1..111}; do
  echo "Testing file open_$i.txt..."
  echo "Running $i" # Debug
  ./eseguibile < /home/nico/OneDrive/API_progetto/Tests/archivio_test_aperti/open_$i.txt > Tests/output.txt
  echo "Finished running ./eseguibile" # Debug
  diff -Z ./Tests/output.txt /home/nico/OneDrive/API_progetto/Tests/archivio_test_aperti/open_$i.output.txt
  if [ $? -eq 0 ]; then
    echo "Test $i passed."
  else
    echo "Test $i failed."
    exit 1
  fi
  echo ""
done

echo "All tests passed!"
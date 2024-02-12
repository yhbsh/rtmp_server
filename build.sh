#!/bin/bash

set -e

rm -f log

cc -O3 -o main main.c
./main 10 &

for i in {1..10}
do
  curl http://127.0.0.1:6969
done


vi log

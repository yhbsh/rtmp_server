#!/bin/bash

set -e

cc -O3 -o main main.c
./main 100 &

for i in {1..100}
do
  curl http://127.0.0.1:6969
done

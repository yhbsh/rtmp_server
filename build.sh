#!/bin/bash

cc -O3 -o main main.c
./main &

pid=$!

#for i in {1..200}
#do
  curl -Ss -X GET http://localhost:6969
#done

kill $pid

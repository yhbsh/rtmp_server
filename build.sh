#!/bin/bash

set -e

HOST=127.0.0.1
PORT=8080
MAX_REQ=10

rm -f log

cc -O3 -o main main.c

./main $MAX_REQ $PORT &

for (( i = 0; i < $((MAX_REQ / 2)); i++))
do
 curl -Ss -X GET  http://$HOST:8080
 curl -Ss -X POST http://$HOST:8080 -H "Content-Type: text/plain; charset=utf-8" -d 'Hello World'
done

vi log

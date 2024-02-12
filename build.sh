#!/bin/bash

set -e

HOST=127.0.0.1
PORT=8080
URL=http://$HOST:$PORT
MAX_REQ=10

rm -f log

cc -O3 -o main main.c

./main $MAX_REQ $PORT &

for (( i = 0; i < $((MAX_REQ / 2)); i++))
do
 curl -Ss -X GET  $URL 
 curl -Ss -X POST $URL -H "Content-Type: text/plain; charset=utf-8" -d 'Hello World'
done

vi log

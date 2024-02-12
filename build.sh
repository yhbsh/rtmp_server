#!/bin/bash

set -xe

HOST=127.0.0.1
PORT=8080
HTTP_URL=http://$HOST:$PORT
RTMP_URL=rtmp://$HOST:$PORT
MAX_REQ=1

rm -f log

cc -O3 -o main main.c

./main $MAX_REQ $PORT &

curl -Ss $RTMP_URL

vi log

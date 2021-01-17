#!/bin/bash

clang id0.c socket.c -lpcap -o id0
clang id1.c socket.c -lpcap -o id1
clang id2.c socket.c -lpcap -o id2

# start the first program and fork it
./id0 &

# store the process ID
./id0_pid=$!

# start the second program and fork it
./id1 &

# store the process ID
./id1_pid=$!

# trap ctrl-c so that we can kill
# the first two programs
trap onexit INT
function onexit() {
  kill -9 $./id0_pid
  kill -9 $./id1_pid
}

# start the third program, don't fork it
./id2

#!/bin/sh

for num_threads in 1 32 64 128; do
    for i in 1 2 3 4 5; do
        echo -n "$num_threads "
        OMP_NUM_THREADS=$num_threads ./a.out | grep 'Copy:' | awk '{ print $2 }'
    done
done

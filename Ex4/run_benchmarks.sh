#!/bin/bash
# Exercise 4: run ex2_sum and ex3_pi on 1,2,4,8 processes, 5 runs each,
# and save the average times to results_sum.csv and results_pi.csv
# Usage: bash run_benchmarks.sh
PROCS="1 2 4 8"
RUNS=5
for prog in ex2_sum ex3_pi; do
  out="results_${prog#*_}.csv"         # results_sum.csv / results_pi.csv
  echo "procs,time" > "$out"
  for p in $PROCS; do
    total=0
    for r in $(seq $RUNS); do
      t=$(mpirun --oversubscribe -np $p ./$prog | grep '^CSV' | cut -d, -f3)
      total=$(echo "$total + $t" | bc -l)
    done
    avg=$(echo "$total / $RUNS" | bc -l)
    printf "%d,%.6f\n" $p $avg >> "$out"
    echo "$prog  np=$p  avg=$avg s"
  done
done

#!/bin/bash
#tests task 3 against list-forming.c
echo "Starting script..."
OUTPUT="results.csv"
echo "K,Threads,Run,Time" > $OUTPUT
echo "Writing header..."
K_VALUES=(200 400 800)
THREAD_VALUES=(2 4 8 16 32)
RUNS=3
for K in "${K_VALUES[@]}"
do
  gcc my_list_forming.c -o program -DK=$K -lpthread

  for T in "${THREAD_VALUES[@]}"
  do
    for ((i=1; i<=RUNS; i++))
    do
      TIME=$(./program $T | grep "Time" | awk '{print $2}')
      echo "$K,$T,$i,$TIME" >> $OUTPUT
      echo "K=$K Threads=$T Run=$i Time=$TIME"
    done
  done
done
echo "Done! Results saved to $OUTPUT"

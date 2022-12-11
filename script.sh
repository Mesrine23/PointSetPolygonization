#!/bin/bash

rm results.txt

#options=( 1a 1b 2a 2b )
options=( local global )

for file in to_test/*; do
	#for((i=1; i<=2; i++)); do
	for n in "${options[@]}"; do
      #printf "%s %s %s\n" "$file" "$i" "$n"
    printf "%s %s\n" "$file" "$n"
			#./main -i "$file" -o output.txt -algorithm incremental -edge_selection "$i" -initialization "$n" >> results.txt
		./main -i "$file" -o output.txt -algorithm simulated_annealing -L 1500 -initAlgorithm incremental -max -annealing "$n" >> results.txt
		done
	#done
done

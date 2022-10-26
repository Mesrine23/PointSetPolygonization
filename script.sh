#!/bin/bash

rm results.txt

options=( 1a 1b 2a 2b )

for file in to_test/*; do
	for((i=1; i<=3; i++)); do
		for n in "${options[@]}"; do
      printf "%s %s %s\n" "$file" "$i" "$n"
			./main -i "$file" -o output.txt -algorithm incremental -edge_selection "$i" -initialization "$n" >> results.txt
		done
	done
done

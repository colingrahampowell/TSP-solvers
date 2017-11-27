#!/bin/bash

now=`date '+%Y_%m_%d_%H'`

mkdir ex-sols-$now

tsp-2opt tsp_example_1.txt
tsp-2opt tsp_example_2.txt
2opt-pp tsp_example_3.txt
python tsp-verifier.py tsp_example_1.txt tsp_example_1.txt.tour
python tsp-verifier.py tsp_example_2.txt tsp_example_2.txt.tour
python tsp-verifier.py tsp_example_3.txt tsp_example_3.txt.tour

mv *.txt.tour ./ex-sols-$now


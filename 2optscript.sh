#!/bin/bash

# run three tests, store three different sets of results
for k in `seq 1 3`
do

	# make a folder with today's date
	now=`date '+%Y_%m_%d_%H_%M'`
	mkdir ex-sols-$now

	# run the example cases (all three)
	for i in `seq 1 3`;
	do
		echo "example_$i:" | tee -a log-$now
		tsp-2opt tsp_example_$i.txt | tee -a log-$now
		python tsp-verifier.py tsp_example_$i.txt tsp_example_$i.txt.tour | tee -a log-$now
	done

	# now, run the timed competition cases (all seven)
	for i in `seq 1 7`;
	do
		echo "test_$i:" | tee -a log-$now
		tsp-2opt test-input-$i.txt -t | tee -a log-$now
		python tsp-verifier.py test-input-$i.txt test-input-$i.txt.tour | tee -a log-$now
	done

	# move everything into our solutions folder
	mv *.txt.tour ./ex-sols-$now
	mv log-$now ./ex-sols-$now

done

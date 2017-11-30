CS325 - Group 34 (Carrie Edwards, Joseph Pfohl, Colin Powell)
Nearest-Neighbor + 2-Opt: 

This program implements a Nearest-Neighbor graph construction algorithm, 
combined with a 2-Opt tour optimization algorithm.

BUILDING THE PROGRAM:
To build the executable using the included makefile, enter:

make tsp

This creates an executable named 'tsp-2opt' in the same folder.

Alternatively, you may manually compile on flip:
g++ -O3 -fopenmp -o 2opt-pp 2opt-pp.c point.c ptList.c

PROGRAM EXECUTION:
The program can be run in 'timed' or 'untimed' mode. Please use timed mode
to test the competition instances.

To run in timed mode, pass the -t flag, as follows. Note that 'filename' is the
name of the .txt file containing each point's x/y coordinates and IDs.

tsp-2opt filename -t

This sets an internal timer that terminates the program after 175 seconds of execution,
outputting the results generated to that point.

To run in untimed mode, omit the -t flag:

tsp-2opt filename

Where, again, filename is the name of the .txt file containing coordinate and ID
data for each point.

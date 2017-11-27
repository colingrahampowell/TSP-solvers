Nearest-Neighbor + 2-Opt: A Fantastic Voyage

To compile on flip:
g++ -O3 -fopenmp -o 2opt-pp 2opt-pp.c point.c ptList.c

Results may vary on OSX (will need to have openmp)

Note that I needed to use g++ to access the "getline" function, which is not
standard for gcc.



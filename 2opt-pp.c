/*
 * CS325 - Project 2
 * Group 34
 * Nearest Neighbor + 2-Opt
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>
#include "lib/point.h"
#include "lib/ptList.h"
#include <omp.h>
#include <unistd.h>
#include <limits.h>

#define TRUE 1
#define FALSE 0
#define TIME_PER_TEST 175.0

int getTourLength( ptList *tour );
void nearestNeighbor( ptList *tour, ptList *points, int start );

void twoOpt( ptList *tour, double startTime, double numSeconds );
void twoOptSwap( ptList *tour, int i, int k);
int swapDistance( ptList *tour, int i, int k );

void readFile( char *filename, ptList *points );
void outputResults( char *filename, ptList *tour );

/*
 * file-handling code adapted from getline() man pages: linux.die.net/man/3/getline
 */

int main( int argc, char **argv ) {

	// verify input
	if ( (argc < 2 || argc > 3) || 
		( argc == 3 && (strcmp(argv[2], "-t") != 0 ) )) {

		printf("Usage: \"tsp-2opt filename.txt [-t]\"\n");
		exit(1);
	}

	// declare variables for storing starting, ending time
	double startTime = omp_get_wtime();
	double endTime;

	// set start time, total time for testing
	double numSeconds = (double) INT_MAX;	// 'default' time limit - essentially inf time 

	if( argc == 3 ) {
		numSeconds = TIME_PER_TEST;
	}

	int startPoint = 0;		// starting vertex in our tour

	char filename[255];
	memset( filename, '\0', sizeof(filename) / sizeof( filename[0]) );
	strcpy(filename, argv[1]);

	ptList* points = initPtList();

	readFile( argv[1], points );

	// nearest-neighbor
	ptList *tspTour = initPtList();
	

	nearestNeighbor( tspTour, points, startPoint );

	// run two-opt optimization with remaining time
	twoOpt( tspTour, startTime, numSeconds );

	outputResults( strcat(filename, ".tour"), tspTour );

	// free memory allocated for points
	cleanPtList( points );
	cleanPtList( tspTour );

	endTime = omp_get_wtime(); 

	// print total time running
	printf("Complete. solution found in: %10.8f seconds.\n", (double) (endTime - startTime));

	return 0;

}

/*
 * readFile: file-handling code adapted from getline() man pages: linux.die.net/man/3/getline
 */

void readFile( char *filename, ptList *points ) {

	FILE *fp;			// file pointer
	char *line = NULL;	// our buffer (to be allocated)
	size_t len = 0;	// length of the buffer
	ssize_t read;		// number of bytes read
	char *tok = NULL;	// token

	// temp variables for making our point structs
	int id;
	int x;
	int y;

	Point *new_pt = NULL;

	fp = fopen( filename, "r" );
	if( fp == NULL )
		exit(1);

	// while there are lines left...
	while ( (read = getline( &line, &len, fp ) != -1) ) {

		// get id, then x coord, then y coord
		tok = strtok( line, " ");
		id = atoi(tok);

		tok = strtok( NULL, " ");
		x = atoi(tok);

		tok = strtok( NULL, " ");
		y = atoi(tok);

		// make a new point, and add it to our points list
		new_pt = makePoint(id, x, y);
		addPtList( points, new_pt );

	}

	free(line);		// free line after it is allocated in 'getline'
	fclose( fp );	// close file

}

/*
 * Performs a nearest-neighbor approximation of a tour on the graph
 * represented by the 'points' argument. the tour is stored in the 'tour'
 * argument, and the starting point for the tour is determined by the 
 * 'start' argument.
 */

void nearestNeighbor( ptList *tour, ptList *points, int start ) {

	Point *curr = NULL;			// tracks 'current' point in our NN tour
	Point *closest = NULL;		// tracks point closest to curr
	Point *candidate = NULL;	// tracks 'candidate' points used to determine closest
	Point *copy = NULL;			// used to temporarily store copies of Point structs

	int i, j;

	curr = listElem(points, start);
	curr->visited = TRUE;

	// add first element to list
	copy = makePoint(curr->id, curr->x, curr->y);
	addPtList( tour, copy );

	// now, while route size < points size:
	while( tour->size < points->size ) {

		i = 0;

		// skip all visited vertices at the front of the points list
		while( listElem( points, i )->visited == TRUE ) {
			i++;
		}

		closest = listElem(points, i);

		// add one to i to account for the chosen point
		for(j = i + 1; j < points->size; j++ ) {

			candidate = listElem(points, j);

			if( candidate->visited == FALSE ) {

				// compare distance to our currently-chosen closest
				if( distanceTo(candidate, curr) < distanceTo(closest, curr) ) {
					closest = candidate;
				}
			}
		}

		// now, we have found the closest point to curr: mark it as visited, 
		// copy it into the tour struct, and set curr to this closest point.

		closest->visited = TRUE;
		copy = makePoint(closest->id, closest->x, closest->y);
		addPtList( tour, copy );
		curr = closest;

	}

}

/*
 * Performs a run of the 2-opt algorithm. Note that we're using the 'best swap'
 * variation of the algorithm (taking the most beneficial swap for each iteration)
 * NOTE: adapted from pseudocode listed in "A High Speed 2-Opt TSP Solver", by 
 * Martin Burtscher, Texas State University. Source: 
 * http://on-demand.gputechconf.com/gtc/2014/presentations/S4534-high-speed-2-opt-tsp-solver.pdfj
 */

void twoOpt( ptList *tour, double startTime, double numSeconds ) {

	int numNodes = (tour)->size;
	int i, k;	// indices into the tour; starting/ending points for our 'slice'

	int best_change;	// best overall change, one iteration
	int min_i, min_k;	// 'slice' points producing best overall change

	// thread-private variables: used to find local mins, slice points per-thread
	int change;
	int local_best;
	int local_min_i;
	int local_min_k;

	// while there are beneficial swaps to make...

	do {

		// reset variables at the start of each iteration
		change = 0;
		best_change = 0;
		min_i = -1;
		min_k = -1;

		// setting up parallel section: divide up iteration among 16 threads,
		// find per-thread minimums, then compare against an overall minimum

		#pragma omp parallel default(none) shared(numNodes, tour, best_change, min_i, min_k) private(k, i, change, local_best, local_min_i, local_min_k) num_threads(16)
		{

			local_best = 0;
			local_min_i = -1;
			local_min_k = -1;

			// for each edge pair combination in the graph...

			# pragma omp for
			for(i = 1; i < numNodes - 1; i++) {
				for(k = i + 1; k < numNodes; k++) {

					// determine the change in tour length resulting from a swap
					change = swapDistance(tour, i, k);

					// if we've found a better swap
					if( local_best > change ) {

						// update our local best variables
						local_best = change;
						local_min_i = i;
						local_min_k = k;
					}
				}

			}

			// one thread can execute this section at a time:
			// compare local values to overall values

			#pragma omp critical
			{
				if( local_best < best_change ) {
					best_change = local_best;
					min_i = local_min_i;
					min_k = local_min_k;
				}
			}

		}

		// if we have found a positive change

		if( best_change < 0 ) {

			// swap endpoints of one 'slice' and re-combine
			twoOptSwap( tour, min_i, min_k );

			//printf("new length: %d\n", getTourLength(tour));
			//printf("delta: %d\n", best_change);

		}

	} while(best_change < 0 && ( omp_get_wtime() - startTime ) <= (double) numSeconds );
		//difftime(time(NULL), startTime) <= (double) numSeconds);

}

/*
 * Reverses items start to end in-place. i is the starting index of the 
 * subarray to be reversed, and k is the ending index. function reverses
 * from tour[i] to tour[k], inclusive.
 */

void twoOptSwap( ptList *tour, int i, int k ) {

	// placeholder 
	Point *tmp = NULL;

	// while there are elements left to reverse 
	while ( i < k ) {

		// swap elements
		tmp = tour->list[i];

		tour->list[i] = tour->list[k];
		tour->list[k] = tmp;

		i++;	// increment "lower" iterator 
		k--;	// decrement "upper" iterator
	}

}

/*
 * Gets the length of the tour passed as a parameter
 */

int getTourLength( ptList *tour ) {

	double tourDistance = 0.0;

	int i;

	// for each point in the tour, get distance to next point
	for(i = 0; i < tour->size - 1; i++) {
		tourDistance += distanceTo( listElem( tour, i ), listElem( tour, i + 1 ) );
	}

	// now, account for last city back to first
	tourDistance += distanceTo( listElem( tour, i ), listElem( tour, 0 ) );

	return tourDistance;
}

/*
 * Output results to file.
 */

void outputResults( char *filename, ptList *tour ) {

	FILE *of = fopen( filename, "w");

	// output tour length
	fprintf( of, "%d\n", getTourLength(tour) );

	int i;

	// output id of each point in the tour, in order
	for( i = 0; i < tour->size; i++ ) {
		fprintf( of, "%d\n", listElem( tour, i )->id );
	}

	fclose(of);

}

/*
 * Determines the change in tour distance as the result of a single endpoint swap.
 * Used to determine the 'best' swap to make per-iteration. tour is a pointer to the 
 * current best tour, i marks the starting point of our swap, and k marks the 
 * ending point (recall that, to swap, we take tour[i..k] and reverse its order.
 */

int swapDistance( ptList *tour, int i, int k ){

	// declare variables: ee1/ee2 are existing edge lengths, ce1/ce2
	// are candidate edge lengths
	int end, ee1, ee2, ce1, ce2;

	// account for edge case where we need to 'wrap around' to the beginning 
	// of the tour - 'end' represents the point after k in the current tour.
	if( k == tour->size - 1 ) {
		end = 0	;
	}
	else {
		end = k + 1;
	}
	
	// candidate edges: distances between vertices if we choose to swap
	ce1 = distanceTo( listElem( tour, i - 1 ), listElem( tour, k) );
	ce2 = distanceTo( listElem( tour, i ), listElem( tour, end ) );

	// existing edges: distances between vertices in the current tour.
	ee1 = distanceTo( listElem( tour, i - 1 ), listElem(tour, i) );
	ee2 = distanceTo( listElem( tour, k ), listElem(tour, end) );

	return ce1 + ce2 - ee1 - ee2;

}

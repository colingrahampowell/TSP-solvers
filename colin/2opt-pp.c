/*
 * CS325 - Project 2
 * Group 34
 * Nearest Neighbor + 2-Opt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include "point.h"
#include "ptList.h" 
#include <omp.h>

#define TRUE 1
#define FALSE 0

int getTourLength( ptList *tour );
void nearestNeighbor( ptList *tour, ptList *points, int start );

void twoOpt( ptList **tour );
void twoOptSwap( ptList *tour, int i, int k);
int swapDistance( ptList *tour, int i, int k );

void readFile( char *filename, ptList *points );
void outputResults( char *filename, ptList *tour );

/*
 * file-handling code adapted from getline() man pages: linux.die.net/man/3/getline
 */

int main( int argc, char **argv ) {

	srand(time(NULL));

	char filename[255];
	memset( filename, '\0', sizeof(filename) / sizeof( filename[0]) );	
	strcpy(filename, argv[1]);

	ptList* points = initPtList();

	readFile( argv[1], points );

	// nearest-neighbor 
	ptList *tspTour = initPtList(); 
	nearestNeighbor( tspTour, points, rand() % (points->size) );

	printf("starting tour length: %d\n", getTourLength(tspTour) );

	// TESTING TESTING: 2-opt
	twoOpt( &tspTour );
	printf("tour length: %d\n", getTourLength(tspTour) );

	outputResults( strcat(filename, ".tour"), tspTour );

	// free memory allocated for points
	cleanPtList( points );
	cleanPtList( tspTour );

	return 0;		

}

/*
 * reads a file containing tsp data
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

		tok = strtok( line, " ");
		id = atoi(tok);

		tok = strtok( NULL, " ");
		x = atoi(tok);

		tok = strtok( NULL, " ");
		y = atoi(tok);

		new_pt = makePoint(id, x, y);
		addPtList( points, new_pt );

	}

	free(line);	
	fclose( fp );	

}


/* 
 * Performs nearest-neighbor tour construction
 */

void nearestNeighbor( ptList *tour, ptList *points, int start ) {

	Point *curr = NULL;	
	Point *closest = NULL;
	Point *candidate = NULL;
	Point *copy = NULL;

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
		
		closest->visited = TRUE;
		copy = makePoint(closest->id, closest->x, closest->y);
		addPtList( tour, copy );

	}	

}

/*
 * Performs 2-opt optimization
 */


void twoOpt( ptList **tour ) {

	int numNodes = (*tour)->size;
	int i, k;

	int best_change;

	int change, max_i, max_k;

	int local_best;
	int local_max_i;
	int local_max_k;

	do {
		change = 0;
		best_change = 0;
		max_i = -1;
		max_k = -1;

		#pragma omp parallel default(none) shared(numNodes, tour, best_change, max_i, max_k) private(k, i, change, local_best, local_max_i, local_max_k) num_threads(16)
		{

			local_best = 0;
			local_max_i = -1;
			local_max_k = -1;

			# pragma omp for
			for(i = 1; i < numNodes - 1; i++) {
				for(k = i + 1; k < numNodes; k++) {
					change = swapDistance(*tour, i, k);

					if( local_best > change ) {

						local_best = change;
						local_max_i = i; 
						local_max_k = k;
					}
				}

			}

			#pragma omp critical
			{
				if( local_best < best_change ) {
					best_change = local_best;
					max_i = local_max_i;
					max_k = local_max_k;	
				}
			}

		}

		if( best_change < 0 ) { 

			twoOptSwap( *tour, max_i, max_k );

			//	printf("new length: %d\n", getTourLength(*tour));
			//printf("delta: %d\n", best_change);

		}

	} while(best_change < 0);

}

/*
 * Reverses items start to end in-place
 */

void twoOptSwap( ptList *tour, int i, int k ) {


	Point *tmp = NULL;
	
	while ( i < k ) {
			
		tmp = tour->list[i];
			
		tour->list[i] = tour->list[k];
		tour->list[k] = tmp;
			
		i++;
		k--;
	}

}


int getTourLength( ptList *tour ) {

	double tourDistance = 0.0;
	
	int i;

	for(i = 0; i < tour->size - 1; i++) {
		tourDistance += distanceTo( listElem( tour, i ), listElem( tour, i + 1 ) );
	}
	
	// now, account for last city back to first
	tourDistance += distanceTo( listElem( tour, i ), listElem( tour, 0 ) );

	return tourDistance;
}


void outputResults( char *filename, ptList *tour ) {

	FILE *of = fopen( filename, "w");

	fprintf( of, "%d\n", getTourLength(tour) );
	
	int i;
	
	for( i = 0; i < tour->size; i++ ) {
		fprintf( of, "%d\n", listElem( tour, i )->id );
	}

	fclose(of);

}

/*
 * checks delta-distance between two edge pairs, if endpoints are swapped
 * returns 
 */

int swapDistance( ptList *tour, int i, int k ){

	int end, ee1, ee2, ce1, ce2;

	if( k == tour->size - 1 ) {
		end = 0	;
	}
	else {
		end = k + 1;
	}
	// candidate edge lengths
	ce1 = distanceTo( listElem( tour, i - 1 ), listElem( tour, k) );
	ce2 = distanceTo( listElem( tour, i ), listElem( tour, end ) );

	// existing edge lengths	
	ee1 = distanceTo( listElem( tour, i - 1 ), listElem(tour, i) );
	ee2 = distanceTo( listElem( tour, k ), listElem(tour, end) );

	// returns delta between candidates and existing edges (negative for improvement)
	return ce1 + ce2 - ee1 - ee2;

}

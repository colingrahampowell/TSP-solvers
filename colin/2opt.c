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

#define TRUE 1
#define FALSE 0

int getTourLength( ptList *tour );
void nearestNeighbor( ptList *tour, ptList *points );

void twoOpt( ptList **tour );
void twoOptSwap( ptList *old_tour, ptList *new_tour, int start, int end );

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

	// TESTING: checking that we're actually getting data in the list	
	/*
	for( int i = 0; i < points->size; i++ ) {
		new_pt = listElem( points, i );
		printf("id: %d x: %d y: %d\n", new_pt->id, new_pt->x, new_pt->y);
	}
	*/

	// nearest-neighbor 
	ptList *tspTour = initPtList(); 

	nearestNeighbor( tspTour, points );

	//printf("length: %d\n", tspptList->tourLength );
	/*
	for( int i = 0; i < tspptList->route->size; i++) {
		printf("id: %d\n", listElem( tspptList->route, i )->id);
	}
	*/

	// TESTING TESTING: 2-opt
	twoOpt( &tspTour );

	printf("tour length: %d\n", getTourLength(tspTour) );

	/*	
	printf("length: %d\n", tspptList->tourLength );
	for( int i = 0; i < tspptList->route->size; i++) {
		printf("id: %d\n", listElem( tspptList->route, i )->id);
	}
	*/
		
	outputResults( strcat(filename, ".tour"), tspTour );

	// free memory allocated for points
	cleanPtList( points );
	cleanPtList( tspTour );

	return 0;		

}

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

void nearestNeighbor( ptList *tour, ptList *points ) {

	Point *curr = NULL;	
	Point *closest = NULL;
	Point *candidate = NULL;
	Point *copy = NULL;

	int i, j;

	curr = listElem(points, rand() % points->size );
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

	// test transferral to new struct	
/*
	for(int i = 0; i < tour->size; i++ ) {
		curr = listElem(tour, i);
		printf("id: %d, x: %d, y:%d\n", curr->id, curr->x, curr->y);			
	}	
*/
}


void twoOpt( ptList **tour ) {

	printf("starting tour length: %d\n", getTourLength(*tour) );

	int improve = TRUE;
	int numNodes = (*tour)->size;

	int i, k;

//	int distance = 0;

//	int existEdge1, existEdge2, canEdge1, canEdge2;
//	int end;

	ptList *new_tour;

	while( improve ) {

		improve = FALSE;
		
		for(i = 1; i < numNodes - 1; i++) {
			
			for(k = i + 1; k < numNodes; k++) {
	
				new_tour = initPtList();				

				twoOptSwap( *tour, new_tour, i, k );
			
				if( getTourLength(new_tour) < getTourLength(*tour) ) {
				
					printf("new tour length: %d\n", getTourLength(new_tour) );

					//printf("Swapped! i=%d, k=%d\n", i, k);
					cleanPtList( *tour );
					*tour = NULL;	

					*tour = new_tour;
					improve = TRUE;

					break;				
				}
		
				cleanPtList( new_tour );

				/*
				// if k is at the last node, we need to 'wrap around' to the first node for k + 1
				if( k == numNodes - 1 ) {
					end = 0;
				}
				else {
					end = k + 1;
				}

				// get length of existing edges
				existEdge1 = distanceTo( listElem(tour, i), listElem(tour, i + 1) );
				existEdge2 = distanceTo( listElem(tour, k), listElem(tour, end) );

				// get length of swapped edges
				canEdge1 = distanceTo( listElem(tour, i), listElem(tour, end) ); 	
				canEdge2 = distanceTo( listElem(tour, i - 1), listElem(tour, k) );
			
				if( existEdge1 + existEdge2 > canEdge1 + canEdge2 ) {
				
					printf("Swap 'em! i=%d, k=%d\n", i, k);
					printf("existing edges: %d\n", existEdge1 + existEdge2);
					printf("candidate edges: %d\n", canEdge1 + canEdge2);
			
					twoOptSwap( tour, i, k );	
					improve = TRUE;
					break;				

				}
			*/

			}

			if( improve ) { break; }

		}	
	}
}

/*
 * Reverses items start to end in-place
 */

void twoOptSwap( ptList *old_tour, ptList *new_tour, int start, int end ) {

	int i = start;
	int k = end;
	
	int r, s, t;

	Point *tmp = NULL;

//	printf("i: %d, k: %d\n", i, k);

	//printf("beginning 2optswap!\n");

	for(r = 0; r < i; r++) {
	//	printf("r=%d\n", r);
		tmp = makePoint( listElem(old_tour, r)->id, listElem(old_tour,r)->x, listElem(old_tour,r)->y);	
		addPtList( new_tour, tmp);
		//addPtList( new_tour, listElem(tour, r) );
	}

	for(s = k; s >= i; s--) {
	
		tmp = makePoint( listElem(old_tour, s)->id, listElem(old_tour,s)->x, listElem(old_tour,s)->y);	
		addPtList( new_tour, tmp);
	//	printf("s=%d\n", r);
//		addPtList( new_tour, listElem(tour, s) );
	}

	for(t = k + 1; t < old_tour->size; t++) {

		tmp = makePoint( listElem(old_tour, t)->id, listElem(old_tour,t)->x, listElem(old_tour,t)->y);	
		addPtList( new_tour, tmp);
//		addPtList( new_tour, listElem(tour, t) );
	}

	//printf("2optswap complete!\n");

	/*
	while ( i < k ) {
			
		tmp = listElem( tour, i);
//		tmp = tour->list[i];

//		printf("tmp id: %d\n", tour->list[i]->id);

		// TODO: add an abstracting function to avoid direct array access
			
		tour->list[i] = tour->list[k];
		tour->list[k] = tmp;
			
		i++;
		k--;
	}
	

	printf("loop complete\n");
	*/

}


int getTourLength( ptList *tour ) {

	double tourDistance = 0.0;
	
	int i;

	for(i = 0; i < tour->size - 1; i++) {
		tourDistance += distanceTo( listElem( tour, i ), listElem( tour, i + 1 ) );
//		printf("city %d in tour, id: %d\n", i, listElem(tour, i)->id );
	}
	
	// now, account for last city back to first
	tourDistance += distanceTo( listElem( tour, i ), listElem( tour, 0 ) );
//	printf("city %d in tour, id: %d\n", i, listElem(tour, i)->id );

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

/*
 * CS325: Group 34 Project
 * Point: implementation file
 *
*/

#include "point.h"
#include <stdlib.h>
#include <math.h>

Point *makePoint(int id, int x, int y) {

	Point *pt = (Point *) malloc ( sizeof( Point ) );
	pt->id = id;
	pt->x = x;
	pt->y = y;
	
	pt->visited = FALSE;

	return pt;

}

double distanceTo( Point *a, Point *b ) {
	
	int dX = a->x - b->x;
	int dY = a->y - b->y;

	double distance = sqrt( (double) (dX * dX + dY * dY) );

	// rounding distance nearest int 
	distance += 0.5;
	return (int) distance;

}


int isVisited( Point *a ) {

	return ( a->visited == TRUE );

}

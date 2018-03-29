#ifndef POINT_H
#define POINT_H

#define FALSE 0
#define TRUE 1
 
// Point struct - basis for our Point library

typedef struct {
	int id;
	int x;
	int y;
	int visited;
} Point;


// Point-related functions:
Point *makePoint( int id, int x, int y);
double distanceTo( Point *a, Point *b );
int isVisited( Point *a );

#endif

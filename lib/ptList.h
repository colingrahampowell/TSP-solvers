/*
 * Colin Powell
 * CS372: Introduction to Computer Networks
 * Programming Assignment 2
 * 8/5/17
 * Description: ptList data structure header file. Declares functions
 * used to manipulate a ptList struct, and defines the struct itself.
 */

// include guards
#ifndef PT_LIST_H 
#define PT_LIST_H 

#include "point.h"

// ptList: struct used to store a variable-length array of Points

typedef struct {
	int size;
	int capacity;
	Point **list;
} ptList;

// list function declarations:

ptList* initPtList();
int addPtList(ptList *dList, Point *pt);
void cleanPtList(ptList *dList);

Point* listElem( ptList *dList, int idx );
int listSize( ptList *dList );

#endif

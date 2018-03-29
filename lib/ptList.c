
/*
 * Name: Colin Powell
 * CS372: Introduction to Computer Networks
 * Programming Assignment 2
 * 8/5/17
 * Description: ptList data structure implementation file. Defines functions
 * for intializing, adding to, and clearing a ptList struct.
 * CITATION: Basic data structure arrangement, general list of functions adapted from:
 * CS261 Lecture Notes, Fall 2016. Oregon State University.
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ptList.h"

/*
 * Name: initPtList()
 * Description: initializes a ptList struct, and returns a pointer to
 * a dynamically-allocated block of memory in which that struct is stored.
 * Sets initial size to 0, and capacity to 4.
 * Receives: none.
 * Returns: A pointer to an initialized ptList struct. This pointer will
 * need to be freed later (i.e., with cleanPtList() to prevent a memory leak.
 * Preconditions: none.
 */

ptList *initPtList( ) {

	ptList *pList = (ptList*) malloc(sizeof(ptList));

	pList->size = 0;				// array is empty at start
	pList->capacity = 32768;		// reasonable initial capacity
	
	// allocate memory for the array of pointers to point
	pList->list = (Point**) malloc( sizeof(Point*) * pList->capacity);

	return pList;		// return a pointer to the struct

}

/*
 * Name: addPtList()
 * Description: Adds the specified command to the ptList's list struct, 
 * updating size and capacity accordingly. If the list array is at capacity, 
 * the function doubles the amount of memory allocated for the list array and
 * updates the capacity attribute. 
 * Receives: pList, a pointer to an initialized ptList struct; item, 
 * a pointer to a point array containing the command to add.
 * Returns: If the realloc() operation fails, the function returns -1
 * Otherwise, it returns the new size of the array.
 * Preconditions: ptList must be initialized, and item must be a null-
 * terminated array of point.
*/

int addPtList(ptList *pList, Point *pt) {

	// if adding to list will overflow it, double the capacity and reallocate 
	// memory for the list array

	if( pList->size >= pList->capacity ) {

		// double capacity, then reallocate memory
		pList->capacity *= 2;
		Point **newArray = (Point **) realloc(pList->list, sizeof(Point*) * pList->capacity);

		// if the realloc returns NULL, something went wrong. terminate.
		if(newArray == NULL) {
			perror("realloc");
			return -1;
		}

		// otherwise, point list to this newly-allocated space.
		else {
			pList->list = newArray;
		}
	}

	// otherwise, add to the list array and increment size.
	// malloc() space for the new point, then copy contents of item into the array
	
//	pList->list[pList->size] = malloc( sizeof(Point) );
	pList->list[pList->size] = pt; 

	pList->size++;

	return pList->size;	// return the new size of the array
}

/*
 * Name: cleanPtList()
 * Description: Frees all memory allocated to the struct's list array, 
 * then frees memory allocated for the struct itself.
 * Receives: pList, a pointer to an initialized ptList struct.
 * Returns: none.
 * Preconditions: ptList must have been initialized with initPtList().
 */

void cleanPtList(ptList *pList) {

	int i;		// loop counter

	// free memory allocated for all elements in the list array
	for( i = 0; i < pList->size; i++ ) {
		free( pList->list[i] );
		pList->list[i] = NULL;
	}
	
	// free the memory allocate for the list array itself
	free( pList->list );
	
	// free the pList struct itself
	free( pList );

}

/* 
 * Name: listElem()
 * Description: gets the specified element in the ptList struct's
 * list array. 
 * Receives: pList, a pointer to a ptList struct; elem, an int 
 * that is the (zero-indexed) number of the element to get from
 * the list.
 * Returns: the element in the list at index elem, or NULL if elem
 * is not a valid index into pList.
 * Preconditions: pList is a pointer to an initialized ptList
 * struct. 
 */

Point* listElem( ptList *pList, int idx ) {

	// if elem is greater than or equal to the list's size
	// return NULL

	if( idx >= pList->size ) {
		return NULL;
	}
	// otherwise, return the point* at index elem of the list array 
	else {
		return pList->list[ idx ];
	}

}


/* 
 * Name: listSize()
 * Description: returns the total number of elements in the ptList
 * struct's list array. 
 * Receives: pList, a pointer to an initialized ptList struct.
 * Returns: the number of elements in the pList struct's list array.
 * Preconditions: pList must be a pointer to an initialized ptList
 * struct. 
 */

int listSize( ptList *pList ) {
	return pList->size;	
}


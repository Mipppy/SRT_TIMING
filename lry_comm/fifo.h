/*
** fifo.h
** 
** first-in, first-out buffer
**
*/

/*
 * lry 16sep00
 *
 * lry 17nov00 generalize from byte to blob as the "item" in the queue
 *
 */


#ifndef FIFO_H
#define FIFO_H

#include <windows.h> // for BOOL (!)

//
// struct containing all the info for a fifo
//
struct Fifo
{
	long head;             // index at which to put newest item
	long tail;             // index from which to pop oldest item
	long size;             // length of the buffer
	BOOL is_full;          // TRUE if an overrun is about to occur
	char *buf;             // ptr to a buffer of length (size * sizeof_item)
   int sizeof_item;       // size of item actually stored in fifo
};


//
// push a new item into the fifo
// return TRUE if there is room to push the item or FALSE if overrun occurs
//
BOOL fifo_push_item( struct Fifo *theFifo, void *new_item );

//
// push an array of new items into the fifo
// return number of items pushed successfully
// VVV implemented as a bunch of calls to push_item()
// but could be more efficient to use a block move
//
long fifo_push_array( struct Fifo *theFifo, void *new_array, long new_array_length );

//
// if an item is available in the fifo
//   copy oldest item to caller's location (*item) and return TRUE
// else (if the fifo is empty) return FALSE 
//
BOOL fifo_pop_item( struct Fifo *theFifo, void *item );

//
// pop up to 'array_length' oldest items from the fifo to 'array'
// return number of items actually copied (which could be zero)
// VVV implemented rather inefficiently as a bunch of calls to pop_item
//
long fifo_pop_array( struct Fifo *theFifo, void *array, long array_length );

//
// empty the buffer
// return number of items flushed in case the caller wants to know
//
long fifo_flush( struct Fifo *theFifo );

//
// return TRUE if the fifo is empty
//
BOOL fifo_is_empty( struct Fifo *theFifo );

#endif//FIFO_H

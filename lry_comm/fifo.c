/*
** fifo.c
** 
** first-in, first-out buffer
**
*/

/*
 * lry 16sep00
 *
 * lry 17nov00 generalize from byte to blob
 *
 * lry 22feb01 add overrun check
 */

#include <windows.h>  // only for BOOL/TRUE/FALSE !
#include "fifo.h"


/* *** fifo logic *** */
/*
 * The 'head' is the index of the queue at which to put a new item.
 * The 'tail' is the index from which to pop the oldest item.
 * In the present design, both indexes increase until they reach the
 * maximum number of item 'size' then are recirculated to zero.
 * 
 * At initialization, or at any time that the queue is empty, 'head'
 * and 'tail' are the same.  When an item is added 'head' is incremented;
 * if 'head' equals 'tail' then an overrun is about to occur:  'is_full'
 * is set to TRUE and will be cleared only if an item is popped before
 * the next push.
 *
 * If an overrun is pending when we try to push another item we
 * refuse to add the new item so the caller can take appropriate
 * measures without losing the information already in the queue.
 *
 * When there is a request to pop the oldest item we test for
 * head/tail equality first to see if the item at 'tail' is valid.
 *
 * The logic would be simpler if we declared an overrun instead of an
 * "is_full" when 'head' catches up with 'tail', but this logic
 * limits the useful size of the fifo to one less than the 'size' of
 * the storage buffer in use.  I chose the more complicated logic
 * because the queue size might be some maximum machine-memory block
 * size and the user might frequently want to transfer a block of that
 * same maximum size.
 *
 * In the applications using this code at present each Fifo is serviced
 * by only one read thread and one write thread.  Under this limitation
 * we can use a simple flag to prevent overrun.
 */

BOOL fifo_push_item( struct Fifo *theFifo, void *new_item )
{
	BOOL okay_to_push = !theFifo->is_full;
   
	if ( okay_to_push )
	{
      long new_head = theFifo->head + theFifo->sizeof_item;
      
      if ( new_head >= theFifo->size ) new_head = 0L;

		memcpy( &(theFifo->buf[ theFifo->head ]), new_item, theFifo->sizeof_item );

		if ( new_head == theFifo->tail ) theFifo->is_full = TRUE;

		theFifo->head = new_head;
	}
	
	return okay_to_push;
}


long fifo_push_array( struct Fifo *theFifo, void *new_array, long new_array_length )
{
	long number_pushed = 0L;
	
   char *ptr_to_bytes = (char *)new_array; // cast so the pointer can be incremented

	// while there is more to push and the last push was successful
	BOOL last_push_succeeded = TRUE;

	while ( new_array_length-- > 0L && last_push_succeeded )
	{
		last_push_succeeded = fifo_push_item( theFifo, (void *)ptr_to_bytes );
		++number_pushed;
      ptr_to_bytes += theFifo->sizeof_item;
	}

	return number_pushed;
}


BOOL fifo_pop_item( struct Fifo *theFifo, void *item )
{
	BOOL item_is_available = !fifo_is_empty( theFifo );
	
	if ( item_is_available )
	{
      long new_tail = theFifo->tail + theFifo->sizeof_item;

		if ( new_tail == theFifo->size ) new_tail = 0L;

		memcpy( item, &(theFifo->buf[ theFifo->tail ]), theFifo->sizeof_item );
      
      theFifo->tail = new_tail;

      theFifo->is_full = FALSE;
	}

	return item_is_available;
}


long fifo_pop_array( struct Fifo *theFifo, void *array, long array_length )
{
	long number_popped = 0L;
	
   char *ptr_to_bytes = (char *)array; // cast so ptr can be incremented

	// while there is more to pop and room remains in the result_string
	BOOL last_pop_succeeded = TRUE;
	
	while ( array_length-- > 0L && last_pop_succeeded )
	{
		// try to get another item
		last_pop_succeeded = fifo_pop_item( theFifo, (void *)ptr_to_bytes );
		
		if ( last_pop_succeeded )
		{
			ptr_to_bytes += theFifo->sizeof_item;
			++number_popped;
		}
	}

	return number_popped;
}


long fifo_flush( struct Fifo *theFifo )
{
	long number_remaining = theFifo->head - theFifo->tail;

	if ( number_remaining < 0L )
	  	number_remaining += theFifo->size / theFifo->sizeof_item;
	
	theFifo->head = theFifo->tail = 0L;
	theFifo->is_full = FALSE;
	
	return number_remaining;
}


BOOL fifo_is_empty( struct Fifo *theFifo )
{
	return ( !theFifo->is_full && theFifo->head == theFifo->tail );
}

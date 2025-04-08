/*
** commbufs.h
** 
** maintain application-specific buffers for comm port operations
**
*/

/*
 * lry 16sep00
 *
 * lry 21feb01
 * add comm_set_overflow_handler()
 */


#ifndef COMMBUFS_H
#define COMMBUFS_H

#include <windows.h> // for BOOL (!)
#include "commport.h"


extern HANDLE commbuf_hEvent_receive;
extern HANDLE commbuf_hEvent_transmit;

// must be called first, to initialize the buffers and the events
BOOL create_commbufs( int sizeof_receive_item, int sizeof_transmit_item );

// must be called last, to destroy events
BOOL destroy_commbufs( void );

// push an item to the buffer
BOOL push_item_receive(  read_t *p_item );
BOOL push_item_transmit(  write_t item );

// push an array of items to the buffer
long push_array_receive( read_t *p_array, long array_length );
long push_array_transmit( write_t *p_array, long array_length );

// pop an item from the buffer
BOOL pop_item_receive( read_t *p_item );
BOOL pop_item_transmit( write_t *p_item );

// pop an array of items from the buffer
long pop_array_receive( read_t *p_array, long array_length );
long pop_array_transmit( write_t *p_array, long array_length );

// flush the buffer
long commbuf_flush_receive( void );
long commbuf_flush_transmit( void );

// register a function to be called if comm buffer overflows
void comm_set_rcvbuf_full_handler( void (*new_handler)(void) );

#endif//COMMBUFS_H

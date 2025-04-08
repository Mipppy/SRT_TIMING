/*
** commbufs.c
** 
** maintain application-specific buffers for comm port operations
**
*/

/*
 * lry 16sep00
 * lry 21feb01 - add overflow handlers
 */

#include <windows.h>
#include <stdio.h>    // printf()
#include "winfile.h"
#include "fifo.h"     // implement the comm buffers with FIFOs
#include "errors.h"   // for error reporting
#include "commbufs.h"

//
// Programming Notes:
//
// The fifos can be accessed from two threads so should be made completely
// thread-safe with some critical sections or semaphores.  The simple accesses
// are thread-safe because single 32-bit reads and writes are atomic.  However,
// the buffer-vacant and buffer-occupied events are signaled after items are
// inserted and removed so they could be set or cleared erroneously in race
// conditions.
//
// The Win32 calls underlying both the win_set_event() and the win_clear_event()
// wrapper functions can fail.  Their return values are tested in the wrappers
// and passed back as wrapper return values.  Here, the return values are
// ignored; the error reporting in the wrapper functions should suffice.
// 

/* ********************************************** */
/* *** buffers for transmitting and receiving *** */
/* ********************************************** */
//
// VVV for now, these buffers are allocated statically
//
// SIZEOF_RECEIVE_FIFO and SIZEOF_TRANSMIT_FIFO are the number of bytes
// reserved for making a buffer.  The number of elements that can be stored
// in the buffer depends on the size of the elements stored.  For example,
// the maximum number of elements that can be stored in the receive Fifo is:
// 
// max_elements_in_receive_buffer = SIZEOF_RECEIVE_FIFO / sizeof(read_t)
//                        
// At present, sizeof(read_t) = 12 and sizeof(write_t) = 1
// 
#define SIZEOF_RECEIVE_FIFO  0x400000L // Divide by 12!  See above note.
#define SIZEOF_TRANSMIT_FIFO 0x40000L // Divide by 1.

static char  receive_byte_fifo [ SIZEOF_RECEIVE_FIFO  ];
static char  transmit_byte_fifo[ SIZEOF_TRANSMIT_FIFO ];

//
// the two FIFOs
//
static struct Fifo receive_fifo =
{
   0L,
   0L,
   0L,
   FALSE,
   receive_byte_fifo,
   0
};

static struct Fifo transmit_fifo =
{
   0L,
   0L,
   0L,
   FALSE,
   transmit_byte_fifo,
   0
};

/* *********************************************** */
/* *** event to signal that buffer has content *** */
/* *********************************************** */

HANDLE commbuf_hEvent_receive = INVALID_HANDLE_VALUE;
HANDLE commbuf_hEvent_transmit = INVALID_HANDLE_VALUE;

/* ***************************************************** */
/* *** variables and functions to report buffer full *** */
/* ***************************************************** */

static void (*rcvbuf_full_handler)(void) = 0;

static void on_rcvbuf_full( void )
{
   if ( rcvbuf_full_handler )
      (*rcvbuf_full_handler)();
}

/* ************************ */
/* *** public functions *** */
/* ************************ */

BOOL create_commbufs( int sizeof_receive_item, int sizeof_transmit_item )
{
   receive_fifo.sizeof_item = sizeof_receive_item;
   transmit_fifo.sizeof_item = sizeof_transmit_item;

   receive_fifo.size = ( SIZEOF_RECEIVE_FIFO / sizeof_receive_item ) * sizeof_receive_item;
   transmit_fifo.size = ( SIZEOF_TRANSMIT_FIFO / sizeof_transmit_item ) * sizeof_transmit_item;

	commbuf_hEvent_receive = CreateEvent( NULL, TRUE, FALSE, NULL );
   commbuf_hEvent_transmit = CreateEvent( NULL, TRUE, FALSE, NULL );

	return (    commbuf_hEvent_receive != INVALID_HANDLE_VALUE
            && commbuf_hEvent_transmit != INVALID_HANDLE_VALUE );
}


BOOL destroy_commbufs( void )
{
   BOOL result = TRUE;
   
   if ( commbuf_hEvent_receive != INVALID_HANDLE_VALUE )
   {
      result = CloseHandle( commbuf_hEvent_receive ) && result;
      commbuf_hEvent_receive = INVALID_HANDLE_VALUE;
	}

   if ( commbuf_hEvent_transmit != INVALID_HANDLE_VALUE )
   {
      result = CloseHandle( commbuf_hEvent_transmit ) && result;
      commbuf_hEvent_transmit = INVALID_HANDLE_VALUE;
	}

	return result;
}

BOOL push_item_receive(  read_t *p_item )
{
   BOOL result = fifo_push_item( &receive_fifo, p_item );
   
   if ( result )
   {
      win_set_event( commbuf_hEvent_receive );
      // ignore a failure in win_set_event
   }
   else
   {
      on_rcvbuf_full();
   }
   
   return result;
}

BOOL push_item_transmit(  write_t item )
{
   BOOL result = fifo_push_item( &transmit_fifo, &item );
   
   if ( result )
   {
      win_set_event( commbuf_hEvent_transmit );
      // ignore a failure in win_set_event
   }
   
   return result;
}

long push_array_receive( read_t *p_array, long array_length )
{
   long num_pushed = fifo_push_array( &receive_fifo, p_array, array_length );

   if ( num_pushed > 0L )
   {
      win_set_event( commbuf_hEvent_receive );
      // ignore a failure in win_set_event
   }
   else
   {
      on_rcvbuf_full();
   }

   return num_pushed;
}

long push_array_transmit( write_t *p_array, long array_length )
{
   long num_pushed = fifo_push_array( &transmit_fifo, p_array, array_length );

   if ( num_pushed > 0L )
   {
      win_set_event( commbuf_hEvent_transmit );
   }

   return num_pushed;
}

BOOL pop_item_receive( read_t *p_item )
{
   BOOL result = fifo_pop_item( &receive_fifo, p_item );
   
   if ( result )

   if ( fifo_is_empty( &receive_fifo ) )
      win_clear_event( commbuf_hEvent_receive );
      // ignore a failure in win_clear_event
   
   return result;
}

BOOL pop_item_transmit( write_t *p_item )
{
   BOOL result = fifo_pop_item( &transmit_fifo, p_item );

   if ( result )

   if ( fifo_is_empty( &transmit_fifo ) )
      win_clear_event( commbuf_hEvent_transmit );
      // ignore a failure in win_clear_event

   return result;
}

long pop_array_receive( read_t *p_array, long array_length )
{
   long num_popped = fifo_pop_array( &receive_fifo, p_array, array_length );
   

   if ( fifo_is_empty( &receive_fifo ) )
      win_clear_event( commbuf_hEvent_receive );
      // ignore a failure in win_clear_event
   
   return num_popped;
}

long pop_array_transmit( write_t *p_array, long array_length )
{
   long num_popped = fifo_pop_array( &transmit_fifo, p_array, array_length );
   

   if ( fifo_is_empty( &transmit_fifo ) )
      win_clear_event( commbuf_hEvent_transmit );
      // ignore a failure in win_clear_event
   
   return num_popped;
}

long commbuf_flush_receive( void )
{
   long num_flushed = fifo_flush( &receive_fifo );
   
   win_clear_event( commbuf_hEvent_receive );
   
   return num_flushed;
}

long commbuf_flush_transmit( void )
{
   long num_flushed = fifo_flush( &transmit_fifo );
   
   win_clear_event( commbuf_hEvent_transmit );
   
   return num_flushed;
}

void comm_set_rcvbuf_full_handler( void (*new_handler)(void) )
{
   rcvbuf_full_handler = new_handler;
}

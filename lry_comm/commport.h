
/*******************************************/
/*******************************************/

/*
** commport.h
**
** the basic commport operations for Ernie
*/

#ifndef COMMPORT_H
#define COMMPORT_H

#include <windows.h>

#if   defined ( _USRDLLx )
  #define xxDLL_EXPORT _declspec ( dllexport )
#endif
#if ! defined ( _USRDLLx )
  #define xxDLL_EXPORT
#endif

#if   defined ( _USRDLL )
#endif

//
// the types of the transmitted and received data
// 

struct ReadInfo
{
   int character;      // the character, or -1 if no character received
   DWORD event;        // the accumulated winbase.h event flags
   DWORD error;        // the winbase.h error flag
};

typedef struct ReadInfo read_t; 

typedef unsigned char write_t;

//
// the type of the callback function
// 

typedef void (* rx_callback_func )( read_t *read_info_struct );

//
// open specified comm port at specified baudrate
// return TRUE if successful or FALSE if unsuccessful
//
// valid port numbers are 0 == COM1 through 3 == COM4
//
// third arg is a pointer to a callback function for processing received info
//   it can be 0, meaning that caller will use comm_read_byte()
//

int comm_open ( int port, long baudrate, int parity_flag, rx_callback_func process_rx_info ) ;


//
// close the (previously opened) comm port
//
void comm_close( void );


//
// put a packet into the transmit buffer
// return the number of elements accepted, which might be zero
//
long comm_write_string( unsigned char *ptr_to_tx_info, long length );

// Puts a single byte into the transmit buffer, and returns the number of elements accepted which might be zero.

long comm_write_byte ( unsigned char byte ) ;

//
// transfer a single element from read buffer into ReadInfo struct passed in
// return TRUE if buffer has an element, or FALSE if buffer is empty
// 
// note that the element may have either a valid character or a comm event
//

int comm_read_byte ( read_t *read_info_struct );

void comm_set_debug_flags ( int local_message_flags, int local_rx_debug_print_to_debug_file, int local_tx_debug_print_to_debug_file, int local_overlapped_io_method ) ; /* function declaration */

BOOL win_escape_comm_function ( DWORD function ) ;

#endif//COMMPORT_H

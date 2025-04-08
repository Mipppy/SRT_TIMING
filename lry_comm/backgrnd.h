/*
** backgrnd.h
**
**
** read and write procedures that will run in separate background threads
**
** these procedures transfer characters between the port opened with comm_open()
** and the application's transmit and receive fifo buffers
*/

/*
 * lry 18sep00
 */


#ifndef BACKGRND_H
#define BACKGRND_H

#include <windows.h>
#include "commport.h"


//
// public functions
// 
BOOL start_read_thread( void );
BOOL stop_read_thread( void );
BOOL start_write_thread( void );
BOOL stop_write_thread( void );

void set_read_processor( rx_callback_func new_read_processor );

#endif//BACKGRND_H

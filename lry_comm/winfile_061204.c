/*
** winfile.c
**
** wrappers for Windows file-handling funcs used in serial-port communications
*/

/*
 * lry 20sep00
 */

#include <windows.h>
#include <stdlib.h> // for _MAX_FNAME
#include <stdio.h>  // for sprintf()
#include "errors.h"
#include "winfile.h"


//
// call Win32 API EscapeCommFunction( function )
// to set serial-port control lines
// 
// function = 
// CLRDTR   = Clears the DTR (data-terminal-ready) signal. 
// CLRRTS   = Clears the RTS (request-to-send) signal. 
// SETDTR   = Sends the DTR (data-terminal-ready) signal. 
// SETRTS   = Sends the RTS (request-to-send) signal. 
// SETXOFF  = Causes transmission to act as if an XOFF character has been received. 
// SETXON   = Causes transmission to act as if an XON character has been received.
// SETBREAK = Suspends character transmission and places the transmission line
//            in a break state until the ClearCommBreak function is called
//            (or EscapeCommFunction is called with the CLRBREAK extended
//            function code). The SETBREAK extended function code is identical
//            to the SetCommBreak function. Note that this extended function
//            does not flush data that has not been transmitted. 
// CLRBREAK = Restores character transmission and places the transmission line
//            in a nonbreak state. The CLRBREAK extended function code is
//            identical to the ClearCommBreak function. 
//

/* *********************** */
/* *** error reporting *** */
/* *********************** */

//
// construct and report an error message from filename, line number
//   and the error number from Windows' GetLastError()
//   

static void report_windows_error( char *file, int line, char *msg )
{
	char system_msg[ 1023 ];

	char output[ 1023 ];
	size_t max_output = sizeof( output );

	char errno_str[ 8 ];
	char line_str[ 8 ];
	int a = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, system_msg, 1020, NULL );
	output[0] = 0; // clear the output
	sprintf( errno_str, "%d", GetLastError() );
	sprintf( line_str, "%d", line );

	safe_string_append( output, "Error ", max_output );
	safe_string_append( output, errno_str, max_output );
	safe_string_append( output, " at line ", max_output );
	safe_string_append( output, line_str, max_output );
	safe_string_append( output, " in file ", max_output );
	safe_string_append( output, file, max_output );
	safe_string_append( output, ":  ", max_output );
	safe_string_append( output, msg, max_output );
	
	report_error( output );
}


/* ***************************************************** */
/* *** static objects used by the winfuncs functions *** */
/* ***************************************************** */

static HANDLE hComm = INVALID_HANDLE_VALUE;

// we want to save some structs for restoration when we close down

static enum { OLD, NEW };

static COMMTIMEOUTS comm_timeouts[2];
static BOOL timeouts_are_initialized = FALSE;

static DCB dcb[2];
static BOOL dcb_is_initialized = FALSE;

static DWORD event_mask[2];
static BOOL event_mask_is_initialized = FALSE;

/* ************************************** */
/* *** wrappers for Windows functions *** */
/* ************************************** */


//
// open the specified port
//
BOOL win_open_file( unsigned int port )
{


    #if ( RELEASE_DATE >= 0x061203 )
      static LPCTSTR szPorts[] = { "COM1:", "COM2:", "COM3:", "COM4:", "COM5:", "COM6:", "COM7:", "COM8:"  } ;
    #endif
    #if ( RELEASE_DATE <  0x061203 )
      static LPCTSTR szPorts[] = { "COM1:", "COM2:", "COM3:", "COM4:" } ;
    #endif

	if ( port >= sizeof( szPorts ) / sizeof( LPCTSTR ) )
	{
		report_error( "win_open_file:  illegal port number" );
	}
	else if ( hComm != INVALID_HANDLE_VALUE )
	{
		report_error( "win_open_file:  handle already in use" );
	}
	else
   {
      hComm = CreateFile
		        (
                szPorts[port],                // LPCTSTR lpFileName - pointer to name of the file
                GENERIC_READ | GENERIC_WRITE, // DWORD dwDesiredAccess - access (read-write) mode 
                0,                            // DWORD dwShareMode - share mode - exclusive access
                0,                            // LPSECURITY_ATTRIBUTES 
                OPEN_EXISTING,                // DWORD dwCreationDistribution - how to create 
                FILE_FLAG_OVERLAPPED,         // DWORD dwFlagsAndAttributes - file attributes 
                0                             // HANDLE hTemplateFile - handle to file with attributes to copy 
	           );
		
		if ( hComm == INVALID_HANDLE_VALUE )
			report_windows_error( __FILE__, __LINE__, "CreateFile failed" );
   }

   return ( hComm != INVALID_HANDLE_VALUE );
}

//
// close the port with CloseHandle
//
BOOL win_close_file( void )
{
	BOOL file_closed = FALSE;

	if ( hComm != INVALID_HANDLE_VALUE )
	{
		if ( CloseHandle( hComm ) == 0 )
		{ 
			// failed to close - report error
			report_windows_error( __FILE__, __LINE__, "CloseHandle failed" );
		}

		// mark hComm as invalid whether CloseHandle succeeded or not
		hComm = INVALID_HANDLE_VALUE;
   }

	// return TRUE if hComm is now invalid
	// ie, an attempt to close an invalid handle is silently ignored
	return ( hComm == INVALID_HANDLE_VALUE );
}


//
// modify the comm_timeouts struct and make it the active one
//
// from the mttty sample:
// We need ReadIntervalTimeout here to cause the read operations
// that we do to actually timeout and become overlapped.
// Specifying 1 here causes ReadFile to return very quickly
// so that our reader thread will continue execution.
//
// from the SDK documentation "COMMTIMEOUTS":
// A value of MAXDWORD, combined with zero values for both the
// ReadTotalTimeoutConstant and ReadTotalTimeoutMultiplier members,
// specifies that the read operation is to return immediately with
// the characters that have already been received, even if no characters
// have been received. 
//
// my implementation (not using WaitCommEvent) must have these fixed values
// therefore there are no caller-configurable parameters in the arg list
BOOL win_modify_comm_timeouts( void )
{
	BOOL result = ( hComm != INVALID_HANDLE_VALUE );

	// if this is the first call, get the existing comm_timeouts
	if ( result && !timeouts_are_initialized )
	{
		if ( GetCommTimeouts( hComm, &comm_timeouts[OLD] ) != 0 )
		{
			// copy the old to the new in case we don't want to override every element
			memcpy( &comm_timeouts[NEW], &comm_timeouts[OLD], sizeof( COMMTIMEOUTS ) );
			timeouts_are_initialized = TRUE;
		}
		else
		{
			report_windows_error( __FILE__, __LINE__, "GetCommTimeouts failed" );
			result = FALSE;
		}
	}

	if ( result )
	{
		comm_timeouts[NEW].ReadIntervalTimeout         =   (DWORD)1;
		comm_timeouts[NEW].ReadTotalTimeoutMultiplier  =   (DWORD)0;
		comm_timeouts[NEW].ReadTotalTimeoutConstant    =   (DWORD)0;
		comm_timeouts[NEW].WriteTotalTimeoutMultiplier =   (DWORD)0;
		comm_timeouts[NEW].WriteTotalTimeoutConstant   =   (DWORD)0;

		if ( SetCommTimeouts( hComm, &comm_timeouts[NEW] ) == 0 )
		{
			report_windows_error( __FILE__, __LINE__, "SetCommTimeouts failed" );
			result = FALSE;
		}
	}

	return result;
}


//
// restore the previous comm_timeouts struct
//
BOOL win_restore_comm_timeouts( void )
{
	BOOL result = ( hComm != INVALID_HANDLE_VALUE && timeouts_are_initialized );

	if ( result )
	{
		if ( SetCommTimeouts( hComm, &comm_timeouts[OLD] ) == 0 )
		{
			report_windows_error( __FILE__, __LINE__, "SetCommTimeouts failed" );
			result = FALSE;
		}
	}

	return result;
}


//
// get the current DCB, modify it, and write it back
//

#if 0 // from MS documentation
typedef struct _DCB {
	DWORD DCBlength;
	DWORD BaudRate; 
	DWORD fBinary:1;
	DWORD fParity:1;
	DWORD fOutxCtsFlow:1;
	DWORD fOutxDsrFlow:1;
	DWORD fDtrControl:2;
	DWORD fDsrSensitivity:1;
	DWORD fTXContinueOnXoff:1;
	DWORD fOutX: 1;
	DWORD fInX: 1;
	DWORD fErrorChar:1;
	DWORD fNull:1; 
	DWORD fRtsControl:2; 
	DWORD fAbortOnError:1;
	DWORD fDummy2:17;
	WORD wReserved;
	WORD XonLim; 
	WORD XoffLim; 
	BYTE ByteSize; 
	BYTE Parity; 
	BYTE StopBits;
	char XonChar; 
	char XoffChar; 
	char ErrorChar;
	char EofChar; 
	char EvtChar; 
	WORD wReserved1; 
} DCB; 
#endif

BOOL win_modify_device_ctrl_block( DWORD baudrate, BYTE parity, BYTE stopbits )
{
	BOOL result = ( hComm != INVALID_HANDLE_VALUE ) ;

	// if this is the first call, get the existing DCB
	if ( result && !dcb_is_initialized )
	{
		dcb[OLD].DCBlength = sizeof( DCB );

		if ( GetCommState( hComm, &dcb[OLD] ) != 0 )
		{
			// copy the old to the new so we don't need to override every element
			memcpy( &dcb[NEW], &dcb[OLD], sizeof( DCB ) );
			dcb_is_initialized = TRUE;
		}
		else
		{
			report_windows_error( __FILE__, __LINE__, "GetCommTimeouts failed" );
			result = FALSE;
		}
	}

	if ( result )

	{

		dcb[NEW].BaudRate = baudrate;          // *** INPUT ARGUMENT ***
		dcb[NEW].fBinary = TRUE;               // Binary mode; no EOF check 
		dcb[NEW].fParity = FALSE;              // Enable parity checking. 
		dcb[NEW].fOutxCtsFlow = FALSE;         // No CTS output flow control 
		dcb[NEW].fOutxDsrFlow = FALSE;         // No DSR output flow control 
		dcb[NEW].fDtrControl = DTR_CONTROL_DISABLE;  // DTS flow control
		dcb[NEW].fDsrSensitivity = FALSE;      // DSR sensitivity 
		dcb[NEW].fTXContinueOnXoff = TRUE;     // XOFF continues Tx 
		dcb[NEW].fOutX = FALSE;                // No XON/XOFF out flow control 
		dcb[NEW].fInX = FALSE;                 // No XON/XOFF in flow control 
		dcb[NEW].fErrorChar = FALSE;           // Disable error replacement 
		dcb[NEW].fNull = FALSE;                // Disable null stripping
		dcb[NEW].fRtsControl = RTS_CONTROL_DISABLE; // RTS flow control 
		dcb[NEW].fAbortOnError = TRUE;         // Force a ReadFile/WriteFile error on overrun
		dcb[NEW].ByteSize = 8;                 // Number of bits/bytes, 4-8 
		dcb[NEW].XonLim = 0;
		dcb[NEW].XoffLim = 0;
		dcb[NEW].Parity = parity;              // *** INPUT ARGUMENT ***
		dcb[NEW].StopBits = stopbits;          // *** INPUT ARGUMENT ***

#if ( 0 )   ////////////////////////

		if ( SetCommState ( hComm, &dcb[NEW] ) == 0 )
		{
			report_windows_error( __FILE__, __LINE__, "SetCommState failed" );
			result = FALSE;
		}

#endif

	}

	return result;
}


BOOL win_restore_device_ctrl_block( void )
{
	BOOL result = ( hComm != INVALID_HANDLE_VALUE && dcb_is_initialized );

	if ( result )
	{
		if ( SetCommTimeouts( hComm, &comm_timeouts[OLD] ) == 0 )
		{
			report_windows_error( __FILE__, __LINE__, "SetCommTimeouts failed" );
			result = FALSE;
		}
	}

	// mark uninitialized regardless of SetCommTimeouts() result
	dcb_is_initialized = FALSE;

	return result;
}

BOOL win_create_event( HANDLE *p_hEvent )
{
   BOOL result = TRUE; // assume success

   *p_hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
   
   if ( *p_hEvent == INVALID_HANDLE_VALUE )
   {
      report_windows_error( __FILE__, __LINE__, "CreateEvent failed" );
      result = FALSE;
   }

   return result;
}

BOOL win_close_handle( HANDLE *p_handle )
{
   BOOL result = TRUE; // assume success, even if passed an invalid handle
   
   if ( *p_handle != INVALID_HANDLE_VALUE )
   {
      result = CloseHandle( *p_handle );
      *p_handle = INVALID_HANDLE_VALUE;
   }
   
   if ( !result )
      report_windows_error( __FILE__, __LINE__, "CloseHandle failed" );

   return result;
}

BOOL win_set_event( HANDLE hEvent )
{
   BOOL result = ( hEvent != INVALID_HANDLE_VALUE );
   
   if ( result )
   {
      result = SetEvent( hEvent );
      
      if ( !result )
         report_windows_error( __FILE__, __LINE__, "SetEvent failed" );
   }
   
   return result;
}

BOOL win_clear_event( HANDLE hEvent )
{
   BOOL result = ( hEvent != INVALID_HANDLE_VALUE );

   if ( result )
   {
      result = ResetEvent( hEvent );
      
      if ( !result )
         report_windows_error( __FILE__, __LINE__, "ResetEvent failed" );
   }

   return result;
}

BOOL win_set_bufsizes( DWORD receive_bufsize, DWORD transmit_bufsize )
{
	BOOL result = ( hComm != INVALID_HANDLE_VALUE );
	
	if ( result )
	{
		if ( SetupComm( hComm, receive_bufsize, transmit_bufsize ) == 0 )
		{
			report_windows_error( __FILE__, __LINE__, "SetupComm failed" );
			result = FALSE;
		}
      else
      {
         COMMPROP cp;
         char errmsg[80];

         //
         // if possible, test that buffers are at least as large as requested
         // buffer size of zero is okay as it means only that
         //   info was not available
         //

         if ( GetCommProperties( hComm, &cp ) == 0 )
         {
            report_windows_error( __FILE__, __LINE__,
                                  "GetCommProperties failed" );
         }
         else
         {
            if ( 0L < cp.dwCurrentRxQueue
                 && cp.dwCurrentRxQueue < receive_bufsize )
            {
               sprintf( errmsg,
                        "receive queue size (%ld) less than requested (%ld)",
                        cp.dwCurrentRxQueue, receive_bufsize );
               report_error( errmsg );
            }
           
            if ( 0L < cp.dwCurrentTxQueue
                 && cp.dwCurrentTxQueue < transmit_bufsize )
            {
               sprintf( errmsg,
                        "transmit queue size (%ld) less than requested (%ld)",
                        cp.dwCurrentTxQueue, transmit_bufsize );
               report_error( errmsg );
            }
         }
      }
	}

	return result;
}


//
// set the comm event mask
// 
BOOL win_modify_event_mask( DWORD new_mask )
{
	BOOL result = ( hComm != INVALID_HANDLE_VALUE );
	
	// if this is the first call, get and store the existing mask
	if ( result && !event_mask_is_initialized )
	{
		if ( GetCommMask( hComm, &event_mask[OLD] ) == FALSE )
		{
			report_windows_error( __FILE__, __LINE__, "GetCommMask failed" );
			result = FALSE;
		}
	}
	
	// set the new mask
	event_mask[NEW] = new_mask;
	
	if ( result )
	{
		if ( SetCommMask( hComm, event_mask[NEW] ) == FALSE )
		{
			report_windows_error( __FILE__, __LINE__, "SetCommMask failed" );
			result = FALSE;
		}
	}
	
	return result;
}

//
// restore the comm event mask to its original value
// 

BOOL win_restore_event_mask( void )
{
	BOOL result = ( hComm != INVALID_HANDLE_VALUE && event_mask_is_initialized );
	
	if ( result )
	{
		if ( SetCommMask( hComm, event_mask[OLD] ) == FALSE )
		{
			report_windows_error( __FILE__, __LINE__, "SetCommMask failed" );
			result = FALSE;
		}
	}
	
	return result;
}

//
// if ReadFile succeeds immediately
//   return RWFILE_SUCCESS and *bytes_read is valid
//
// if ReadFile starts but does not complete a read operation (ERROR_IO_PENDING)
//   return RWFILE_PENDING and *bytes_read is invalid
//
// if ReadFile encounters any error except ERROR_IO_PENDING
//   report the error and
//   return RWFILE_FAILURE and *bytes_read is invalid
//
enum WIN_RW_FILE_STATUS win_read_file( unsigned char *read_buf, DWORD read_buf_size, LPDWORD bytes_read, LPOVERLAPPED os_read )
{
	enum WIN_RW_FILE_STATUS func_result;  // the return value for this function

	if ( hComm == INVALID_HANDLE_VALUE )
	{
		 func_result = RWFILE_FAILURE;
	}
	else
	{
		if ( ReadFile( hComm, read_buf, read_buf_size, bytes_read, os_read ) != 0 )

		{
			func_result = RWFILE_SUCCESS;
		}
		else
		{
			if ( GetLastError() == ERROR_IO_PENDING )
			{
				func_result = RWFILE_PENDING;
			}
			else
			{
				report_windows_error( __FILE__, __LINE__, "ReadFile failed" );
				func_result = RWFILE_FAILURE;
			}
		}
	}

	return func_result;
}


//
// if WriteFile succeeds immediately
//   return RWFILE_SUCCESS and *bytes_written is valid
//
// if WriteFile starts but does not complete a write operation (ERROR_IO_PENDING)
//   return RWFILE_PENDING and *bytes_written is invalid
//
// if WriteFile encounters any error except ERROR_IO_PENDING
//   report the error and
//   return RWFILE_FAILURE and *bytes_written is invalid
//
enum WIN_RW_FILE_STATUS win_write_file( unsigned char *write_buf, DWORD bytes_to_write, DWORD *bytes_written, LPOVERLAPPED os_write )
{
	enum WIN_RW_FILE_STATUS func_result;  // the return value for this function

	BOOL WriteFile_result; // the value returned by WriteFile()
	
	if ( hComm == INVALID_HANDLE_VALUE )
	{
		 func_result = RWFILE_FAILURE;
	}
	else
	{
		WriteFile_result = WriteFile( hComm, write_buf, bytes_to_write, bytes_written, os_write );

		if ( WriteFile_result != 0 )
		{
			func_result = RWFILE_SUCCESS;
		}
		else
		{
			if ( GetLastError() == ERROR_IO_PENDING )
			{
				func_result = RWFILE_PENDING;
			}
			else
			{
				report_windows_error( __FILE__, __LINE__, "WriteFile failed" );
				func_result = RWFILE_FAILURE;
			}
		}
	}

	return func_result;
}


#if 0
BOOL GetOverlappedResult
(
	HANDLE hFile,                       // handle to file, pipe, or device
	LPOVERLAPPED lpOverlapped,          // overlapped structure
	LPDWORD lpNumberOfBytesTransferred, // bytes transferred
	BOOL bWait                          // wait option
);
#endif

BOOL win_get_overlapped_result( LPOVERLAPPED overlapped_struct, LPDWORD bytes_transferred )
{
	BOOL result = ( hComm != INVALID_HANDLE_VALUE );
	
	if ( result )
	{
		result = GetOverlappedResult( hComm, overlapped_struct, bytes_transferred, FALSE );
		
		if ( !result )
		{
			report_windows_error( __FILE__, __LINE__, "GetOverlappedResult failed" );
		}
	}
	
	return result;
}

//
// return the index of the event that signalled
// or -1 for other conditions
//
#if 0
	DWORD WaitForMultipleObjects
	(
		  DWORD nCount,             // number of handles in array
		  CONST HANDLE *lpHandles,  // object-handle array
		  BOOL fWaitAll,            // wait option
		  DWORD dwMilliseconds      // time-out interval
	);
#endif
	

int win_wait( HANDLE *events, int num_events )
{
	int event_index = -1;

	DWORD get_overlapped_result;

	get_overlapped_result = WaitForMultipleObjects( num_events, events, FALSE, INFINITE );

	if ( WAIT_OBJECT_0 <= get_overlapped_result && get_overlapped_result < WAIT_OBJECT_0 + num_events )
	{
		event_index = get_overlapped_result - WAIT_OBJECT_0;
	}
	else if ( get_overlapped_result == WAIT_FAILED )
	{
		report_windows_error( __FILE__, __LINE__, "WaitForMultipleObjects failed" );
	}
	
	return event_index;
}

//
// start a WaitCommEvent, which succeeds or is left pending
// if operation completes, OR the result into the CommStatus member 'event'
// DWORD pointed to by p_event must live until following Wait... completes
// 
enum WIN_RW_FILE_STATUS win_wait_comm_event( DWORD *p_event, LPOVERLAPPED p_os )
{
	enum WIN_RW_FILE_STATUS func_result;  // the return value for this function

	if ( hComm == INVALID_HANDLE_VALUE )
	{
		 func_result = RWFILE_FAILURE;
       *p_event = 0L;
	}
	else
	{
      if ( WaitCommEvent( hComm, p_event, p_os ) != 0 )
      {
         func_result = RWFILE_SUCCESS;
      }
      else
      {
         if ( GetLastError() == ERROR_IO_PENDING )
         {
            func_result = RWFILE_PENDING;
         }
         else
         {
            report_windows_error( __FILE__, __LINE__, "WaitCommEvent failed" );
            func_result = RWFILE_FAILURE;
         }
      }
   }
   
   return func_result;
}

//
//  call Win32 API ClearCommError() to get the error
//  
BOOL win_clear_comm_error( LPDWORD p_error )
{
   BOOL result = ( hComm != INVALID_HANDLE_VALUE );
   
   if ( result )
   {
      result = ClearCommError( hComm, p_error, 0 );
      
      if ( !result )
      {
         report_windows_error( __FILE__, __LINE__, "ClearCommError failed" );
      }
   }
   
   return result;
}



// Following added 6/20/03
//

BOOL win_escape_comm_function( DWORD function )
{
   BOOL result = ( hComm != INVALID_HANDLE_VALUE );
   
   if ( result )
   {
      result = EscapeCommFunction( hComm, function );
      
      if ( !result )
      {
         report_windows_error( __FILE__, __LINE__, "EscapeCommFunction failed");
      }
   }

   return result;
}

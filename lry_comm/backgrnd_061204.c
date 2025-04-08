/*
** backgrnd.c
**
** read and write procedures will run in separate background threads
*/
  
/*
 * lry 18sep00
 *
 * lry 11jul01
 *  - recover from ERROR_OPERATION_ABORTED
 *  - do not terminate read and write threads on error except overflow
 */

#include <windows.h>

#include "commbufs.h"
#include "winfile.h"
#include "errors.h"

//
// PROCESS PRIORITIES
// 
// priority classes:
//
//   IDLE_PRIORITY_CLASS
//   BELOW_NORMAL_PRIORITY_CLASS  (Win2K only)
//   NORMAL_PRIORITY_CLASS
//   ABOVE_NORMAL_PRIORITY_CLASS  (Win2K only)
//   HIGH_PRIORITY_CLASS
//   REALTIME_PRIORITY_CLASS
//
//    
// THREAD PRIORITIES
//
// priority levels:
// 
//   THREAD_PRIORITY_IDLE
//   THREAD_PRIORITY_LOWEST
//   THREAD_PRIORITY_BELOW_NORMAL
//   THREAD_PRIORITY_NORMAL
//   THREAD_PRIORITY_ABOVE_NORMAL
//   THREAD_PRIORITY_HIGHEST
//   THREAD_PRIORITY_TIME_CRITICAL
//     

#define READ_PRIORITY_LEVEL    THREAD_PRIORITY_NORMAL
#define WRITE_PRIORITY_LEVEL   THREAD_PRIORITY_NORMAL

//
// thread handles
// 
static HANDLE hRead = INVALID_HANDLE_VALUE;
static HANDLE hWrite = INVALID_HANDLE_VALUE;

//
// events to signal procedures to stop
//

static HANDLE hEvent_stop_read_proc = INVALID_HANDLE_VALUE;
static HANDLE hEvent_stop_write_proc = INVALID_HANDLE_VALUE;

//
// functions to communicate with the user's read buffer
// this allows Ernie to use the comm_read_byte function
// or to register a process_rx_byte function to be called by the read thread
//
// use two function pointers so background thread can decide when to switch
// to the new function
//
rx_callback_func process_rx_info;
rx_callback_func new_process_rx_info;

void set_read_processor( rx_callback_func new_read_processor )
{
   new_process_rx_info = new_read_processor;
}

// process the ReadInfo structure addressed by the input pointer
// by invoking a user-supplied processing function or pushing it to a queue
static BOOL process_read_item( struct ReadInfo *read_info_ptr )
{
   BOOL result = TRUE;
   
   if ( process_rx_info )      // if caller has provided a handler
      (*process_rx_info)( read_info_ptr );
   else                        // default is to use comm buffer
      result = push_item_receive( read_info_ptr );
   
   return result;
}

// use this function to process a string of received bytes
// each character must be loaded into an appropriate ReadInfo struct for processing
// return TRUE if all bytes are processed, or FALSE if not all bytes can be processed
static BOOL process_received_string( unsigned char *string_from_readport, long num_bytes )
{
   BOOL result = TRUE;

   static struct ReadInfo read_info = { 0, 0L, 0L };

   while ( result && num_bytes-- )
   {
      read_info.character = *string_from_readport++;

      result = process_read_item( &read_info );
   }
   
   return result;
}

// use this function to process an i/o event detected by port_read_proc()
static BOOL process_event( DWORD io_event )
{
   static struct ReadInfo read_info = { -1, 0L, 0L };
   
   BOOL result = TRUE;
   
   read_info.event = io_event;

   if ( ( io_event & EV_ERR ) != 0 )
   {
      result = win_clear_comm_error( &(read_info.error) );
   }
   else                               
   {                               
      read_info.error = 0L;
   }

   if ( result )
      result = process_read_item( &read_info ) ;
   
   return result;
}

// this function clears the error condition
// and places an error code in the stream
static BOOL process_error( void )
{
   static struct ReadInfo read_info = { -1, 0L, 0L };

   BOOL result = win_clear_comm_error( &(read_info.error) );
   
   if ( result )
      result = process_read_item( &read_info ) ;

	return result;
}

//
// buffer sizes
//

#define READ_BUF_SIZE      1      // Changed from 512 on 8/29/01
#define WRITE_BUF_SIZE  2048

//
// allow on-the-fly switching of the event mask
// 
static DWORD event_mask = EV_BREAK | EV_ERR;
static DWORD next_event_mask = EV_BREAK | EV_ERR;

void set_new_event_mask( DWORD new_event_mask )
{
   next_event_mask = new_event_mask;
}

//
// background procedure to read from the port
//
// sit in a loop:
//   wait for a status event, a read-character event, or external end-of-thread
//   process event, and possibly read from port into a local buffer
//   process the local buffer with process_received_string()
//
// VVV could eliminate the local buffer and go direct from port to fifo?
//

DWORD WINAPI port_read_proc( LPVOID lpv )
{
   /* ************* */
   /* *** setup *** */
   /* ************* */

   OVERLAPPED os_status = {0}; // overlapped structure for status operation
   OVERLAPPED os_read = {0};   // overlapped structure for read operation
   HANDLE     events_list[3];  // list of events to wait for
   DWORD      bytes_read;
	DWORD      io_event;
   char       read_buf[ READ_BUF_SIZE ];

   //
   // port_read_proc control flags
   //
   BOOL terminate_thread = FALSE;
   BOOL waiting_for_bytes = FALSE;
   BOOL waiting_for_status = FALSE;

   // create events for the overlapped structures
   os_read.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
   if ( os_read.hEvent == INVALID_HANDLE_VALUE )
       report_windows_error( __FILE__, __LINE__, "CreateEvent failed" );

   os_status.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL );
   if ( os_status.hEvent == INVALID_HANDLE_VALUE )
       report_windows_error( __FILE__, __LINE__, "CreateEvent failed" );

   // detect these events:
   //   - read event completion, after ReadFile returns ERROR_IO_PENDING
   //   - status event completion, after WaitCommEvent returns ERROR_IO_PENDING
   //   - thread exit event from the stop_thread function
   events_list[0] = os_read.hEvent;
   events_list[1] = os_status.hEvent;
   events_list[2] = hEvent_stop_read_proc;

   // now we're ready to receive, so set "Data Terminal Ready"
   // ignore return value for now VVV

///////////   win_escape_comm_function( SETDTR );              // Added 6/20/03

   /* ************************************ */
   /* *** loop until termination event *** */
   /* ************************************ */

   while ( !terminate_thread  )
   {
      //
      // no read pending so issue new read request
      //
      if ( !waiting_for_bytes )
      {
         // update handler
         process_rx_info = new_process_rx_info;
            
         switch ( win_read_file( read_buf, READ_BUF_SIZE, &bytes_read, &os_read ) )
         {
            case RWFILE_FAILURE:

                // process_error() will identify an overflow
                if ( !process_error() )
                {
                   report_function_error( "port_read_proc",
                                          "read_file error" );
                   terminate_thread = TRUE; // can't recover from this failure
                }

                break;

            case RWFILE_SUCCESS:

                if ( bytes_read != 0L )
                   process_received_string( read_buf, bytes_read );

                break;

            case RWFILE_PENDING:

                waiting_for_bytes = TRUE;

                break;
               
            default:
               
                report_function_error( "port_read_proc", "illegal case" );
                terminate_thread = TRUE;

                break;
         }
      }
   
      //
      // if no status check is pending, start a new status check
      //
      if ( !waiting_for_status )
      {
			//
         // this is the time to change the mask if necessary
         // 
         if ( next_event_mask != event_mask )
         {
            if ( win_modify_event_mask( next_event_mask ) )
            {
               event_mask = next_event_mask;
            }
         }
         
         // this seems necessary - VVV lry - to be studied more later
         io_event = event_mask;

         switch ( win_wait_comm_event( &io_event, &os_status ) )
         {
            case RWFILE_FAILURE:

                // process_error() will identify an overflow
                if ( !process_error() )
                {
                   report_function_error( "port_read_proc",
                                          "wait_comm_event error" );
                   terminate_thread = TRUE; // can't recover from this failure
                }

                break;

            case RWFILE_SUCCESS:

                process_event( io_event );

                break;

            case RWFILE_PENDING:

                waiting_for_status = TRUE;

                break;
               
            default:
               
                report_function_error( "port_read_proc", "illegal case" );
                terminate_thread = TRUE;

                break;
         }
      }

      //
      // wait for pending operations to complete
      // 
      if ( waiting_for_bytes && waiting_for_status )
      {
         switch ( win_wait( events_list, sizeof(events_list)/sizeof(events_list[0]) ) )
         {
            //
            // these cases must match the events in events_list[]
            //
            case 0: // read is done

               if ( win_get_overlapped_result( &os_read, &bytes_read ) )
               {
                  if ( bytes_read > 0L )
                     process_received_string( read_buf, bytes_read );
               }
               else
               {
                  report_function_error( "port_read_proc", "overlapped error" );
                  // 11jul01 - remove this termination
                  // VVV could remove error reporting, too
                  // terminate_thread = TRUE; // unrecoverable error
               }

               waiting_for_bytes = FALSE;

               break;

            case 1: // status

               if ( win_get_overlapped_result( &os_status, &io_event ) )
               {
                  process_event( io_event );
               }
               else
               {
                  report_function_error( "port_read_proc", "overlapped error" );
                  // 11jul01 - remove this termination
                  // VVV could remove error reporting, too
                  // terminate_thread = TRUE; // unrecoverable error
               }

					waiting_for_status = FALSE;
               
               break;

            case 2: // stop

               terminate_thread = TRUE;

               break;
               
            case -1:
               
               // 11jul01 - WaitForMultipleObjects returned WAIT_FAILED
               waiting_for_status = FALSE;
               break;

            default:
               report_function_error( "port_read_proc", "win_wait error" );
               terminate_thread = TRUE; // something bad happened
               break; 
         }

      }

   } // end while not done

   /* ***************** */
   /* *** shut down *** */
   /* ***************** */

   // close event handles
   CloseHandle( os_read.hEvent );
   CloseHandle( os_status.hEvent );

   return 1L;
}


//
// background procedure to write to the port
//
// 'lpv' is a handle to an event that signals there is something in the queue
//

DWORD WINAPI port_write_proc( LPVOID lpv )
{
   /* ************* */
   /* *** setup *** */
   /* ************* */

   OVERLAPPED os_write = {0};  // overlapped structure for write operations
   HANDLE     input_events_list[2];
   HANDLE     output_events_list[2];
   DWORD      bytes_to_write;  // number of bytes collected from the queue
   DWORD      bytes_written;   // number of bytes written to the port
   char   	  write_buf[ WRITE_BUF_SIZE ];

   //
   // possible states of the write proc
   //
   enum PROC_STATE { PROC_DONE, PROC_IDLE, PROC_IO_PENDING, PROC_BUF_READY };
   enum PROC_STATE state = PROC_IDLE;

   // create a write event for the overlapped structure
   os_write.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
   if ( os_write.hEvent == INVALID_HANDLE_VALUE )
       report_windows_error( __FILE__, __LINE__, "CreateEvent failed" );

   //
   // detect these events in the input:
   //   thread exit events from the stop_thread function
   //   new entries in the application's transmit queue
   //
   input_events_list[0] = commbuf_hEvent_transmit;
   input_events_list[1] = hEvent_stop_write_proc;

   //
   // detect these events in the output:
   //   thread exit events from the stop_thread function
   //   overlapped write events from WriteFile
   //
   output_events_list[0] = os_write.hEvent;
   output_events_list[1] = hEvent_stop_write_proc;

   /* ************************************ */
   /* *** loop until termination event *** */
   /* ************************************ */

   while ( state != PROC_DONE )
   {
      switch ( state )
      {
     		/* ************************************* */
     		/* *** write_buf is empty so fill it *** */
     		/* ************************************* */
         case PROC_IDLE:

            switch ( win_wait( input_events_list, sizeof(input_events_list)/sizeof(input_events_list[0]) ) )
            {
               // these cases must match the events in input_events_list

               case 0: // something in queue

               	// VVV pop_string doesn't return until the transfer is complete
               	bytes_to_write = pop_array_transmit( write_buf, WRITE_BUF_SIZE );

               	if ( bytes_to_write != 0L )
               		state = PROC_BUF_READY;

               	break;

               case 1: // stop

               	state = PROC_DONE;

               	break;
               	
               case -1:

                  // 11jul01 - WaitForMultipleObjects returned WAIT_FAIL
                  state = PROC_IDLE;

                  break;

               default:

               	report_function_error( "port_write_proc", "win_wait error" );
               	state = PROC_DONE; // something bad happened

               	break; 
            }

            break;
   
         /* ************************************ */
         /* *** pending, wait for completion *** */
         /* ************************************ */
         case PROC_IO_PENDING:     

            switch ( win_wait( output_events_list, sizeof(output_events_list)/sizeof(output_events_list[0]) ) )
            {
               // these cases must match the events in output_events_list[]

               case 0: // write is done

               	if ( win_get_overlapped_result( &os_write, &bytes_written ) )
               		state = ( bytes_written == bytes_to_write ) ? PROC_IDLE : PROC_DONE;
               	else
               	{
               		report_function_error( "port_write_proc", "overlapped error" );
               		state = PROC_DONE; // unrecoverable error
               	}

               	break;
               	
               case 1: // stop

               	state = PROC_DONE;

               	break;
               	
               case -1:
               default:

               	report_function_error( "port_write_proc", "win_wait error" );

               	state = PROC_DONE; // something bad happened

               	break; 
            }
            break;

         /* ************************************ */
         /* *** ready to transfer the buffer *** */
         /* ************************************ */
         case PROC_BUF_READY:      

            switch ( win_write_file( write_buf, bytes_to_write, &bytes_written, &os_write) )
            
            {
               case RWFILE_FAILURE:
               	state = PROC_DONE; // might as well give up
               	break;

               case RWFILE_PENDING:
               	state = PROC_IO_PENDING;
               	break;
               	
               case RWFILE_SUCCESS:
               	state = PROC_IDLE;
               	break;
               	
               default:
               	report_function_error( "win_write_file", "illegal return value" );
               	state = PROC_DONE; // might as well give up
               	break;
            }
            break;

         /* ************************************ */
         /* *** should never get to default  *** */
         /* ************************************ */
         default:
            report_function_error( "port_write_proc", "illegal state" );
            state = PROC_DONE;
            break;
      }

   }

   /* ***************** */
   /* *** shut down *** */
   /* ***************** */

   // close event handle
   CloseHandle(os_write.hEvent);

   return 2L;
}


static HANDLE win_start_thread
   (
      LPTHREAD_START_ROUTINE thread_proc,
     	LPVOID thread_arg,
      int priority_level
  	)
{
   HANDLE hThread = INVALID_HANDLE_VALUE;
   
   DWORD thread_id; // ignored but required for Win95/Win98

   hThread = CreateThread( 0, 0L, thread_proc, thread_arg, 0L, &thread_id );

   if ( hThread == INVALID_HANDLE_VALUE )
      report_windows_error( __FILE__, __LINE__, "CreateThread failed" );
   else if ( SetThreadPriority( hThread, priority_level ) == FALSE )
      report_windows_error( __FILE__, __LINE__, "SetThreadPriority failed" );
   
   return hThread;
}


static BOOL win_stop_thread
   (
      HANDLE thread_handle,
     	HANDLE termination_event,
     	DWORD timeout_ms,
     	BOOL force_termination
  	)
{
   BOOL thread_is_stopped = FALSE;


#if ( RELEASE_DATE >= 0x061203 )    // Change 11/22/2004

   // if there is a termination event available, signal the thread to stop
   if ( termination_event != INVALID_HANDLE_VALUE )
      SetEvent( termination_event );
   
   // wait for timeout (millisecs, could also be 0 or INFINITE)
   switch ( WaitForSingleObject( thread_handle, timeout_ms ) )
   {
      case WAIT_OBJECT_0: // the thread has exited normally
         thread_is_stopped = TRUE;
         break;
         
      case WAIT_TIMEOUT: // timeout period has expired; thread didn't signal
         break;
         
      default: // some system disaster; should never get here
         report_windows_error( __FILE__, __LINE__, "WaitForSingleObject disaster" );
         break;
   }
   
#endif

   // use the potentially disastrous TerminateThread if necessary
   if ( !thread_is_stopped && force_termination )
   {
      report_error( "win_stop_thread:  force termination" );

      if ( TerminateThread( thread_handle, 0 ) == 0 )
         report_windows_error( __FILE__, __LINE__, "TerminateThread failed" );
   }
   
   return thread_is_stopped;
}


BOOL start_read_thread( void )
{
   // set up a termination event with which to kill the thread eventually
   // this is a manual-reset event (arg2==TRUE), initially cleared (arg3==FALSE)

   hEvent_stop_read_proc = CreateEvent( 0, TRUE, FALSE, 0 );

   if ( hEvent_stop_read_proc == INVALID_HANDLE_VALUE )
      report_windows_error( __FILE__, __LINE__, "CreateEvent failed" );

   // cannot start the thread if its handle is already in use
   // only one read thread allowed
   if ( hRead != INVALID_HANDLE_VALUE )
   {
      report_error( "start_read_thread(): hRead in use" );
   }
   else
   {
      hRead = win_start_thread( (LPTHREAD_START_ROUTINE) port_read_proc,
                                0, 
                                READ_PRIORITY_LEVEL );
   }
   
   return ( hRead != INVALID_HANDLE_VALUE );
}


BOOL stop_read_thread( void )
{

   if ( hRead != INVALID_HANDLE_VALUE )

   {


     #if ( RELEASE_DATE >= 0x061203 )     
                                                                           // TARGUS 04 nov 2004
       win_modify_event_mask ( 0 ) ;
       Sleep ( 200 ) ;
       CloseHandle ( hRead ) ;
       hRead = INVALID_HANDLE_VALUE ;

     #endif

     #if ( RELEASE_DATE <  0x061203 )     // old method that hangs the Targus adapter

       if ( win_stop_thread( hRead, hEvent_stop_read_proc, 200L, TRUE ) )

        {
           CloseHandle( hRead );
           hRead = INVALID_HANDLE_VALUE;
        }

     #endif

   }

   // done with the termination event

   if ( hEvent_stop_read_proc != INVALID_HANDLE_VALUE
         && CloseHandle( hEvent_stop_read_proc ) == FALSE )
   {
      report_windows_error( __FILE__, __LINE__, "CloseHandle failed" );
   }

   hEvent_stop_read_proc = INVALID_HANDLE_VALUE;

   // return TRUE if read proc handle is now invalid

   return ( hRead == INVALID_HANDLE_VALUE );
}

BOOL start_write_thread( void )
{
   // set up a termination event with which to kill the thread eventually
   // this is a manual-reset event (arg2==TRUE), initially cleared (arg3==FALSE)
   hEvent_stop_write_proc = CreateEvent( 0, TRUE, FALSE, 0 );

   if ( hEvent_stop_write_proc == INVALID_HANDLE_VALUE )
      report_windows_error( __FILE__, __LINE__, "CreateEvent failed" );

   // cannot start the thread if its handle is already in use
   // only one read thread allowed
   if ( hWrite != INVALID_HANDLE_VALUE )
   {
      report_error( "start_write_thread(): hWrite in use" );
   }
   else
   {
      hWrite = win_start_thread( (LPTHREAD_START_ROUTINE) port_write_proc,
                                 0, 
                                 WRITE_PRIORITY_LEVEL );
   }
   
   return ( hWrite != INVALID_HANDLE_VALUE );
}

BOOL stop_write_thread( void )
{
   if ( hWrite != INVALID_HANDLE_VALUE )
   {
      if ( win_stop_thread( hWrite, hEvent_stop_write_proc, 200L, TRUE ) )
      {
         CloseHandle( hWrite );
         hWrite = INVALID_HANDLE_VALUE;
      }
   }

   // done with the termination event

   if ( hEvent_stop_write_proc != INVALID_HANDLE_VALUE
         && CloseHandle( hEvent_stop_write_proc ) == FALSE )
   {
      report_windows_error( __FILE__, __LINE__, "CloseHandle failed" );
   }

   hEvent_stop_write_proc = INVALID_HANDLE_VALUE;

   // return TRUE if write proc handle is now invalid

   return ( hWrite == INVALID_HANDLE_VALUE );
}

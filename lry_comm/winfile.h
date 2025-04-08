/*
** winfile.h
**
** wrappers for Windows file-handling funcs used in serial-port communications
*/

/*
 * lry 20sep00
 */

#define RELEASE_DATE 0x090604


#ifndef WINFILE_H
#define WINFILE_H

#include <windows.h>

/* ************************************** */

// BOOL functions return TRUE if successful
// otherwise, issue an error message via report_error and return FALSE
//
// these functions are called from comm_open() and comm_close()

// open the port with CreateFile
// ports are numbered 0 through 3 for COMM1 to COMM4
BOOL win_open_file( unsigned port );

// close the port with CloseHandle
BOOL win_close_file( void );

// modify the comm_timeouts struct and make it the active one
BOOL win_modify_comm_timeouts( void );

// restore the previous comm_timeouts struct
BOOL win_restore_comm_timeouts( void );

//
BOOL win_modify_device_ctrl_block( DWORD baudrate, BYTE parity, BYTE stopbits );

//
BOOL win_restore_device_ctrl_block( void );

//
BOOL win_set_bufsizes( DWORD receive_bufsize, DWORD transmit_bufsize );

//
BOOL win_modify_event_mask( DWORD new_mask );

//
BOOL win_restore_event_mask( void );

//
BOOL win_create_event( HANDLE *p_handle );

//
BOOL win_close_handle( HANDLE *p_handle );

//
BOOL win_set_event( HANDLE hEvent );

//
BOOL win_clear_event( HANDLE hEvent );

//
BOOL win_clear_comm_error( LPDWORD p_error );

// return values of win_read_file() and win_write_file()
enum WIN_RW_FILE_STATUS { RWFILE_FAILURE, RWFILE_SUCCESS, RWFILE_PENDING };

//
enum WIN_RW_FILE_STATUS win_read_file( unsigned char *read_buf, DWORD read_buf_size, LPDWORD bytes_read, LPOVERLAPPED os_read );

//
enum WIN_RW_FILE_STATUS win_write_file( unsigned char *write_buf, DWORD bytes_to_write, DWORD *bytes_written, LPOVERLAPPED os_write );

//
enum WIN_RW_FILE_STATUS win_wait_comm_event( LPDWORD p_event, LPOVERLAPPED os_write );

// 
BOOL win_get_overlapped_result( LPOVERLAPPED overlapped_struct, LPDWORD bytes_transferred );

// 
int win_wait( HANDLE *events, int num_events );
int win_wait_xmit( HANDLE *events, int num_events ); // LRY extra

//
enum WIN_RW_FILE_STATUS win_wait_comm_event( DWORD *p_event, LPOVERLAPPED p_os );


//
//
BOOL win_escape_comm_function( DWORD function );   // Added 6/20/03


#endif//WINFILE_H

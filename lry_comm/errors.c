#include <windows.h>
#include <conio.h> // for cprintf()
#include <stdio.h> // for sprintf()
#include "errors.h"


#define max_output_strlen 120

static void (*report_to_caller)( char *message ) = NULL ;  // Ernie Modified 7/6/01


//
// utility to add strings safely
//
// add the 'append' string to the end of the 'output' string
//
// no more than 'max_length' characters (including null terminator)
// will be written to 'output'
// 

void safe_string_append( char *output, char *append, size_t max_length )
{
	size_t length_to_add = max_length - strlen( output );
	
	length_to_add -= 1; // make room for a terminating null char

	if ( length_to_add > 0 )
		strncat( output, append, length_to_add );
}


//
// call this function to register an error-reporting function
//

void xxDLL_EXPORT set_error_reporter( void (* new_reporter)( char *message ) )
{
	report_to_caller = new_reporter;
}


//
// wrapper to issue an error message when an error reporter is available
//
// put a breakpoint here if you want to trap error messages
//   before calling (*report_error)
//

void report_error( char *msg )

{
	if ( report_to_caller != NULL ) {
      (*report_to_caller)( msg );    // Ernie Modified 7/6/01 for his stupidity
    }
    else {
      MessageBox ( GetFocus(), msg, NULL, MB_OK);
    }
}


//
// construct an error string from a function name and an error message
//

void report_function_error( char *function, char *msg )
{
	char output[ max_output_strlen + 1 ];

	output[0] = 0;

	safe_string_append( output, "Error in ", max_output_strlen );
	safe_string_append( output, function, max_output_strlen );
	safe_string_append( output, ":  ", max_output_strlen );
	safe_string_append( output, msg, max_output_strlen );
	
	report_error( output );
}


//
// construct and report an error message from filename, line number
//   and the error number from Windows' GetLastError()
//   

void report_windows_error( char *file, int line, char *msg )
{
	char output[ max_output_strlen + 1 ];
	size_t max_output = sizeof( output );

	char errno_str[ 8 ];
	char line_str[ 8 ];

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


//
// normal console-based error-reporting function
//
//
void report_error_conio( char *message )
{
	cprintf( "\nERROR:  %s\n", message );
}

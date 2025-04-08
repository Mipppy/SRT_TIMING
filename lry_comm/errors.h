#ifndef ERRORS_H
#define ERRORS_H

#include <windows.h>

#if   defined ( _USRDLLx )
  #define xxDLL_EXPORT _declspec ( dllexport )
#endif
#if ! defined ( _USRDLLx )
  #define xxDLL_EXPORT
#endif

//
// call this function to register an error-reporting function
//
void xxDLL_EXPORT set_error_reporter( void (* new_reporter)( char *message ) );

//
// wrapper to issue an error message when an error reporter is available
//
void report_error( char *msg );

//
// construct an error string from a function name and an error message
//
void report_function_error( char *function, char *msg );

//
// construct and report an error message from filename, line number
//   and the error number from Windows' GetLastError()
//   
void report_windows_error( char *file, int line, char *msg );


//
// normal console-based error-reporting function
//

void report_error_conio( char *message );


//
// utility to append strings safely
//

void safe_string_append( char *output, char *append, size_t max_length );

#endif//ERRORS_H


/****************************************************************/

// This module contains utility functions that should not be platform dependent

#define MISC_MODULE  0

#if  defined ( __C51__ )
  #include <header.h>
#endif

#if ! defined ( __C51__ )
  #include <win_header.h>
#endif


/**************************************************************************/

void DLL_EXPORT output_formatted_file_string ( char * file_path_and_name, char * format_string,... ) /* function declaration */

{

  #if ( 0 ) 

    char debug_string [ 512 ] ;

    FILE * output_file ;

    va_list marker ;                   // Declaration of typedef??
      
    if ( format_string == NULL ) {

      if ( ( output_file = fopen ( file_path_and_name,  "w" ) ) == NULL ) {
        sprintf ( debug_string, "Can't Open file %s", file_path_and_name ) ;
        MessageBox ( GetFocus(), debug_string, "Timer Error Message", MB_OK);
      endif

      fclose  ( output_file ) ;

    endif

    else {

      va_start ( marker, format_string ) ;

      vsprintf ( debug_string, format_string, marker ) ;

      if ( ( output_file = fopen ( file_path_and_name,  "a" ) ) == NULL ) {
        sprintf ( debug_string, "Can't Open file %s", file_path_and_name ) ;
        MessageBox ( GetFocus(), debug_string, "Timer Error Message", MB_OK);
      endif

      fprintf ( output_file, "%s", debug_string ) ;

      fclose  ( output_file ) ;

    endif

  #endif

}

/*****************************************************************************************************************************/
/*****************************************************************************************************************************/

static void copy_generic_structure ( unsigned char * d, unsigned char * s, int number_of_bytes ) /* function declaration */

{

  int index ;
  unsigned char * destination_pointer ;
  unsigned char * source_pointer ;

  destination_pointer = (unsigned char *) d ;
  source_pointer      = (unsigned char *) s ;

  for ( index = 0 ; index < number_of_bytes ; index ++ ) {
    * destination_pointer ++ = * source_pointer ++ ;
  next

}

/*****************************************************************************************************************************/

int DLL_EXPORT dll_get_file_path_and_name_from_cwd ( char * file_path_and_name, char * file_name, char * file_extension  ) /* function declaration */

{

  char cwd [ 500 ] ;

  _getcwd ( file_path_and_name, 500 ) ;

  if ( cwd [ strlen ( file_path_and_name ) - 1 ] != '\\' ) {
    strcat ( file_path_and_name, "\\" ) ;
  endif

  strcat ( file_path_and_name, file_name ) ;

  if ( cwd [ strlen ( file_name ) - 1 ] != '.' ) {
    strcat ( file_path_and_name, "." ) ;
  endif

  strcat ( file_path_and_name, file_extension ) ;

  return 1 ;

}

/*********************************************************************/

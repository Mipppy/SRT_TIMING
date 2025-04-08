
/****************************************************************/

// This module contains utility functions that should not be platform dependent

#define MISC_MODULE  0

#include <x_header.h>

/********************************************************************/
/**********************************************************************/

/* This function removes leading white spaces   */
/*   Note: Involves an "in-place" move.         */

void remove_leading_white ( char * ptr_string )  /* function declaration    */

{
  strcpy ( ptr_string, get_ptr_to_first_non_white ( ptr_string ) ) ;
}

/**************************************/

/* This function removes leading and trailing white spaces */
/*   Note: Involves an "in-place" move.                    */

int remove_leading_and_trailing_white ( char * ptr_string )  /* function declaration    */

{
  remove_leading_white ( ptr_string ) ;
  return ( remove_trailing_white ( ptr_string ) ) ;
}

/***********************************************************/

/* This function removes trailing white spaces, returns length */

int remove_trailing_white ( char * ptr_string )  /* function declaration    */

{

  char * ptr_stringx, * ptr_stringy, character ;

  ptr_stringx = ptr_string ;
  ptr_stringy = ptr_string ;
  while ( ( character = * ptr_stringx++ ) != '\0' ) {
    if ( ( character != ' ' )  && 
         ( character != '\t' ) && 
         ( character != '\r' ) && 
         ( character != '\n' ) && 
         ( character != 12 ) ) {    // Form feed
       ptr_stringy = ptr_stringx ;
    endiff

  wend
  *ptr_stringy = '\0' ;
  return ( (int ) ( ptr_stringy - ptr_string ) ) ;

}

/*********************************************/

/* This function returns pointer to first occurance of character */

char * return_ptr_to_first_occurance ( char * ptr_string, char character )  /* function declaration    */

{

  while ( ( *ptr_string != '\0' ) && ( *ptr_string != character ) ) {
    ptr_string ++ ;
  wend
  if ( *ptr_string == '\0' ) ptr_string = NULL ;
  return ( ptr_string ) ;

}

/**********************************************************/

/* This function returns pointer to first non-white character */
/*   Note that '\0' counts as non_white.                      */

char * get_ptr_to_first_non_white ( char * ptr_string )  /* function declaration    */

{
  char character ;

  while ( ( ( character = *ptr_string++ ) != '\0' ) && 
          ( ( character == ' '  ) || 
            ( character == '\t' ) || 
            ( character == '\r' ) || 
            ( character == '\n' ) ) ) ;

  return ( --ptr_string ) ;

}

/**************************************/

/* This function returns pointer to first white character */
/*   Note that '\0' counts as white.                      */

char * get_ptr_to_first_white ( char * ptr_string )  /* function declaration    */

{
  char character ;

  while ( ( ( character = * ptr_string++ ) != '\0' ) && 
            ( character != ' '  ) && 
            ( character != '\t' ) && 
            ( character != '\r' ) && 
            ( character != '\n' ) ) ;

  return ( --ptr_string ) ;

}

/************************************/

// Converts ASCII String to hex number

unsigned int hex_atoi ( char XDATA_SPACE * ptr_character_array ) /* function declaration */

{

  char         character ;
  unsigned int value = 0 ;

  while ( ( character = ( * ( ptr_character_array ++ ) ) ) != '\0' ) { 
    value = ( value << 4 ) + get_hex_value ( character ) ;
  wend

  return ( value ) ;

}

/****************************/

void quick_int_printf  ( unsigned char XDATA_SPACE * ptr_to_string, unsigned int ivalue ) /* function declaration */

{

  unsigned char XDATA_SPACE * tmp_ptr_to_string ;
  unsigned char XDATA_SPACE * ptr_to_end_of_string ;


  if ( ivalue == 0 ) {
    ptr_to_string [ 0 ] = '0'  ;
    ptr_to_string [ 1 ] = '\0' ;
  endiff

  else {

    ptr_to_end_of_string = ptr_to_string + 5 ;     // Allow for 65000

    * ptr_to_end_of_string = '\0' ;

    while ( ivalue > 0 ) {

      ptr_to_end_of_string -- ;

      * ptr_to_end_of_string = ivalue % 10  + '0' ;

      ivalue /= 10 ;

    wend

    tmp_ptr_to_string = ptr_to_string ;

    while ( ( * tmp_ptr_to_string ++ = * ptr_to_end_of_string ++ ) != '\0' ) ;   // move string left

  endiff

}

/****************************/

void quick_hex_printf  ( unsigned char XDATA_SPACE * ptr_to_string, unsigned int ivalue ) /* function declaration */

{

  unsigned char XDATA_SPACE * tmp_ptr_to_string ;
  unsigned char XDATA_SPACE * ptr_to_end_of_string ;

  if ( ivalue == 0 ) {
    ptr_to_string [ 0 ] = '0'  ;
    ptr_to_string [ 1 ] = '\0' ;
  endiff

  else {

    ptr_to_end_of_string = ptr_to_string + 5 ;     // Allow for 65000

    * ptr_to_end_of_string = '\0' ;

    while ( ivalue > 0 ) {

      ptr_to_end_of_string -- ;

      * ptr_to_end_of_string = hex_digit_array [ ivalue & 0x0F ] ;

      ivalue = ivalue >> 4 ;

    wend

    tmp_ptr_to_string = ptr_to_string ;

    while ( ( * tmp_ptr_to_string ++ = * ptr_to_end_of_string ++ ) != '\0' ) ;   // move string left

  endiff

}

/****************************/

void quick_long_printf  ( unsigned char XDATA_SPACE * ptr_to_string, unsigned long lvalue ) /* function declaration */

{

  unsigned char XDATA_SPACE * tmp_ptr_to_string ;
  unsigned char XDATA_SPACE * ptr_to_end_of_string ;

  if ( lvalue == 0 ) {
    ptr_to_string [ 0 ] = '0'  ;
    ptr_to_string [ 1 ] = '\0' ;
  endiff

  else {

    ptr_to_end_of_string = ptr_to_string + 5 ;     // Allow for 65000

    * ptr_to_end_of_string = '\0' ;

    while ( lvalue > 0 ) {

      ptr_to_end_of_string -- ;

      * ptr_to_end_of_string = (unsigned char) ( lvalue % 10L ) + '0' ;

      lvalue /= 10 ;

    wend

    tmp_ptr_to_string = ptr_to_string ;

    while ( ( * tmp_ptr_to_string ++ = * ptr_to_end_of_string ++ ) != '\0' ) ;   // move string left

  endiff

}

/***********************************/

int string_input ( char ROM_SPACE * message, char XDATA_SPACE * character_array )  /* function declaration    */

{

  int character_ctr ;

  BIT_TYPE tmp_ignore_keyboard_timing_flag ;

  tmp_ignore_keyboard_timing_flag = ignore_keyboard_timing_flag ;

  ignore_keyboard_timing_flag = 1 ;

  rom_scr_printf ( message ) ;
  rom_scr_printf ( "\r\n" ) ;

  scr_eol () ;

  string_inputx ( character_array ) ;

  character_ctr = remove_leading_and_trailing_white ( character_array ) ;  // Added 3/21/01

  gotoxy ( 1, lcd_row ) ;
  scr_eol () ;
  gotoxy ( 1, (unsigned char) (lcd_row - 1) ) ;
  scr_eol () ;

  ignore_keyboard_timing_flag = tmp_ignore_keyboard_timing_flag ;

  return ( character_ctr ) ;

}

/***********************************/

int string_inputx ( char XDATA_SPACE * local_character_buffer )  /* function declaration    */

{

  int character ;
  unsigned char local_character_buffer_index = 0 ;

  BIT_TYPE done_flag ;

  while ( scr_getc () != -1 ) ;

  done_flag = 0 ;

  while ( done_flag == 0 ) {

    #if defined ( __C51__ )
      debounce_switches () ;
    #endif

    if ( ( character = scr_getc () ) != -1 ) {

      beep ( 20 ) ;

      if ( character == ENTER_KEY ) {

        local_character_buffer [ local_character_buffer_index ] = '\0' ;

        gotoxy ( 1, lcd_row ) ;

        scr_eol    () ;

        done_flag = 1 ;

      endiff

      else if ( character == BKSP_KEY ) {
        if ( local_character_buffer_index > 0 ) {
          local_character_buffer [ -- local_character_buffer_index ] = '\0' ;
          gotoxy ( 1, lcd_row ) ;
          scr_printf    ( local_character_buffer ) ;
          scr_eol () ;
        endiff
      endiff

      else if ( character == CTRL_KEY ) {
      endiff

      else if ( local_character_buffer_index < NUMBER_OF_LCD_COLUMNS ) {

        if ( character == '*' ) {
          if ( ( control_key_on_flag != 0 ) || ( colon_flag != 0 ) ) {
            character = ':' ;
          endiff
        endiff

        local_character_buffer [ local_character_buffer_index ++ ] = character ;

        gotoxy ( local_character_buffer_index, lcd_row ) ;

        scr_putc ( (unsigned char) character ) ;

      endiff

      else {
        beep ( 200 ) ;
      endiff

    endiff

  wend

  return ( local_character_buffer_index ) ;

}

/************************************/

// This function get hex value (ie 0-15) from hex character "0-9" and "a-f"

signed char get_hex_value ( char character ) /* function declaration */

{

  if      ( ( character >= '0' ) && ( character <= '9' ) ) return character - '0' ;
  else if ( ( character >= 'a' ) && ( character <= 'f' ) ) return character - 'a' + 10 ;
  else if ( ( character >= 'A' ) && ( character <= 'F' ) ) return character - 'A' + 10 ;
  else                                                     return -1 ;

}

/***********************************************************************/

void serial_print_float_variable ( char ROM_SPACE * string, float variable ) /* function declaration */

{

  rom_scr_printf     ( string ) ;
  rom_scr_printf     ( (char ROM_SPACE *) " " ) ;
  quick_long_printf  ( scratch_string, (long) ( variable * 10000.0 + 0.5 ) ) ;
  serial_printf      ( scratch_string ) ;
  rom_scr_printf     ( (char ROM_SPACE *) "\r\n" ) ;

}

/****************************/

void serial_print_long_variable ( char ROM_SPACE * string, unsigned long variable ) /* function declaration */

{

  rom_scr_printf     ( string ) ;
  rom_scr_printf     ( (char ROM_SPACE *) " " ) ;
  quick_long_printf  ( scratch_string, variable ) ;
  serial_printf      ( scratch_string ) ;
  rom_scr_printf     ( (char ROM_SPACE *) "\r\n" ) ;

}

/****************************/

void serial_print_int_variable ( char ROM_SPACE * string, unsigned int variable ) /* function declaration */

{

  rom_serial_printf  ( string ) ;
  rom_serial_printf  ( (char ROM_SPACE *) " " ) ;
  quick_int_printf   ( scratch_string, variable ) ;
  serial_printf      ( scratch_string ) ;
  rom_xserial_printf ( (char ROM_SPACE *) "\r\n" ) ;

}

/****************************/

void serial_print_hex_variable ( char ROM_SPACE * string, unsigned int variable ) /* function declaration */

{

  rom_serial_printf  ( string ) ;
  rom_serial_printf  ( (char ROM_SPACE *) " " ) ;
  quick_hex_printf   ( scratch_string, variable ) ;
  serial_printf      ( scratch_string ) ;
  rom_xserial_printf ( (char ROM_SPACE *) "\r\n" ) ;

}

/*************************************************************/

BIT_TYPE get_yes_flag ( char ROM_SPACE * string ) /* function declaration */

{

  BIT_TYPE return_flag, done_flag ;
  int character ;
  BIT_TYPE tmp_ignore_keyboard_timing_flag ;

  tmp_ignore_keyboard_timing_flag = ignore_keyboard_timing_flag ;

  ignore_keyboard_timing_flag = 1 ;

  while ( scr_getc () != -1 ) ;

  rom_scr_printf ( string ) ;

  rom_scr_printf ( "(y/n)?" ) ;

  scr_eol () ;

  done_flag = 0 ;

  while ( done_flag == 0 ) {
    #if defined ( __C51__ )
      debounce_switches () ;
    #endif
    character = scr_getc () ;
    if ( character != -1 ) {
      beep ( 20 ) ;
    endiff
    if ( character == YES_KEY ) {
      done_flag = 1 ;
      return_flag = 1 ;
    endiff
    else if ( character == NO_KEY ) {
      done_flag = 1 ;
      return_flag = 0 ;
    endiff
  wend

  gotoxy ( 1, 1 ) ;                                            

  scr_eol () ;

  ignore_keyboard_timing_flag = tmp_ignore_keyboard_timing_flag ;

  return return_flag ;

}

/*************************************/

void hex_inputx ( char ROM_SPACE * description_string, unsigned int * ptr_value )         /* function declaration    */

{

  string_input ( description_string, scratch_string ) ;
  *ptr_value = hex_atoi ( scratch_string ) ;

}

/***************************************/

void serial_crlf ( unsigned char number ) /* function declaration */

{

  while ( number -- != 0 ) {
    serial_putc ( '\n' ) ;
    serial_putc ( '\r' ) ;
  wend

}

/*********************************************************************/

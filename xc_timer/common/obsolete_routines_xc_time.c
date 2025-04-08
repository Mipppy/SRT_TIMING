/*********************************************************************/

// This function returns a time string from time in tenths past midnight.

#if ! defined ( __C51__ )

#if ( 0 )

short DLL_EXPORT get_time_string_from_tenths ( char * timer_string, long timer_tenths ) /* function declaration */

{

  short  seconds, minutes, hours ;
  long   tenths ;
  char   character ;
  char * ptr_to_timer_string ;

  tenths = timer_tenths ;

  hours   = (short) ( tenths  / 36000L ) ;

  tenths -= hours   * 36000L ;

  minutes = (short) ( tenths  /   600L ) ;

  tenths -= minutes *   600L ;

  seconds = (short) ( tenths  /    10L ) ;

  tenths -= seconds *    10L ;

  sprintf ( timer_string, "%2d:%2d:%2d.%1d", hours, minutes, seconds, tenths ) ;

  ptr_to_timer_string = timer_string ;

// Here we pad timer string with leading zeros

  while ( ( character = * ptr_to_timer_string ) != '\0' ) {
    if ( character == ' ' ) {
      * ptr_to_timer_string = '0' ;
    endif
    ptr_to_timer_string ++ ;
  endif

  return 1 ;

}

/**********************************************************************/
     
// This function parses a military time string, and returns a long integer for the number of tenths past midnight

long get_tenths_from_time_string ( char * ptr_to_time_string ) /* function declaration */

{

  long  time_tenths = 0 ;
  short hours       = 0 ;
  short minutes     = 0 ;
  short seconds     = 0 ;
  short tenths      = 0 ;
  short msec   ;
  short length ;     
  char  tmp_time_string [ 50 ] ;
  char  * ptr_to_tmp_time_string ;

  char  * ptr_to_decimal, * ptr_to_left_colon, * ptr_to_right_colon ;

  strcpy ( tmp_time_string, ptr_to_time_string ) ;

  ptr_to_tmp_time_string = tmp_time_string ;

  if ( remove_leading_and_trailing_white ( ptr_to_tmp_time_string ) > 0 ) {

    ptr_to_decimal = return_ptr_to_first_occurance ( ptr_to_tmp_time_string, '.' ) ;

    if ( ptr_to_decimal != NULL ) {

      * ptr_to_decimal ++ = '\0' ;
      length = strlen ( ptr_to_decimal ) ;
      
      if ( length == 1 ) {
          msec = 100 * atoi ( ptr_to_decimal ) ;
      endif
      else if ( length == 2 ) {
          msec = 10 * atoi ( ptr_to_decimal )  ;
      endif
      else if ( length == 3 ) {
          msec =  1 * atoi ( ptr_to_decimal )  ;
      endif
      else {
          msec = 0 ;
      endif

      tenths = ( 10 * msec  + 500 ) / 1000 ;

    endif

    
    ptr_to_left_colon = return_ptr_to_first_occurance ( ptr_to_tmp_time_string,     ':' ) ;

    if ( ptr_to_left_colon != NULL ) {

      * ptr_to_left_colon ++ = '\0' ;

      ptr_to_right_colon   = return_ptr_to_first_occurance ( ptr_to_left_colon,  ':' ) ;

      if ( ptr_to_right_colon != NULL ) {

        * ptr_to_right_colon ++ = '\0' ;

        hours   = atoi ( ptr_to_tmp_time_string  ) ;
        minutes = atoi ( ptr_to_left_colon   ) ;
        seconds = atoi ( ptr_to_right_colon  ) ;

      endif

      else {

        minutes = atoi ( ptr_to_tmp_time_string  ) ;
        seconds = atoi ( ptr_to_left_colon   ) ;

      endif

    endif

    else {
      seconds = atoi ( ptr_to_tmp_time_string ) ;
    endif
  
    time_tenths  = 36000L * (long) hours   ;
    time_tenths +=   600L * (long) minutes ;
    time_tenths +=    10L * (long) seconds ;
    time_tenths +=          (long) tenths  ;

  endif

  return time_tenths ;

}

#endif

/*********************************************************************/

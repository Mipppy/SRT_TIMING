
/****************************************************************/

// This module contains utility functions that should not be platform dependent

#define MISC_MODULE  0

#include <x_header.h>



/*********************************************************************/
/*********************************************************************/

// Max time is 100 hours, or     (long) ( 3600L * 100L * 1000L )      3.6 * 10 ^ 8  msec
//                        or                                          6000          minutes
//                        or                                          3.6 * 10 ^ 5  sec      ( ie 360,000 )

/*********************************************************************/

// 12 bytes max length  00:00:00.000 

void get_time_string_from_msec ( long long_msec, char XDATA_SPACE * string, BIT_TYPE time_in_tics_flag ) /* function declaration */

{

  unsigned int  digit ;
  unsigned int  int_msec ;
  unsigned int  minutes ;
  BIT_TYPE negative_flag ;

  char XDATA_SPACE * ptr_to_string ;

  ptr_to_string = string ;

  if ( long_msec < 0 ) {
    negative_flag = 1 ;
    long_msec *= -1 ;
  endiff
  else {
    negative_flag = 0 ;
  endiff

  {

    long long_max_time ;

    if ( clock_24_hour_bit != 0 ) {
      if ( time_in_tics_flag == 0 ) long_max_time = TWENTY_FOUR_HOURS_MSEC ;
      else                          long_max_time = TWENTY_FOUR_HOURS_TICS ;
    endiff
    else {
      if ( time_in_tics_flag == 0 ) long_max_time = HUNDRED_HOURS_MSEC ;
      else                          long_max_time = HUNDRED_HOURS_TICS ;
    endiff

    while ( long_msec > long_max_time ) {
      long_msec      -= long_max_time ;
    wend

  }

// Here we handle rounding to nearest unit of interest

  if ( time_in_tics_flag == 0 ) {

    unsigned char rounder = 0 ;

    if ( tenths_only_resolution_flag != 0 ) {
      rounder   = 50 ;                                         // ie 1000 / 20
    endiff
    else if ( msec_resolution_flag == 0 ) {
      if ( hundredths_resolution_flag == 0 ) {
        rounder = 50 ;                                         // ie 1000 / 20
      endiff
      else {
        rounder =  5 ;
      endiff
    endiff

    long_msec += rounder ;

    minutes      = (unsigned int) ( long_msec   /  60000L                              ) ;
    int_msec     = (unsigned int) ( long_msec   -  60000L  *  (unsigned long) minutes  ) ;

  endiff

  else {   // if ( time_in_tics_flag != 0 )

// Here we handle rounding to nearest unit of interest

    unsigned char rounder = 0 ;

    if ( tenths_only_resolution_flag != 0 ) {
      rounder   = ( 1024 / 20 ) ;                              // ie 1000 / 20
    endiff
    else if ( msec_resolution_flag == 0 ) {
      if ( hundredths_resolution_flag == 0 ) {
        rounder = ( 1024 / 20 ) ;                              // ie 1000 / 20
      endiff
      else {
        rounder = ( 1024 / 200 ) ;
      endiff
    endiff

    long_msec += rounder ;

    minutes      = (unsigned int) ( long_msec   /  ( 60L * 1024L )                              ) ;
    int_msec     = (unsigned int) ( long_msec   -  ( 60L * 1024L )  *  (unsigned long) minutes  ) ;

  endiff

// First, we found minutes.  We split the problem up here, because we only have to perform a single long divide!!! Good for 1000 hours!!

// Note: Following is valid for both msec and tics

  digit    = minutes / (unsigned int) 600 ;                   // This is the ten hours digit               
  minutes -= digit   * (unsigned int) 600 ;
 
  if ( digit == 0 ) {
    if ( negative_flag == 0 ) {
      * ptr_to_string ++ = ' ' ;
    endiff
    else {
      * ptr_to_string ++ = '-' ;
    endiff
  endiff
  else {
    if ( negative_flag != 0 ) {
      * ptr_to_string ++ = '-' ;
    endiff
    * ptr_to_string ++ = '0' +  (unsigned char) digit ;
  endiff

  digit    = minutes / (unsigned int) 60 ;                    // This is the     hours digit
  minutes -= digit   * (unsigned int) 60 ;

  * ptr_to_string ++   = '0' +  (unsigned char) digit ;
  * ptr_to_string ++   = ':' ;

  digit    = minutes / (unsigned int) 10 ;                    // This is the ten minutes digit
  minutes -= digit   * (unsigned int) 10 ;                    // This is the     minutes digit

  * ptr_to_string ++   = '0' +  (unsigned char) digit ;
  * ptr_to_string ++   = '0' +  (unsigned char) minutes ;
  * ptr_to_string ++   = ':' ;

/****************************/

// Here we are left with int_msec equal to seconds

  if ( time_in_tics_flag == 0 ) {

    digit      = int_msec  /  (unsigned int) 10000 ;            //  This is the ten seconds digit
    int_msec  -= digit     *  (unsigned int) 10000 ;

    * ptr_to_string ++   = '0' +  (unsigned char) digit ;

    digit      = int_msec  /  (unsigned int) 1000 ;             //  This is the     seconds digit
    int_msec  -= digit     *  (unsigned int) 1000 ;

    * ptr_to_string ++   = '0' +  (unsigned char) digit ;
    * ptr_to_string ++   = '.'          ;                       // Decimal

  endiff

//****************************************

// Following handles time measured in tics where there are 1024 tics per second

  else { // if ( time_in_tics_flag != 0 ) {

    unsigned char seconds ;

    seconds   = get_int_msb ( int_msec ) >> 2 ;

    int_msec &= 0x03FF ;                                                 // 10 bits only

    digit      = seconds   /  10 ;                                       //  This is the ten seconds digit
    seconds   -= digit     *  10 ;

    * ptr_to_string ++   = '0' +  (unsigned char) digit   ;
    * ptr_to_string ++   = '0' +  (unsigned char) seconds ;
    * ptr_to_string ++   = '.'          ;                                // Decimal

    int_msec = (unsigned int) ( ( (long) int_msec * 125L + 64 ) >> 7 ) ;

  endiff

// Here we are left with int_msec = msec

  digit              = int_msec  /  (unsigned int) 100 ;

  * ptr_to_string ++       = '0' +  (unsigned char) digit ;

  if   ( tenths_only_resolution_flag == 0 ) {
    if ( hundredths_resolution_flag  != 0 ) { 

      int_msec        -= digit     *  (unsigned int) 100 ;

      digit            = int_msec  /  (unsigned int) 10 ;

      * ptr_to_string ++     = '0' +  (unsigned char) digit ;

      if ( msec_resolution_flag != 0 ) {

        int_msec      -= digit     *  (unsigned int) 10 ;

        * ptr_to_string ++   = '0' +  (unsigned char) int_msec ;

      endiff

    endiff

  endiff

  * ptr_to_string        = '\0' ;

}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
     
// This function parses a time string (0-99 hours), and returns a long integer for the number of msec past midnight

// This function returns -1 if not a valid time string.

// WARNING: This function modifies the original string, so be CAREFUL

// WARNING: This function also handles tics

long get_msec_from_time_string ( char * ptr_to_time_string, BIT_TYPE time_in_tics_flag ) /* function declaration */

{

  long           time_msec   = 0 ;
  short          hours       = 0 ;
  short          minutes     = 0 ;
  short          seconds     = 0 ;

  char  * ptr_to_left_colon ;
  char  * ptr_to_right_colon ;
  BIT_TYPE negative_flag ;

  if ( remove_leading_and_trailing_white ( ptr_to_time_string ) > 0 ) {

    #define ptr_to_decimal ptr_to_left_colon

    if ( * ptr_to_time_string == '-' ) {
      negative_flag = 1 ;
      ptr_to_time_string ++ ;
      remove_leading_white ( ptr_to_time_string ) ;
    endiff 
    else {
      negative_flag = 0 ;
    endiff

    ptr_to_decimal = return_ptr_to_first_occurance ( ptr_to_time_string, '.' ) ;

    if ( ptr_to_decimal != NULL ) {

      unsigned char length ;

      * ptr_to_decimal ++ = '\0' ;

      length = strlen ( ptr_to_decimal ) ;
      
      if ( length == 1 ) {
        time_msec = 100 * atoi ( ptr_to_decimal ) ;
      endiff
      else if ( length == 2 ) {
        time_msec = 10 * atoi ( ptr_to_decimal )  ;
      endiff
      else if ( length == 3 ) {
        time_msec =  1 * atoi ( ptr_to_decimal )  ;
      endiff
      else {
        time_msec = 0 ;
      endiff

    endiff

    #undef ptr_to_decimal 
    
    ptr_to_left_colon = return_ptr_to_first_occurance ( ptr_to_time_string,  ':' ) ;

    if ( ptr_to_left_colon != NULL ) {

      * ptr_to_left_colon ++ = '\0' ;

      ptr_to_right_colon   = return_ptr_to_first_occurance ( ptr_to_left_colon,  ':' ) ;

      if ( ptr_to_right_colon != NULL ) {

        * ptr_to_right_colon ++ = '\0' ;

        hours   = atoi ( ptr_to_time_string  ) ;
        minutes = atoi ( ptr_to_left_colon   ) ;
        seconds = atoi ( ptr_to_right_colon  ) ;

      endiff

      else {

        minutes = atoi ( ptr_to_time_string  ) ;
        seconds = atoi ( ptr_to_left_colon       ) ;

      endiff

    endiff

    else {
      seconds = atoi ( ptr_to_time_string ) ;
    endiff

    if ( time_in_tics_flag == 0 ) {
  
      time_msec += 3600000L * (unsigned long) hours   ;
      time_msec +=   60000L * (unsigned long) minutes ;
      time_msec +=    1000L * (unsigned long) seconds ;

    endiff

    else {

      time_msec  = ( ( time_msec << 7 ) +  0L ) / 125L ;

      time_msec += (    3600L * 1024L ) * (unsigned long) hours   ;
      time_msec += (      60L * 1024L ) * (unsigned long) minutes ;
      time_msec += (       1L * 1024L ) * (unsigned long) seconds ;

    endiff

    if ( negative_flag != 0 ) {
      time_msec *= -1 ;
    endiff

  endiff

  return time_msec ;

}

/********************************************************************/
/*********************************************************************/
/*********************************************************************/

#if ( 0 ) 

#if ! defined ( __C51__ )

long DLL_EXPORT __stdcall dll_test_time_routines ( void ) /* function declaration */

{

  int task ;

  double double_time_msec ;
  double double_time_tics ;
  double double_new_time_msec ;
  double double_diff ;

  long   time_msec ;
  long   time_tics ;
  long   new_time_msec ;

  long error_counter = 0 ;
  BOOL done_flag = 0 ;
 
  char time_string    [ 50 ] ;
  char scratch_string [ 50 ] ;
  char debug_file_path_and_name [ 200 ] ;

  dll_get_file_path_and_name_from_parts ( debug_file_path_and_name, g . race_directory, "time_test", ".txt" ) ;
  output_formatted_file_string        ( debug_file_path_and_name, NULL ) ;
  output_formatted_file_string        ( debug_file_path_and_name, "\nTime Test 7/8/1 at 4:32PM\n\n" ) ;

  msec_resolution_flag              = 0 ;
  hundredths_resolution_flag        = 1 ;
  tenths_only_resolution_flag       = 1 ;

  #define TOLERANCE 50.50

  task = 3 ;

// Here we handle msec counts

  if ( task == 1 ) {

    for ( time_msec = 0 ; ( ( time_msec < HUNDRED_HOURS_MSEC ) && ( done_flag == 0 ) ) ; time_msec += 1 ) {

      get_time_string_from_msec ( (long) time_msec, scratch_string, 0 ) ;

      strcpy ( time_string, scratch_string ) ;

      new_time_msec = get_msec_from_time_string ( scratch_string, 0 ) ;

      if ( ( time_msec % 60000L ) == 0 ) {

        output_formatted_file_string ( debug_file_path_and_name, "Up to time_string=***%s*** \n\n", time_string ) ;

      endif

      if ( error_counter > 1000 ) {
        done_flag = 1 ;
      endif

      if ( abs ( new_time_msec - time_msec ) > TOLERANCE ) {

        error_counter ++ ;

        double_time_msec = (double) time_msec / (double) 1000.0 ;
        double_new_time_msec = (double) new_time_msec / (double) 1000.0 ;
        double_diff = 1000.0 * abs ( double_time_msec - double_new_time_msec ) ;

        output_formatted_file_string ( debug_file_path_and_name, "Failure time=%9ld  time_string=***%s***  new_time_msec=%9ld \n", time_msec, time_string, new_time_msec    ) ;
        output_formatted_file_string ( debug_file_path_and_name, "     Time=%7.4f    New Time=%7.4f  Diff=%7.8f \n\n",  double_time_msec, double_new_time_msec, double_diff ) ;

// The following is put here, only so we can set breakpoint here

        get_time_string_from_msec ( (long) time_msec, scratch_string, 1 ) ;

        strcpy ( time_string, scratch_string ) ;

        new_time_msec = get_msec_from_time_string ( scratch_string, 0 ) ;

      endif


    next

  endif

// Here we handle tics

  else if ( task == 2 ) {
 
    for ( time_tics = 0 ; ( ( time_tics < HUNDRED_HOURS_TICS ) && ( done_flag == 0 ) ) ; time_tics ++ ) {

      double_time_tics = (double) time_tics / (double) 1024.0 ;

      get_time_string_from_msec ( (long) time_tics, scratch_string, 1 ) ;

      strcpy ( time_string, scratch_string ) ;

      new_time_msec = get_msec_from_time_string ( scratch_string, 0 ) ;

      double_new_time_msec = (double) new_time_msec / (double) 1000.0 ;

      if ( ( time_tics & 0xFFFFL ) == 0 ) {

        output_formatted_file_string ( debug_file_path_and_name, "Up to time_string=***%s*** \n\n", time_string ) ;

      endif

      if ( error_counter > 1000 ) {
        done_flag = 1 ;
      endif

      double_diff = 1000.0 * abs ( double_time_tics - double_new_time_msec ) ;

      if ( double_diff > TOLERANCE ) {

        error_counter ++ ;

        output_formatted_file_string ( debug_file_path_and_name, "Failure time=%9ld  time_string=***%s***  new_time_msec=%9ld \n", time_tics, time_string, new_time_msec    ) ;
        output_formatted_file_string ( debug_file_path_and_name, "     Time=%7.4f    New Time=%7.4f  Diff=%7.8f \n\n",  double_time_tics, double_new_time_msec, double_diff ) ;

// The following is put here, only so we can set breakpoint here

        get_time_string_from_msec ( (long) time_tics, scratch_string, 1 ) ;

        strcpy ( time_string, scratch_string ) ;

        new_time_msec = get_msec_from_time_string ( scratch_string, 0 ) ;

      endif

    next
 
  endif

  if ( task == 3 ) { 

    for ( time_msec = 0 ; ( ( time_tics < HUNDRED_HOURS_MSEC ) && ( done_flag == 0 ) ) ; time_msec += 1 ) {

      double_time_msec = time_msec / 1000.0 ;

      time_tics = (long) ( double_time_msec * 1024.0 + 0.5 ) ;

      double_time_tics = (double) time_tics / (double) 1024.0 ;

      get_time_string_from_msec ( (long) time_tics, scratch_string, 1 ) ;

      strcpy ( time_string, scratch_string ) ;

      new_time_msec = get_msec_from_time_string ( scratch_string, 0 ) ;

      double_new_time_msec = (double) new_time_msec / (double) 1000.0 ;

      if ( ( time_msec % 60000L ) == 0 ) {

        output_formatted_file_string ( debug_file_path_and_name, "Up to time_string=***%s*** \n\n", time_string ) ;

      endif

      if ( error_counter > 1000 ) {
        done_flag = 1 ;
      endif

      double_diff = 1000.0 * abs ( double_time_tics - double_new_time_msec ) ;

      if ( double_diff > TOLERANCE ) {

        error_counter ++ ;

        output_formatted_file_string ( debug_file_path_and_name, "Failure time=%9ld  time_string=***%s***  new_time_msec=%9ld \n", time_tics, time_string, new_time_msec    ) ;
        output_formatted_file_string ( debug_file_path_and_name, "     Time=%7.4f    New Time=%7.4f  Diff=%7.8f \n\n",  double_time_tics, double_new_time_msec, double_diff ) ;

// The following is put here, only so we can set breakpoint here

        get_time_string_from_msec ( (long) time_tics, scratch_string, 1 ) ;

        strcpy ( time_string, scratch_string ) ;

        new_time_msec = get_msec_from_time_string ( scratch_string, 0 ) ;

      endif


    next

  endif

  output_formatted_file_string ( debug_file_path_and_name, "Finished Time Test\n" ) ;

}



/*********************************************************************/

#endif   // if __C51__ not defined

#endif

/*********************************************************************/
/**********************************************************************/


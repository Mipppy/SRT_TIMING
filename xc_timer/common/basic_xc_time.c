
' /*********************************************************************/

' This module contains utility functions that should not be platform dependent

' /*********************************************************************/
' /*********************************************************************/

' Max time is 100 hours, or     (long) ( 3600L * 100L * 1000L )      3.6 * 10 ^ 8  msec
'                        or                                          6000          minutes
'                        or                                          3.6 * 10 ^ 5  sec      ( ie 360,000 )

' /*********************************************************************/

' 12 bytes max length  00:00:00.000 

Function get_time_string_from_msec ( long long_msec As Long ) As String '   /* function declaration */
  on error goto err_get_time_string_from_msec

    Dim digit         As Integer
    Dim int_msec      As Integer
    Dim minutes       As Integer
    Dim negative_flag As Integer
    Dim long_max_time As Long
    Dim long_minutes  As Long

    Dim rounder       As Integer
    Dim ptr_to_string As String 
    Dim time_string   As String

    rounder = 0  

    time_string = ""

    if ( long_msec < 0 ) Then
      negative_flag = 1  
      long_msec = long_msec * -1  
'   endif
    else
      negative_flag = 0  
    endif

    if ( clock_24_hour_bit <> 0 ) Then
      long_max_time = TWENTY_FOUR_HOURS_MSEC  
'   endif
    else
      long_max_time = HUNDRED_HOURS_MSEC  
    endif

    while ( long_msec > long_max_time ) 
      long_msec       = long_msec - long_max_time  
    wend

'   Here we handle rounding to nearest unit of interest

    if ( tenths_only_resolution_flag <> 0 ) Then
      rounder   = 50                                           ' ie 1000 / 20
'   endif
    else if ( msec_resolution_flag = 0 ) Then
      if ( hundredths_resolution_flag = 0 ) Then
        rounder = 50                                           ' ie 1000 / 20
'     endif
      else
        rounder =  5  
      endif
    endif

    long_msec = long_msec +  rounder  

    long_minutes = long_msec / 60000

    minutes      = long_minutes
           
    int_msec     = long_msec   -  ( 60000  * long_minutes  )  

'   First, we found minutes.  We split the problem up here, because we only have to perform a single long divide!!! Good for 1000 hours!!

'   Note: Following is valid for both msec and tics

    digit    = minutes /  600                      ' This is the ten hours digit               
    minutes  = minutes     - digit   *  600  
  
    if ( digit = 0 ) Then
      if ( negative_flag = 0 ) Then
        time_string = time_string + " "  
'     endif
      else
        time_string = time_string + "-"
      endif
'   endif
    else
      if ( negative_flag <> 0 ) Then
        time_string = time_string + "-"  
      endif
      time_string = time_string  +  chr$ ( 48 + digit )
    endif

    digit    = minutes / 60                        ' This is the     hours digit
    minutes  = minutes - digit   *  60  

    time_string = time_string    +   chr$ ( 48 + digit )  
    time_string = time_string    + ":"  

    digit    = minutes /  10                       ' This is the ten minutes digit
    minutes  = minutes -  digit   *  10            ' This is the     minutes digit

    time_string = time_string    +  chr$ ( 48 + digit )  
    time_string = time_string    +  chr$ ( 48 + minutes )  
    time_string = time_string    +  ":"  

'   /****************************/

'   Here we are left with int_msec equal to seconds

    digit      = int_msec  /  10000              '  This is the ten seconds digit
    int_msec   = int_msec -  digit     *   10000  

    time_string = time_string + chr$ ( 48 + digit )  

    digit      = int_msec  /   1000               '  This is the     seconds digit
    int_msec   = int_msec -  digit     *   1000  

    time_string = time_string + chr$ ( 48 + digit )  
    time_string = time_string + "."                                  ' Decimal

'*'****************************************

'   Here we are left with int_msec = msec

    digit              = int_msec  /   100  

    time_string = time_string + chr$ ( 48 + digit )  

    if   ( tenths_only_resolution_flag = 0 ) Then
      if ( hundredths_resolution_flag  <> 0 ) Then 

        int_msec         = int_msec -  digit     *   100  

        digit            = int_msec  /   10  

        time_string = time_string + chr$ ( 48 + digit )  

        if ( msec_resolution_flag <> 0 ) Then

          int_msec      = int_msec -  digit     *   10  

          time_string = time_string + chr$ ( 48 + int_msec )  

        endif

      endif

    endif

    exit function 
  err_get_time_string_from_msec:
      MsgBox "get_time_string_from_tenths:  Error: " & Error$ & " Description:"& err.description

End Function ' get_time_string_from_tenths

' /**********************************************************************/
' /**********************************************************************/
' /**********************************************************************/
     
' This function parses a time string (0-99 hours), and returns a long integer for the number of msec past midnight

' This function returns -1 if not a valid time string.

' WARNING: This function modifies the original string, so be CAREFUL

' WARNING: This function also handles tics

Function get_msec_from_time_string ( time_string As String ) As Long ' /* function declaration */

  on error goto err_get_msec_from_time_string

    Dim  time_msec     As Long  
    Dim  hours         As Integer  
    Dim  minutes       As Integer  
    Dim  seconds       As Integer  
    Dim  negative_flag As Integer

    Dim location_of_left_colon    As Integer
    Dim location_of_right_colon   As Integer
    Dim negative_flag             As Integer
    Dim location_of_decimal       As Integer
    Dim time_string_length        As Integer

    time_msec     = 0  
    hours         = 0  
    minutes       = 0  
    seconds       = 0  

    get_msec_from_time_string = 0 

    time_string        = trim$ ( time_string ) 
    time_string_length = len ( time_string ) 

    if ( time_string_length > 0 ) Then

      if ( left$ ( time_string, 1 ) = "-" ) Then
        negative_flag = 1  
        time_string = right$ ( time_string, time_string_length - 1 )   
        time_string = trim$ ( time_string ) 
        time_string_length = len ( time_string ) 
'     endif 
      else
        negative_flag = 0  
      endif

      location_of_decimal = InStr ( 1, time_string, ".", 0 )

      if ( location_of_decimal > 0 ) Then

        decimal_length = time_string_length - location_of_decimal 

        decimal_string = mid$  ( time_string, location_of_decimal + 1, 99 )

        time_string    = left$ ( time_string, location_of_decimal - 1     ) 
        
        if ( decimal_length = 1 ) Then
          time_msec = 100 * val ( decimal_string )  
'       endif
        else if ( decimal_length = 2 ) Then
          time_msec = 10 * val ( decimal_string )   
'       endif
        else if ( decimal_length = 3 ) Then
          time_msec =  1 * val ( decimal_string )   
'       endif
        else
          time_msec = 0  
        endif

      endif
      
      location_of_left_colon = InStr ( 1, time_string, ":", 0 )  

      if ( location_of_left_colon > 0 ) Then

        location_of_right_colon   = InStr ( location_of_left_colon + 1, time_string, ":", 0 )   

        if ( location_of_right_colon > 0 ) Then

          hours   = val ( left$ ( time_string, location_of_left_colon  - 1 ) )  
          minutes = val ( mid$  ( time_string, location_of_left_colon  + 1, location_of_right_colon - location_of_left_colon - 1 )  
          seconds = val ( mid$  ( time_string, location_of_right_colon + 1, 99 )  

'       endif

        else    ' Left Colon, but No Right Colon

          minutes = val ( left$ ( time_string, location_of_left_colon - 1     ) )  
          seconds = val ( mid$  ( time_string, location_of_left_colon + 1, 99 ) ) 

        endif

'     endif

      else
        seconds = val ( time_string ) ;   ' No Colons
      endif

      time_msec = time_msec + 3600000  *  hours    
      time_msec = time_msec +   60000  *  minutes  
      time_msec = time_msec +    1000  *  seconds  

      if ( negative_flag <> 0 ) Then
        time_msec *= -1  
      endif

      get_msec_from_time_string = time_msec

    endif

  exit Function 
    err_get_msec_from_time_string:
    MsgBox "get_msec_from_time_string:  Error: " & Error$ & " Description:"& err.description

End Function ' get_msec_from_time_string


' /********************************************************************/
' /*********************************************************************/
' /*********************************************************************/
 
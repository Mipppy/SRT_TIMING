
/***********************/

/* timer_header.h */

/***********************/

#if   defined ( __C51__ )

//#include <REG52.H>
  #include <REG52FB.H>
  #include <absacc.h>

#endif

/***********************/

#if ! defined ( __C51__ )

  #include <windows.h>    // Needed for BOOL, and lots of other stuff
  #include <io.h>               // needed for access
  #include <stdio.h>             // needed for fprintf, fputc, fgets, etc
  #include <direct.h>             // needed for _getcwd
  #include <time.h>                 // needed for _strtime

#endif

/***********************/

#include <cdef.h>
#include <main.h>

#include <external.h>
#include <function.h>

#include <xc_io.h>

/******************************************************/
/******************************************************/



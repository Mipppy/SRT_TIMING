
#if ( 0 )

  Ernie's Notes:
    ???? Questions
    !!!! For Important Note
    $$$$ Can be Optimized
    VVVV To be improved in Future Version

#endif

#define begin_block {
#define end_block   }
#define  wend       }
#define  next       }
#define  endiff     }
#define  end_switch }
#define  end_do     }

#if ! defined ( __C51__ ) 
  #define  endif    }
#endif

#define escape 27
#define abort   3

#if ! defined ( __C51__ ) 
  #define abs(x)     ( ( (x) >= 0 )  ? (x) : -(x) )
#endif

#if ! defined ( min )
  #define min(x,y)   ( ( (x) < (y) ) ? (x) :  (y) )
#endif

#if ! defined ( max )
  #define max(x,y)   ( ( (x) > (y) ) ? (x) :  (y) )
#endif

#define bounds(x,y,z)  ( min ( (z), ( max ( (x), (y) ) ) ) )

#define cintx(x)        ( (int) ( (x) + 0.5 ) )
#define cint(x)         ( ( (x) >  0  ) ? (int) ( (x) + 0.5 ) : (int) ( (x) - 0.5 ) )
#define fint(x)         ( floor ( (x) + 0.5 ) )

#define flong(x) ( (long) ( x + 0.5 ) ) 

#ifndef pi
  #define pi 3.141592654
#endif

#define CODE_NULL ( void ( * ) () ) 0

#ifndef NULL
  #define NULL      (void *) 0
#endif

/*************************/

#if defined ( __C51__ )
  #define INT51_BYTEWISE_EQUAL(d,s) *(((unsigned char*)(d))+1)=*(((unsigned char*)(s))+0);*(((unsigned char*)(d))+0)=*(((unsigned char*)(s))+1)
#endif

#if ! defined ( __C51__ )
  #define INT51_BYTEWISE_EQUAL(d,s) *(((unsigned char*)(d))+0)=*(((unsigned char*)(s))+0);*(((unsigned char*)(d))+1)=*(((unsigned char*)(s))+1)
#endif

#define FLOAT_BYTEWISE_EQUAL(x,y)   *(((unsigned char*)(&x))+0)=*(((unsigned char*)(&y))+0);*(((unsigned char*)(&x))+1)=*(((unsigned char*)(&y))+1);*(((unsigned char*)(&x))+2)=*(((unsigned char*)(&y))+2);*(((unsigned char*)(&x))+3)=*(((unsigned char*)(&y))+3);

/*************************/


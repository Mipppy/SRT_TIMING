
/******************************************/

/* main.h                              */

/* This module contains miscellaneous system constants of interest to all modules */

/******************************************/

/* Note: Must check startup.m51 for stack beginning       */
/*       Also must check EXTERNAL.H for high end of stack */

#define DEBUG_FLAG                                0

#define NEWEST_STUFF_FLAG                         1
#define NEW_AUX_SERIAL_082103_STUFF_FLAG          1
#define NEW_061603_STUFF_FLAG                     1
#define NEW_102402_STUFF_FLAG                     1
#define AUX_SERIAL_081902_STUFF_FLAG              1

#if ( ( NEWEST_STUFF_FLAG != 0 ) && ( NEW_AUX_SERIAL_082103_STUFF_FLAG != 0 ) && ( NEW_061603_STUFF_FLAG != 0 ) && ( NEW_102402_STUFF_FLAG != 0 ) && ( AUX_SERIAL_081902_STUFF_FLAG != 0 ) )
  #define RELEASE_VERSION_NUMBER                  17                                                                           
  #define SOFTWARE_VERSION_DATE              "1/3/04"            // ????  Rubber Ducky Version
  #define SOFTWARE_VERSION_TIME              "6:50"             
  #define SOFTWARE_VERSION_NUMBER            "1.7.09"
  #define PRODUCTION_FLAG                          1
#endif

#if ( ( NEWEST_STUFF_FLAG == 0 ) && ( NEW_AUX_SERIAL_082103_STUFF_FLAG != 0 ) && ( NEW_061603_STUFF_FLAG != 0 ) && ( NEW_102402_STUFF_FLAG != 0 ) && ( AUX_SERIAL_081902_STUFF_FLAG != 0 ) )
  #define RELEASE_VERSION_NUMBER                  16                                                                           
  #define SOFTWARE_VERSION_DATE              "8/21/03"            // A352   Padilla's Second Version
  #define SOFTWARE_VERSION_TIME              "16:55"             
  #define SOFTWARE_VERSION_NUMBER            "1.6.01"
  #define PRODUCTION_FLAG                          1
#endif

#if ( ( NEWEST_STUFF_FLAG == 0 ) && ( NEW_AUX_SERIAL_082103_STUFF_FLAG == 0 ) && ( NEW_061603_STUFF_FLAG != 0 ) && ( NEW_102402_STUFF_FLAG != 0 ) && ( AUX_SERIAL_081902_STUFF_FLAG != 0 ) )
  #define RELEASE_VERSION_NUMBER                  15                                                                           
  #define SOFTWARE_VERSION_DATE              "6/16/03"            // 4886   Midnight Sun Run with Scanner
  #define SOFTWARE_VERSION_TIME              "18:47"
  #define SOFTWARE_VERSION_NUMBER            "1.5.13"
  #define PRODUCTION_FLAG                          0
#endif

#if ( ( NEWEST_STUFF_FLAG == 0 ) && ( NEW_AUX_SERIAL_082103_STUFF_FLAG == 0 ) && ( NEW_061603_STUFF_FLAG != 0 ) && ( NEW_102402_STUFF_FLAG != 0 ) && ( AUX_SERIAL_081902_STUFF_FLAG == 0 ) )
  #define RELEASE_VERSION_NUMBER                  14                                                                           
  #define SOFTWARE_VERSION_DATE              "6/16/03"            // 275F   Midnight Sun Run without Scanner
  #define SOFTWARE_VERSION_TIME              "18:47"
  #define SOFTWARE_VERSION_NUMBER            "1.5.11"
  #define PRODUCTION_FLAG                          0
#endif

#if ( ( NEWEST_STUFF_FLAG == 0 ) && ( NEW_AUX_SERIAL_082103_STUFF_FLAG == 0 ) && ( NEW_061603_STUFF_FLAG == 0 ) && ( NEW_102402_STUFF_FLAG != 0 ) && ( AUX_SERIAL_081902_STUFF_FLAG == 0 ) )
  #define RELEASE_VERSION_NUMBER                  13                                                                           
  #define SOFTWARE_VERSION_DATE              "10/24/02"           // 5C61   Best Released Version
  #define SOFTWARE_VERSION_TIME              "8:48"
  #define SOFTWARE_VERSION_NUMBER            "1.5.07"
  #define PRODUCTION_FLAG                          1
#endif

#if ( ( NEWEST_STUFF_FLAG == 0 ) && ( NEW_AUX_SERIAL_082103_STUFF_FLAG == 0 ) && ( NEW_061603_STUFF_FLAG == 0 ) && ( NEW_102402_STUFF_FLAG == 0 ) && ( AUX_SERIAL_081902_STUFF_FLAG != 0 ) )
  #define RELEASE_VERSION_NUMBER                  12                                                                           
  #define SOFTWARE_VERSION_DATE              "8/19/02"            // DB8F   Padilla's First Version
  #define SOFTWARE_VERSION_TIME              "10:59"
  #define SOFTWARE_VERSION_NUMBER            "1.4.01"
  #define PRODUCTION_FLAG                          0
#endif


#if ( ( NEWEST_STUFF_FLAG == 0 ) && ( NEW_AUX_SERIAL_082103_STUFF_FLAG == 0 ) && ( NEW_061603_STUFF_FLAG == 0 ) && ( NEW_102402_STUFF_FLAG == 0 ) && ( AUX_SERIAL_081902_STUFF_FLAG == 0 ) )
  #define RELEASE_VERSION_NUMBER                  11
  #define SOFTWARE_VERSION_DATE              "5/27/02"            // 6AF9
  #define SOFTWARE_VERSION_TIME              "21:41"
  #define SOFTWARE_VERSION_NUMBER            "1.3.00"
  #define PRODUCTION_FLAG                          1
#endif

/******************************************/
/******************************************/

#define ATMEL_FLAG                                1

/******************************************/
/******************************************/

#if ( ( PRODUCTION_FLAG != 0 ) && ( DEBUG_FLAG != 0 ) ) 
  whoops
#endif

/******************************************/
/******************************************/

#define FLASH_SOFTWARE_VERSION_DATE               "8/21/01"
#define FLASH_SOFTWARE_VERSION_TIME               "11:55 AM"
#define FLASH_SOFTWARE_VERSION_NUMBER             "1.0.51"

/******************************************/
/******************************************/

#define XBYTE(x) ((unsigned char xdata volatile *) ( (x) ) [0] )

#define CPU_BOARD_FLAG                             0

#define FACTORY_RESET_CODE                      0x5A

#if ( ATMEL_FLAG == 0 ) 
  #define NUMBER_OF_EEPROM_BYTES                 256
#endif
#if ( ATMEL_FLAG != 0 ) 
  #define NUMBER_OF_EEPROM_BYTES                2048
#endif

/******************************************/

#define NUMBER_OF_KEYBOARD_COLUMNS   4
#define NUMBER_OF_KEYBOARD_ROWS      6

#define NUMBER_OF_SWITCHES           9

#define NUMBER_OF_LCD_COLUMNS       16

#define TOKEN_DELAY_AMOUNT         125

/******************************************/

#if   defined ( __C51__ )

  #define XDATA_SPACE       xdata
  #define IDATA_SPACE       idata
  #define DATA_SPACE        data
  #define ROM_SPACE         code

  #define BIT_TYPE          bit

#endif

/******************************************/

#if ! defined ( __C51__ )

  #define XDATA_SPACE               
  #define IDATA_SPACE
  #define DATA_SPACE
  #define ROM_SPACE

  #define BIT_TYPE    BOOL        // Keyword for Ernie for a BOOL or a bit (bit is C51 keyword, BOOL is Windows keyword)
 
  #define bit         BOOL        // Keyword for a bit  of data in Bit Addressable Memory 
  #define sbit        BOOL        // Keyword for a bit  of data in Bit Addressable Memory within a byte
  #define bdata                   // Keyword for a byte of data in Bit Addressable Memory

  #define read_lcd_control_register             0 

#endif

/******************************************/

#define LONG_INFINITY 0x7FFFFFFFL

#define MAX_ULONG     0xFFFFFFFFL

/***********************************/

// Following hex digits have two zeros and two ones:  3, 5, 6, 9, A, C, 

#define RESUME_EXECUTION_ON_NEXT_RESET                  0x55   
#define HANDLE_ERROR_ON_NEXT_RESET                      0x59
#define PROGRAM_HIGH_FLASH_ON_NEXT_RESET                0x6A
#define JMP_TO_ISP_FLASH_PROGRAM_ON_NEXT_RESET          0x99
#define FACTORY_RESET_RESET                             0xA3
#define RESUME_EXECUTION_ON_NEXT_RESET_AFTER_POWERDOWN  0xAA   
#define PROGRAM_LOW_FLASH_ON_NEXT_RESET                 0xCC

/****************************************************/
/****************************************************/

// 37.5 timer interrupts per second 

#define CRYSTAL_FREQ                  ( 3.6864 )  // MHz
#define CLOCK_TICS_PER_SECOND         ( CRYSTAL_FREQ * MILLION / 12.0 )
#define SECONDS_PER_CLOCK_TIC         ( 1.0 / CLOCK_TICS_PER_SECOND )

#define HUNDRED_HOURS_MSEC            (long) ( 3600L * 100L * 1000L )     // 3.6 * 10 ^ 8
#define HUNDRED_HOURS_TICS            (long) ( 3600L * 100L * 1024L )
#define TWENTY_FOUR_HOURS_MSEC        (long) ( 3600L *  24L * 1000L )     // 3.6 * 10 ^ 8
#define TWENTY_FOUR_HOURS_TICS        (long) ( 3600L *  24L * 1024L )

/************************************/

#define  SERIAL_TX_FIFO_OVERFLOW_ERROR          1
#define  SERIAL_RX_FIFO_OVERFLOW_ERROR          2
#define  RECORD_FIFO_OVERFLOW_ERROR             3
#define  INTERNAL_IRAM_ERROR                    4
#define  INTERNAL_XRAM_ERROR                    5
#define  EXTERNAL_XRAM_ERROR                    6
#define  LCD_DRIVER_NOT_RESPONDING_ERROR        7
#define  EEPROM_ERROR                           8
#define  EEPROM_NOT_RESPONDING_ERROR            9
#define  STACK_OVERFLOW_ERROR                  10
#define  UNKNOWN_RESET_BYTE_ERROR              11
#define  NO_FACTORY_RESET_CODE_ERROR           12
#define  FLASH_PROGRAM_ERROR                   13
#define  FLASH_CHECKSUM_ERROR                  14
#define  NEW_BATTERY_INSTALLED_ERROR           15
#define  DEBOUNCE_FIFO_OVERFLOW_ERROR          16
#define  PENDING_RECORD_FIFO_OVERFLOW_ERROR    17
#define  KEYBOARD_FIFO_OVERFLOW_ERROR          18
#define  CONTACT_VENDOR_ERROR                  19
#define  TIMER_500_HOUR_LIMIT_EXCEEDED_ERROR   20
#define  DUMMY_ERROR                           21

/************************************************************/
/************************************************************/

#define BKSP_KEY    'B' 
#define CTRL_KEY    '/' 
#define ENTER_KEY   '\n'
#define SPACE_KEY   ' '
#define ZERO_KEY    '0'
#define OK_KEY      ENTER_KEY
#define YES_KEY     SPACE_KEY
#define NO_KEY      '0'

#define CARRIAGE_RETURN_BYTE  '\r'
#define LINE_FEED_BYTE        '\n'

/************************************************************/

#if defined ( __C51__ )

  #define get_int_lsb(x)      ( * ( ( ( unsigned char * ) ( &x ) ) + 1 ) )
  #define get_int_msb(x)      ( * ( ( ( unsigned char * ) ( &x ) ) + 0 ) )

  #define get_long_msb(x)     ( * ( ( ( unsigned char * ) ( &x ) ) + 0 ) )
  #define get_long_lsb(x)     ( * ( ( ( unsigned char * ) ( &x ) ) + 3 ) )

#endif

/************************************************************/

#if ! defined ( __C51__ )

  #define get_int_lsb(x)      ( * ( ( ( unsigned char * ) ( &x ) ) + 0 ) )
  #define get_int_msb(x)      ( * ( ( ( unsigned char * ) ( &x ) ) + 1 ) )

  #define get_long_lsb(x)     ( * ( ( ( unsigned char * ) ( &x ) ) + 0 ) )
  #define get_long_msb(x)     ( * ( ( ( unsigned char * ) ( &x ) ) + 3 ) )

#endif

/************************************************************/

#define key_f1_on_flag      ( key_on_byte [ 0 ] & 0x02 )   
#define key_f2_on_flag      ( key_on_byte [ 1 ] & 0x02 )   
#define key_f3_on_flag      ( key_on_byte [ 2 ] & 0x02 )   
#define key_f4_on_flag      ( key_on_byte [ 3 ] & 0x02 )   

#define bksp_key_on_flag    ( key_on_byte [ 0 ] & 0x04 )   
#define control_key_on_flag ( key_on_byte [ 1 ] & 0x04 )   
#define star_key_on_flag    ( key_on_byte [ 2 ] & 0x04 )   
#define minus_key_on_flag   ( key_on_byte [ 3 ] & 0x04 )   

#define key_7_on_flag       ( key_on_byte [ 0 ] & 0x08 )   
#define key_8_on_flag       ( key_on_byte [ 1 ] & 0x08 )   
#define key_9_on_flag       ( key_on_byte [ 2 ] & 0x08 )   
#define key_x_on_flag       ( key_on_byte [ 3 ] & 0x08 )   // Space key

#define key_4_on_flag       ( key_on_byte [ 0 ] & 0x10 )   
#define key_5_on_flag       ( key_on_byte [ 1 ] & 0x10 )   
#define key_6_on_flag       ( key_on_byte [ 2 ] & 0x10 )   
#define space_key_on_flag   ( key_on_byte [ 3 ] & 0x10 )   

#define key_1_on_flag       ( key_on_byte [ 0 ] & 0x20 )   
#define key_2_on_flag       ( key_on_byte [ 1 ] & 0x20 )   
#define key_3_on_flag       ( key_on_byte [ 2 ] & 0x20 )   
#define key_y_on_flag       ( key_on_byte [ 3 ] & 0x20 )   // Enter key

#define key_0_on_flag       ( key_on_byte [ 0 ] & 0x40 )   
#define key_z_on_flag       ( key_on_byte [ 1 ] & 0x40 )   // Zero Key
#define decimal_key_on_flag ( key_on_byte [ 2 ] & 0x40 )   
#define enter_key_on_flag   ( key_on_byte [ 3 ] & 0x40 )   

/************************************************************/
/************************************************************/

#define XON   17  // Control Q
#define XOFF  19  // Control S
#define ENQ    5
#define ACK    6
#define NAK   21

/**************************************************************/

#define LAST_SWITCH_ROW              2
#define LAST_BIB_ROW                 3
#define CURRENT_ENTRY_ROW            4

#define LAST_LAST_BIB_ROW            2
#define LAST_LAST_LAST_BIB_ROW       1


#define FIRST_COL_FOR_TIME_DISPLAY   7

/************************************************************/

#if ( 0 ) 

 Block    Address Range   Block Size        Command


 Block 0: 0x0000-0x1FFF       8k            0x0000  
 Block 1: 0x2000-0x3FFF       8k            0x2000  
 Block 2: 0x4000-0x7FFF      16k            0x4000  
 Block 3: 0x8000-0xBFFF      16k            0x6000  
 Block 4: 0xC000-0xFFFF      16k            0x8000  

#endif

/************************************************************/

#define FLASH_API_READ_MANUFACTURER_BYTE        0
#define FLASH_API_READ_DEVICE_ID1_BYTE          0
#define FLASH_API_READ_DEVICE_ID2_BYTE          0
#define FLASH_API_READ_DEVICE_ID3_BYTE          0

#define FLASH_API_ERASE_BLOCK                   1
#define FLASH_API_PROGRAM_BYTE                  2
#define FLASH_API_READ_DATA_BYTE                3
#define FLASH_API_ERASE_BOOT_VECTOR             4
#define FLASH_API_PROGRAM_SOFTWARE_SECURITY_BIT 5
#define FLASH_API_PROGRAM_BOOT_STATUS_BYTE      6
#define FLASH_API_PROGRAM_BOOT_VECTOR           6
#define FLASH_API_READ_BOOT_VECTOR              7
#define FLASH_API_READ_BOOT_STATUS_BYTE         7
#define FLASH_API_READ_SOFTWARE_SECURITY_BITS   7
#define FLASH_API_READ_HARDWARE_SECURITY_BITS   7

#define FLASH_API_READ_BOOTLOADER_VERSION       8

#define FLASH_API_PROGRAM_PAGE                  9

#define FLASH_LAUNCH_PHILIPS_ISP_PROGRAM       30

/************************************************************/

#define IO_CRYSTAL_FREQ                ( 3.6864 )

/************************************************************/

#define MSEC_PER_DEBOUNCE_TIC                   4

/************************************************************/

#define FLOW_CONTROL_OK_FLAG   ( flow_control_bit & last_flow_control_bit & last_last_flow_control_bit ) 

/************************************************************/

#define uchar unsigned char
#define uint  unsigned int

/************************************************************/
/************************************************************/

// Posting is 0x80 bit,  F4 is 0x40 bit

#define SWITCH_RECORD_TYPE          0x00                    /* 's' */
#define BIB_RECORD_TYPE             0x10                    /* 'b' */
#define SYNCH_RECORD_TYPE           0x20                    /* 'y' */
#define TEXT_RECORD_TYPE            0x30                    /* 't' */
#define EVENT_RECORD_TYPE           0x50                    /* 'e' */

#define POSTING_BIT_MASK            0x40
#define F4_BIT_MASK                 0x80

/************************************************************/
/************************************************************/


/**********************************************************/

#if ( 0 ) 

  Notes:  
  
    All XDATA variables that are used while waiting for records should be in Internal External RAM

    At the present time,  we will use 89C51RD CPUs.  These have 1 k internal RAM.

    As a result,  we will state apriori, that the Interrupt routine will not diddle the RAM enable bit.

#endif

/**********************************************************/

/* EXTERNAL.H  File */ 

/**********************************************************/
/**********************************************************/

 #ifdef  MAIN_MODULE        
   #define EXTERN /**/
 #endif                     
 #ifndef MAIN_MODULE        
   #define EXTERN extern
 #endif

 #ifdef  REGISTER_MODULE
   #define REG_EXTERN /**/
 #endif                     
 #ifndef REGISTER_MODULE
   #define REG_EXTERN extern
 #endif

 #ifdef  STACK_DATA_VARIABLES_MODULE
   #define STACK_DATA_EXTERN /**/
 #endif                     
 #ifndef STACK_DATA_VARIABLES_MODULE
   #define STACK_DATA_EXTERN extern
 #endif

 #ifdef  NON_VOLATILE_MODULE
   #define NON_VOL_EXTERN /**/
 #endif                     
 #ifndef NON_VOLATILE_MODULE
   #define NON_VOL_EXTERN extern
 #endif

 #ifdef  INTERNAL_XRAM_VARIABLES_MODULE
   #define INTERNAL_XRAM_EXTERN /**/
 #endif                     
 #ifndef INTERNAL_XRAM_VARIABLES_MODULE
   #define INTERNAL_XRAM_EXTERN extern
 #endif

 #ifdef  EXTERNAL_XRAM_VARIABLES_MODULE
   #define EXTERNAL_XRAM_EXTERN /**/
 #endif                     
 #ifndef EXTERNAL_XRAM_VARIABLES_MODULE
   #define EXTERNAL_XRAM_EXTERN extern
 #endif

/************************************************/

#define STARTING_STACK_ADDRESS             0x00B4   // STARTING_STACK_ADDRESS should be the first byte of 
                                                    // Gap from linker xx.M51 file.  8031 Stack grows up.  
                                                    // 8031 increments SP before PUSH.  Therefore,
                                                    // we assign SP = ( STARTING_STACK_ADDRESS - 1 ),
                                                    // so first byte of stack is stored in STARTING_STACK_ADDRESS

#define ENDING_STACK_ADDRESS               0x00FC   // This is the last byte of the stack  0xFC - 0xB4 = 72 (decimal)
                                                    // Most used is 19-20 bytes
                                                    // STACK_TOLERANCE of ( 0xFC - 0xC7 - 1 )  = 52   Caused a failure

#if ( PRODUCTION_FLAG == 0 ) 
  #define STACK_TOLERANCE                  ( 4 + 16 )
#endif
#if ( PRODUCTION_FLAG != 0 ) 
  #define STACK_TOLERANCE                  ( 4 )
#endif

#define STARTING_NON_VOLATILE_ADDRESS      0x00FD           
#define ENDING_NON_VOLATILE_ADDRESS        0x00FF

#define STARTING_INT_XRAM_ADDRESS          0x0000
#define ENDING_INT_XRAM_ADDRESS            0x03FF

#define STARTING_EXT_XRAM_ADDRESS          0x00400L
#define ENDING_EXT_XRAM_ADDRESS            0x1FFFFL
                     
#define ENDING_EPROM_ADDRESS             ( 0x8000 - 1 ) 

/************************************************/
/************************************************/

// The following fifos are in internal-external RAM

#if ( PRODUCTION_FLAG == 0 ) 
  #define DEBOUNCE_FIFO_MAX_LENGTH         50  // Internal External RAM - This cannot be bigger than 255,because we want char index
#endif
#if ( PRODUCTION_FLAG != 0 ) 
  #define DEBOUNCE_FIFO_MAX_LENGTH        250  // Internal External RAM - This cannot be bigger than 255,because we want char index
#endif

#define SCRATCH_STRING_LENGTH               50  // Internal External RAM
#define SERIAL_TX_FIFO_MAX_LENGTH          100  // Internal External RAM
#define SERIAL_RX_FIFO_MAX_LENGTH           66  // Internal External RAM

#if    defined ( __C51__ )
  #define SERIAL_RX_PACKET_STRING_LENGTH      32  // Internal External RAM
#endif
#if  ! defined ( __C51__ )
  #define SERIAL_RX_PACKET_STRING_LENGTH     200  // Internal External RAM
#endif

#define AUX_SERIAL_TX_FIFO_MAX_LENGTH       20  // Internal External RAM
#define AUX_SERIAL_RX_FIFO_MAX_LENGTH       20  // Internal External RAM
#define AUX_SERIAL_RX_PACKET_STRING_LENGTH  20  // Internal External RAM

// The following fifos are in external-external RAM

#define KEYBOARD_FIFO_MAX_LENGTH            32  // External RAM
#define PENDING_RECORD_FIFO_MAX_LENGTH      64  // External RAM
#define SAVED_RECORD_FIFO_MAX_LENGTH     10000  // External RAM ( Paged Memory )

/************************************************/
/************************************************/                                       
/************************************************/
/************************************************/

/* The following variables are located in EEPROM */                                  

/* The following are bytes */                                  

#define ptr_to_eeprom_factory_reset_code                  0
#define ptr_to_eeprom_configure0_byte                     1
#define ptr_to_eeprom_timer_resolution                    2
#define ptr_to_eeprom_printer_crlf_lines                  3
#define ptr_to_eeprom_configure1_byte                     4
#define ptr_to_eeprom_max_bib_string_length               5
#define ptr_to_eeprom_switch0_xor_mask                    6
#define ptr_to_eeprom_token_delay_amount                  7

/* The following are ints */                                  

#define ptr_to_eeprom_rom_checksum                       10   
#define ptr_to_eeprom_baud_rate                          12  
#define ptr_to_eeprom_this_timer_device_number           14
#define ptr_to_eeprom_printer_crlf_delay                 16

/* The following are strings */

#define ptr_to_eeprom_string1                           100
#define ptr_to_eeprom_string2                           200
#define ptr_to_eeprom_string3                           300
#define ptr_to_eeprom_string4                           400

#define ptr_to_eeprom_serial_number1                   2040
#define ptr_to_eeprom_serial_number0                   2044   

/*********************************************************************************/
/*********************************************************************************/

/* The following variables are located in bit addressable memory */

#if ( AUX_SERIAL_081902_STUFF_FLAG != 0 ) 

  EXTERN bit aux_serial_rx_fifo_overflow_flag             ;
  EXTERN bit aux_serial_transmission_in_progress_flag     ;

#endif

#if ( ( NEW_AUX_SERIAL_082103_STUFF_FLAG != 0 ) || ( NEWEST_STUFF_FLAG != 0 ) ) 
  EXTERN bit dummy_bit                                    ;
#endif

EXTERN bit i_am_talking_to_pc_bit                         ;
EXTERN bit i_am_talking_to_printer_bit                    ;
EXTERN bit i_got_the_token_bit                            ;
EXTERN bit i_got_something_to_say_bit                     ;
EXTERN bit i_need_to_ack_bit                              ;
EXTERN bit i_need_to_give_version_bit                     ;
EXTERN bit i_want_to_tell_time_bit                        ;
EXTERN bit i_got_a_low_battery_bit                        ;

EXTERN bit distal_device_is_talking_to_pc_flag            ;
EXTERN bit need_to_enable_or_disable_distal_device_flag   ;
EXTERN bit posting_bib_bit                                ;
EXTERN bit printing_stopped_due_to_flow_control_flag      ;
EXTERN bit flow_control_bit                               ;
EXTERN bit last_flow_control_bit                          ;
EXTERN bit last_last_flow_control_bit                     ;

EXTERN bit ignore_keyboard_timing_flag                    ;
EXTERN bit we_had_a_low_battery_warning_flag              ;
EXTERN bit need_refresh_flag                              ;
EXTERN bit display_latch_info_flag                        ;
EXTERN bit need_to_synch_up_flag                          ;
EXTERN bit accecpt_serial_input_flag                      ;
EXTERN bit beeper_flag                                    ;
EXTERN bit printer_crlf_delay_flag                        ;
EXTERN bit timer_interrupt_enabled_flag                   ;
EXTERN bit update_running_time_flag                       ;
EXTERN bit inputs_blocked_flag                            ;
EXTERN bit tenths_only_resolution_flag                    ;

EXTERN bit serial_transmission_in_progress_flag           ;
     
EXTERN bit serial_rx_fifo_overflow_flag                   ;
EXTERN bit serial_tx_fifo_overflow_flag                   ;
EXTERN bit keyboard_fifo_overflow_flag                    ;
EXTERN bit display_fifo_overflow_flag                     ;
EXTERN bit record_fifo_overflow_flag                      ;
EXTERN bit flash_burn_routine_flag                        ;

EXTERN bit old_pending_records_bit                        ;
EXTERN bit switch_on_bit                                  ;
EXTERN bit last_switch_image_bit                          ;
EXTERN bit anns_display_mode_flag                         ;
EXTERN bit blow_off_software_reset_flag                   ;
EXTERN bit kill_print_job_flag                            ;
EXTERN bit colon_flag                                     ;

/*********************************************************************************/
/*********************************************************************************/

/* The following bits are stored in a byte of bit addressable memory */

EXTERN bdata unsigned char xc_non_volatile_status_bits   ;
EXTERN bdata unsigned char xc_configure0_byte            ;
EXTERN bdata unsigned char xc_configure1_byte            ;
EXTERN bdata unsigned char communications_latch_byte     ;

#ifdef  MAIN_MODULE

#if defined ( __C51__ )

// Default 0x1E

EXTERN sbit keep_power_on_bit               = communications_latch_byte ^ 0 ; // Default 0 - Passes TXD to distal   RS232 transmitter
EXTERN sbit proximal_422_not_used_bit       = communications_latch_byte ^ 1 ; // Default 1 - Disables      proximal RS422 receiver (active low)
EXTERN sbit proximal_232_used_bit           = communications_latch_byte ^ 2 ; // Default 1 - Passes from   proximal RS232 receiver to RXD
EXTERN sbit me_talk_to_pc_latch_bit         = communications_latch_byte ^ 3 ; // Default 1 - Passes TXD to proximal RS232 transmitter, also turns off passing TXD to distal RS232 (for printer)

EXTERN sbit distal_not_talk_to_pc_bit       = communications_latch_byte ^ 4 ; // Default 1 - Disables      distal   RS422 receiver (active low) onto proximal RS232 transmitter
EXTERN sbit proximal_422_talk_bit           = communications_latch_byte ^ 5 ; // Default 0 - Enables       proximal RS422 transmitter
EXTERN sbit distal_232_rx_pass_bit          = communications_latch_byte ^ 6 ; // Default 0 - Passes from   distal   RS232 receiver to RXD
EXTERN sbit distal_422_used_bit             = communications_latch_byte ^ 7 ; // Default 0 - Enables       distal   RS422 transmitter               


EXTERN sbit printer_used_flag               = xc_configure0_byte ^ 0 ;
EXTERN sbit rs422_repeater_flag             = xc_configure0_byte ^ 1 ;
EXTERN sbit proximal_rs422_flag             = xc_configure0_byte ^ 2 ;
EXTERN sbit only_device_flag                = xc_configure0_byte ^ 3 ;

EXTERN sbit hundredths_resolution_flag      = xc_configure0_byte ^ 4 ;
EXTERN sbit msec_resolution_flag            = xc_configure0_byte ^ 5 ;
EXTERN sbit clock_24_hour_bit               = xc_configure0_byte ^ 6 ;
EXTERN sbit swap_first_two_cols_flag        = xc_configure0_byte ^ 7 ;        // For Membrane Switch, vs silicon rubber

EXTERN sbit use_hardware_flow_control_flag  = xc_configure1_byte ^ 0 ;
EXTERN sbit full_length_bib_entry_flag      = xc_configure1_byte ^ 1 ;
EXTERN sbit switch1_xor_mask                = xc_configure1_byte ^ 2 ;
EXTERN sbit proximal_wireless_flag          = xc_configure1_byte ^ 3 ;
EXTERN sbit radio_on_flag                   = xc_configure1_byte ^ 4 ;
EXTERN sbit wireless_flag                   = xc_configure1_byte ^ 5 ;


/* Many of the following could be volatile if desired (see mask above) */

EXTERN sbit xc_non_volatile_status_bit0     = xc_non_volatile_status_bits ^ 0 ;
EXTERN sbit xc_non_volatile_status_bit1     = xc_non_volatile_status_bits ^ 1 ;
EXTERN sbit xc_non_volatile_status_bit2     = xc_non_volatile_status_bits ^ 2 ;
EXTERN sbit xc_non_volatile_status_bit3     = xc_non_volatile_status_bits ^ 3 ;
EXTERN sbit xc_non_volatile_status_bit4     = xc_non_volatile_status_bits ^ 4 ;
EXTERN sbit xc_non_volatile_status_bit5     = xc_non_volatile_status_bits ^ 5 ;
EXTERN sbit xc_non_volatile_status_bit6     = xc_non_volatile_status_bits ^ 6 ;
EXTERN sbit xc_non_volatile_status_bit7     = xc_non_volatile_status_bits ^ 7 ;

#endif

#if ! defined ( __C51__ )

EXTERN sbit keep_power_on_bit         ;
EXTERN sbit proximal_422_not_used_bit      ;
EXTERN sbit proximal_232_used_bit          ;
EXTERN sbit me_talk_to_pc_latch_bit        ;

EXTERN sbit distal_not_talk_to_pc_bit      ;
EXTERN sbit proximal_422_talk_bit          ;
EXTERN sbit distal_232_rx_pass_bit         ;
EXTERN sbit distal_422_used_bit            ;
EXTERN sbit only_device_flag               ;

EXTERN sbit hundredths_resolution_flag     ;
EXTERN sbit msec_resolution_flag           ;
EXTERN sbit swap_first_two_cols_flag       ;
EXTERN sbit clock_24_hour_bit              ;

EXTERN sbit use_hardware_flow_control_flag ;
EXTERN sbit full_length_bib_entry_flag     ;

EXTERN sbit switch1_xor_mask               ;
EXTERN sbit proximal_wireless_flag         ;
EXTERN sbit radio_on_flag                  ;
EXTERN sbit wireless_flag                  ;

#endif

#endif

/***************/

// Following is necessary for bits in bytes in bit addressable memory

#ifndef MAIN_MODULE        

extern bit keep_power_on_bit     ;
extern bit proximal_422_not_used_bit  ;
extern bit proximal_232_used_bit      ;
extern bit me_talk_to_pc_latch_bit    ;

extern bit distal_not_talk_to_pc_bit  ;
extern bit proximal_422_talk_bit      ;
extern bit distal_232_rx_pass_bit     ;
extern bit distal_422_used_bit        ;

extern bit printer_used_flag          ;
extern bit rs422_repeater_flag        ;
extern bit proximal_rs422_flag        ;
extern bit only_device_flag           ;

extern bit hundredths_resolution_flag ;
extern bit msec_resolution_flag       ;
extern bit clock_24_hour_bit          ;

extern bit swap_first_two_cols_flag        ;
extern bit use_hardware_flow_control_flag  ;
extern bit full_length_bib_entry_flag      ;
extern bit switch1_xor_mask                ;
extern bit proximal_wireless_flag          ;
extern bit radio_on_flag                   ;
extern bit wireless_flag                   ;

#endif

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

// Here are the fifo indexes

/*****************************************************************************/
/*****************************************************************************/

// The following indexes are stored in Register memory for super fast access (eventually)

#if ( AUX_SERIAL_081902_STUFF_FLAG == 0 ) 

REG_EXTERN DATA_SPACE unsigned char debounce_fifo_read_index     ;
REG_EXTERN DATA_SPACE unsigned char debounce_fifo_write_index    ;

REG_EXTERN DATA_SPACE unsigned char serial_rx_fifo_read_index    ;
REG_EXTERN DATA_SPACE unsigned char serial_rx_fifo_write_index   ;

REG_EXTERN DATA_SPACE unsigned char old_pending_records ;
REG_EXTERN DATA_SPACE unsigned char column_counter      ;
REG_EXTERN DATA_SPACE unsigned char beeper_counter      ;
REG_EXTERN DATA_SPACE unsigned char deepest_stack_entry ;

#endif

#if ( AUX_SERIAL_081902_STUFF_FLAG != 0 ) 

EXTERN IDATA_SPACE unsigned char debounce_fifo_read_index     ;
EXTERN IDATA_SPACE unsigned char debounce_fifo_write_index    ;

EXTERN IDATA_SPACE unsigned char serial_rx_fifo_read_index    ;
EXTERN IDATA_SPACE unsigned char serial_rx_fifo_write_index   ;

EXTERN IDATA_SPACE unsigned char old_pending_records ;
EXTERN IDATA_SPACE unsigned char column_counter      ;
EXTERN IDATA_SPACE unsigned char beeper_counter      ;

REG_EXTERN DATA_SPACE unsigned char deepest_stack_entry                 ;

#if ( NEW_AUX_SERIAL_082103_STUFF_FLAG == 0 ) 
  EXTERN IDATA_SPACE unsigned char timer_counts ;
#endif

#endif

/*****************************************************************************/
/*****************************************************************************/

#if ! defined ( __C51__ )

EXTERN int crap ;  // This is used 

#endif

/*****************************************************************************/
/*****************************************************************************/

// The following indexes are stored in internal RAM memory for fast access

EXTERN IDATA_SPACE          char keyboard_buffer_index               ;

EXTERN IDATA_SPACE unsigned char serial_tx_fifo_read_index           ;
EXTERN IDATA_SPACE unsigned char serial_tx_fifo_write_index          ;

#if ( AUX_SERIAL_081902_STUFF_FLAG != 0 ) 

  EXTERN XDATA_SPACE unsigned char aux_serial_rx_fifo_read_index       ;
  EXTERN XDATA_SPACE unsigned char aux_serial_rx_fifo_write_index      ;

#endif

#if ( NEW_061603_STUFF_FLAG != 0 )
  EXTERN XDATA_SPACE unsigned char token_delay_counter ;
#endif

EXTERN IDATA_SPACE unsigned char keyboard_fifo_read_index            ;
EXTERN IDATA_SPACE unsigned char keyboard_fifo_write_index           ;

EXTERN IDATA_SPACE unsigned char pending_record_fifo_read_index      ;
EXTERN IDATA_SPACE unsigned char pending_record_fifo_write_index     ;

EXTERN IDATA_SPACE unsigned int  tx_saved_record_fifo_read_index        ;
EXTERN IDATA_SPACE unsigned int  display_saved_record_fifo_read_index   ;
EXTERN IDATA_SPACE unsigned int  printer_saved_record_fifo_read_index   ;
EXTERN IDATA_SPACE unsigned int  saved_record_fifo_write_index          ;

/*****************************************************************************/
/*****************************************************************************/

// The following variables are also stored in internal RAM memory for fast access

#if ( NEW_102402_STUFF_FLAG != 0 )
  EXTERN IDATA_SPACE unsigned char switch0_xor_mask                    ;
#endif

EXTERN IDATA_SPACE   unsigned int  this_timer_device_number            ;
EXTERN IDATA_SPACE   unsigned int  device_with_token                   ;
EXTERN IDATA_SPACE   unsigned int  record_requested_by_pc              ;

EXTERN IDATA_SPACE   unsigned int  second_counter                      ;
EXTERN IDATA_SPACE   long          long_running_time_tics              ;

#if ( AUX_SERIAL_081902_STUFF_FLAG == 0 ) 
  EXTERN IDATA_SPACE   long          synch_offset_tics                   ;    // Nominally a positive value equal to T.O.D when timer was powered up
#endif
#if ( AUX_SERIAL_081902_STUFF_FLAG != 0 ) 
  EXTERN XDATA_SPACE   long          synch_offset_tics                   ;    // Nominally a positive value equal to T.O.D when timer was powered up
#endif

EXTERN IDATA_SPACE   unsigned char user_channel_number                 ;

EXTERN IDATA_SPACE   unsigned char lcd_row                             ;
EXTERN IDATA_SPACE   unsigned char lcd_col                             ;

/********************************************************************/

EXTERN IDATA_SPACE unsigned char switch_on_byte          ;
EXTERN IDATA_SPACE unsigned char last_switch_image_byte  ;

EXTERN IDATA_SPACE unsigned char key_on_byte            [  NUMBER_OF_KEYBOARD_COLUMNS ] ;
EXTERN IDATA_SPACE unsigned char last_key_image_byte    [  NUMBER_OF_KEYBOARD_COLUMNS ] ;

EXTERN IDATA_SPACE unsigned char strobe_address_array   [ 4 ] ;

/******************************************************************/
                                                     
STACK_DATA_EXTERN  IDATA_SPACE char stack_data [ ENDING_STACK_ADDRESS - STARTING_STACK_ADDRESS + 1 ] ;

/******************************************************************/

/* The following are non-volatile Variables, Stored in high memory (ie above the Stack ) */
/* At powerup, the variables are not set to zero */

NON_VOL_EXTERN IDATA_SPACE unsigned char debug_uchar       ;
NON_VOL_EXTERN IDATA_SPACE unsigned char error_byte_code   ;

// The following syntax is necessary, only because the linker must find at least one byte of IRAM for the Stack

#define reset_byte                 ( ( ( unsigned char IDATA_SPACE * ) ( 0xFF ) ) [ 0 ] )

/******************************************************************/

// Internal External XRAM is used for variables that are diddled frequently without user input (eg debounce fifos), or 
//          variables that are diddled in many places in code (eg sprintf ( scratch_string, .... ), in order to mininmize the 
//          number of places in code that we are required to turn on and off External XRAM

EXTERN XDATA_SPACE               unsigned int   this_event_number                                       ;
EXTERN XDATA_SPACE               unsigned char  this_heat_number                                        ;
EXTERN XDATA_SPACE               long           long_new_synch_up_time_tics                             ;
EXTERN XDATA_SPACE               long           long_old_synch_offset_tics                              ;
EXTERN XDATA_SPACE               unsigned int   printer_crlf_delay                                      ;
EXTERN XDATA_SPACE               unsigned char  printer_crlf_lines                                      ;
EXTERN XDATA_SPACE               unsigned int   printer_crlf_delay_counter                              ;
EXTERN XDATA_SPACE               unsigned char  printer_crlf_counter                                    ;
EXTERN XDATA_SPACE               unsigned char  timer_resolution                                        ;
EXTERN XDATA_SPACE               unsigned int   calculated_rom_checksum                             ;
EXTERN XDATA_SPACE               unsigned int   baud_rate                                               ;
EXTERN XDATA_SPACE               unsigned int   actual_saved_record_fifo_max_length                     ;
EXTERN XDATA_SPACE               unsigned char  max_bib_string_length                                   ;
EXTERN XDATA_SPACE               unsigned char  low_battery_counter                                     ;
EXTERN XDATA_SPACE               unsigned long  serial_number0                                          ;
EXTERN XDATA_SPACE               unsigned long  serial_number1                                          ;


EXTERN XDATA_SPACE               unsigned char  debug_value                                             ;

EXTERN XDATA_SPACE               char           scratch_string               [ SCRATCH_STRING_LENGTH ] ;

EXTERN XDATA_SPACE               unsigned char  serial_tx_fifo               [ SERIAL_TX_FIFO_MAX_LENGTH ] ;

EXTERN XDATA_SPACE               unsigned char  serial_rx_fifo               [ SERIAL_RX_FIFO_MAX_LENGTH ] ;

EXTERN XDATA_SPACE               unsigned char  serial_rx_packet_string      [ SERIAL_RX_PACKET_STRING_LENGTH ] ;

#if ( NEW_102402_STUFF_FLAG != 0 )
  EXTERN XDATA_SPACE             unsigned int   calculated_64k_rom_checksum                                 ;
#endif

#if ( NEW_061603_STUFF_FLAG != 0 )
  EXTERN XDATA_SPACE             unsigned char  token_delay_amount ;
#endif

#if ( AUX_SERIAL_081902_STUFF_FLAG != 0 ) 

EXTERN XDATA_SPACE               unsigned char aux_serial_tx_fifo_read_index       ;
EXTERN XDATA_SPACE               unsigned char aux_serial_tx_fifo_write_index      ;

EXTERN XDATA_SPACE               unsigned char  aux_serial_tx_fifo           [ AUX_SERIAL_TX_FIFO_MAX_LENGTH ] ;

EXTERN XDATA_SPACE               unsigned char  aux_serial_rx_fifo           [ AUX_SERIAL_RX_FIFO_MAX_LENGTH ] ;

EXTERN XDATA_SPACE               unsigned char  aux_serial_rx_packet_string  [ AUX_SERIAL_RX_PACKET_STRING_LENGTH ] ;

#endif


EXTERN XDATA_SPACE               unsigned char  pending_index_array          [ NUMBER_OF_SWITCHES + 1 ] ;
EXTERN XDATA_SPACE               unsigned char  pending_time_array           [ NUMBER_OF_SWITCHES + 1 ] ;

EXTERN XDATA_SPACE               unsigned int   total_switch_hits_array      [ NUMBER_OF_SWITCHES + 1 ] ;
EXTERN XDATA_SPACE               unsigned int   total_bib_hits_array         [ NUMBER_OF_SWITCHES + 1 ] ;

EXTERN XDATA_SPACE               unsigned char  keyboard_debounce_fifo       [ DEBOUNCE_FIFO_MAX_LENGTH ] ;

INTERNAL_XRAM_EXTERN XDATA_SPACE unsigned char  aa_switch_debounce_fifo      [ 256 ] ;  // This allows 256 byte buffer for EPROM burning

/*******************************************************************/

// External External XRAM is used for variables that are only diddled as a result of an record or user action, and where 
//          there are only a few places in code where it is necessary to turn on/off External XRAM chip
//          In general, only arrays are located in External External XRAM

// Warning:  Must explicitly initialize all of these variables!!!!!!!! (if necessary)

EXTERNAL_XRAM_EXTERN XDATA_SPACE long          saved_record_time_fifo         [ SAVED_RECORD_FIFO_MAX_LENGTH   ] ; 
EXTERNAL_XRAM_EXTERN XDATA_SPACE unsigned char saved_record_channel_fifo      [ SAVED_RECORD_FIFO_MAX_LENGTH   ] ;  // Also contains type
EXTERNAL_XRAM_EXTERN XDATA_SPACE unsigned char saved_record_event_number_fifo [ SAVED_RECORD_FIFO_MAX_LENGTH   ] ; 
EXTERNAL_XRAM_EXTERN XDATA_SPACE unsigned char saved_record_heat_number_fifo  [ 4000 ] ; 

EXTERNAL_XRAM_EXTERN XDATA_SPACE          char keyboard_buffer                [ NUMBER_OF_LCD_COLUMNS + 1      ] ;  // + 1 for '\0' at end of string

EXTERNAL_XRAM_EXTERN XDATA_SPACE          char keyboard_fifo                  [ KEYBOARD_FIFO_MAX_LENGTH       ] ;  // For records that have been debounced, but not handled in foreground

EXTERNAL_XRAM_EXTERN XDATA_SPACE long          pending_record_time_fifo       [ PENDING_RECORD_FIFO_MAX_LENGTH ] ;  // For records that have been debounced, but not handled in foreground
EXTERNAL_XRAM_EXTERN XDATA_SPACE unsigned char pending_record_channel_fifo    [ PENDING_RECORD_FIFO_MAX_LENGTH ] ;  // For records that have been debounced, but not handled in foreground

EXTERNAL_XRAM_EXTERN XDATA_SPACE          char posting_bib_string             [ 20 ] ;
EXTERNAL_XRAM_EXTERN XDATA_SPACE          char last_switch_row_text           [ 20 ] ;
EXTERNAL_XRAM_EXTERN XDATA_SPACE          char last_bib_row_text              [ 20 ] ;
EXTERNAL_XRAM_EXTERN XDATA_SPACE          char last_last_bib_row_text         [ 20 ] ;
EXTERNAL_XRAM_EXTERN XDATA_SPACE          char last_last_last_bib_row_text    [ 20 ] ;
                                                                       
/*******************************************************************/
/*******************************************************************/
/*******************************************************************/

// Put ROM_SPACE arrays here that are referenced in two or more code modules

 #ifdef  MAIN_MODULE        

    EXTERN ROM_SPACE          char hex_digit_array    [] = { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' } ;
    EXTERN ROM_SPACE unsigned char bit_mask_array     [] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 } ;
    EXTERN ROM_SPACE unsigned char  switch_name_array [] = { 2, 6, 3, 7, 4, 8, 1, 5, 9 } ; 

    EXTERN ROM_SPACE unsigned char reset_byte_array [] = {
        RESUME_EXECUTION_ON_NEXT_RESET           ,  /* 0x00 - All keys pressed     */
        RESUME_EXECUTION_ON_NEXT_RESET           ,  /* 0x01                        */
        RESUME_EXECUTION_ON_NEXT_RESET           ,  /* 0x02                        */
        FACTORY_RESET_RESET                      ,  /* 0x03 - F1 and F2            */
        RESUME_EXECUTION_ON_NEXT_RESET           ,  /* 0x04                        */
        PROGRAM_HIGH_FLASH_ON_NEXT_RESET         ,  /* 0x05 - F1 and F3            */
        JMP_TO_ISP_FLASH_PROGRAM_ON_NEXT_RESET   ,  /* 0x06 - F1 and F4            */
        RESUME_EXECUTION_ON_NEXT_RESET           ,  /* 0x07                        */
        RESUME_EXECUTION_ON_NEXT_RESET           ,  /* 0x08                        */
        PROGRAM_HIGH_FLASH_ON_NEXT_RESET         ,  /* 0x09 - F2 and F3            */
        JMP_TO_ISP_FLASH_PROGRAM_ON_NEXT_RESET   ,  /* 0x0A - F2 and F4            */
        RESUME_EXECUTION_ON_NEXT_RESET           ,  /* 0x0B                        */
        PROGRAM_LOW_FLASH_ON_NEXT_RESET          ,  /* 0x0C - F3 and F4            */
        RESUME_EXECUTION_ON_NEXT_RESET           ,  /* 0x0D                        */
        RESUME_EXECUTION_ON_NEXT_RESET           ,  /* 0x0E                        */
        RESUME_EXECUTION_ON_NEXT_RESET              /* 0x0F - No  keys pressed     */
    } ;

    #if ( AUX_SERIAL_081902_STUFF_FLAG == 0 )

//                                                   0    1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
      EXTERN ROM_SPACE char record_type_array [] = { 's', 'b','y','t','p','e',' ',' ','S',' ','Y',' ','P',' ',' ',' ',' ' } ;

    #endif

    #if ( AUX_SERIAL_081902_STUFF_FLAG != 0  )

//                                                   0    1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
      EXTERN ROM_SPACE char record_type_array [] = { 's', 'b','y','t','p','e',' ',' ','S','B','Y',' ','P',' ',' ',' ',' ' } ;

    #endif


    EXTERN ROM_SPACE char hyphen_text [] = { "---------------------------\r\n" } ;

 #endif                     

 #ifndef MAIN_MODULE        
   EXTERN ROM_SPACE          char hex_digit_array   [] ;
   EXTERN ROM_SPACE unsigned char bit_mask_array    [] ;
   EXTERN ROM_SPACE unsigned char switch_name_array [] ;
   EXTERN ROM_SPACE unsigned char reset_byte_array  [] ;
   EXTERN ROM_SPACE          char record_type_array [] ;
   EXTERN ROM_SPACE          char hyphen_text       [] ;
 #endif

/*******************************************************************/
/*******************************************************************/


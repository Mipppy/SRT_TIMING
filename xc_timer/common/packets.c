
/*********************************************************************/

#define PACKET_MODULE  0

#include <x_header.h>

#if ! defined ( __C51__ )
  #include <win_header.h>
#endif

/*********************************************************************/

#define RX_DEBUG_PRINT_TO_DEBUG_FILE 0
#define TX_DEBUG_PRINT_TO_DEBUG_FILE 0

#define MAX_NUMBER_OF_RX_BYTES_IN_TAG_PACKET 2000 

/*********************************************************************/

extern ROM_SPACE unsigned crctable [] ;

#if ! defined ( __C51__ )

  extern XC_CONFIGURE_STRUCTURE_TYPE        c ;
  extern GLOBAL_VARIABLE_STRUCTURE_TYPE     g ;

  extern long time_of_last_reception_array [] ;

  static long            device_record_number ;
  static unsigned char   rx_packet_buffer [ MAX_NUMBER_OF_RX_BYTES_IN_TAG_PACKET ] ;
  static          short  number_of_bytes_in_rx_packet ;
  extern char            tx_file_path_and_name [] ;
//void output_formatted_file_string ( char * file_path_and_name, char * format_string,... ) ; /* function declaration */
#endif

/*********************************************************************/
/*********************************************************************/

#if ( 0 ) 

Note:  This code module will also be used for PC

Beep text 

#endif

/*********************************************************************/

#define START_OF_PACKET_BYTE  '{'
#define END_OF_PACKET_BYTE    '}'
#define CARRIAGE_RETURN_BYTE  '\r'
#define LINE_FEED_BYTE        '\n'

enum {       
       WAITING_FOR_START_OF_PACKET_BYTE_STATE  = 20,
       WAITING_FOR_END_OF_PACKET_BYTE_STATE        ,
       WAITING_FOR_CRC0_BYTE_STATE                 , 
       WAITING_FOR_CRC1_BYTE_STATE                 , 
       WAITING_FOR_CRC2_BYTE_STATE                 , 
       WAITING_FOR_CRC3_BYTE_STATE                 , 
       WAITING_FOR_CR_BYTE_STATE                   , 
       WAITING_FOR_LF_BYTE_STATE              
} ;

/***********************************************************************/

static unsigned char   rx_packet_index ;
static unsigned int    calculated_rx_packet_crc ;

static unsigned char   rx_packet_state = WAITING_FOR_START_OF_PACKET_BYTE_STATE ;

/***********************************************************************/
/***********************************************************************/

// This routine, is called from foreground after a complete packet of data has already been received, and the CRC checks.
// Data is in serial_rx_packet_string,    rather than in serial rx fifo, to make parsing much easier so data does not wrap around

unsigned char process_rx_packet ( void ) /* function declaration */

{

  unsigned char * ptr_to_rx_packet ;

  ptr_to_rx_packet = & ( serial_rx_packet_string [ 1 ] ) ;

// Here, we accept token  "TK 0" "TK 1" or "TK 1 125"

  if ( ( ptr_to_rx_packet [ 0 ] == 'T' ) && ( ptr_to_rx_packet [ 1 ] == 'K' ) && ( ptr_to_rx_packet [ 2 ] == ' ' ) ) {    // Token

    #if defined ( __C51__ )

      char * ptr1 ;
      char * ptr2 ;

      BIT_TYPE record_requested_flag ;

      ptr1 = get_ptr_to_first_non_white ( ptr_to_rx_packet + 3 ) ;

      ptr2 = get_ptr_to_first_white ( ptr1 ) ;

      record_requested_flag = 0 ;

      if ( * ptr2 != '\0' ) {

        * ptr2 ++ = '\0' ;

        record_requested_flag = 1 ;

        record_requested_by_pc = atoi ( ptr2 ) ;

      endiff

      device_with_token = atoi ( ptr1 ) ;

/*****************************************************************************************************/
/*****************************************************************************************************/

      if ( this_timer_device_number == device_with_token ) {

        #if ( NEW_061603_STUFF_FLAG != 0 )
          token_delay_counter = TOKEN_DELAY_AMOUNT ;
        #endif

        i_got_the_token_bit        = 1 ;
        i_need_to_ack_bit          = 1 ;
        i_got_something_to_say_bit = 1 ;

        distal_device_is_talking_to_pc_flag = 0 ;  // This variable only matters for Devices which are a RS422 Repeater

        if ( record_requested_flag != 0 ) {

          if ( ( tx_saved_record_fifo_read_index + 1 ) > record_requested_by_pc ) {
            tx_saved_record_fifo_read_index = record_requested_by_pc - 1 ;
          endiff

        endiff

 
      endiff

      else {   

        i_got_the_token_bit = 0 ;

        if ( ( rs422_repeater_flag != 0 ) && ( proximal_rs422_flag != 0 ) ) {   // Following does not matter much, unless this is a RS422 repeater
 
          if ( device_with_token > this_timer_device_number ) {                 // Distal device gets the token
            if ( distal_device_is_talking_to_pc_flag == 0 ) {
              need_to_enable_or_disable_distal_device_flag = 1 ;
            endiff
            distal_device_is_talking_to_pc_flag = 1 ;                           // This variable only matters for Devices which are a RS422 Repeater
          endiff

          else {
            if ( device_with_token != 0 ) {
              distal_device_is_talking_to_pc_flag          = 0 ;                // I think that the intent of this is to keep repeater repeating, when device 0 has the token.
              need_to_enable_or_disable_distal_device_flag = 1 ;
            endiff
          endiff

        endiff

        else {
        endiff

      endiff

/*****************************************************************************************************/
/*****************************************************************************************************/

      #if ( 0 )  // The following was 12/8/01 version

        else if ( device_with_token > this_timer_device_number ) {
          i_got_the_token_bit = 0 ;
          if ( ( proximal_rs422_flag != 0 ) && ( rs422_repeater_flag != 0 ) ) {
            if ( distal_device_is_talking_to_pc_flag == 0 ) {
              need_to_enable_or_disable_distal_device_flag   = 1 ;
            endiff
          endiff
          distal_device_is_talking_to_pc_flag = 1 ;
        endiff

        else if ( ( proximal_rs422_flag != 0 ) && ( rs422_repeater_flag != 0 ) ) {
          i_got_the_token_bit = 0 ;
          if ( device_with_token != 0 ) {
            distal_device_is_talking_to_pc_flag = 0 ;   // Can this line be moved ???? I think that the intent is to keep repeater repeating, when device 0 has the token.
            need_to_enable_or_disable_distal_device_flag   = 1 ;
          endiff
        endiff

        else {
          i_got_the_token_bit = 0 ;
          distal_device_is_talking_to_pc_flag = 0 ;
        endiff

      #endif

    #endif

  endiff

/*****************************************************************/

// Reset Packet {RS }  or {RS x} where latter blows off resets in the future    "RS"

  else if ( ( ptr_to_rx_packet [ 0 ] == 'R' ) && ( ptr_to_rx_packet [ 1 ] == 'S' ) && ( ptr_to_rx_packet [ 2 ] == ' ' ) ) {    // Reset

    #if defined ( __C51__ )

      if ( ptr_to_rx_packet [ 3 ] == 'x' ) { 
        blow_off_software_reset_flag = 1 ;
      endiff
      if ( blow_off_software_reset_flag == 0 ) {
        reset_byte = RESUME_EXECUTION_ON_NEXT_RESET ;
        exit_routine () ;
      endiff
 
    #endif

  endiff

/*****************************************************************/

// Configure Packet {CFG }  or {CFG x} 

#if ( NEW_102402_STUFF_FLAG != 0 ) 

  else if ( ( ptr_to_rx_packet [ 0 ] == 'C' ) && ( ptr_to_rx_packet [ 1 ] == 'F' ) && ( ptr_to_rx_packet [ 2 ] == 'G' ) ) {    // CFG

    #if defined ( __C51__ )

      if ( ptr_to_rx_packet [ 3 ] == '0' ) {               // Default Configuration
        burn_default_configuration () ;
      endiff

      if ( ptr_to_rx_packet [ 3 ] == 'x' ) {               // Toggle Configuration
        if ( this_timer_device_number == 1 ) {
          this_timer_device_number = 2 ;
          rs422_repeater_flag      = 0 ;
          proximal_rs422_flag      = 1 ;
        endiff
        else {
          this_timer_device_number = 1 ;
          rs422_repeater_flag      = 1 ;
          proximal_rs422_flag      = 0 ;
        endiff
      endiff

      write_configuration_variables () ;
      read_configuration_variables  () ;

      i_am_talking_to_pc_bit      = 0 ;
      i_am_talking_to_printer_bit = 0 ;
      set_up_communications_latch () ;

    #endif

  endiff

#endif

/*****************************************************************/

// Program Flash {FL l}

  else if ( ( ptr_to_rx_packet [ 0 ] == 'F' ) && ( ptr_to_rx_packet [ 1 ] == 'L' ) && ( ptr_to_rx_packet [ 2 ] == ' ' ) ) {    // Flash
    #if defined ( __C51__ )

      char * ptr1 ;

      if ( blow_off_software_reset_flag == 0 ) {

        ptr1 = get_ptr_to_first_non_white ( ptr_to_rx_packet + 3 ) ;

        if  ( * ptr1 == 'h' ) { 
          reset_byte = PROGRAM_HIGH_FLASH_ON_NEXT_RESET  ;
          exit_routine () ;
        endiff

        else  if  ( * ptr1 == 'i' ) {       // ISP
          turn_off_interrupts () ;

          #if ( NEW_102402_STUFF_FLAG != 0 ) 

            #if ( NEW_061603_STUFF_FLAG != 0 ) 

              scr_clear () ;

              rom_scr_printf ( (char ROM_SPACE *) "Atmel\r\nProgramming Routine\r\nHere" ) ;

            #endif

            CS_LATCH_PORT = DEFAULT_COMMUNICATION_LATCH_BITS ;
          #endif
          jmp_to_isp_flash_program_FC00 () ;
        endiff

        else  {
          turn_off_interrupts () ;        
          jmp_to_flash_startup_0D00 () ;
        endiff

      endiff

    #endif
 
 endiff

/*****************************************************************/

// Here, we get Event Number and Heat Number "EV 0 26 20"
//       Where 0 is the timer device #, 26 is the event, and 20 is the heat

  else if ( ( ptr_to_rx_packet [ 0 ] == 'E' ) && ( ptr_to_rx_packet [ 1 ] == 'V' ) & ( ptr_to_rx_packet [ 2 ] == ' ' ) ) {    // Event

    #if defined ( __C51__ )

      char * ptr1 ;
      char * ptr2 ;
      char * ptr3 ;

      short tmp_device_number ;

      tmp_device_number = -1 ;

      strcpy ( scratch_string, ptr_to_rx_packet ) ;

      ptr1      = get_ptr_to_first_non_white ( ptr_to_rx_packet + 3 ) ;

      ptr2      = get_ptr_to_first_white     ( ptr1 ) ;
      * ptr2 ++ = '\0' ;
      ptr2      = get_ptr_to_first_non_white ( ptr2 ) ;

      ptr3      = get_ptr_to_first_white     ( ptr2 ) ;
      * ptr3 ++ = '\0' ;
      ptr3      = get_ptr_to_first_non_white ( ptr3 ) ;

      tmp_device_number = atoi ( ptr1 ) ;

      if ( ( tmp_device_number == 0 ) || ( tmp_device_number == this_timer_device_number ) ) {

        this_event_number = atoi ( ptr2 ) ;
        this_heat_number  = atoi ( ptr3 ) ;

        record_strcpy_to_xram  ( saved_record_fifo_write_index, scratch_string ) ;

        write_char_to_external_xram ( & ( saved_record_channel_fifo        [ saved_record_fifo_write_index ] ), EVENT_RECORD_TYPE ) ;

        if ( max_bib_string_length == 16 ) {
          write_char_to_external_xram ( & ( saved_record_event_number_fifo [ saved_record_fifo_write_index ] ), this_event_number ) ;
          write_char_to_external_xram ( & ( saved_record_heat_number_fifo  [ saved_record_fifo_write_index ] ), this_heat_number  ) ;
        endiff
        else {
          write_char_to_external_xram ( & ( saved_record_event_number_fifo [ saved_record_fifo_write_index ] ), ( this_event_number & 0x0F ) | ( ( this_heat_number & 0x0F ) << 4 ) ) ;
        endiff

        write_long_to_external_xram   ( & ( saved_record_time_fifo           [ saved_record_fifo_write_index ] ), long_running_time_tics ) ;

        save_this_record () ;

        gotoxy ( 1,                         LAST_BIB_ROW ) ;
        scr_printf ( ptr_to_rx_packet [ 3 ] ) ;
        gotoxy ( keyboard_buffer_index + 1, CURRENT_ENTRY_ROW ) ;

      endiff

    #endif

  endiff

/*****************************************************************/

// Here, we get synched: "{SY 0:0:0.0}"   

  else if ( ( ptr_to_rx_packet [ 0 ] == 'S' ) && ( ptr_to_rx_packet [ 1 ] == 'Y' ) ) {    // Synch

    #if defined ( __C51__ )

      if ( ptr_to_rx_packet [ 2 ] == ' ' ) {

        long_old_synch_offset_tics = synch_offset_tics ;

        synch_offset_tics  = get_msec_from_time_string ( & ( ptr_to_rx_packet [ 3 ] ) , 1 ) ;   // Note: gets tics
        synch_offset_tics -= long_running_time_tics ;                                           // Nominally a positive value equal to TOD when timer started up

        second_counter           = 0 ;

        update_running_time_flag = 1 ;

        i_got_something_to_say_bit = 1 ;
        i_want_to_tell_time_bit    = 1 ;

      endiff

      else if  ( ptr_to_rx_packet [ 2 ] == 'O' ) {
 
        long_old_synch_offset_tics = synch_offset_tics ;

        synch_offset_tics += get_msec_from_time_string ( & ( ptr_to_rx_packet [ 3 ] ) , 1 ) ;   // Note: gets tics

        second_counter = 0 ;

        update_running_time_flag = 1 ;

      endiff

    #endif

  endiff

/*****************************************************************/

// Here we handle ACK packet {AK 1}

  else if ( ( ptr_to_rx_packet [ 0 ] == 'A' ) && ( ptr_to_rx_packet [ 1 ] == 'K' ) && ( ptr_to_rx_packet [ 2 ] == ' ' ) ) { 

    #if ! defined ( __C51__ )

      int ack_device_number ;

      ack_device_number = atoi ( & ( ptr_to_rx_packet [ 3 ] ) ) ;

      time_of_last_reception_array [ ack_device_number ] = GetTickCount () ;

    #endif

  endiff

/**************************************/

// Here is where PC receives data packet

  else if ( ( ptr_to_rx_packet [ 0 ] < 'A' ) || ( ptr_to_rx_packet [ 0 ] >  'Z' ) ) {    // First character is not letter (A-Z), so must be data packet

    #if ! defined ( __C51__ )

      unsigned char  device_number         ;
      unsigned short record_number         ;
      unsigned short event_number          ;
      unsigned short heat_number           ;
      unsigned char  channel_number        ;
      unsigned char  old_channel_number    ;
      signed   short index                 ;

      char * ptr_to_tab            ;
      char * ptr_to_space          ;
      char * ptr_to_device_number  ;
      char * ptr_to_record_number  ;
      char * ptr_to_event_number   ;
      char * ptr_to_heat_number    ;
      char * ptr_to_record_type    ;
      char * ptr_to_channel_number ;
      char * ptr_to_user_string    ;
      char * ptr_to_time_string    ;
                            
      char time_string    [ 100 ] ;
      char scratch_string [ 100 ] ;

      BOOL done_flag = 0 ;

      XC_TIMER_RECORD_STRUCTURE_TYPE local_timing_structure ;

      ptr_to_device_number = get_ptr_to_first_non_white ( ptr_to_rx_packet ) ;
     
      ptr_to_tab = return_ptr_to_first_occurance ( ptr_to_device_number, '\t' ) ;

      * ptr_to_tab ++ = '\0' ;


      ptr_to_record_number = ptr_to_tab ;

      ptr_to_tab = return_ptr_to_first_occurance ( ptr_to_tab, '\t' ) ;

      * ptr_to_tab ++ = '\0' ;


      ptr_to_event_number = ptr_to_tab ;

      ptr_to_tab = return_ptr_to_first_occurance ( ptr_to_tab, '\t' ) ;

      * ptr_to_tab ++ = '\0' ;


      ptr_to_heat_number = ptr_to_tab ;

      ptr_to_tab = return_ptr_to_first_occurance ( ptr_to_tab, '\t' ) ;

      * ptr_to_tab ++ = '\0' ;


      ptr_to_channel_number = ptr_to_tab ;

      ptr_to_tab = return_ptr_to_first_occurance ( ptr_to_tab, '\t' ) ;

      * ptr_to_tab ++ = '\0' ;


      ptr_to_record_type = ptr_to_tab ;

      ptr_to_tab = return_ptr_to_first_occurance ( ptr_to_tab, '\t' ) ;

      * ptr_to_tab ++ = '\0' ;


      ptr_to_user_string = ptr_to_tab ;

      ptr_to_tab = return_ptr_to_first_occurance ( ptr_to_tab, '\t' ) ;

      * ptr_to_tab ++ = '\0' ;


      ptr_to_time_string = ptr_to_tab ;

/***************/

      device_number = atoi ( ptr_to_device_number ) ;

      record_number = atoi ( ptr_to_record_number ) ;

      event_number  = atoi ( ptr_to_event_number ) ;

      heat_number    = atoi ( ptr_to_heat_number    ) ;

      channel_number = atoi ( ptr_to_channel_number ) ;

      strcpy ( time_string, ptr_to_time_string ) ;

/***************/

// Here we parse the user string fields

      for ( index = 0 ; index < MAX_NUMBER_OF_USER_STRING_FIELDS ; index ++ ) {
        strcpy ( local_timing_structure . userxfield [ index ], "" ) ;
      next

      old_channel_number = 0 ;

      strcpy ( local_timing_structure . userstring, ptr_to_user_string ) ;

      index     = -1 ;
      done_flag =  0 ;

      while ( done_flag == 0 ) {

        ptr_to_space = return_ptr_to_first_occurance ( ptr_to_user_string, ' ' ) ;

        if ( ptr_to_space == NULL ) {
          done_flag = 1 ;
        endif

        else {
          * ptr_to_space ++ = '\0' ;
        endif

        if ( index < 0 ) {
          strcpy ( local_timing_structure . bib_string,            ptr_to_user_string ) ;
        endif
        else {
          strcpy ( local_timing_structure . userxfield [ index ],  ptr_to_user_string ) ;
        endif

        ptr_to_user_string = ptr_to_space ;

        if ( ++ index == MAX_NUMBER_OF_USER_STRING_FIELDS ) done_flag = 1 ;

      wend

/******************************/

      _strtime ( scratch_string ) ;

      local_timing_structure          . table_id               = 0                      ;
      local_timing_structure          . device_num             = device_number          ;
      local_timing_structure          . record_num             = record_number          ;
      local_timing_structure          . event_num              = event_number           ;
      local_timing_structure          . heat_num               = heat_number            ;

      local_timing_structure          . channel                = channel_number         ;

      strcpy ( local_timing_structure . record_typ,            ptr_to_record_type   ) ;

      strcpy ( local_timing_structure . timer_time,            time_string          ) ;

      strcpy ( local_timing_structure . pc_time,               scratch_string       ) ;

      strcpy ( local_timing_structure . notes,                 " "                  ) ;

      if ( record_number == ( 1 + c . highest_record_received_array [ device_number ] ) ) {    // only increment if we have every single record!!!!!

        dll_put_timer_structure_into_fifo ( & local_timing_structure ) ;      // Fixed ???? 7/7/01

        c . highest_record_received_array [ device_number ] = record_number ;

      endif

      time_of_last_reception_array [ (int) device_number ] = GetTickCount () ;

    #endif
 
  endiff


// Here is where PC receives a txt packet   {TX Hello World}

  else if ( ( ptr_to_rx_packet [ 0 ] == 'T' ) && ( ptr_to_rx_packet [ 1 ] == 'X' ) && ( ptr_to_rx_packet [ 2 ] == 'X' ) ) {    // Reset

    #if ! defined ( __C51__ )
      output_formatted_file_string ( g . log_file_path_and_name, "%s\n", ptr_to_rx_packet ) ;
    #endif

  endiff

// Here is where PC receives a message packet   {MSG 1 Hello World}

  else if ( ( ptr_to_rx_packet [ 0 ] == 'M' ) && ( ptr_to_rx_packet [ 1 ] == 'S' ) && ( ptr_to_rx_packet [ 2 ] == 'G' ) && ( ptr_to_rx_packet [ 3 ] == ' ' ) ) {    // Reset

    #if ! defined ( __C51__ )
      output_formatted_file_string ( g . log_file_path_and_name, "%s\n", ptr_to_rx_packet ) ;
    #endif
    #if   defined ( __C51__ )
      scr_clear () ;
      scr_printf ( ptr_to_rx_packet [ 4 ] ) ;
      beep_beep_beep () ;
      hit_ok_key ( NULL ) ;
      need_refresh_flag = 1 ;
    #endif
  endiff


  else {
    #if ! defined ( __C51__ )
      output_formatted_file_string ( g . log_file_path_and_name, "%s\n", ptr_to_rx_packet ) ;
    #endif
  endiff

  return 0 ;

}

/**************************************************************************/
/***********************************************************************/

// This routine, called from foreground,  processes one byte of incoming data, 
//      This routine returns:
//                             0 when a valid data packet is not available 
//                             2 when a valid data packet is     available 

unsigned char timer_process_rx_byte ( char rx_byte ) /* function declaration */

{

  unsigned char return_flag = 0 ;
  unsigned char new_rx_packet_state ;
 
// Protocol: "{ ( ASCII Data ) } CRC16 CR LF

  new_rx_packet_state = rx_packet_state + 1 ;   // May as well anticipate success

  if ( ( ( rx_byte == START_OF_PACKET_BYTE ) && ( rx_packet_state != WAITING_FOR_START_OF_PACKET_BYTE_STATE ) ) ||
       ( ( rx_byte == END_OF_PACKET_BYTE   ) && ( rx_packet_state != WAITING_FOR_END_OF_PACKET_BYTE_STATE   ) ) ||
       ( ( rx_byte == CARRIAGE_RETURN_BYTE ) && ( rx_packet_state != WAITING_FOR_CR_BYTE_STATE              ) ) ||
       ( ( rx_byte == LINE_FEED_BYTE       ) && ( rx_packet_state != WAITING_FOR_LF_BYTE_STATE              ) ) ) {

    rx_packet_state = WAITING_FOR_START_OF_PACKET_BYTE_STATE ;
    i_got_the_token_bit = 0 ;

  endiff

  if ( rx_packet_state == WAITING_FOR_START_OF_PACKET_BYTE_STATE ) {
    calculated_rx_packet_crc     = 0 ;
    rx_packet_index              = 0 ;
    if ( rx_byte != START_OF_PACKET_BYTE ) {
      new_rx_packet_state = WAITING_FOR_START_OF_PACKET_BYTE_STATE ;
    endiff
    else {
      i_got_the_token_bit = 0 ;
    endiff
  endiff

// Here is where we get the data

  else if ( rx_packet_state == WAITING_FOR_END_OF_PACKET_BYTE_STATE ) {
    if ( rx_byte == END_OF_PACKET_BYTE ) {
    endiff
    else if ( ( ( rx_byte & 0x80 ) != 0 ) || ( ( rx_byte < ' ' ) && ( rx_byte != '\t' ) ) ) {
      new_rx_packet_state = WAITING_FOR_START_OF_PACKET_BYTE_STATE ;     // Non Printing ASCII Character
      i_got_the_token_bit       = 0 ;
    endiff
    else {
      new_rx_packet_state = WAITING_FOR_END_OF_PACKET_BYTE_STATE ;
      calculated_rx_packet_crc = ( crctable [ ( rx_byte ^ calculated_rx_packet_crc ) & 0xff ] ^ ( calculated_rx_packet_crc >> 8 ) ) ;
      if ( rx_packet_index > ( SERIAL_RX_PACKET_STRING_LENGTH - 12 ) ) {   // We do this here to minimize calculations  // Why 12 here ???????????
        new_rx_packet_state = WAITING_FOR_START_OF_PACKET_BYTE_STATE ;
      endiff
    endiff
  endiff

  else if ( rx_packet_state == WAITING_FOR_CR_BYTE_STATE ) {
    if ( rx_byte != CARRIAGE_RETURN_BYTE ) {
      new_rx_packet_state = WAITING_FOR_START_OF_PACKET_BYTE_STATE ;
    endiff
    else {
      i_got_the_token_bit = 0 ;
    endiff
  endiff

// Here we must check received CRC

  else if ( rx_packet_state == WAITING_FOR_LF_BYTE_STATE ) {
    new_rx_packet_state = WAITING_FOR_START_OF_PACKET_BYTE_STATE ;                 // Regardless of good or bad packe

    if ( rx_byte == LINE_FEED_BYTE ) {                                             // Looks like a good packet if CRC checks out

      serial_rx_packet_string [ rx_packet_index - 1 ] = '\0' ;                     // for CRC string

      if ( calculated_rx_packet_crc == hex_atoi ( & ( serial_rx_packet_string [ rx_packet_index - 5 ] ) ) ) {  // ????

        serial_rx_packet_string [ rx_packet_index - 6 ] = '\0' ;

        return_flag = 2 ;

      endiff

      else {
        i_got_the_token_bit = 0 ;
      endiff

    endiff

    else {
      i_got_the_token_bit = 0 ;
    endiff

  endiff

  serial_rx_packet_string [ rx_packet_index ++ ] = rx_byte ;

  rx_packet_state = new_rx_packet_state ;

  return return_flag ;

}

/***********************************************************************/
/**************************************************************************/

void transmit_generic_packet ( char * ptr_to_string_buffer ) /* function declaration */

{

  unsigned int  packet_crc ;
  unsigned char byte ;

  #if ! defined ( __C51__ )
    if ( ( c . diagnostic_flags & 0x01 ) != 0 ) {
      output_formatted_file_string ( g.tx_file_path_and_name, "{%s}\r\n", ptr_to_string_buffer ) ;
    endiff
  #endif

  packet_crc = 0 ;

  serial_putc ( START_OF_PACKET_BYTE ) ;

  while ( ( byte = * ptr_to_string_buffer ++ ) != '\0' ) {
    serial_putc ( byte ) ;
    packet_crc = ( crctable [ ( byte ^ packet_crc ) & 0xff ] ^ ( packet_crc >> 8 ) ) ;
  wend

  serial_putc ( END_OF_PACKET_BYTE ) ;

  byte = get_int_msb ( packet_crc ) ;

  serial_putc ( hex_digit_array [ ( ( byte >> 4 ) & 0x0f ) ] ) ;
  serial_putc ( hex_digit_array [ ( ( byte >> 0 ) & 0x0f ) ] ) ;

  byte = get_int_lsb ( packet_crc ) ;

  serial_putc ( hex_digit_array [ ( byte >> 4 ) & 0x0f ] ) ;
  serial_putc ( hex_digit_array [ ( byte >> 0 ) & 0x0f ] ) ;

  serial_putc ( CARRIAGE_RETURN_BYTE ) ;
  serial_putc ( LINE_FEED_BYTE ) ;

}

/****************************************************************************/

// Here we format packet for printer and for transmission

#if defined ( __C51__ )

void transmit_record_packet ( BIT_TYPE printer_bit, unsigned int fifo_index ) /* function declaration */

{

  unsigned char * ptr_to_scratch_string ;
  unsigned char * ptr_to_end_of_scratch_string ;
  long            event_time ;
  unsigned char   tmp_record_type_and_channel ;
  unsigned char   event_number ;
  unsigned char   heat_number  ;
  unsigned char   tab_char ;

  if ( printer_bit == 0 ) {
    tab_char = '\t' ;
  endiff
  else {
    tab_char = ' ' ;
  endiff

  ptr_to_scratch_string        = scratch_string ;

// First, we write this_timer_device_number

  ptr_to_end_of_scratch_string = scratch_string + 35 ;  

  quick_int_printf ( ptr_to_end_of_scratch_string, this_timer_device_number ) ; 

  while ( ( * ptr_to_scratch_string ++ = * ptr_to_end_of_scratch_string ++ ) != '\0' ) ;

  ptr_to_scratch_string -- ;

  * ptr_to_scratch_string ++ = tab_char ;

// Next, we add record number

  ptr_to_end_of_scratch_string = scratch_string + 35 ;  

  quick_int_printf ( ptr_to_end_of_scratch_string, fifo_index + 1 ) ;       // Record number

  if ( printer_bit != 0 ) {
    if ( fifo_index < 99 ) {
      * ptr_to_scratch_string ++ = ' ' ;
      if ( fifo_index < 9 ) {
        * ptr_to_scratch_string ++ = ' ' ;
      endiff
    endiff
  endiff

  while ( ( * ptr_to_scratch_string ++ = * ptr_to_end_of_scratch_string ++ ) != '\0' ) ;

  ptr_to_scratch_string -- ;

  * ptr_to_scratch_string ++ = tab_char ;

// Next, we add event number

  if ( max_bib_string_length == 16 ) {
    event_number = read_char_from_external_xram ( & ( saved_record_event_number_fifo    [ fifo_index ] ) ) ; // Fifo r2
    heat_number  = read_char_from_external_xram ( & ( saved_record_heat_number_fifo     [ fifo_index ] ) ) ; // Fifo r2
  endiff
  else {
    event_number = read_char_from_external_xram ( & ( saved_record_event_number_fifo    [ fifo_index ] ) ) ; // Fifo r2
    heat_number  =   event_number & 0x0F ;
    event_number = ( event_number >> 4 ) & 0x0F ;
  endiff

  if ( ( printer_bit == 0 ) || ( event_number != 0 ) ) {

    ptr_to_end_of_scratch_string = scratch_string + 35 ;  

    quick_int_printf ( ptr_to_end_of_scratch_string, event_number ) ;       // Event number

    while ( ( * ptr_to_scratch_string ++ = * ptr_to_end_of_scratch_string ++ ) != '\0' ) ;

    ptr_to_scratch_string -- ;

    * ptr_to_scratch_string ++ = tab_char ;

  endiff

  if ( ( printer_bit == 0 ) || ( heat_number != 0 ) ) {

    ptr_to_end_of_scratch_string = scratch_string + 35 ;  

    quick_int_printf ( ptr_to_end_of_scratch_string, heat_number ) ;       // Event number

    while ( ( * ptr_to_scratch_string ++ = * ptr_to_end_of_scratch_string ++ ) != '\0' ) ;

    ptr_to_scratch_string -- ;

    * ptr_to_scratch_string ++ = tab_char ;

  endiff

// Next, we add channel

  tmp_record_type_and_channel = read_char_from_external_xram ( & ( saved_record_channel_fifo [ fifo_index ] ) ) ; // Fifo r2

  * ptr_to_scratch_string ++ = '0' + ( tmp_record_type_and_channel & 0x0F ) ; 

  * ptr_to_scratch_string ++ = tab_char ;

// Next, we add record type

  * ptr_to_scratch_string ++ = record_type_array [ ( tmp_record_type_and_channel & 0xF0 ) >> 4  ] ;

  * ptr_to_scratch_string ++ = tab_char ;

// Next, we add total_user_string

  ptr_to_scratch_string = record_strcpy_from_xram ( ptr_to_scratch_string, fifo_index ) ;

  * ptr_to_scratch_string ++ = tab_char ;

// Finally, we add record time

  if ( printer_bit != 0 ) {

    unsigned char width ;

    if ( msec_resolution_flag != 0 ) {
      width = 15 ;
    endiff
    else if ( hundredths_resolution_flag != 0 ) {
      width = 16 ;
    endiff
    else {
      width = 17 ;
    endiff

    while ( ( ptr_to_scratch_string - scratch_string ) < width ) {
      * ptr_to_scratch_string ++ = tab_char ;
    wend
  endiff
  
  event_time = read_long_from_external_xram ( & ( saved_record_time_fifo   [ fifo_index ] ) ) ;  // Fifo r2

  get_time_string_from_msec ( ( synch_offset_tics  +  event_time ), ptr_to_scratch_string, 1 ) ;

}

#endif

/*********************************************************************/

/* The following routine calculates a CRC for a single bytes */

#if ( 0 ) 

unsigned int calculate_crc_for_byte ( unsigned char byte, unsigned int initial_crc )  /* function declaration */

{

  return ( crctable [ ( byte ^ initial_crc ) & 0xff ] ^ ( initial_crc >> 8 ) ) ;

}

#endif

/***********************/

ROM_SPACE unsigned crctable [ 256 ] = {

    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
    };

/****************************************************************/
/****************************************************************/

#if ! defined ( __C51__ )

/***********************************************************************/
/***********************************************************************/
 
/*************************************************************/
/*************************************************************/
/*************************************************************/

// This routine, calledp from foreground,  processes one byte of incoming data, 
//      This routine returns:
//                             0 when no data       is available 
//                             1 when a data byte   is available 
//                             2 when a data packet is available 

unsigned char process_tag_rx_data ( char character ) /* function declaration */

{

  short rx_byte ;

  unsigned char return_flag ;

  return_flag = 0 ;

  rx_byte = character ;

  if ( rx_byte == '\r' ) {
  endif

  else if ( rx_byte < 32 ) {
    return_flag = 1 ;            // Must keep reading buffer
    rx_byte = -1 ;
  endif

  else if ( rx_byte > 127 ) {
    return_flag = 1 ;            // Must keep reading buffer
    rx_byte = 0xFF ;
  endif

  if ( rx_byte != -1 ) {

    return_flag = 1 ;

// Here is where we get the data

    if ( rx_byte == 0xFF ) {

      number_of_bytes_in_rx_packet = 0 ;
      rx_packet_buffer [ number_of_bytes_in_rx_packet ++ ] = '*' ;

    endif

    else if ( rx_byte == '\r' ) {    // Note Tag sends 0x0D which is \r which is Carriage Return at end of lines 

      rx_packet_buffer [ number_of_bytes_in_rx_packet ] = '\0' ;

      number_of_bytes_in_rx_packet = 0 ;

      return_flag = 2 ;

    endif

    else if ( rx_byte == '\n' ) {
    endif
    else if ( rx_byte == '\t' ) {
    endif
    else if ( rx_byte == ' '  ) {
    endif
    else if ( rx_byte == '\0' ) {
    endif

    else {
      if ( ( number_of_bytes_in_rx_packet >= 0 ) &&
           ( number_of_bytes_in_rx_packet < ( MAX_NUMBER_OF_RX_BYTES_IN_TAG_PACKET - 2 ) ) ) {
        rx_packet_buffer [ number_of_bytes_in_rx_packet ++ ] = (unsigned char) rx_byte ;
      endif
      else {
      endif
    endif

  endif

  return return_flag ;

}

/*******************************************************************/
/*******************************************************************/

// The following are miscellaneous DLL Functions

/**************************************************************************/
/**************************************************************************/

#if ! defined ( LUNG_PROJECT ) 
  #define pause(x) delay ( x * 55 )
#endif

/**********************************************************************/
    
void handle_tag_505_rx_packet ( void ) /* function declaration */

{

  short  channel_number ;
  char * bib_pointer ;
  char * ptr_to_rx_packet_buffer ;
  char   time_string           [ 30 ] ;
  char   bib_string            [ 30 ] ;
  char   local_scratch_string  [ 30 ] ;
  int    packet_length ;
  int    index ;

  XC_TIMER_RECORD_STRUCTURE_TYPE local_timing_structure ;

// Note: rx_packet_buffer white space has already been removed

// 0xFF  Bib  Input   Hour Minute Second . Msec  \r

//  *    111    1      23    59     59     999   \r  Note: \r is 0x0D

//  0    123    4      56    78     90     123   \r


// * 153 2 00 57 49 600
// * 166 2 00 58 55 671
// * 000 2 00 59 02 182
// * 198 2 01 00 53 069
// * 242 2 01 05 45 574
// * 000 2 01 11 16 172

//  2    166         00:58:55.7
//  2    000         00:59:02.2
//  2    198         00:06:53.1
//  2    242         00:11:45.6
//  2    000         00:17:16.2


  ptr_to_rx_packet_buffer = rx_packet_buffer ;

//output_formatted_file_string ( raw_file_name, "%s\n", ptr_to_rx_packet_buffer ) ;

  packet_length = strlen ( ptr_to_rx_packet_buffer ) ;

  if ( packet_length >= 13  ) {

    ptr_to_rx_packet_buffer = ptr_to_rx_packet_buffer + packet_length - 14 ;  // Here we reference beginning of text to the end of the packet in case we did not receive the first character

    strncpy ( bib_string, ptr_to_rx_packet_buffer + 1, 3 ) ;
    bib_string [ 3 ] = '\0' ;
    bib_pointer = bib_string ;

// Here we get rid of leading zeros

    if ( * bib_pointer == '0' ) {
      bib_pointer ++ ;          
      if ( * bib_pointer == '0' ) {
        bib_pointer ++ ;
      endif
    endif

    channel_number = * ( ptr_to_rx_packet_buffer + 4 ) - '0' ;

    time_string [ 0 ] = '\0' ;

    strncat ( time_string, ptr_to_rx_packet_buffer +  5,   2 ) ;
    strcat  ( time_string, ":" ) ;
    strncat ( time_string, ptr_to_rx_packet_buffer +  7,   2 ) ;
    strcat  ( time_string, ":" ) ;
    strncat ( time_string, ptr_to_rx_packet_buffer +  9,   2 ) ;
    strcat  ( time_string, "." ) ;
    strcat  ( time_string, ptr_to_rx_packet_buffer + 11      ) ;

/**********************************************************************/

    device_record_number ++ ;

    _strtime ( local_scratch_string ) ;

    strcpy ( local_timing_structure . userstring, "" ) ;

    for ( index = 0 ; index < MAX_NUMBER_OF_USER_STRING_FIELDS ; index ++ ) {
      strcpy ( local_timing_structure . userxfield [ index ], "" ) ;
    next

    local_timing_structure          . table_id               = 0                      ;
    local_timing_structure          . device_num             = 0                      ;
    local_timing_structure          . record_num             = device_record_number   ;
    local_timing_structure          . event_num              = 1                      ;
    local_timing_structure          . heat_num               = 1                      ;
    local_timing_structure          . channel                = channel_number         ;

    strcpy ( local_timing_structure . record_typ,          "s"                  ) ;
    strcpy ( local_timing_structure . bib_string,          bib_pointer          ) ;
    strcpy ( local_timing_structure . timer_time,          time_string          ) ;
    strcpy ( local_timing_structure . pc_time,             local_scratch_string ) ;
    strcpy ( local_timing_structure . notes,               " "                  ) ;

    dll_put_timer_structure_into_fifo ( & local_timing_structure ) ;

  endif

}

/**********************************************************************/
    
void handle_tag_605_rx_packet ( void ) /* function declaration */

{

  short  channel_number ;
  char * ptr_to_rx_packet_buffer ;
  char   time_string           [ 30 ] ;
  char   bib_string            [ 30 ] ;
  char   channel_string        [ 30 ] ;
  char   record_type_string    [ 30 ] ;
  char   local_scratch_string  [ 30 ] ;
  int    packet_length ;
  int    index ;

  XC_TIMER_RECORD_STRUCTURE_TYPE local_timing_structure ;

#if ( 0 ) 

Ernie - TAG 605 data strings are 30 characters plus CR 

                   bytes  offset
Rec type            1       0 
Unit ID             4       1
      space         1       5
seq # (1-49,999)    5       6
      space         1      11
Channel #           2      12
      space         1      14
hours               2      15
separator :         1      17
Minutes             2      18
separator :         1      20
seconds             2      21
separator .         1      23
millionths          6      24
<CR>                1      30

Record Type 
     N    - New Session 
     S    - At Synchronization 
     T    - Timing 
            If manual activation on keypad for channels 1-4, 1st char in 
channel # is M 

      There are numerous commands from the PC that control the timer and 
display port. Send me a FAX # and I will send them out to you if you need 
them. 

#endif

  ptr_to_rx_packet_buffer = rx_packet_buffer ;

//output_formatted_file_string ( raw_file_name, "%s\n", ptr_to_rx_packet_buffer ) ;

  packet_length = strlen ( ptr_to_rx_packet_buffer ) ;

  if ( packet_length >= 30  ) {

    ptr_to_rx_packet_buffer = ptr_to_rx_packet_buffer + packet_length - 30 ;  // Here we reference beginning of text to the end of the packet

    strncpy ( bib_string, ptr_to_rx_packet_buffer + 6, 5 ) ;

    bib_string [ 5 ] = '\0' ;

    remove_leading_and_trailing_white ( bib_string ) ;

    strncpy ( record_type_string, ptr_to_rx_packet_buffer +  0, 1 ) ;
    record_type_string [ 1 ] = '\0' ;

    strncpy ( channel_string, ptr_to_rx_packet_buffer + 12, 2 ) ;
    channel_string [ 2 ] = '\0' ;

    if ( ( * channel_string < '0' ) || ( * channel_string > '9' ) ) {
      strncat ( record_type_string, channel_string, 1 ) ;
      * channel_string = ' ' ;
    endif
    channel_number = atoi ( channel_string ) ;

    strcpy  ( time_string, ptr_to_rx_packet_buffer +  15 ) ;
    if ( time_string [ 0 ] == '0' ) {
      time_string    [ 0 ] = ' ' ;
    endif
    remove_leading_and_trailing_white ( time_string ) ;

/**********************************************************************/

    device_record_number ++ ;

    _strtime ( local_scratch_string ) ;

    strcpy ( local_timing_structure . userstring, "" ) ;

    for ( index = 0 ; index < MAX_NUMBER_OF_USER_STRING_FIELDS ; index ++ ) {
      strcpy ( local_timing_structure . userxfield [ index ], "" ) ;
    next

    local_timing_structure          . table_id               = 0                      ;
    local_timing_structure          . device_num             = 0                      ;
    local_timing_structure          . record_num  = device_record_number              ;
    local_timing_structure          . event_num              = 1                      ;
    local_timing_structure          . heat_num               = 1                      ;
    local_timing_structure          . channel  = channel_number                       ;

    strcpy ( local_timing_structure . record_typ,         record_type_string    ) ;
    strcpy ( local_timing_structure . bib_string,                 bib_string    ) ;
    strcpy ( local_timing_structure . timer_time,          time_string          ) ;
    strcpy ( local_timing_structure . pc_time,             local_scratch_string ) ;
    strcpy ( local_timing_structure . notes,                " "                 ) ;

    dll_put_timer_structure_into_fifo ( & local_timing_structure ) ;

  endif

}

/**********************************************************************/
/**********************************************************************/

#endif

/**********************************************************************/
/**********************************************************************/



/*******************************************************/
                                           
#include <x_header.h>

#if ! defined ( __C51__ )
  #include <win_header.h>
#endif


/****************************************/

#define INPUT_ROW 2

/****************************************/


/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
/**********************************************************************/
 
BIT_TYPE kbhit ( void ) /* function declaration */

{

  if ( keyboard_fifo_read_index != keyboard_fifo_write_index ) {     // Fifo r1
    return 1 ;
  endiff
  else {
    return 0 ;
  endiff

}

/**********************************************************************/

// This routine WAITS till a key is hit, and returns with that key

char get_a_key ( void ) /* function declaration */

{

  BIT_TYPE tmp_ignore_keyboard_timing_flag ;

  tmp_ignore_keyboard_timing_flag = ignore_keyboard_timing_flag ;

  ignore_keyboard_timing_flag = 1 ;

  while ( kbhit () == 0 ) {
    debounce_switches () ;
  wend

  ignore_keyboard_timing_flag = tmp_ignore_keyboard_timing_flag ;

  return scr_getc () ;

}

/***************************************/

// This routine waits till OK key has been hit (ie Enter)

void hit_ok_key ( char ROM_SPACE * rom_string_ptr ) /* function declaration */

{

  BIT_TYPE tmp_ignore_keyboard_timing_flag ;

  tmp_ignore_keyboard_timing_flag = ignore_keyboard_timing_flag ;

  ignore_keyboard_timing_flag = 1 ;

  if ( rom_string_ptr != NULL ) {
    rom_scr_printf ( rom_string_ptr ) ;
  endiff

  gotoxy ( 1, 4 ) ;
  rom_scr_printf ( "(Hit Enter)" ) ;

  while ( get_a_key () != ENTER_KEY ) {
    debounce_switches () ;
  wend

  clear_keyboard_buffer () ;

  ignore_keyboard_timing_flag = tmp_ignore_keyboard_timing_flag ;

}

/***************************************/

// This routine waits till all keys have been released

BIT_TYPE any_key_is_pressed ( void ) /* function declaration */

{

  unsigned char index ;
  BIT_TYPE key_pressed ;
 
  do {

    debounce_switches () ;

    key_pressed = 0 ;

    for ( index = 0 ; index < 4 ; index ++ ) {
      if ( key_on_byte [ index ] != 0 ) {
        key_pressed = 1 ;
      endiff
    next

  } while ( key_pressed == 1 ) ;

  return 0 ;

}

/****************************************/
/****************************************/

// This is the newer version 

/* This function is called from foreground to transmit serial characters */

//      This function writes to serial_tx_fifo if it is unable to transmit
//      character immediately

// Must be very careful here!!!!

void serial_putc ( char character )    /* function declaration    */

{

  #if   defined ( __C51__ ) 

    unsigned char tmp_serial_tx_fifo_write_index ;

    turn_off_interrupts () ;

    if ( serial_transmission_in_progress_flag == 0 ) {
      if ( serial_tx_fifo_read_index == serial_tx_fifo_write_index ) {    
        SBUF = character ;
        serial_transmission_in_progress_flag = 1 ;
        turn_on_interrupts () ;
      endiff
      else {
        #if ( PRODUCTION_FLAG == 0 ) 
          error_byte_code = CONTACT_VENDOR_ERROR ;  // This error occurs if ever ( serial_transmission_in_progress_flag == 0 ) 
          handle_error () ;                         //                       AND there are still characters in the serial tx buffer
        #endif
        #if ( PRODUCTION_FLAG != 0 ) 
          TI = 0 ;
          SBUF = serial_tx_fifo [ serial_tx_fifo_read_index ] ;       // Fifo r2
          if ( ++ serial_tx_fifo_read_index >= SERIAL_TX_FIFO_MAX_LENGTH ) serial_tx_fifo_read_index = 0 ;   // Fifo r3
          turn_on_interrupts () ;
        #endif
      endiff
    endiff

    else {

      tmp_serial_tx_fifo_write_index = serial_tx_fifo_write_index ;

      serial_tx_fifo [ tmp_serial_tx_fifo_write_index ] = character ; // Fifo w1
      if ( ++ tmp_serial_tx_fifo_write_index >= SERIAL_TX_FIFO_MAX_LENGTH ) tmp_serial_tx_fifo_write_index = 0 ;   // Fifo w2

      if ( tmp_serial_tx_fifo_write_index != serial_tx_fifo_read_index ) {      // Fifo w3
        serial_tx_fifo_write_index = tmp_serial_tx_fifo_write_index ;
        turn_on_interrupts () ;
      endiff

// Here, we wait till char is transmitted, so we can stuff another char into buffer

      else {
        turn_on_interrupts () ;
        while ( tmp_serial_tx_fifo_write_index == serial_tx_fifo_read_index ) {      // Fifo w3
          if ( flash_burn_routine_flag != 0 ) {
            handle_serial_interrupt () ;
          endiff
        wend
        serial_tx_fifo_write_index = tmp_serial_tx_fifo_write_index ;
      endiff

      #if ( 0 )
        if (    serial_tx_fifo_write_index == serial_tx_fifo_read_index ) {      // Fifo w3
          error_byte_code = SERIAL_TX_FIFO_OVERFLOW_ERROR ;
          handle_error () ;
        endiff
      #endif

    endiff

  #endif

  #if ! defined ( __C51__ ) 
    comm_write_string ( & character, 1 ) ;
  #endif

}

/***************************************/

// Here we read from serial_rx_fifo

int serial_getc ( void )       /* function declaration    */

{

  unsigned int character ;

  turn_off_interrupts () ;

  if ( serial_rx_fifo_read_index != serial_rx_fifo_write_index ) {        // Fifo r1
    character = serial_rx_fifo [ serial_rx_fifo_read_index ] ;            // Fifo r2
    if ( ++ serial_rx_fifo_read_index >= SERIAL_RX_FIFO_MAX_LENGTH ) serial_rx_fifo_read_index = 0 ;   // Fifo r3
  endiff
  else {
    character = -1 ;
  endiff

  turn_on_interrupts () ;

  return ( character ) ;

}

/***************************************************************************/

// Here we read from keyboard fifo if one is available

int scr_getc ( void ) /* function declaration */

{

  int character ;

  if ( keyboard_fifo_read_index != keyboard_fifo_write_index ) {                // Fifo r1

    character = read_char_from_external_xram ( & ( keyboard_fifo [ keyboard_fifo_read_index ] ) ) ;  // Fifo r2

    if ( ++ keyboard_fifo_read_index >= KEYBOARD_FIFO_MAX_LENGTH ) keyboard_fifo_read_index = 0 ;    // Fifo r3

  endiff

  else {
    character = -1 ;
  endiff

  return ( character ) ;

}

/**********************************************************************/
/************************************************************/

void scr_eol ( void ) /* function declaration */

{

  unsigned char tmp_lcd_col ;

//lcd_col = 1 - 16 inclusive

  tmp_lcd_col  = lcd_col ;

  while ( lcd_col <= NUMBER_OF_LCD_COLUMNS ) {

    scr_putc ( ' ' ) ;

  wend
 
  lcd_col = tmp_lcd_col ;

  gotoxy ( lcd_col, lcd_row ) ;

}

/********************************************************/

// Here we write to display fifo

void scr_putc ( char character ) /* function declaration */

{

  if ( character == '\r' ) {
    lcd_col = 1 ;
    gotoxy ( lcd_col, lcd_row ) ;
  endiff

  else if ( character == '\n' ) {
    if ( lcd_row < 4 ) lcd_row ++ ;
    else               lcd_row = 1 ;
    gotoxy ( lcd_col,  lcd_row ) ;
  endiff

  else {

    if ( lcd_col <= NUMBER_OF_LCD_COLUMNS ) {

      wait_for_lcd_control_register () ;

      write_lcd_data_register = character ;

      lcd_col ++ ;

    endiff

    else {
//    Scroll vvvv    
    endiff

  endiff

}

/********************************************************/

void scr_clear ( void ) /* function declaration */

{

  wait_for_lcd_control_register () ;
  write_lcd_control_register = 1 ;

  gotoxy ( 1, 1 ) ;

}

/**********************************************************/

void gotoxy ( unsigned char column, unsigned char row ) /* function declaration */

{

  lcd_col = column ;
  lcd_row = row    ;

  if ( lcd_row == 1 ) {
    column += ( 0x80 - 1 ) ;
  endiff
  else if ( lcd_row == 2 ) {
    column += ( 0xC0 - 1 ) ;
  endiff
  else if ( lcd_row == 3 ) {
    column += ( 0x90 - 1 ) ;
  endiff
  else if ( lcd_row == 4 ) {      
    column += ( 0xD0 - 1 ) ;
  endiff

  wait_for_lcd_control_register () ;
  write_lcd_control_register = column ;

}

/*************************************************************/

void lcd_test_display ( void ) /* function declaration */

{

  scr_clear () ;

  gotoxy ( 1, 1 ) ;
  rom_scr_printf       ( "0123456789abcdef" ) ;

  gotoxy ( 1, 2 ) ;
  rom_scr_printf       ( "1123456789abcdef" ) ;

  gotoxy ( 1, 3 ) ;
  rom_scr_printf       ( "2123456789abcdef" ) ;

  gotoxy ( 1, 4 ) ;
  rom_scr_printf       ( "3123456789abcdef" ) ;

 }

/**********************************************************************/
/**********************************************************************/

void strcpy_from_eeprom ( unsigned char XDATA_SPACE * destination_ptr, unsigned int xram_source_ptr ) /* function declaration */

{

  #if   defined ( __C51__ )

    unsigned char character ;

    do {

      peek_n_bytes ( xram_source_ptr ++,  ( (unsigned char *) ( & character ) ), 1 ) ;

      * destination_ptr ++ = character ;

    } while ( character != '\0' ) ;

  #endif

}

/**********************************************************************/

void strcpy_to_eeprom ( unsigned int xram_destination_ptr, unsigned char XDATA_SPACE * source_ptr ) /* function declaration */

{

  #if   defined ( __C51__ )

    unsigned char character ;

    do {

      character =  * source_ptr ++ ;

      poke_n_bytes ( xram_destination_ptr ++,  ( (unsigned char *) ( & character ) ), 1 ) ;

    } while ( character != '\0' ) ;

  #endif

}

/**********************************************************************/
/**********************************************************************/

unsigned char strcpy_from_xram ( unsigned char XDATA_SPACE * destination_ptr, unsigned char XDATA_SPACE * xram_source_ptr ) /* function declaration */

{

  unsigned char character ;
  unsigned char XDATA_SPACE * tmp_destination_ptr ;

  tmp_destination_ptr = destination_ptr ;

  do {

    character = read_char_from_external_xram ( xram_source_ptr ++ ) ;

    * destination_ptr ++ = character ;

  } while ( character != '\0' ) ;

  return ( destination_ptr - tmp_destination_ptr - 1 ) ;

}

/**********************************************************************/

unsigned char strcpy_to_xram ( unsigned char XDATA_SPACE * xram_destination_ptr, unsigned char XDATA_SPACE * source_ptr ) /* function declaration */

{

  unsigned char character ;
  unsigned char XDATA_SPACE * tmp_source_ptr ;

  tmp_source_ptr = source_ptr ;

  do {

    character =  * source_ptr ++ ;

    write_char_to_external_xram ( xram_destination_ptr ++, character ) ;

  } while ( character != '\0' ) ;

  return ( source_ptr - tmp_source_ptr - 1 ) ;

}

/**********************************************************************/
/**********************************************************************/

// Note:  return string may have two terminating zeros.

unsigned char XDATA_SPACE * record_strcpy_from_xram ( unsigned char XDATA_SPACE * destination_ptr, unsigned int fifo_index ) /* function declaration */

{

  #if   defined ( __C51__ )
    #if ( RELEASE_VERSION_NUMBER <= 16 ) 
      unsigned char max_length ;                    // This is only here to generate proper CRC
    #endif
  #endif

  unsigned char character ;
  unsigned char number_of_bytes_copied = 0 ;
  unsigned char XDATA_SPACE * xram_source_ptr ;

  if ( max_bib_string_length == 16 ) {
    xram_source_ptr = (unsigned char XDATA_SPACE *) ( ( fifo_index << 4 ) + ( (unsigned int) STARTING_EXT_XRAM_ADDRESS ) ) ;
  endiff
  else {
    xram_source_ptr = (unsigned char XDATA_SPACE *) ( ( fifo_index *  6 ) + ( (unsigned int) STARTING_EXT_XRAM_ADDRESS ) ) ;
  endiff
  
  ab16 = 0 ;

  do {

    character = read_char_from_external_xram ( xram_source_ptr ++ ) ;

    * destination_ptr ++ = character ;

    number_of_bytes_copied ++ ;

  } while ( ( character != '\0' ) && ( number_of_bytes_copied < max_bib_string_length ) ) ;

  ab16 = 1 ;

  if ( character != '\0' ) {
    * destination_ptr = '\0' ;   // Just in case number_of_bytes_copied == max_bib_string_length
  endiff
  else {
    destination_ptr -- ;
  endiff

  return destination_ptr ;

}

/**********************************************************************/

void record_strcpy_to_xram ( unsigned int fifo_index, unsigned char XDATA_SPACE * source_ptr ) /* function declaration */

{

  unsigned char character ;
  unsigned char number_of_bytes_copied = 0 ;
  unsigned char XDATA_SPACE * xram_destination_ptr ;

  if ( max_bib_string_length == 16 ) {
    xram_destination_ptr = (unsigned char XDATA_SPACE *) ( ( fifo_index << 4 ) + ( (unsigned int) STARTING_EXT_XRAM_ADDRESS ) ) ;
  endiff
  else {
    xram_destination_ptr = (unsigned char XDATA_SPACE *) ( ( fifo_index  * 6 ) + ( (unsigned int) STARTING_EXT_XRAM_ADDRESS ) ) ;
  endiff

  ab16 = 0 ;

  do {

    character =  * source_ptr ++ ;

    write_char_to_external_xram ( xram_destination_ptr ++, character ) ;

    number_of_bytes_copied ++ ;

  } while ( ( character != '\0' ) && ( number_of_bytes_copied < max_bib_string_length ) ) ;

  ab16 = 1 ;

}

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

unsigned char read_char_from_external_xram ( unsigned char XDATA_SPACE * xram_pointer ) /* function declaration */

{

  unsigned char return_value ;

  turn_off_interrupts () ;

  not_cs_ram = 0 ;                           // Turn on  External RAM
  cs_ram2    = 1 ;
  return_value = * xram_pointer ;
  cs_ram2    = 0 ;
  not_cs_ram = 1 ;                           // Turn off External RAM

  turn_on_interrupts () ;

  return return_value ;

}

/**********************************************************************/

void write_char_to_external_xram ( unsigned char XDATA_SPACE * xram_pointer, unsigned char value ) /* function declaration */

{

  turn_off_interrupts () ;

  not_cs_ram = 0 ;                           // Turn on  External RAM
  cs_ram2    = 1 ;
  * xram_pointer = value ;
  cs_ram2    = 0 ;
  not_cs_ram = 1 ;                           // Turn off External RAM

  turn_on_interrupts () ;

}

/**********************************************************************/

long read_long_from_external_xram ( long XDATA_SPACE * xram_pointer ) /* function declaration */

{

  long return_value ;

  turn_off_interrupts () ;

  not_cs_ram = 0 ;                           // Turn on  External RAM
  cs_ram2    = 1 ;
  return_value = * xram_pointer ;
  cs_ram2    = 0 ;
  not_cs_ram = 1 ;                           // Turn off External RAM

  turn_on_interrupts () ;

  return return_value ;

}          

/**********************************************************************/

void write_long_to_external_xram ( long XDATA_SPACE * xram_pointer, long value ) /* function declaration */

{

  turn_off_interrupts () ;

  not_cs_ram = 0 ;                           // Turn on  External RAM
  cs_ram2    = 1 ;
  * xram_pointer = value ;
  cs_ram2    = 0 ;
  not_cs_ram = 1 ;                           // Turn off External RAM

  turn_on_interrupts () ;

}

/**********************************************************************/
/******************************************************************************/

void scr_printf ( char XDATA_SPACE * string ) /* function declaration */

{

  char character ;

  while ( ( character = * string ++ ) != '\0' ) {
    scr_putc ( character ) ;
  wend

}

/*****************************************************************/

void scr_and_serial_printf ( unsigned char line, char XDATA_SPACE * string ) /* function declaration */

{

  if ( line != 0 ) {
    gotoxy ( 1, line ) ;
  endiff

  scr_printf    ( string ) ;
  xserial_printf ( string ) ;

}

/*****************************************************************/
/*****************************************************************/

void rom_scr_and_serial_printf ( unsigned char line, char ROM_SPACE * string ) /* function declaration */

{

  if ( line != 0 ) {
    gotoxy ( 1, line ) ;
  endiff

  rom_scr_printf     ( string ) ;
  rom_xserial_printf ( string ) ;

}

/******************************************************************************/

void xserial_printf ( char XDATA_SPACE * string ) /* function declaration */

{

  char character ;

  while ( ( character = * string ++ ) != '\0' ) {
    serial_putc ( character ) ;
  wend

  while ( serial_transmission_in_progress_flag != 0 ) {
    if ( flash_burn_routine_flag != 0 ) {
      handle_serial_interrupt () ;
    endiff
  wend
}

/******************************************************************************/

void fast_flicker ( unsigned char phase ) /* function declaration */

{

  if ( phase == 0 ) {
    fast_flicker1 ( 0 ) ;
    fast_flicker1 ( 1 ) ;
    fast_flicker1 ( 0 ) ;
    fast_flicker1 ( 2 ) ;
  endiff
  if ( phase == 1 ) {
    fast_flicker1 ( 1 ) ;
    fast_flicker1 ( 0 ) ;
    fast_flicker1 ( 1 ) ;
    fast_flicker1 ( 2 ) ;
  endiff

}

/***********************************************************/
            
void fast_flicker1 ( unsigned char phase ) /* function declaration */

{

  unsigned int index ;

  for ( index = 0; index < 6000 ; index ++ ) {
    if ( phase == 0 ) {
      flicker0_bit = 0 ;      
      flicker1_bit = 0 ;
      flicker0_bit = 1 ;      
      flicker1_bit = 1 ;
      flicker0_bit = 1 ;      
      flicker1_bit = 1 ;
    endiff
    else if ( phase == 1 ) {
      flicker0_bit = 0 ;      
      flicker1_bit = 1 ;
      flicker0_bit = 1 ;      
      flicker1_bit = 0 ;
      flicker0_bit = 1 ;      
      flicker1_bit = 0 ;       
    endiff
    else {
    endiff 
  next

}

/************************************************************/

void beep ( int beep_time ) /* function declaration */

{

  beeper ( 1 ) ;
  delay ( beep_time ) ;
  beeper ( 0 ) ;

}

/*************************************************************/

void beep_beep_beep ( void ) /* function declaration */

{

  beeper ( 1 ) ;
  delay ( 100 ) ;
  beeper ( 0 ) ;

  delay ( 200 ) ;
  beeper ( 1 ) ;
  delay ( 100 ) ;
  beeper ( 0 ) ;

  delay ( 200 ) ;
  beeper ( 1 ) ;
  delay ( 100 ) ;
  beeper ( 0 ) ;

}

/*************************************************************/

void quick_beep_beep ( void ) /* function declaration */

{

  beeper ( 1 ) ;
  delay ( 200 ) ;
  beeper ( 0 ) ;

  delay ( 200 ) ;
  beeper ( 1 ) ;
  delay ( 200 ) ;
  beeper ( 0 ) ;

}

/*************************************************************/
/*************************************************************/

void trigger ( void ) /* function declaration */
{
  tp_trigger = 0 ;
  tp_trigger = 1 ;
}

//**********************************************************/

void initialize_lcd_display ( void ) /* function declaration */

{

  write_lcd_control_register = 0x30 ;
  delay ( 10 ) ;

  write_lcd_control_register = 0x30 ;
  delay ( 10 ) ;

  write_lcd_control_register = 0x30 ;
  delay ( 10 ) ;

  write_lcd_control_register = 0x38 ;    // 8 bit, 2 rows, 5x7  
  delay ( 10 ) ;

  write_lcd_control_register = 0x08 ;    // Display off, cursor off, blink off
  delay ( 10 ) ;

  write_lcd_control_register = 0x01 ;    // Clears display
  delay ( 10 ) ;

//write_lcd_control_register = 0x07 ;    // Sets curor move direction to increment 
                                        //      cursor, with display shift
 
  write_lcd_control_register = 0x06 ;    // Sets curor move direction to increment 
  delay ( 10 ) ;

  write_lcd_control_register = 0x0F ;    // Display on, cursor on, blink on
  delay ( 10 ) ;

  write_lcd_control_register = 0x0E ;    // Display on, cursor on, blink off
  delay ( 10 ) ;

}

/********************************************************************/
/********************************************************************/

void set_up_communications_latch ( void ) /* function declaration */

{

  #if ( 0 ) 

     lsb - keep_power_on_bit           = 0 ; // Default 0 - Passes TXD to distal   RS232 transmitter
           proximal_422_not_used_bit   = 1 ; // Default 1 - Disables      proximal RS422 receiver (active low)
           proximal_232_used_bit       = 1 ; // Default 1 - passes from   proximal RS232 receiver to RXD
           me_talk_to_pc_latch_bit     = 1 ; // Default 1 - passes TXD to proximal RS232 transmitter, also turns off passing TXD to distal RS232 (for printer)

           distal_not_talk_to_pc_bit   = 1 ; // Default 1 - Disables      distal   RS422 receiver (active low) onto proximal RS232 transmitter
           proximal_422_talk_bit       = 0 ; // Default 0 - Enables       proximal RS422 transmitter
           distal_232_rx_pass_bit      = 0 ; // Default 0 - passes        distal   RS232 receiver    to RXD
     msb - distal_422_used_bit         = 0 ; // Default 0 - Enables       distal   RS422 transmitter (active high)

  #endif

//#############################

// Here is new stuff

  #if ( NEWEST_STUFF_FLAG != 0 ) 

    delay ( 1 ) ;   // This makes sure that the stop bit of the last byte has actually been fully transmitted!!! Is this good enough ??????

    communications_latch_byte       = 0x17 ;               // ie 0001 x111

    if ( proximal_wireless_flag != 0 ) {

      proximal_232_used_bit         = 0 ; 

      radio_sleep                   = 0 ;

      distal_422_used_bit           = 1 ;                  // ie 100x t11t
  
      if      ( i_am_talking_to_pc_bit != 0 ) {
        me_talk_to_pc_latch_bit     = 1 ;                  // modulated 
        distal_not_talk_to_pc_bit   = 1 ;                  // modulated      0x9e
      endiff
  
      else {
        me_talk_to_pc_latch_bit     = 0 ;                  // modulated 
        distal_not_talk_to_pc_bit   = 0 ;                  // modulated      0x87
      endiff

    endiff

    else {

      radio_sleep                       = 1 ;

      if    ( proximal_rs422_flag == 0 ) {                 // So proximal RS232
  
        if  ( rs422_repeater_flag == 0 ) {                 // Stand Alone - proximal RS232, no repeater,               single device
  
          if ( i_am_talking_to_pc_bit != 0 ) {
            me_talk_to_pc_latch_bit     = 1 ;              //         0x1e    
          endiff
  
          else {
            me_talk_to_pc_latch_bit     = 0 ;              //         0x17
          endiff
  
        endiff
  
        else {                                             // Base Unit   - proximal RS232, RS422 repeater,            multiple devices
  
          distal_422_used_bit           = 1 ;              // ie 100x t11t
  
          if      ( i_am_talking_to_pc_bit != 0 ) {
            me_talk_to_pc_latch_bit     = 1 ;              // modulated 
            distal_not_talk_to_pc_bit   = 1 ;              // modulated      0x9e
          endiff
  
          else {
            me_talk_to_pc_latch_bit     = 0 ;              // modulated 
            distal_not_talk_to_pc_bit   = 0 ;              // modulated      0x87
          endiff
  
        endiff
  
      endiff
  
      else {
  
        if  ( rs422_repeater_flag == 0 ) {                 // RS422 Node ( ie Remote Unit - proximal RS422, no repeater )
  
          proximal_422_not_used_bit     = 0 ; 
          proximal_232_used_bit         = 0 ;              // ie 00t1 t00t
  
          if ( i_am_talking_to_pc_bit != 0 ) {
            me_talk_to_pc_latch_bit     = 1 ;              // modulated 
            proximal_422_talk_bit       = 1 ;              // modulated      0x38
          endiff
  
          else {
            me_talk_to_pc_latch_bit     = 0 ;              // modulated 
            proximal_422_talk_bit       = 0 ;              // modulated      0x11
          endiff
  
        endiff
  
        else {                                             // Case 3 - Remote Unit - proximal RS422, RS422 repeater,   multiple devices 
  
          proximal_422_not_used_bit     = 0 ; 
          proximal_232_used_bit         = 0 ;
          distal_422_used_bit           = 1 ;              // ie 10xx t00t
  
          if ( i_am_talking_to_pc_bit != 0 ) {
            me_talk_to_pc_latch_bit     = 1 ;              // modulated 
            proximal_422_talk_bit       = 1 ;              // modulated
            distal_not_talk_to_pc_bit   = 1 ;              // modulated      0xB8
          endiff
  
          else if ( distal_device_is_talking_to_pc_flag != 0 ) {
            me_talk_to_pc_latch_bit     = 0 ;              // modulated 
            proximal_422_talk_bit       = 1 ;              // modulated
            distal_not_talk_to_pc_bit   = 0 ;              // modulated      0xA1
          endiff
  
          else {
            me_talk_to_pc_latch_bit     = 0 ;              // modulated 
            proximal_422_talk_bit       = 0 ;              // modulated
            distal_not_talk_to_pc_bit   = 1 ;              // modulated      0x91
          endiff
  
        endiff
  
      endiff
  
    endiff

    CS_LATCH_PORT = communications_latch_byte ;
  
  #endif
  
//#############################

// Here is old stuff


  #if ( NEWEST_STUFF_FLAG == 0 ) 

    delay ( 1 ) ;   // This makes sure that the stop bit of the last byte has actually been fully transmitted!!! Is this good enough ??????
  
    communications_latch_byte           = 0x16 ;      // ie 0001 x11x
  
    if    ( proximal_rs422_flag == 0 ) {  
  
      if  ( rs422_repeater_flag == 0 ) {        // Stand Alone - proximal RS232, no repeater,               single device
  
        if ( i_am_talking_to_pc_bit != 0 ) {
          keep_power_on_bit           = 0 ; 
          me_talk_to_pc_latch_bit     = 1 ; //                0x1e    
        endiff
  
        else {
          keep_power_on_bit           = 1 ; 
          me_talk_to_pc_latch_bit     = 0 ;        //         0x17
        endiff
  
      endiff
  
      else {                                    // Base Unit   - proximal RS232, RS422 repeater,            multiple devices
  
        distal_422_used_bit           = 1 ;       // ie 100x t11t
  
        if      ( i_am_talking_to_pc_bit != 0 ) {
          keep_power_on_bit           = 0 ; // modulated
          me_talk_to_pc_latch_bit     = 1 ; // modulated 
          distal_not_talk_to_pc_bit   = 1 ; // modulated      0x9e
        endiff
  
        else {
          keep_power_on_bit           = 1 ; // modulated
          me_talk_to_pc_latch_bit     = 0 ; // modulated 
          distal_not_talk_to_pc_bit   = 0 ; // modulated      0x87
        endiff
  
      endiff
  
    endiff
  
    else {
  
      if  ( rs422_repeater_flag == 0 ) {        // RS422 Node ( ie Remote Unit - proximal RS422, no repeater )
  
        proximal_422_not_used_bit     = 0 ; 
        proximal_232_used_bit         = 0 ;     // ie 00t1 t00t
  
        if ( i_am_talking_to_pc_bit != 0 ) {
          keep_power_on_bit           = 0 ; // modulated
          me_talk_to_pc_latch_bit     = 1 ; // modulated 
          proximal_422_talk_bit       = 1 ; // modulated      0x38
        endiff
  
        else {
          keep_power_on_bit           = 1 ; // modulated
          me_talk_to_pc_latch_bit     = 0 ; // modulated 
          proximal_422_talk_bit       = 0 ; // modulated      0x11
        endiff
  
      endiff
  
      else {                                    // Case 3 - Remote Unit - proximal RS422, RS422 repeater,   multiple devices 
  
        proximal_422_not_used_bit     = 0 ; 
        proximal_232_used_bit         = 0 ;
        distal_422_used_bit           = 1 ;       // ie 10xx t00t
  
        if ( i_am_talking_to_pc_bit != 0 ) {
          keep_power_on_bit           = 0 ; // modulated
          me_talk_to_pc_latch_bit     = 1 ; // modulated 
          proximal_422_talk_bit       = 1 ; // modulated
          distal_not_talk_to_pc_bit   = 1 ; // modulated      0xB8
        endiff
  
        else if ( distal_device_is_talking_to_pc_flag != 0 ) {
          keep_power_on_bit           = 1 ; // modulated
          me_talk_to_pc_latch_bit     = 0 ; // modulated 
          proximal_422_talk_bit       = 1 ; // modulated
          distal_not_talk_to_pc_bit   = 0 ; // modulated      0xA1
        endiff
  
        else {
          keep_power_on_bit           = 1 ; // modulated
          me_talk_to_pc_latch_bit     = 0 ; // modulated 
          proximal_422_talk_bit       = 0 ; // modulated
          distal_not_talk_to_pc_bit   = 1 ; // modulated      0x91
        endiff
  
      endiff
  
    endiff
  
    #if ( AUX_SERIAL_081902_STUFF_FLAG != 0 )
        keep_power_on_bit     = 0 ;    // VVVV Get rid of this for all other cases
    #endif
  
    CS_LATCH_PORT = communications_latch_byte ;
  
  #endif

//#############################

  if ( display_latch_info_flag != 0 ) {
    gotoxy ( 1, 1 ) ;
    sprintf ( scratch_string, "%2x    ", (unsigned int) communications_latch_byte ) ;
    scr_printf ( scratch_string ) ;
  endiff

}

/*************************************************************/
/***********************************************************************************/

unsigned int calculate_eprom_checksum ( unsigned int initial_checksum, unsigned int starting_rom_address, unsigned int ending_rom_address ) /* function declaration */

{

  unsigned int rom_address, checksum ;

  checksum    = initial_checksum ;
  rom_address = starting_rom_address ; 

  ending_rom_address ++ ;

  do {

    checksum += * ( (unsigned char ROM_SPACE *) rom_address )  ;

  } while ( ++ rom_address != ending_rom_address ) ;

  return ( checksum ) ;

}
                                                                    
/***********************************************************************************/
/***********************************************************************************/

void rom_scr_printf ( char ROM_SPACE * string ) /* function declaration */

{

  char character ;

  while ( ( character = * string ++ ) != '\0' ) {
    scr_putc ( character ) ;
  wend

}

/*****************************************************************/
/******************************************************************************/

void serial_printf ( char XDATA_SPACE * string ) /* function declaration */

{

  char character ;

  while ( ( character = * string ++ ) != '\0' ) {
    serial_putc ( character ) ;
  wend

}

/*****************************************************************/
/******************************************************************************/

void serial_printf_and_wait ( char XDATA_SPACE * string ) /* function declaration */

{

  char character ;

  if ( kill_print_job_flag == 0 ) {

    while ( ( character = * string ++ ) != '\0' ) {
      serial_putc ( character ) ;
    wend

    wait_for_printer_ready () ;

  endiff

}

/******************************************************************************/
/******************************************************************************/

void rom_serial_printf ( char ROM_SPACE * string ) /* function declaration */

{

  unsigned char character ;
  
  while ( ( character = * string ++ ) != '\0' ) {
    serial_putc ( character ) ;
  wend
}

/******************************************************************************/
/******************************************************************************/

void rom_serial_printf_and_wait ( char ROM_SPACE * string ) /* function declaration */

{

  char character ;
  
  if ( kill_print_job_flag == 0 ) {

    while ( ( character = * string ++ ) != '\0' ) {
      serial_putc ( character ) ;
    wend

    wait_for_printer_ready () ;

  endiff

}

/******************************************************************************/

void rom_xserial_printf ( char ROM_SPACE * string ) /* function declaration */

{

  unsigned char character ;
  
  while ( ( character = * string ++ ) != '\0' ) {
    serial_putc ( character ) ;
  wend
	  
  while ( serial_transmission_in_progress_flag != 0 ) {
   if ( flash_burn_routine_flag != 0 ) {
      handle_serial_interrupt () ;
    endiff
  wend

}

/**********************************************************************/
/**********************************************************************/

void clear_keyboard_buffer ( void ) /* function declaration */

{
  while ( scr_getc () != -1 ) ;
}

/**********************************************************************/
/**********************************************************************/

void wait_for_lcd_control_register ( void ) /* function declaration */

{

  unsigned char tmp_counter = 0 ;

  while ( ( ( read_lcd_control_register & 0x80 ) != 0 ) && ( ++ tmp_counter < 255 ) ) ;

}

/**********************************************************************/
 
// Here we wait for serial_transmission_in_progress, and for either timeout or for flow control

void wait_for_printer_ready ( void ) /* function declaration */

{

  BIT_TYPE done_flag ;

  if ( use_hardware_flow_control_flag == 0 ) {
    printer_crlf_delay_counter = printer_crlf_delay ;
    printer_crlf_delay_flag    =   1 ;
  endiff

  done_flag = 0 ;
 
  while ( done_flag == 0 ) {

    #if defined ( __C51__ )
      debounce_switches () ;
    #endif

    if ( scr_getc () == BKSP_KEY ) {
      kill_print_job_flag = 1 ;
    endiff

    if ( serial_transmission_in_progress_flag == 0 ) {
      if ( use_hardware_flow_control_flag == 0 ) {
        if ( printer_crlf_delay_flag == 0 ) {
          done_flag = 1 ;
        endiff
      endiff
      else {
        done_flag = 1 ;
      endiff
    endiff
  wend

}

/**********************************************************************/
/**********************************************************************/

#if ( 0 ) 

void serial_printf_and_wait ( char XDATA_SPACE * string ) /* function declaration */

{

  char character ;

  if ( kill_print_job_flag == 0 ) {

    while ( ( character = * string ++ ) != '\0' ) {
      serial_putc ( character ) ;
    wend

    wait_for_printer_ready () ;

  endiff

}

/******************************************************************************/
/******************************************************************************/

void rom_serial_printf_and_wait ( char ROM_SPACE * string ) /* function declaration */

{

  char character ;
  
  if ( kill_print_job_flag == 0 ) {

    while ( ( character = * string ++ ) != '\0' ) {
      serial_putc ( character ) ;
    wend

    wait_for_printer_ready () ;

  endiff

}

#endif

/******************************************************************************/
/******************************************************************************/

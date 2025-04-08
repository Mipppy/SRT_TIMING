
#define EEPROM_SPACE /* */

#if defined ( __C51__ ) 

       char * strcat  ( char * s1, char * s2 ) ;
       char * strcpy  ( char * s1, char * s2 ) ;
       int sprintf ( char * string, const char * format_string, ... ) ;
       int atoi ( void * string ) ;
       int strlen ( char * source ) ;

#endif 

  // File F:\xc_timer\code\config.c:
       void configure_routine ( void ) ; /* function declaration */
       void burn_default_configuration ( void ) ; /* function declaration */
       void print_configuration_stuff ( void ) ; /* function declaration */
       void read_configuration_variables ( void ) ; /* function declaration */
       void write_configuration_variables ( void ) ; /* function declaration */


   // File F:\xc_timer\code\debounce.c:
       void debounce_switches ( void ) ; /* function declaration */

   // File F:\xc_timer\code\EXIT.C:
       void exit_routine ( void ) ; /* function declaration */
       void exit_from_isr ( void ) ; /* function declaration */
       void perform_ret_from_interrupt ( void ) ; /* function declaration */
       unsigned char flash_api ( unsigned char flash_command, unsigned char flash_data_byte, unsigned short flash_address ) ; /* function declaration */
       void jmp_to_flash_startup_0D00 ( void ) ; /* function declaration */
       void jmp_to_isp_flash_program_FC00 ( void ) ; /* function declaration */
       void load_x_xram_page ( unsigned char XDATA_SPACE * xram_pointer, unsigned char data_byte ) ;/* function declaration */
       BIT_TYPE test_x_xram_page ( unsigned char XDATA_SPACE * xram_pointer, unsigned char data_byte ) ;/* function declaration */
       void push_registers ( void ) ;
       void pop_registers ( void ) ;


  // File F:\xc_timer\code\FLASH.C:

       void burn_routine ( unsigned char context_flag ) ; /* function declaration */
       void flash_initialize_io ( void ) ; /* function declaration */
       void erase_block ( unsigned char block ) ; /* function declaration */
       void jmp_to_flash_startup_0D00 ( void ) ; /* function declaration */
       void handle_serial_interrupt ( void ) ; 

#if ( 0 )


       //    flash_api ( unsigned char r1_byte, unsigned char ROM_SPACE * address, unsigned char value ) ; /* function declaration */
       void flash_test ( BIT_TYPE set_flag ) ; /* function declaration */
       //    unsigned char flash_api ( unsigned char r1_byte, unsigned char ROM_SPACE * address, unsigned char value ) ; /* function declaration */
       void flash_program ( void ) ; /* function declaration */
       void flash_dump ( void ) ; /* function declaration */

#endif



  // File F:\xc_timer\code\IIC.C:
       void test_eeprom_memory ( BIT_TYPE zero_flag ) ; /* function declaration */
       void test_and_poke_n_bytes ( unsigned int eeprom_address, unsigned char EEPROM_SPACE * ram_pointer, unsigned char number_of_bytes ) ; /* function declaration */
       void poke_n_bytes ( unsigned int eeprom_address, unsigned char EEPROM_SPACE * ram_pointer, unsigned char number_of_bytes ) ; /* function declaration */
       void peek_n_bytes ( unsigned int eeprom_address, unsigned char EEPROM_SPACE * ram_pointer, unsigned char number_of_bytes ) ; /* function declaration */
       unsigned char shift_i2c_byte_in ( void ) ; /* function declaration */
       void shift_i2c_byte_out ( unsigned char data_byte ) ; /* function declaration */
       void transmit_i2c_byte( unsigned char data_byte ) ; /* function declaration */
       void i2c_stop ( void ) ; /* function declaration */
       void i2c_start ( void ) ; /* function declaration */
       void i2c_ack ( void ) ; /* function declaration */
       BIT_TYPE i2c_get_ack ( void ) ; /* function declaration */
       void i2c_end_ack ( void ) ; /* function declaration */
       BIT_TYPE i2c_clock ( void ) ; /* function declaration */
       void delay_5_usec ( void ) ; /* function declaration */
       void i2c_reset ( void ) ; /* function declaration */
       void poke_1_byte ( unsigned int eeprom_address, unsigned char EEPROM_SPACE * ram_pointer ) ; /* function declaration */
       void peek_1_byte ( unsigned int eeprom_address, unsigned char EEPROM_SPACE * ram_pointer ) ; /* function declaration */


   // File F:\xc_timer\code\INT.C:
       void handle_error ( void ) ; /* function declaration */
       void handle_error_from_isr ( void ) ; /* function declaration */

  // File F:\xc_timer\code\misc.c:
       void get_time_string_from_msec ( long long_msec, char XDATA_SPACE * string, BIT_TYPE time_in_tics_flag ) ; /* function declaration */
       long get_msec_from_time_string ( char * ptr_to_time_string, BIT_TYPE time_in_tics_flag ) ; /* function declaration */
       void remove_leading_white ( char * ptr_string )  ; /* function declaration    */
       int remove_leading_and_trailing_white ( char * ptr_string )  ; /* function declaration    */
       int remove_trailing_white ( char * ptr_string )  ; /* function declaration    */
       char * return_ptr_to_first_occurance ( char * ptr_string, char character )  ; /* function declaration    */
       char * get_ptr_to_first_non_white ( char * ptr_string )  ; /* function declaration    */
       char * get_ptr_to_first_white ( char * ptr_string )  ; /* function declaration    */
       unsigned int hex_atoi ( char XDATA_SPACE * ptr_character_array ) ; /* function declaration */
       void quick_int_printf  ( unsigned char XDATA_SPACE * ptr_to_string, unsigned int ivalue ) ; /* function declaration */
       void quick_hex_printf  ( unsigned char XDATA_SPACE * ptr_to_string, unsigned int ivalue ) ; /* function declaration */
       void quick_long_printf  ( unsigned char XDATA_SPACE * ptr_to_string, unsigned long lvalue ) ; /* function declaration */
       int string_input ( char ROM_SPACE * message, char XDATA_SPACE * character_array )  ; /* function declaration    */
       int string_inputx ( char XDATA_SPACE * local_character_buffer )  ; /* function declaration    */
       signed char get_hex_value ( char character ) ; /* function declaration */
       void serial_print_float_variable ( char ROM_SPACE * string, float variable ) ; /* function declaration */
       void serial_print_long_variable ( char ROM_SPACE * string, unsigned long variable ) ; /* function declaration */
       void serial_print_int_variable ( char ROM_SPACE * string, unsigned int variable ) ; /* function declaration */
       void serial_print_hex_variable ( char ROM_SPACE * string, unsigned int variable ) ; /* function declaration */
       void serial_printf ( char XDATA_SPACE * string ) ; /* function declaration */
       void serial_printf_and_wait ( char XDATA_SPACE * string ) ; /* function declaration */
       void xserial_printf ( char XDATA_SPACE * string ) ; /* function declaration */
       void rom_serial_printf ( char ROM_SPACE * string ) ; /* function declaration */
       void rom_serial_printf_and_wait ( char ROM_SPACE * string ) ; /* function declaration */
       void rom_xserial_printf ( char ROM_SPACE * string ) ; /* function declaration */
       void rom_scr_printf ( char ROM_SPACE * string ) ; /* function declaration */
       BIT_TYPE get_yes_flag ( char ROM_SPACE * string ) ; /* function declaration */
       void hex_inputx ( char ROM_SPACE * description_string, unsigned int * ptr_value )         ; /* function declaration    */
       void serial_crlf ( unsigned char number ) ; /* function declaration */


  // File F:\xc_timer\code\packets.c:
       void output_formatted_file_string ( char * file_path_and_name, char * format_string,... ) ; /* function declaration */
       unsigned char timer_process_rx_byte ( char rx_byte ) ; /* function declaration */
       unsigned char timer_process_aux_rx_byte ( char rx_byte ) ; /* function declaration */
       unsigned char process_rx_packet ( void ) ; /* function declaration */
       void transmit_generic_packet ( char * ptr_to_string_buffer ) ; /* function declaration */
       void transmit_record_packet ( BIT_TYPE printer_bit, unsigned int fifo_index ) ; /* function declaration */
       unsigned int calculate_crc_for_byte ( unsigned char byte, unsigned int initial_crc )  ; /* function declaration */
       unsigned char process_tag_rx_data ( char character ) ; /* function declaration */
       void handle_tag_505_rx_packet ( void ) ; /* function declaration */
       void handle_tag_605_rx_packet ( void ) ; /* function declaration */

   // File F:\xc_timer\code\STDIO.C:
       BIT_TYPE kbhit ( void ) ; /* function declaration */
       void hit_ok_key ( char ROM_SPACE * rom_string_ptr ) ; /* function declaration */
       BIT_TYPE any_key_is_pressed ( void ) ; /* function declaration */
       int serial_getc ( void )       ; /* function declaration    */
       int aux_serial_getc ( void )       ; /* function declaration    */
       void lcd_test_display ( void ) ; /* function declaration */
       void strcpy_from_eeprom ( unsigned char XDATA_SPACE * destination_ptr, unsigned int xram_source_ptr ) ; /* function declaration */
       void strcpy_to_eeprom ( unsigned int xram_destination_ptr, unsigned char XDATA_SPACE * source_ptr ) ; /* function declaration */

       unsigned char strcpy_from_xram ( unsigned char XDATA_SPACE * destination_ptr,      unsigned char XDATA_SPACE * xram_source_ptr ) ; /* function declaration */
       unsigned char strcpy_to_xram   ( unsigned char XDATA_SPACE * xram_destination_ptr, unsigned char XDATA_SPACE * source_ptr ) ; /* function declaration */

       unsigned char XDATA_SPACE * record_strcpy_from_xram ( unsigned char XDATA_SPACE * destination_ptr, unsigned int fifo_index ) ; /* function declaration */
       void record_strcpy_to_xram ( unsigned int fifo_index, unsigned char XDATA_SPACE * source_ptr ) ; /* function declaration */
       long read_long_from_external_xram ( long XDATA_SPACE * xram_pointer ) ; /* function declaration */

       void scr_and_serial_printf ( unsigned char line, char XDATA_SPACE * string ) ;/* function declaration */
       void rom_scr_and_serial_printf ( unsigned char line, char ROM_SPACE * string ) ;/* function declaration */
 
       void wait_for_lcd_control_register ( void ) ; /* function declaration */
       void clear_keyboard_buffer ( void ) ; /* function declaration */
       void wait_for_printer_ready ( void ) ; /* function declaration */

       void serial_putc ( char character ) ; /* function declaration    */
       void scr_eol ( void ) ; /* function declaration */
       void scr_putc ( char character ) ; /* function declaration */
       void scr_clear ( void ) ; /* function declaration */
       void gotoxy ( unsigned char column, unsigned char row ) ; /* function declaration */
       int scr_getc ( void ) ; /* function declaration */
       void scr_printf ( char XDATA_SPACE * string ) ; /* function declaration */
       unsigned char read_char_from_external_xram ( unsigned char XDATA_SPACE * xram_pointer ) ; /* function declaration */
       void write_char_to_external_xram ( unsigned char XDATA_SPACE * xram_pointer, unsigned char value ) ; /* function declaration */
      
                     

  // File F:\xc_timer\code\TIMER.C:
	   #if   defined ( __C51__ )
         void main ( void ) ; /* function declaration */
	   #endif
	   #if ! defined ( __C51__ )
         void  main ( int number_of_arguments, char * argument_array [] )  ; /* function declaration    */
	   #endif
       void initialize_io_completely ( void ) ; /* function declaration */
       void fast_flicker ( unsigned char phase ) ; /* function declaration */
       void fast_flicker1 ( unsigned char phase ) ; /* function declaration */
       void beep ( int beep_time ) ; /* function declaration */
       void beep_beep_beep ( void ) ; /* function declaration */
       void quick_beep_beep ( void ) ; /* function declaration */
       void trigger ( void ) ; /* function declaration */
       void set_up_communications_latch ( void ) ; /* function declaration */
       void initialize_lcd_display ( void ) ; /* function declaration */
       void write_long_to_external_xram ( long XDATA_SPACE * xram_pointer, long value ) ; /* function declaration */
       void print_eeprom_strings ( void ) ; /* function declaration */


  // File F:\xc_timer\code\TIMER1.C:

       void test_xram_memory ( BIT_TYPE external_xram_flag, BIT_TYPE walk_test_flag, unsigned char set_value ) ; /* function declaration */
       void walk_test_external_xram_memory ( unsigned long starting_ext_xram_address, unsigned long ending_ext_xram_address ) ; /* function declaration */
       void walk_test_internal_xram_memory ( unsigned int starting_int_xram_address, unsigned int ending_int_xram_address ) ; /* function declaration */
       void byte_test_internal_xram_memory ( unsigned int starting_int_xram_address, unsigned int ending_int_xram_address, unsigned char set_value ) ; /* function declaration */
       void byte_test_external_xram_memory ( unsigned long starting_ext_xram_address, unsigned long ending_ext_xram_address, unsigned char set_value ) ; /* function declaration */
       void xx_print_char ( unsigned int address ) ; /* function declaration */
       void primitive_serial_crlf ( void ) ; /* function declaration */

       void dump_xram_contents ( unsigned char xram_page, unsigned int starting_ext_xram_address, unsigned int ending_ext_xram_address ) ; /* function declaration */
       void dump_eeprom_contents ( void ) ; /* function declaration */
       void delay ( unsigned int n ) ; /* function declaration */
       void flicker_led ( unsigned char number_of_flashes, unsigned char led_number ) ; /* function declaration */
       void halt ( void ) ; /* function declaration */
       void tickle_watchdog ( void ) ; /* function declaration */
       void powerdown ( void ) ; /* function declaration */
       unsigned int calculate_eprom_checksum ( unsigned int initial_checksum, unsigned int starting_rom_address, unsigned int ending_rom_address ) ; /* function declaration */
       void dump_flash_contents ( unsigned int starting_rom_address, unsigned int ending_rom_address ) ; /* function declaration */
       void uncalled_functions ( void ) ; /* function declaration */

  // File F:\xc_timer\code\TIMERXX.C:
       void acquire_data ( void )  ; /* function declaration */
       void i_beep ( unsigned char counts ) ; /* function declaration */
       void refresh_screen ( void ) ; /* function declaration */
       void display_record_packet_on_lcd ( unsigned int fifo_index, unsigned char row, BIT_TYPE add_posting_flag ) ; /* function declaration */
       void display_help ( BIT_TYPE more_flag ) ; /* function declaration */
       void handle_bibs_equals_plunges ( void ) ; /* function declaration */
       void display_bib_channel_and_device_number ( void ) ; /* function declaration */
       void bib_history_routine ( void )  ;/* function declaration */
       BIT_TYPE get_record_type ( unsigned int read_index, unsigned char type ) ; /* function declaration */
       char get_a_key ( void ) ;/* function declaration */
       void results_routine ( void ) ; /* function declaration */
       void bib_and_switch_totals_routine ( void ) ; /* function declaration */
       void save_this_record ( void ) ; /* function declaration */

  // File F:\xc_timer\code\AUX_SER.C:

       unsigned char timer_process_aux_rx_byte ( char rx_byte ) ; /* function declaration */
       unsigned char process_aux_rx_packet ( void ) ; /* function declaration */
       unsigned char spi_write_word ( unsigned char aux_serial_status, unsigned char aux_serial_byte ) ; /* function declaration */
       void aux_serial_putc ( char character )    ; /* function declaration    */
       int aux_serial_getc ( void )       ; /* function declaration    */
       void rom_aux_serial_printf           ( char ROM_SPACE * string ) ; /* function declaration */
       void rom_aux_serial_printf_test_mode ( char ROM_SPACE * string ) ; /* function declaration */
       void set_up_aux_serial_port ( void ) ; /* function declaration */

                                       



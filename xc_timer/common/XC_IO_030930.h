
#if   defined ( __C51__ )

/***********************************************************************/
/***********************************************************************/

  #define IE_INT0     0x01       /* 0x01 - INT0 Interrupt (EX0)               */
  #define IE_TIMER0   0x02       /* 0x02 - Timer0 Interrupt (timer interrupt) */
  #define IE_INT1     0x04       /* 0x04 - INT1 Interrupt1 (EX1)              */
  #define IE_TIMER1   0x08       /* 0x08 - Timer1 Interrupt (baud timer)      */
  #define IE_SERIAL   0x10       /* 0x10 - Serial Interrupts                  */
  #define IE_TIMER2   0x20       /* 0x20 - Timer2 Capture (ET2) Interrupt     */
  #define IE_PCA      0x40       /* 0x40 - PCA Interrupt                      */ 

  #if ( 0 ) 

    IE_INT0                /* 0x01 - INT0   Interrupt (Serial Wake Up)   */
    IE_TIMER0              /* 0x02 - Timer0 Interrupt (Timer Interrupt)  */
    IE_INT1                /* 0x04 - INT1   Interrupt1 ( Key Wake Up )   */
    IE_SERIAL              /* 0x10 - Serial Interrupts                   */
    IE_TIMER2              /* 0x20 - Timer2 Capture Interrupt (Not Used) */
    IE_PCA                 /* 0x40 - PCA Interrupt ( Start )             */

  #endif

/********************************************************************/
/********************************************************************/

#define DATA_BUS                         ( P0 )
#define ADDRESS_BUS_LSB                  ( P0 )
#define ADDRESS_BUS_MSB                  ( P2 )

#define turn_off_interrupts()            EA = 0 
#define turn_on_interrupts()             EA = 1

#define turn_off_aux_serial_interrupts() EX1 = 0 
#define turn_on_aux_serial_interrupts()  EX1 = 1 

#define idle()                           ( PCON = 0xC1 )

/********************************************************************/
/********************************************************************/

#define rxd                          ( P3_0 )       // Pin 11  -  aka rxd

#define txd                          ( P3_1 )       // Pin 13  -  aka txd

#define CPU_WR                       ( P3_6 )       // Pin 18  -  aka /WR

#define CPU_RD                       ( P3_7 )       // Pin 19  -  aka /RD

/********************************************************************/
/********************************************************************/

#if ( NEW_AUX_SERIAL_FLAG != 0 ) 

/********************************************************************/
/********************************************************************/

// The following bits are defined on Port 3

#define aux_uart_irq                 ( P3_3 )       // Pin 15  -  aka Int1

#define tied_to_rxd                  ( P3_2 )       // Pin 14  -  aka Int0

#define not_cs_ram                   ( P3_5 )       // Pin 17  -  aka T1: 0 Turns on Power to LCD 

#define timebase                     ( P3_4 )       // Pin 16  -  aka T0   Low for RAM access, High for Powerdown.  Cannot use as test point,  because of io decoding

/********************************************************************/
/********************************************************************/

// The following bits are defined on Port 1

#define aux_uart_clk                 ( P1_0 )       // Pin  2  -  aka T2  TP1

#define rs232_not_force_off_bit      ( P1_1 )       // Pin  3  -  aka T2EX                                               
#define spi_slave_select             ( P1_1 )

#define aux_uart_cs                  ( P1_2 )       

#define low_batt                     ( P1_3 )       // Pin  5  -  aka CEX0

#define beeper                       ( P1_4 )       // Pin  6  -  aka CEX1

#define iic_data                     ( P1_5 )       // Pin  7  -  aka CEX2
#define set_iic_data_bit(x)          ( P1_5 = x )
#define read_iic_data_bit()          ( P1_5 )
#define ab16                         ( P1_5 )
#define spi_input_bit                ( P1_5 )       //            SPI MISO (ie Master Input, Slave Output)                             

#define ab17                         ( P1_6 )       // Pin  8  -  aka CEX
#define cs_ram2                      ( P1_6 )       //            Low forces RAM into Powerdown
#define spi_clock                    ( P1_6 )

#define ab18                         ( P1_7 )       // Pin  9  -  aka CEX4
#define iic_clock                    ( P1_7 )       //            no effect on IIC if iic_data line is stable
#define set_iic_clock_bit(x)         ( P1_7 = x ) 
#define spi_output_bit               ( P1_7 )       //            SPI MOSI (ie Master Output, Slave Input)                  

#define lcd_power_off_bit            ( dummy_bit )
#define flash_disable                ( dummy_bit )

//*******************************************************************/
//*******************************************************************/

#endif

/********************************************************************/
/********************************************************************/

#if ( NEW_AUX_SERIAL_FLAG == 0 ) 

/********************************************************************/
/********************************************************************/

// The following bits are defined on Port 3

#define rs232_valid                  ( P3_3 )       // aka Int1

#define rx_wakeup                    ( P3_2 )       // aka Int0

#define lcd_power_off_bit            ( P3_5 )       // aka T1: 0 Turns on Power to LCD 

#define not_cs_ram                   ( P3_4 )       // aka T0   Low for RAM access, High for Powerdown.  Cannot use as test point,  because of io decoding

#define spi_clock                    ( P3_6 )       // CPU_WR

#define spi_input_bit                ( P3_7 )       // CPU_RD

/********************************************************************/
/********************************************************************/

// The following bits are defined on Port 1

#define timebase                     ( P1_0 )       // aka T2  TP1

#define rs232_not_force_off_bit      ( P1_1 )       // aka T2EX                                               

#define flash_disable                ( P1_2 )       // aka ECI PCA input 
#define aux_uart_cs                  ( P1_2 )       

#define low_batt                     ( P1_3 )       // aka CEX0

#define beeper                       ( P1_4 )       // aka CEX1

#define iic_data                     ( P1_5 )       // aka CEX2
#define set_iic_data_bit(x)          ( P1_5 = x )
#define read_iic_data_bit()          ( P1_5 )
#define ab16                         ( P1_5 )
#define spi_output_bit               ( P1_5 )       // IIC_DATA      */

#define ab17                         ( P1_6 )       // aka CEX
#define cs_ram2                      ( P1_6 )       // Low forces RAM into Powerdown

#define ab18                         ( P1_7 )       // aka CEX4
#define iic_clock                    ( P1_7 )       // no effect on IIC if iic_data is stable
#define set_iic_clock_bit(x)         ( P1_7 = x ) 

//*******************************************************************/
//*******************************************************************/

#endif

//*******************************************************************/
//*******************************************************************/

// Note: AB8 is LCD RS    (register select) 0 Selects Control Register, 1 selects data register
//       AB9 is LCD RD/WR                   0 for Write,                1 for read

//                          AB9   AB8
//                                                      
//                       LCD_RD   Data  
//

//   write control byte      0     0
//   write data    byte      0     1
//   read  control byte      1     0
//   read  data    byte      1     1

  #define CS_LCD                       ( ( ( unsigned char XDATA_SPACE * ) ( 0xB000 ) ) [ 0 ] ) 
    #define write_lcd_control_register ( ( ( unsigned char XDATA_SPACE * ) ( 0xB000 ) ) [ 0 ] )
    #define write_lcd_data_register    ( ( ( unsigned char XDATA_SPACE * ) ( 0xB100 ) ) [ 0 ] )
    #define read_lcd_control_register  ( ( ( unsigned char XDATA_SPACE * ) ( 0xB200 ) ) [ 0 ] )
    #define read_lcd_data_register     ( ( ( unsigned char XDATA_SPACE * ) ( 0xB300 ) ) [ 0 ] )
    #define LCD_WRITE_CONTROL_ADDRESS_MSB  ( 0xB0 )
    #define LCD_WRITE_DATA_ADDRESS_MSB     ( 0xB1 )

  #define CS_KEYS                      ( ( ( unsigned char XDATA_SPACE * ) ( 0xE000 ) ) [ 0 ] )   //
    #define STROBE_COL0_MSB                (  0xEE )
    #define STROBE_COL1_MSB                (  0xED )
    #define STROBE_COL2_MSB                (  0xEB )
    #define STROBE_COL3_MSB                (  0xE7 )

  #define CS_TIMER_SWITCH                  ( ( ( unsigned char XDATA_SPACE * ) ( 0xD000 ) ) [ 0 ] ) 
  #define TIMER_SWITCH_PORT_ADDRESS_MSB    ( 0xD0 )

  #define CS_LATCH_PORT                    ( ( ( unsigned char XDATA_SPACE * ) ( 0x7000 ) ) [ 0 ] ) 
  #define LATCH_PORT_ADDRESS_MSB           ( 0x70 )

  #define DEFAULT_COMMUNICATION_LATCH_BITS 0x1E 

/********************************************************************/
/********************************************************************/

// Note that either (of the two) RAM CS lines will keep RAM in powerdown mode, therefore we can use CS_RAM2 as an interrupt test point.
//       (as long as interrupts are turned off while writing to external RAM)

// Note that IIC Clock can wiggle, as long as IIC Data does not wiggle.  Therefore, IIC Clock can be used as an interrupt testpoint 
//       (as long as interrupts are turned off while writing to iic)

  #if ( AUX_SERIAL_FLAG == 0 )                                                 
    #define serial_interrupt_diagnostic_bit(x)      ( rs232_valid         =  (x) )   // VVVV get rid of this, two outputs contending
    #define timer_interrupt_diagnostic_bit(x)       ( lcd_power_off_bit   =  (x) ) 
  #endif

  #if ( ( AUX_SERIAL_FLAG != 0 ) && ( NEW_AUX_SERIAL_FLAG == 0 ) )
    #define timer_interrupt_diagnostic_bit(x)       distal_232_tx_pass_bit = (x) ; CS_LATCH_PORT = communications_latch_byte ;
    #define serial_interrupt_diagnostic_bit(x)      ( cs_ram2             =  (x) ) 
    #define aux_serial_interrupt_diagnostic_bit(x)  ( lcd_power_off_bit   =  (x) )
  #endif

  #if ( ( AUX_SERIAL_FLAG != 0 ) && ( NEW_AUX_SERIAL_FLAG != 0 ) )
    #define timer_interrupt_diagnostic_bit(x)       distal_232_tx_pass_bit = (x) ; CS_LATCH_PORT = communications_latch_byte ;
    #define serial_interrupt_diagnostic_bit(x)      ( dummy_bit           =  (x) ) 
    #define aux_serial_interrupt_diagnostic_bit(x)  ( dummy_bit           =  (x) )
  #endif

  #if ( NEW_AUX_SERIAL_FLAG == 0 ) 
    #define flash_diagnostic_bit(x)                   ( iic_clock         =  (x) )
    #define debounce_diagnostic_bit(x)                ( low_batt          = ~(x) )      // VVVV get rid of this, contending outputs
    #define tp_rs232_valid                            rs232_valid
    #define tp_trigger                                lcd_power_off_bit
    #define tp1                                       iic_clock
    #define flicker0_bit                              iic_clock
    #define flicker1_bit                              rs232_valid
  #endif

  #if ( NEW_AUX_SERIAL_FLAG != 0 ) 
    #define flash_diagnostic_bit(x)                   ( iic_clock         =  (x) )
    #define debounce_diagnostic_bit(x)                ( dummy_bit         = ~(x) )      // VVVV get rid of this, contending outputs
    #define tp_rs232_valid                            dummy_bit
    #define tp_trigger                                dummy_bit
    #define tp1                                       dummy_bit
    #define flicker0_bit                              iic_clock
    #define flicker1_bit                              iic_data
  #endif

/********************************************************************/
/********************************************************************/
/********************************************************************/

  #define tp_flash_enable                      flash_enable

  #define flash_12_volt_turn_off(x)          (  flash_disable = (x) )

/********************************************************************/
/********************************************************************/

#define xUSE_BIT_BANGING_RD_WR_FLAG            yep 

#if defined ( PM2_HARDWARE_FLAG ) 

  #undef  read_lcd_control_register

  #define read_lcd_control_register             0 

  #define pm2_rs232_not_force_off_bit           P0_2

#endif

/********************************************************************/
/********************************************************************/

#if ( NEW_AUX_SERIAL_FLAG == 0 ) 



#endif

/********************************************************************/
/********************************************************************/

#endif

#if ! defined ( __C51__ )

  #define turn_off_interrupts()   /* */
  #define turn_on_interrupts()    /* */
  #define debounce_switches()     /* */

  #define write_lcd_control_register  crap
  #define write_lcd_data_register     crap
  #define flicker0_bit                crap
  #define flicker1_bit                crap
  #define SBUF                        crap
  #define TI                          crap
  #define not_cs_ram                  crap
  #define cs_ram2                     crap
  #define beeper                      crap
  #define tp_trigger                  crap
  #define CS_LATCH_PORT               crap
  #define ab16                        crap
  #define rs422_repeater_flag         crap
  #define proximal_rs422_flag         crap

#endif

/********************************************************************/
/********************************************************************/
  




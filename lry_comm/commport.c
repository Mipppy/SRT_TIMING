
#include <windows.h>
#include <memory.h> // for memcpy()
#include "commbufs.h"
#include "commport.h"
#include "winfile.h"
#include "backgrnd.h"
  
#define WIN_TRANSMIT_FIFO_BUFSIZE      16384L   // Windows Buffer
#define WIN_RECEIVE_FIFO_BUFSIZE       16384L

// the four functions for Ernie
// 

int xxDLL_EXPORT comm_open ( int port, long baudrate, int parity_flag, rx_callback_func process_rx_info )

{           
   BOOL port_is_open = FALSE;

   comm_close();
 
   if ( win_open_file( port ) == TRUE )            

   {
        if ( win_modify_comm_timeouts() == TRUE )
        {
            if ( win_modify_device_ctrl_block( baudrate, (BYTE) parity_flag, ONESTOPBIT ) == TRUE )
            {
                if ( win_modify_event_mask( EV_BREAK | EV_ERR ) == TRUE )
                {
                    create_commbufs( sizeof(read_t), sizeof(write_t) );
                
                    #if ( 0 )  // 4/0901
                      start_read_thread();
                      set_read_processor(  process_rx_info ); 
                      start_write_thread();
                    #endif               

                    port_is_open = TRUE;
                }
                else
                {
                    win_restore_device_ctrl_block();
                }
            }
            else
            {
                win_restore_comm_timeouts();
            }
        }
        else
        {
            win_close_file();
        }
    }

    if ( port_is_open ) {
      win_set_bufsizes( WIN_TRANSMIT_FIFO_BUFSIZE, WIN_RECEIVE_FIFO_BUFSIZE );
      #if ( 1 )  // 4/0901
        start_read_thread();
        set_read_processor(  process_rx_info ); 
        start_write_thread();
      #endif                 
    }
    
    return port_is_open;
}


// XXX need to check results
void xxDLL_EXPORT comm_close( void )
{
    commbuf_flush_receive();
    commbuf_flush_transmit();

    stop_read_thread();
    stop_write_thread();

    destroy_commbufs();
        
    win_restore_event_mask();
    win_restore_device_ctrl_block();
    win_restore_comm_timeouts();
    win_close_file();
}


long xxDLL_EXPORT comm_write_string( unsigned char *ptr_to_tx_info, long length )
{
    return push_array_transmit( ptr_to_tx_info, length );
}

long xxDLL_EXPORT comm_write_byte ( unsigned char tx_byte )
{
        
    return push_array_transmit ( & tx_byte, 1 );
}


int xxDLL_EXPORT comm_read_byte( read_t *p_read_info_struct )
{  
   return pop_item_receive( p_read_info_struct );
}

void xxDLL_EXPORT comm_set_debug_flags ( int local_message_flags, int local_rx_debug_print_to_debug_file, int local_tx_debug_print_to_debug_file, int local_overlapped_io_method ) /* function declaration */

{}

/***********************************************************************************/

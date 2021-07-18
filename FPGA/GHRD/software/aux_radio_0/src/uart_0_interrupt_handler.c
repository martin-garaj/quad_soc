//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//
//==============================================================================================//
//											CREDITS												//
//==============================================================================================//
//																								//
//	derived from previous work by :																//
//		Cheong Min LEE																			//
// 		Email: lcm2559@yahoo.co.kr																//
//																								//
//==============================================================================================//

#include "uart_0_interrupt_handler.h"

#include "system.h"						// FPGA memory mapping
#include "altera_avalon_uart_regs.h"	// low-level uart register functions
#include <stdint.h> 					// uint8_t, uint16_t, uint32_t

// buffer length
#define UART_0_RX_BUFFER_SIZE 1024
#define UART_0_TX_BUFFER_SIZE 1024

// UART_0 (internal) variables
// TX fifo
unsigned short UART_0_TxHead=0;
unsigned short UART_0_TxTail=0;
unsigned char UART_0_tx_buffer[UART_0_TX_BUFFER_SIZE];
// RX fifo
unsigned short UART_0_RxHead=0;
unsigned short UART_0_RxTail=0;
unsigned char UART_0_rx_buffer[UART_0_RX_BUFFER_SIZE];
// UART RS-232 IP core address
uint32_t uart_module_address;


/** Initializes UART_0
 * @param address of the UART RS-232 IP core
 * @param desired baud rate
 * @return void
 */
void uart_0_initialize(uint32_t uart_module_addr, unsigned int BaudRate, unsigned int fpga_freq){
	unsigned int divisor;
	uart_module_address = uart_module_addr;

	divisor = (fpga_freq/BaudRate) +1;
	IOWR_ALTERA_AVALON_UART_DIVISOR(uart_module_address, divisor);
	IOWR_ALTERA_AVALON_UART_CONTROL(uart_module_address, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
}

/** Interrupt Service Routine
 * @param context (pointer to data structure - NOT USED)
 * @param interrupt ID
 * @return void
 */
void uart_0_ISR(void* context, unsigned int id){
	int sr;
	sr = IORD_ALTERA_AVALON_UART_STATUS(uart_module_address);
	if(sr & ALTERA_AVALON_UART_STATUS_RRDY_MSK){
		UART_0_rx_buffer[UART_0_RxHead] = IORD_ALTERA_AVALON_UART_RXDATA(uart_module_address);
		IOWR_ALTERA_AVALON_UART_STATUS(uart_module_address, 0);
		if (++UART_0_RxHead > (UART_0_RX_BUFFER_SIZE-1)) UART_0_RxHead = 0;
	}
	if(sr & ALTERA_AVALON_UART_STATUS_TRDY_MSK){
		if(IORD_ALTERA_AVALON_UART_CONTROL(uart_module_address) & ALTERA_AVALON_UART_CONTROL_TRDY_MSK){
			if (UART_0_TxTail != UART_0_TxHead){
				IOWR_ALTERA_AVALON_UART_TXDATA(uart_module_address, UART_0_tx_buffer[UART_0_TxTail]);
				if (++UART_0_TxTail > (UART_0_TX_BUFFER_SIZE -1)) UART_0_TxTail = 0;
			}else{
				IOWR_ALTERA_AVALON_UART_CONTROL(uart_module_address, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
			}
		}
	}
}

/** Check whether the RX fifo is empty
 * @return empty (1) / full (at least 1 char inside) (0)
 */
unsigned char uart_0_fifo_empty(){

	if(UART_0_RxHead == UART_0_RxTail){
		return 1;
	}
	return 0;
}


/** Fetch a char from RX fifo
 * @return char from RX fifo
 */
unsigned char uart_0_get_fifo_char(void){
	unsigned char rxChar;

	/* buffer is empty */
	rxChar=UART_0_rx_buffer[UART_0_RxTail];
	if (++UART_0_RxTail > (UART_0_RX_BUFFER_SIZE-1)) UART_0_RxTail=0;

	return rxChar;
}


/** Sends a char by UARTs TX line
 * @param char to be sent
 * @return success (1) / failed (-1)
 */
unsigned char uart_0_write_char(unsigned char in_char){
	unsigned short size;
	unsigned int z;

	z = IORD_ALTERA_AVALON_UART_STATUS(uart_module_address) & ALTERA_AVALON_UART_STATUS_TRDY_MSK;

	if ((UART_0_TxHead==UART_0_TxTail) && z){
		IOWR_ALTERA_AVALON_UART_TXDATA(uart_module_address, in_char);
	}else{
		if (UART_0_TxHead >= UART_0_TxTail){
			size = UART_0_TxHead - UART_0_TxTail;
		}else{
			size = ((UART_0_TX_BUFFER_SIZE-1) - UART_0_TxTail) + UART_0_TxHead;
		}

		if (size > (UART_0_TX_BUFFER_SIZE - 3)) return (-1);

		UART_0_tx_buffer[UART_0_TxHead] = in_char;

		if (++UART_0_TxHead > (UART_0_TX_BUFFER_SIZE-1)) UART_0_TxHead = 0;

		z = IORD_ALTERA_AVALON_UART_CONTROL(uart_module_address) | ALTERA_AVALON_UART_CONTROL_TRDY_MSK;
		IOWR_ALTERA_AVALON_UART_CONTROL(uart_module_address, z);
	}
	return(1);
}

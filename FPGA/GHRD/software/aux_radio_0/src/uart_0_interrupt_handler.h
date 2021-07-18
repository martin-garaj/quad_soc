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

#include <stdint.h> 		// uint8_t, uint16_t, uint32_t


void uart_0_initialize(uint32_t uart_module_addr, unsigned int BaudRate, unsigned int fpga_freq);
void uart_0_ISR(void* context, unsigned int id);
unsigned char uart_0_fifo_empty();
unsigned char uart_0_get_fifo_char(void);
unsigned char uart_0_write_char(unsigned char in_char);


//============================================================================
// Name        : MPSO_PVP.cpp
// Authors     : Roberto Fernandez Molanes 	(robertofem@gmail.com)
//				 Martin Garaj 				(garaj.martin@gmail.com)
//				 Jianfeng Zhou				(599102934@qq.com)
// Version     : 1.0 (February 2017)
// Copyright   : -
// Description : This files define a class to easily control the UART_module
//				 component in the FPGA. The UART_module is capable of transmitting
//				 and receiving 1 byte at the time. No FIFO buffer is present, as
//				 for the current application.
//				 The communication is based on POLLING, the flags for receiving and
//				 transmitting needs to be checked prior to reading/writing to RX
//				 and TX registers.
//============================================================================

#ifndef API_UART_MODULE_HPP_
#define API_UART_MODULE_HPP_

// DEBUG
#include <iostream>

#include <stdio.h> 	// memcpy
#include <stdint.h> // uint8_t, uint16_t
#include "system_const.h"

//Macros to decode the Avalon slave registers in UART_module
// registers are 16 bits long
#define RXDATA 			0
#define TXDATA 			1
#define STATUS 			2
#define CONTROL 		3
#define DIVISOR 		4
#define ENDOFPACKET 	5

//masks to access the STATUS bits
#define PE		0x0001   //	parity error
#define FE		0x0002   // framing error
#define BRK		0x0004   //	break detect
#define ROE		0x0008   // receive overrun error
#define TOE		0x0010   // transmit overrun error
#define TMT		0x0020   // transmit empty - if(TMT == 1) then transmitted byte was successfully received by receiver
#define TRDY	0x0040   // transmit ready - if(TRDY == 1) then UART is ready for a new character
#define RRDY	0x0080   // received char ready - if(rrdy == 1) then newly received character is in RXDATA
#define E		0x0100   // exception - The E bit is a logical-OR of the TOE, ROE, BRK, FE, and PE bits
// not used		0x0200
#define DCTS	0x0400   // change in clear to send (CTS) signal
#define CTS		0x0800   // clear-to-send (CTS) signal
#define EOP		0x1000   // end-of-packet encountered

//masks to access the CONTROL bits
#define IPE		0x0001   // enable interrupt for a parity error
#define IFE		0x0002   // enable interrupt for a framing error
#define IBRK	0x0004   // enable interrupt for a break detect
#define IROE	0x0008   // enable interrupt for a receiver overrun error
#define ITOE	0x0010   // enable interrupt for a transmitter overrun error
#define ITMT	0x0020   // enable interrupt for a transmitter shift register empty
#define ITRDY	0x0040   // enable interrupt for a transmission ready
#define IRRDY	0x0080   // enable interrupt for a read ready
#define IE		0x0100   // enable interrupt for an exception
#define TRBK	0x0200   // transmit break - if(TRBK == 1) then the TXD pin is set LOW until (TRBK == 0)
#define IDCTS	0x0400   // enable interrupt for a change in CTS signal
#define RTS		0x0800   // request to send (RTS) signal
#define IEOP	0x1000   // enable interrupt for end-of-packet condition

//values of baud rates
#define BR_115200	115200
#define BR_57600	57600
#define BR_38400	38400
#define BR_31250	31250
#define BR_28800	28800
#define BR_19200	19200
#define BR_14400	14400
#define BR_9600		9600
#define BR_4800		4800
#define BR_2400		2400
#define BR_1200		1200
#define BR_300		300


/* CLASS DECLARATION */
class API_UART_module
{
// VARIABLES //
private:
	uint32_t* uart_module_addr; //base addresses of UART_module
public:
	static void *virtual_base;
	static int instance_counter;

// FUNCTIONS //
public:
	//constructor and destructor
	API_UART_module(uint32_t * uart_module_address);
	~API_UART_module();

// access UART_module
	// set baude_rate
	void set_baude_rate(int baude_rate, int clock_freq);
	// resets errors and interrupts
	void reset_module(void);
	// check for logical-OR of the TOE, ROE, BRK, FE, and PE bits
	int check_error(void);
	// reset all error bits
	void reset_error(void);
	// check UART_module ready for TRANSMISSION
	int check_TRDY(void);
	// transmit byte
	void write_byte(uint8_t byte);
	// check UART_module RECEIVED byte
	int check_RRDY(void);
	// read byte
	uint8_t read_byte(void);

	// check UART_module ready for TRANSMISSION with TIMEOUT
	int check_TRDY_timeout(int timeout_limit);
	// check UART_module RECEIVED byte with TIMEOUT
	int check_RRDY_timeout(int timeout_limit);

};

void * API_UART_module::virtual_base = NULL;
int API_UART_module::instance_counter = 0;

// DEFINITIONS //

API_UART_module::API_UART_module(uint32_t * uart_module_address){
#ifdef TESTING_UART

#else // REAL CODE
	//set pointer
	uart_module_addr = uart_module_address;
#endif
}


API_UART_module::~API_UART_module(){
#ifdef TESTING_UART

#else // REAL CODE

#endif
}


void API_UART_module::set_baude_rate(int baude_rate, int clock_freq){
#ifdef TESTING_UART

#else // REAL CODE
	// formula 		BAUDE_RATE = clock_freq/(divider+1)
	uart_module_addr[DIVISOR] = (clock_freq/baude_rate)-1;
#endif
}


int API_UART_module::check_error(void){
#ifdef TESTING_UART
	return RETURN_NO_ERROR;
#else // REAL CODE
	uint16_t status;
	status = uart_module_addr[STATUS];

	if( (status & E) == E){
		return 1;
	}else{
		return 0;
	}
#endif
}


int API_UART_module::check_TRDY(void){
#ifdef TESTING_UART
	return RETURN_SUCCESS;
#else // REAL CODE
	uint16_t status;
	status = uart_module_addr[STATUS];

	if( (status & TRDY) == TRDY ){
		return 1;
	}else{
		return 0;
	}
#endif
}


int API_UART_module::check_RRDY(void){
#ifdef TESTING_UART
	return RETURN_SUCCESS;
#else // REAL CODE
	uint16_t status;
	status = uart_module_addr[STATUS];

	if( (status & RRDY) == RRDY ){
		return 1;
	}else{
		return 0;
	}
#endif
}


void API_UART_module::reset_module(void){
#ifdef TESTING_UART

#else // REAL CODE
	uart_module_addr[CONTROL] = 0x00000000;
	uart_module_addr[STATUS] =  0x00000000;
#endif
}


void API_UART_module::reset_error(void){
#ifdef TESTING_UART

#else // REAL CODE
	uart_module_addr[STATUS] =  0x00000000;
#endif
}


uint8_t API_UART_module::read_byte(void){
#ifdef TESTING_UART
	return 0x55;
#else // REAL CODE
	return (uint8_t)( uart_module_addr[RXDATA] & 0x00FF );
#endif
}


void API_UART_module::write_byte(uint8_t byte){
#ifdef TESTING_UART

#else // REAL CODE
	uart_module_addr[TXDATA] = byte;
#endif
}


int API_UART_module::check_TRDY_timeout(int timeout_limit){
#ifdef TESTING_UART
	return RETURN_SUCCESS;
#else // REAL CODE
	uint16_t status;
	int timeout_counter;
	bool timeout;

	// initialization
	timeout_counter = 0;
	timeout = false;

	while(!timeout){
		status = uart_module_addr[STATUS];
		if( (status & TRDY) == TRDY ){
			return 1;
		}
		if(timeout_counter >= timeout_limit){
			return 0;
		}
		timeout_counter++;
	}

	return 0;
#endif
}


int API_UART_module::check_RRDY_timeout(int timeout_limit){
#ifdef TESTING_UART
	return RETURN_SUCCESS;
#else // REAL CODE
	uint16_t status;
	int timeout_counter;
	bool timeout;

	// initialization
	timeout_counter = 0;
	timeout = false;

	while(!timeout){
		status = uart_module_addr[STATUS];
		if( (status & RRDY) == RRDY ){
			return 1;
		}
		if(timeout_counter >= timeout_limit){
			return 0;
		}
		timeout_counter++;
	}
	return 0;
#endif
}


#endif /* API_UART_MODULE_HPP_ */

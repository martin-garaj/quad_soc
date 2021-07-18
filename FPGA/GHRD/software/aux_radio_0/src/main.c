//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//


//==============================================================================================//
//											COMMENTS											//
//==============================================================================================//
// LED Patterns

// | ON | ON | ON | - Reset state
#define RESET_STATE			(0b00000111)
// | OFF| ON | OFF| - Initialization done
#define INITIALIZATION		(0b00000010)
// | ON | OFF| ON | - Packet's checksum correct
#define CHECKSUM_CORRECT	(0b00000101)
// | ON | OFF| ON | - Packet's checksum incorrect
#define CHECKSUM_INCORRECT	(0b00000011)
// | ON | OFF| OFF| - Stalling
#define STALLING			(0b00000100)

//==============================================================================================//
//											#INCLUDEs											//
//==============================================================================================//
#include <string.h>
#include <unistd.h>			// usleep()
#include <io.h>				// low-level Avalon MM access : IOWR_8DIRECT(), ...
#include "system.h"			// FPGA memory mapping
#include "sys/alt_irq.h"	// interrupt NIOS II functions

#include "config.h"						// configuration file
#include "uart_0_interrupt_handler.h"	// low-level uart register functions
#include "mem_functions.h"		// shared memory functions2

#include "AUX_RADIO_constants.h"		// shared memory composition & constants
#include "AUX_RADIO.h" 	// sensor function declarations
//==============================================================================================//
//											#DEFINEs											//
//==============================================================================================//


//==============================================================================================//
//											MAIN												//
//==============================================================================================//
int main(){
	//==============================================================================================//
	//											DECLARATIONS										//
	//==============================================================================================//
	// CONSTANTS
	aux_radio_memory_map radio;
	init_aux_radio(&radio, AUX_RADIO_0_MEM_BASE);

	// VARIABLES
	// UART variables for Radio communication
	unsigned char byte = 0x00;
	unsigned char previous_byte = 0x00;
	unsigned char checksum = 0x00;
	int packet_pointer = 0;
	// Memory pointer
	int pointer = 0;
	// Data delivered to AUX_IMU
	// UART baud-rate
	uint32_t UART_baude_rate = 0;
	// number of channels
	uint32_t number_of_channels = 0;			// NOT USED IN CURRENT DESIGN
	// check-sum initial value
	uint32_t checksum_init = 0;
	// control_reg value
	uint32_t control_reg = 0;
	uint32_t routine_control_reg = 0;
	// bool for initialization, to prevent repeating
	bool init_done = false;

	// FUNCTION CALLS
	// register UART interrupt
	void* uart_0_context;
	alt_ic_isr_register(	UART_0_IRQ_INTERRUPT_CONTROLLER_ID,
							UART_0_IRQ,
							uart_0_ISR,
							uart_0_context,
							NULL);
	alt_ic_irq_enable(	UART_0_IRQ_INTERRUPT_CONTROLLER_ID,
						UART_0_IRQ);

#ifdef _DEBUG_
	alt_printf("AUX_RADIO\n");
#endif
	//==============================================================================================//
	//												MAIN LOOP										//
	//==============================================================================================//
	while(true){
		control_reg = get_control_reg(radio);
		//==============================================================================================//
		//												RESET STATE										//
		//==============================================================================================//
		if(	!((control_reg&radio.control.reset_n)==radio.control.reset_n) ){

			// Routine control register
			routine_control_reg = 0;
			// UART variables for Radio communication
			byte = 0x00;
			previous_byte = 0x00;
			checksum = 0x00;
			packet_pointer = 0;
			// Memory pointer
			pointer = 0;
			// hps2aux data
			// UART baud-rate
			UART_baude_rate = 0;
			// number of channels
			number_of_channels = 0;
			// check-sum initial value
			checksum_init = 0;
			// Initialization bool
			init_done = false;

			// set status flag : reset
			set_status_flag_reset(radio);
			// Reset state
			SET_32REGISTER(AUX_RADIO_0_LED_BASE, 0x00, RESET_STATE);
#ifdef _DEBUG_
			alt_printf("R");
#endif

		//==============================================================================================//
		//											EXECUTION STATE										//
		//==============================================================================================//
		}else if( (control_reg & radio.control.enable)==radio.control.enable ){
			// set status flag : enable
			set_status_flag_enable(radio);
#ifdef _DEBUG_
			alt_printf("E");
#endif
			//==============================================================================================//
			//											INITIALIZATION										//
			//==============================================================================================//
			if( !init_done ){
#ifdef _DEBUG_
			alt_printf("Initialization ...");
#endif
				// Receive initialization data
				// UART baud-rate
				UART_baude_rate = get_hps2aux_reg(radio, 0);
				// number of channels
				number_of_channels = get_hps2aux_reg(radio, 1);
				// check-sum initial value
				checksum_init = get_hps2aux_reg(radio, 2);

				// UART initialization
				uart_0_initialize(UART_0_BASE, UART_baude_rate, ALT_CPU_FREQ);

				// set initialization bool, to stop looping
				init_done = true;

				// set status flag : init
				set_status_flag_init(radio);
				// Initialization
				SET_32REGISTER(AUX_RADIO_0_LED_BASE, 0x00, INITIALIZATION);
#ifdef _DEBUG_
			alt_printf("Initialization DONE");
#endif
			//==============================================================================================//
			//												ROUTINES										//
			//==============================================================================================//
			}else{
				routine_control_reg = get_routine_control_reg(radio);
				//==============================================================================================//
				//										GENERAL ROUTINE											//
				//==============================================================================================//
				// this routine is executed if AUX is not in reset and is enabled
#ifdef _DEBUG_
			alt_printf("GR");
#endif
				//
				// ENTER ROUTINE HERE
				//

				//==============================================================================================//
				//										ENABLE-SPECIFIC ROUTINE									//
				//==============================================================================================//
				// routines executed only when specific enable-bit is set
					// RADIO ROUTINE-------------------------------------------------------//
					if((routine_control_reg & radio.routine.control.enable[0]) == radio.routine.control.enable[0]){
						set_routine_status_flag_enable(radio, 0);
						// check RX fifo
						if(!uart_0_fifo_empty()) {
							// new char received
							previous_byte = byte;
							byte = uart_0_get_fifo_char();
							packet_pointer++;

							// detect start of the radio packet
							if( (previous_byte == 0x20) && (byte == 0x40) ){
								packet_pointer = 1; // previous byte was the first one, with byte order 0
								checksum = checksum_init;	// initial value of checksum is ~0x20 = 0xDF
							};

							// current_char inside radio packet
							if( (packet_pointer>=1) && (packet_pointer<=29) ){
								// calculate checksum
								checksum = checksum + (~byte) + 1;

								// bytes including CHANNEL values
								if( (packet_pointer>=2) && (packet_pointer<=21) ){
									// store in shared memory
									pointer = packet_pointer - 2;
									IOWR_8DIRECT(radio.base_address, radio.routine.packet.offset[0] + pointer, byte);
								}
							}

							// checksum control, status register update, set LED
							if( (packet_pointer==30) ){
								if (checksum == byte){
#ifdef _DEBUG_
			alt_printf("r+");
#endif
									set_routine_status_flag_valid(radio, 0);
									set_routine_status_flag_pkt_rdy(radio, 0);
									// set LED | ON | ON |
									SET_32REGISTER(AUX_RADIO_0_LED_BASE, 0x00, CHECKSUM_CORRECT);
								}else{
#ifdef _DEBUG_
			alt_printf("r-");
#endif
									clear_routine_status_flag_valid(radio, 0);
									set_routine_status_flag_pkt_rdy(radio, 0);
									// set LED | ON | OFF|
									SET_32REGISTER(AUX_RADIO_0_LED_BASE, 0x00, CHECKSUM_INCORRECT);
								}
							}
						}
					}
					// OTHER ROUTINE-------------------------------------------------------//
					// other routine goes below this line

					//
					// ENTER ROUTINE HERE
					//
			}
			// NO CODE BELOW THIS LINE---------------------------------------------//

		//==============================================================================================//
		//												STALLING										//
		//==============================================================================================//
		}else{
#ifdef _DEBUG_
			alt_printf("S");
#endif
			clear_status_flag_enable(radio);
			// set LED | OFF| OFF|
			SET_32REGISTER(AUX_RADIO_0_LED_BASE, 0x00, STALLING);
		}// control : enable
#ifdef _DEBUG_
			alt_printf("\n");
#endif
	}// while
	return 0;
}// main

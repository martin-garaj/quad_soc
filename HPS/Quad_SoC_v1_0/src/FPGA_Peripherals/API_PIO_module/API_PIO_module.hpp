//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Diagnostic system of photo-voltaic panels           //
//                                                                //
//================================================================//
#ifndef _API_PIO_MODULE_HPP_
#define _API_PIO_MODULE_HPP_

// DEBUG
#include <iostream>
// others
#include "system_const.h"
#include <stdio.h> 	// memcpy
#include <stdint.h> // uint8_t, uint16_t

// Macros to decode the Avalon slave registers in PIO
// register length depends on PIO settings
#define PIO_DATA 			0
#define PIO_DIRECTION 		1
#define PIO_INTERRUPTMASK	2
#define PIO_EDGECAPTURE 	3
#define PIO_OUTSET 			4
#define PIO_OUTCLEAR 		5

//================================================================================================//
//                                                                								  //
//	CLASS	: PIO_module represents a Parallel Input/Output IP core in FPGA and provides 		  //
// 			  an interface for low-level interaction with this object.             				  //
//                                                          								      //
//================================================================================================//
class API_PIO_module{

// VARIABLES //
private:
	uint32_t* pio_module; //base addresses of PIO_module

// FUNCTIONS //
public:
	//constructor and destructor
	API_PIO_module(uint32_t * pio_module_addr);
	~API_PIO_module();

	// set outputs
	void set(uint8_t value);
	// get outputs
	uint8_t get();
	// resets errors and interrupts
	void reset(void);

};

//----------------------------------------------------------------DEFINITIONS : PUBLIC----------------------------------------------------------------//
/** Constructor
 * @param program address of PIO IP Core
 * @return object of type FPGA_ACCESS
 */
API_PIO_module::API_PIO_module(uint32_t * pio_module_addr){
	this->pio_module = pio_module_addr;
};

/** Destructor
 */
API_PIO_module::~API_PIO_module(){

};

/** Data setter
 * @param value to be assigned to internal data register of PIO IP Core
 */
void API_PIO_module::set(uint8_t value){
	pio_module[PIO_DATA] = value;
}

/** Data getter
 * @return returns value of internal data register of PIO IP Core
 */
uint8_t API_PIO_module::get(){
	return pio_module[PIO_DATA];
}

/** Reset of internal data register of PIO IP Core
 */
void API_PIO_module::reset(){
	pio_module[PIO_DATA] = 0x00;
}

#endif /* _API_PIO_MODULE_HPP_ */

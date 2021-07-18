//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//
#ifndef SRC_FPGA_API_SHARED_MEMORY_HPP_
#define SRC_FPGA_API_SHARED_MEMORY_HPP_
//==============================================================================================//
//											INCLUDES											//
//==============================================================================================//
#include <stdint.h>		// uint8_t


//==============================================================================================//
//												CLASS											//
//==============================================================================================//
//																								//
//	UART_module represents an UART (RS-232 Serial Port) IP Core in FPGA and 					//
// 	provides an interface for low-level interaction with this object.       		  	   		//
//						                                                      					//
//==============================================================================================//
class API_mem
{
// VARIABLES //
public:

// FUNCTIONS //
public:
	//constructor and destructor
	API_mem(){};
	API_mem(uint32_t * memory_virtual_address);
	~API_mem(){};

	// read from intermediate memory
	int read_dword(uint32_t * data, uint32_t offset, uint32_t length);
	int read_word(uint16_t * data, uint32_t offset, uint32_t length);
	int read_byte(uint8_t * data, uint32_t offset, uint32_t length);

	// write to intermediate memory
	int write_dword(uint32_t * data, uint32_t offset, uint32_t length);
	int write_word(uint16_t * data, uint32_t offset, uint32_t length);
	int write_byte(uint8_t * data, uint32_t offset, uint32_t length);

	// bit-wise access to registers
	bool check_bit(uint32_t reg_ofst, uint32_t bit);
	int set_bit(uint32_t reg_ofst, uint32_t bit);
	int clear_bit(uint32_t reg_ofst, uint32_t bit);

// VARIABLES
private:
	uint32_t* mem_addr;

};



#endif /* API_INTERMEDIATE_MEM_HPP_ */

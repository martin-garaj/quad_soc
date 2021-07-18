//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//


//==============================================================================================//
//											INCLUDES											//
//==============================================================================================//
#include "API_mem.hpp"
#include "system_const.h"
#include <string.h> // memcpy()


//----------------------------------------------------------------DEFINITIONS : PUBLIC----------------------------------------------------------------//
/** Constructor
 * @param program address of SHARED MEMORY IP Core
 * @return address of SHARED MEMORY in HPS program space
 */
API_mem::API_mem(uint32_t * memory_virtual_address){
	mem_addr = memory_virtual_address;
}

// read from intermediate memory
int API_mem::read_dword(uint32_t * data, uint32_t offset, uint32_t length){
	memcpy((void *) (data), (void *) (mem_addr+offset), (length * DWORD_SIZE) );
	return RETURN_SUCCESS;
}

int API_mem::read_word(uint16_t * data, uint32_t offset, uint32_t length){
	memcpy((void *) (data), (void *) (mem_addr+offset), (length * WORD_SIZE) );
	return RETURN_SUCCESS;
}

int API_mem::read_byte(uint8_t * data, uint32_t offset, uint32_t length){
	memcpy((void *) (data), (void *) (mem_addr+offset), (length * BYTE_SIZE) );
	return RETURN_SUCCESS;
}


// write to intermediate memory
int API_mem::write_dword(uint32_t * data, uint32_t offset, uint32_t length){
	memcpy((void *) (mem_addr+offset), (void *) (data), (length * DWORD_SIZE) );
	return RETURN_SUCCESS;
}

int API_mem::write_word(uint16_t * data, uint32_t offset, uint32_t length){
	memcpy((void *) (mem_addr+offset), (void *) (data), (length * WORD_SIZE) );
	return RETURN_SUCCESS;
}

int API_mem::write_byte(uint8_t * data, uint32_t offset, uint32_t length){
	memcpy((void *) (mem_addr+offset), (void *) (data), (length * BYTE_SIZE) );
	return RETURN_SUCCESS;
}


// bit-wise access to registers
bool API_mem::check_bit(uint32_t reg_ofst, uint32_t bit){
	if( ( mem_addr[reg_ofst] & bit) == bit){
		return true;
	};
	return false;
}

int API_mem::set_bit(uint32_t reg_ofst, uint32_t bit){
	uint32_t register_value;
	register_value = mem_addr[reg_ofst];
	mem_addr[reg_ofst] = (register_value | bit);
	return RETURN_SUCCESS;
}

int API_mem::clear_bit(uint32_t reg_ofst, uint32_t bit){
	uint32_t register_value;
	register_value = mem_addr[reg_ofst];
	mem_addr[reg_ofst] = (register_value & (~bit));
	return RETURN_SUCCESS;
}

//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//

#include "stdbool.h"				// bool type
#include <io.h>						// low level Avalon access macros IORD_8DIRECT(), IOWR_8DIRECT(), ...
#include <stdint.h> 				// uint8_t, uint16_t, uint32_t
#include <string.h> 				// memcpy()

bool CHECK_BIT(uint32_t base_address, uint32_t reg_byte_offset, uint32_t bit){
	if( (IORD_32DIRECT(base_address, reg_byte_offset) & bit) == bit){
		return true;
	};
	return false;
};

void SET_BIT(uint32_t base_address, uint32_t reg_byte_offset, uint32_t bit){
	uint32_t register_value;
	register_value = IORD_32DIRECT(base_address, reg_byte_offset);
	IOWR_32DIRECT(base_address, reg_byte_offset, (register_value | bit) );
};

void CLEAR_BIT(uint32_t base_address, uint32_t reg_byte_offset, uint32_t bit){
	uint32_t register_value;
	register_value = IORD_32DIRECT(base_address, reg_byte_offset);
	IOWR_32DIRECT(base_address, reg_byte_offset, (register_value & (~bit)) );
};



int32_t GET_32REGISTER(uint32_t base_address, uint32_t reg_byte_offset){
	return IORD_32DIRECT(base_address, reg_byte_offset);
};

void SET_32REGISTER(uint32_t base_address, uint32_t reg_byte_offset, int32_t value){
	IOWR_32DIRECT(base_address, reg_byte_offset, value );
};



void STORE_8BIT_DATA(uint32_t base_address, uint32_t byte_offset, uint16_t length, int8_t* data){
	memcpy((void *)base_address+byte_offset,(void *) data, length*sizeof(int8_t));
};

void STORE_16BIT_DATA(uint32_t base_address, uint32_t byte_offset, uint16_t length, int16_t* data){
	memcpy((void *) base_address+byte_offset,(void *) data, length*sizeof(int16_t));
};

void STORE_32BIT_DATA(uint32_t base_address, uint32_t byte_offset, uint16_t length, int32_t* data){
	memcpy((void *) base_address+byte_offset,(void *) data, length*sizeof(int32_t));
};



void READ_8BIT_DATA(uint32_t base_address, uint32_t byte_offset, uint16_t length, int8_t* data){
	memcpy((void *) data, (void *)base_address+byte_offset, length*sizeof(int8_t));
};

void READ_16BIT_DATA(uint32_t base_address, uint32_t byte_offset, uint16_t length, int16_t* data){
	memcpy((void *) data, (void *) base_address+byte_offset, length*sizeof(int16_t));
};

void READ_32BIT_DATA(uint32_t base_address, uint32_t byte_offset, uint16_t length, int32_t* data){
	memcpy((void *) data, (void *) base_address+byte_offset, length*sizeof(int32_t));
};

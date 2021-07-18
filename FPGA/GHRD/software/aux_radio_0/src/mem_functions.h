//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//

#ifndef MEM_FUNCTIONS_H_
#define MEM_FUNCTIONS_H_


#include "stdbool.h"		// bool type
#include <stdint.h> 		// uint8_t, uint16_t, uint32_t


//==============================================================================================//
//							NIOS II SHARED MEMORY FUNCTIONS										//
//==============================================================================================//
bool 	CHECK_BIT		(uint32_t base_address, uint32_t reg_byte_offset, int32_t bit);
void 	SET_BIT			(uint32_t base_address, uint32_t reg_byte_offset, int32_t bit);
void 	CLEAR_BIT		(uint32_t base_address, uint32_t reg_byte_offset, int32_t bit);

int32_t	GET_32REGISTER	(uint32_t base_address, uint32_t reg_byte_offset);
void 	SET_32REGISTER	(uint32_t base_address, uint32_t reg_byte_offset, int32_t value);

void 	STORE_8BIT_DATA	(uint32_t base_address, uint32_t reg_byte_offset, uint16_t length, int8_t* data);
void 	STORE_16BIT_DATA(uint32_t base_address, uint32_t reg_byte_offset, uint16_t length, int16_t* data);
void 	STORE_32BIT_DATA(uint32_t base_address, uint32_t reg_byte_offset, uint16_t length, int32_t* data);

void 	READ_8BIT_DATA	(uint32_t base_address, uint32_t reg_byte_offset, uint16_t length, int8_t* data);
void 	READ_16BIT_DATA	(uint32_t base_address, uint32_t reg_byte_offset, uint16_t length, int16_t* data);
void 	READ_32BIT_DATA	(uint32_t base_address, uint32_t reg_byte_offset, uint16_t length, int32_t* data);


#endif /* MEM_FUNCTIONS_H_ */
/*
 * AUX_IMU.h
 *
 *  Created on: Jan 14, 2018
 *      Author: mgaraj2
 */


#ifndef AUX_RADIO_H_
#define AUX_RADIO_H_

#include "config_AUX_RADIO.h"
#include "AUX_RADIO_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================================//
//					MEMORY STRUCTURE - AUXILIARY CONTROLLER for RADIO ROUTINE					//
//==============================================================================================//
typedef struct aux_radio_memory_map{
	uint32_t base_address;
	aux_radio_control 	control;
	aux_radio_status 		status;
	aux_radio_hps2aux 	hps2aux;
	aux_radio_aux2hps 	aux2hps;
	aux_radio_routine 	routine;
} aux_radio_memory_map;

#if defined(AUX_PLATFORM) || defined(HPS_PLATFORM)
// Init
void init_aux_radio(aux_radio_memory_map * radio, uint32_t aux_radio_base_address);
#endif

#ifdef AUX_PLATFORM
// Control register
int32_t get_control_reg(aux_radio_mem radio);
// Status register
uint8_t set_status_reg(aux_radio_mem radio, int32_t value);
uint8_t set_status_flag_reset(aux_radio_mem radio);
uint8_t set_status_flag_enable(aux_radio_mem radio);
uint8_t set_status_flag_init(aux_radio_mem radio);

uint8_t clear_status_reg(aux_radio_mem radio);
uint8_t clear_status_flag_reset(aux_radio_mem radio);
uint8_t clear_status_flag_enable(aux_radio_mem radio);
uint8_t clear_status_flag_init(aux_radio_mem radio);
// hps2aux
int32_t  get_hps2aux_reg(aux_radio_mem radio, uint8_t which);
// aux2hps
uint8_t set_hps2aux_reg(aux_radio_mem radio, uint8_t which, int32_t data);
uint8_t clear_hps2aux_reg(aux_radio_mem radio, uint8_t which);

// Routine - Control
int32_t get_routine_control_reg(aux_radio_mem radio);
// Routine - Status
uint8_t set_routine_status_flag_enable(aux_radio_mem radio, uint8_t which);
uint8_t set_routine_status_flag_pkt_rdy(aux_radio_mem radio, uint8_t which);
uint8_t set_routine_status_flag_valid(aux_radio_mem radio, uint8_t which);
uint8_t clear_routine_status_flag_enable(aux_radio_mem radio, uint8_t which);
uint8_t clear_routine_status_flag_pkt_rdy(aux_radio_mem radio, uint8_t which);
uint8_t clear_routine_status_flag_valid(aux_radio_mem radio, uint8_t which);
// Routine - Packet
uint8_t store_packet8(aux_radio_mem radio, uint8_t which, uint16_t length, int8_t* data);
uint8_t store_packet16(aux_radio_mem radio, uint8_t which, uint16_t length, int16_t* data);
uint8_t store_packet32(aux_radio_mem radio, uint8_t which, uint16_t length, int32_t* data);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* AUX_RADIO_H_ */






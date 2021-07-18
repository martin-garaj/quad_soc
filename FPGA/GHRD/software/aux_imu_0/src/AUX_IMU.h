/*
 * AUX_IMU.h
 *
 *  Created on: Jan 14, 2018
 *      Author: mgaraj2
 */


#ifndef AUX_IMU_H_
#define AUX_IMU_H_

#include "AUX_IMU_struct.h"

#ifdef __cplusplus
extern "C" {
#endif

//==============================================================================================//
//					MEMORY STRUCTURE - AUXILIARY CONTROLLER for IMU ROUTINE						//
//==============================================================================================//
typedef struct aux_imu_memory_map{
	uint32_t base_address;
	aux_imu_control 	control;
	aux_imu_status 		status;
	aux_imu_hps2aux 	hps2aux;
	aux_imu_aux2hps 	aux2hps;
	aux_imu_routine 	routine;
} aux_imu_memory_map;

#if defined(AUX_PLATFORM) || defined(HPS_PLATFORM)
// Init
void init_aux_imu(aux_imu_memory_map * imu, uint32_t aux_imu_base_address);
#endif

#ifdef AUX_PLATFORM
// Control register
int32_t get_control_reg(aux_imu_memory_map imu);
// Status register
uint8_t set_status_reg(aux_imu_memory_map imu, int32_t value);
uint8_t set_status_flag_reset(aux_imu_memory_map imu);
uint8_t set_status_flag_enable(aux_imu_memory_map imu);
uint8_t set_status_flag_init(aux_imu_memory_map imu);

uint8_t clear_status_reg(aux_imu_memory_map imu);
uint8_t clear_status_flag_reset(aux_imu_memory_map imu);
uint8_t clear_status_flag_enable(aux_imu_memory_map imu);
uint8_t clear_status_flag_init(aux_imu_memory_map imu);
// hps2aux
int32_t  get_hps2aux_reg(aux_imu_memory_map imu, uint8_t which);
// aux2hps
uint8_t set_hps2aux_reg(aux_imu_memory_map imu, uint8_t which, int32_t data);
uint8_t clear_hps2aux_reg(aux_imu_memory_map imu, uint8_t which);

// Routine - Control
int32_t get_routine_control_reg(aux_imu_memory_map imu);
// Routine - Status
uint8_t set_routine_status_flag_enable(aux_imu_memory_map imu, uint8_t which);
uint8_t set_routine_status_flag_pkt_rdy(aux_imu_memory_map imu, uint8_t which);
uint8_t clear_routine_status_flag_enable(aux_imu_memory_map imu, uint8_t which);
uint8_t clear_routine_status_flag_pkt_rdy(aux_imu_memory_map imu, uint8_t which);
// Routine - Packet
uint8_t store_packet8(aux_imu_memory_map imu, uint8_t which, uint16_t length, int8_t* data);
uint8_t store_packet16(aux_imu_memory_map imu, uint8_t which, uint16_t length, int16_t* data);
uint8_t store_packet32(aux_imu_memory_map imu, uint8_t which, uint16_t length, int32_t* data);
#endif

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* AUX_IMU_H_ */

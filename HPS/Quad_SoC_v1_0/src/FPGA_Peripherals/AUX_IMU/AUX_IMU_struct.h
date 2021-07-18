/*
 * AUX_IMU.h
 *
 *  Created on: Jan 14, 2018
 *      Author: mgaraj2
 */

#ifndef AUX_IMU_STRUCT_H_
#define AUX_IMU_STRUCT_H_


#include "config_AUX_IMU.h"
#include "AUX_IMU_constants.h"
#include <stdint.h> 				// uint8_t, uint16_t, uint32_t
/*	
	(struct) aux_imu_mem
		base_address
		(reg) control
			offset
			(bit) reset_n
			(bit) enable
		(reg) status
			offset
			(bit) flag_reset
			(bit) flag_enable
			(bit) flag_init
		(struct) hps2aux
			offset
			length
		(struct) aux2hps
			offset
			length
		(struct) routine
			count
			(reg) control
				offset
				(bit) enable [count]
			(reg) status
				offset
				(bit) flag_enable [count]
				(bit) flag_pkt_rdy [count]
				(bit) flag_valid [count]
			(struct) packet
				offset [count]
				length
*/
//==============================================================================================//
//											CHECK FORMAT										//
//==============================================================================================//
#ifdef BYTE_FORMAT
	#define AUX_IMU_FORMAT	(4)
#endif

#ifdef DWORD_FORMAT
	#define AUX_IMU_FORMAT	(1)
#endif
//==============================================================================================//
//										CONTROL REGISTER										//
//==============================================================================================//
//																								//
//	The Control Register is 32-bit long register to control the Auxiliary Controller.			//
//																								//
//==============================================================================================//
typedef struct aux_imu_control{
	// register offset
	uint32_t offset;
	// bits
	uint32_t reset_n;
	uint32_t enable;
} aux_imu_control;

//==============================================================================================//
//										STATUS REGISTER											//
//==============================================================================================//
//																								//
//	The Status Register is 32-bit long register to provide the status of Auxiliary Controller.	//
//																								//
//==============================================================================================//
typedef struct aux_imu_status{
	// register offset
	uint32_t offset;
	// bits
	uint32_t flag_reset;
	uint32_t flag_enable;
	uint32_t flag_init;
} aux_imu_status;

//==============================================================================================//
//										HPS2AUX STRUCTURE										//
//==============================================================================================//
//																								//
//	The Hps2aux Structure maps the registers passing information from hps to aux.				//
//																								//
//==============================================================================================//
typedef struct aux_imu_hps2aux{
	uint32_t offset;
	uint32_t length;
} aux_imu_hps2aux;

//==============================================================================================//
//										AUX2HPS STRUCTURE										//
//==============================================================================================//
//																								//
//	The Nios2hps Structure maps the registers passing information from aux to hps.				//
//																								//
//==============================================================================================//
typedef struct aux_imu_aux2hps{
	uint32_t offset;
	uint32_t length;
} aux_imu_aux2hps;

//==============================================================================================//
//										ROUTINE CONTROL REGISTER								//
//==============================================================================================//
//																								//
//	The Routine Control Register is 32-bit long register to control the Routine.				//
//																								//
//==============================================================================================//
typedef struct aux_imu_routine_control{
	// register offset
	uint32_t offset;
	// bits
	uint32_t enable[AUX_IMU_NUMBER_OF_ROUTINES];
} aux_imu_routine_control;

//==============================================================================================//
//										ROUTINE STATUS REGISTER									//
//==============================================================================================//
//																								//
//	The Routine Status Register is 32-bit long register to provide the status of Routine.		//
//																								//
//==============================================================================================//
typedef struct aux_imu_routine_status{
	// register offset
	uint32_t offset;
	// bits
	uint32_t flag_enable[AUX_IMU_NUMBER_OF_ROUTINES];
	uint32_t flag_pkt_rdy[AUX_IMU_NUMBER_OF_ROUTINES];
	uint32_t flag_valid[AUX_IMU_NUMBER_OF_ROUTINES];
} aux_imu_routine_status;

//==============================================================================================//
//										PACKET STRUCTURE										//
//==============================================================================================//
//																								//
//	The Routine Packet encapsulates all information related to Data Packets passed to HPS.		//
//																								//
//==============================================================================================//
typedef struct aux_imu_routine_packet{
	// register offset
	uint32_t offset[AUX_IMU_NUMBER_OF_ROUTINES];
	uint32_t length;
} aux_imu_routine_packet;

//==============================================================================================//
//										ROUTINE STRUCTURE										//
//==============================================================================================//
//																								//
//	The Routine Structure encapsulates all Routine Registers and other parameters.				//
//																								//
//==============================================================================================//
typedef struct aux_imu_routine{
	uint32_t count;
	aux_imu_routine_control 	control;
	aux_imu_routine_status 		status;
	aux_imu_routine_packet		packet;
} aux_imu_routine;



#endif /* AUX_IMU_STRUCT_H_ */

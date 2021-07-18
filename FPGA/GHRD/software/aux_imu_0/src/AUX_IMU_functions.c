
// return values
#include "return_values.h"
// aux_imu memory definitions
#include "AUX_IMU_constants.h"
#include "AUX_IMU_struct.h"
#include "config_AUX_IMU.h"
#include "AUX_IMU.h"

#ifdef AUX_PLATFORM
// functions for accessing memory
#include "mem_functions.h"
#endif

//==============================================================================================//
//											CHECK FORMAT										//
//==============================================================================================//
#ifdef BYTE_FORMAT
	#define AUX_IMU_FORMAT	(4)
#endif

#ifdef DWORD_FORMAT
	#define AUX_IMU_FORMAT	(1)
#endif

#if defined(AUX_PLATFORM) || defined(HPS_PLATFORM)
// Init
void init_aux_imu(aux_imu_memory_map * imu, uint32_t aux_imu_base_address){
	// aux_imu_memory_map
	imu->base_address = aux_imu_base_address;

	// Control
	imu->control.offset					= (uint32_t)(AUX_IMU_CONTROL_REG_DWORD_OFST * AUX_IMU_FORMAT);
	imu->control.reset_n 				= (uint32_t)AUX_IMU_CONTROL_RESET_N;
	imu->control.enable 				= (uint32_t)AUX_IMU_CONTROL_ENABLE;

	// Status
	imu->status.offset 					= (uint32_t)(AUX_IMU_STATUS_REG_DWORD_OFST * AUX_IMU_FORMAT);
	imu->status.flag_reset 				= (uint32_t)AUX_IMU_STATUS_FLAG_RESET;
	imu->status.flag_enable 			= (uint32_t)AUX_IMU_STATUS_FLAG_ENABLE;
	imu->status.flag_init 				= (uint32_t)AUX_IMU_STATUS_FLAG_INIT;

	// Hps2aux
	imu->hps2aux.offset					= (uint32_t)(AUX_IMU_DATA_HPS2AUX_0_DWORD_OFST * AUX_IMU_FORMAT);
	imu->hps2aux.length 				= (uint32_t)(AUX_IMU_DATA_HPS2AUX_LENGTH_DWORD * AUX_IMU_FORMAT);

	// Aux2hps
	imu->aux2hps.offset					= (uint32_t)(AUX_IMU_DATA_AUX2HPS_0_DWORD_OFST * AUX_IMU_FORMAT);
	imu->aux2hps.length 				= (uint32_t)(AUX_IMU_DATA_AUX2HPS_LENGTH_DWORD * AUX_IMU_FORMAT);

	// Routine
	imu->routine.count 					= (uint32_t)AUX_IMU_NUMBER_OF_ROUTINES;

	// Routine - Control
	imu->routine.control.offset 		= (uint32_t)(AUX_IMU_ROUTINE_CONTROL_REG_DWORD_OFST * AUX_IMU_FORMAT);
	imu->routine.control.enable[0]		= (uint32_t)AUX_IMU_ROUTINE_0_ENABLE;
	imu->routine.control.enable[1]		= (uint32_t)AUX_IMU_ROUTINE_1_ENABLE;
	imu->routine.control.enable[2]		= (uint32_t)AUX_IMU_ROUTINE_2_ENABLE;
	imu->routine.control.enable[3]		= (uint32_t)AUX_IMU_ROUTINE_3_ENABLE;

	// Routine - Status
	imu->routine.status.offset 			= (uint32_t)(AUX_IMU_ROUTINE_STATUS_REG_DWORD_OFST * AUX_IMU_FORMAT);
	imu->routine.status.flag_enable[0]	= (uint32_t)AUX_IMU_ROUTINE_0_FLAG_ENABLE;
	imu->routine.status.flag_enable[1]	= (uint32_t)AUX_IMU_ROUTINE_1_FLAG_ENABLE;
	imu->routine.status.flag_enable[2]	= (uint32_t)AUX_IMU_ROUTINE_2_FLAG_ENABLE;
	imu->routine.status.flag_enable[3]	= (uint32_t)AUX_IMU_ROUTINE_3_FLAG_ENABLE;
	imu->routine.status.flag_pkt_rdy[0]	= (uint32_t)AUX_IMU_ROUTINE_0_FLAG_PKT_RDY;
	imu->routine.status.flag_pkt_rdy[1]	= (uint32_t)AUX_IMU_ROUTINE_1_FLAG_PKT_RDY;
	imu->routine.status.flag_pkt_rdy[2]	= (uint32_t)AUX_IMU_ROUTINE_2_FLAG_PKT_RDY;
	imu->routine.status.flag_pkt_rdy[3]	= (uint32_t)AUX_IMU_ROUTINE_3_FLAG_PKT_RDY;
	imu->routine.status.flag_valid[0]	= (uint32_t)AUX_IMU_ROUTINE_0_FLAG_VALID;
	imu->routine.status.flag_valid[1]	= (uint32_t)AUX_IMU_ROUTINE_1_FLAG_VALID;
	imu->routine.status.flag_valid[2]	= (uint32_t)AUX_IMU_ROUTINE_2_FLAG_VALID;
	imu->routine.status.flag_valid[3]	= (uint32_t)AUX_IMU_ROUTINE_3_FLAG_VALID;

	// Packet
	imu->routine.packet.offset[0]		= (uint32_t)(AUX_IMU_DATA_PACKET_0_DWORD_OFST * AUX_IMU_FORMAT);
	imu->routine.packet.offset[1]		= (uint32_t)(AUX_IMU_DATA_PACKET_1_DWORD_OFST * AUX_IMU_FORMAT);
	imu->routine.packet.offset[2]		= (uint32_t)(AUX_IMU_DATA_PACKET_2_DWORD_OFST * AUX_IMU_FORMAT);
	imu->routine.packet.offset[3]		= (uint32_t)(AUX_IMU_DATA_PACKET_3_DWORD_OFST * AUX_IMU_FORMAT);
};
#endif

#ifdef AUX_PLATFORM
// Control register
int32_t get_control_reg(aux_imu_memory_map imu){
	return GET_32REGISTER(imu.base_address, imu.control.offset);
};
// Status register
uint8_t  set_status_reg(aux_imu_memory_map imu, int32_t value){
	SET_32REGISTER(imu.base_address, imu.status.offset, value);
	return _SUCCESS_;
};
uint8_t  set_status_flag_reset(aux_imu_memory_map imu){
	SET_BIT(imu.base_address, imu.status.offset, imu.status.flag_reset);
	return _SUCCESS_;
};
uint8_t  set_status_flag_enable(aux_imu_memory_map imu){
	SET_BIT(imu.base_address, imu.status.offset, imu.status.flag_enable);
	return _SUCCESS_;
};
uint8_t  set_status_flag_init(aux_imu_memory_map imu){
	SET_BIT(imu.base_address, imu.status.offset, imu.status.flag_init);
	return _SUCCESS_;
};
uint8_t  clear_status_reg(aux_imu_memory_map imu){
	SET_32REGISTER(imu.base_address, imu.status.offset, 0x00000000);
	return _SUCCESS_;
};
uint8_t  clear_status_flag_reset(aux_imu_memory_map imu){
	CLEAR_BIT(imu.base_address, imu.status.offset, imu.status.flag_reset);
	return _SUCCESS_;
};
uint8_t  clear_status_flag_enable(aux_imu_memory_map imu){
	CLEAR_BIT(imu.base_address, imu.status.offset, imu.status.flag_enable);
	return _SUCCESS_;
};
uint8_t  clear_status_flag_init(aux_imu_memory_map imu){
	CLEAR_BIT(imu.base_address, imu.status.offset, imu.status.flag_init);
	return _SUCCESS_;
};
// hps2aux
int32_t get_hps2aux_reg(aux_imu_memory_map imu, uint8_t which){
	if(which < imu.hps2aux.length){
		return GET_32REGISTER(imu.base_address, imu.hps2aux.offset + (which * AUX_IMU_FORMAT));
	}else{
		return _ERROR_;
	}
};
// aux2hps
uint8_t set_aux2hps_reg(aux_imu_memory_map imu, uint8_t which, int32_t data){
	SET_32REGISTER(imu.base_address, imu.hps2aux.offset + (which * AUX_IMU_FORMAT), data);
	return _SUCCESS_;
};
uint8_t clear_aux2hps_reg(aux_imu_memory_map imu, uint8_t which){
	SET_32REGISTER(imu.base_address, imu.hps2aux.offset + (which * AUX_IMU_FORMAT), 0x00000000);
	return _SUCCESS_;
};

// Routine - Control
int32_t get_routine_control_reg(aux_imu_memory_map imu){
	return GET_32REGISTER(imu.base_address, imu.routine.control.offset);
};
// Routine - Status
uint8_t  set_routine_status_reg(aux_imu_memory_map imu, int32_t value){
	SET_32REGISTER(imu.base_address, imu.routine.status.offset, value);
	return _SUCCESS_;
};
uint8_t  clear_routine_status_reg(aux_imu_memory_map imu){
	SET_32REGISTER(imu.base_address, imu.routine.status.offset, 0x00000000);
	return _SUCCESS_;
};
uint8_t  set_routine_status_flag_enable(aux_imu_memory_map imu, uint8_t which){
	SET_BIT(imu.base_address, imu.routine.status.offset, imu.routine.status.flag_enable[which]);
	return _SUCCESS_;
};
uint8_t set_routine_status_flag_pkt_rdy(aux_imu_memory_map imu, uint8_t which){
	SET_BIT(imu.base_address, imu.routine.status.offset, imu.routine.status.flag_pkt_rdy[which]);
	return _SUCCESS_;
};
uint8_t set_routine_status_flag_valid(aux_imu_memory_map imu, uint8_t which){
	SET_BIT(imu.base_address, imu.routine.status.offset, imu.routine.status.flag_valid[which]);
	return _SUCCESS_;
};


uint8_t  clear_routine_status_flag_enable(aux_imu_memory_map imu, uint8_t which){
	CLEAR_BIT(imu.base_address, imu.routine.status.offset, imu.routine.status.flag_enable[which]);
	return _SUCCESS_;
};
uint8_t clear_routine_status_flag_pkt_rdy(aux_imu_memory_map imu, uint8_t which){
	CLEAR_BIT(imu.base_address, imu.routine.status.offset, imu.routine.status.flag_pkt_rdy[which]);
	return _SUCCESS_;
};
uint8_t clear_routine_status_flag_valid(aux_imu_memory_map imu, uint8_t which){
	CLEAR_BIT(imu.base_address, imu.routine.status.offset, imu.routine.status.flag_valid[which]);
	return _SUCCESS_;
};
// Routine - Packet
uint8_t  store_packet8(aux_imu_memory_map imu, uint8_t which, uint16_t length, int8_t* data){
	STORE_8BIT_DATA	(imu.base_address, imu.routine.packet.offset[which], length, data);
	return _SUCCESS_;
};
uint8_t store_packet16(aux_imu_memory_map imu, uint8_t which, uint16_t length, int16_t* data){
	STORE_16BIT_DATA	(imu.base_address, imu.routine.packet.offset[which], length, data);
	return _SUCCESS_;
};
uint8_t store_packet32(aux_imu_memory_map imu, uint8_t which, uint16_t length, int32_t* data){
	STORE_32BIT_DATA	(imu.base_address, imu.routine.packet.offset[which], length, data);
	return _SUCCESS_;
};
#endif

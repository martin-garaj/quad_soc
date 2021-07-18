
#include "config_AUX_RADIO.h"
// return values
#include "return_values.h"
// aux_radio memory definitions
#include "AUX_RADIO_constants.h"
#include "AUX_RADIO_struct.h"
#include "AUX_RADIO.h"

#ifdef AUX_PLATFORM
// functions for accessing memory
#include "mem_functions.h"
#endif


//==============================================================================================//
//											CHECK FORMAT										//
//==============================================================================================//
#ifdef BYTE_FORMAT
	#define AUX_RADIO_FORMAT	(4)
#endif

#ifdef DWORD_FORMAT
	#define AUX_RADIO_FORMAT	(1)
#endif



#if defined(AUX_PLATFORM) || defined(HPS_PLATFORM)
// Init
void init_aux_radio(aux_radio_memory_map * radio, uint32_t aux_radio_base_address){
	// Aux_radio_mem
	radio->base_address = aux_radio_base_address;

	// Control
	radio->control.offset					= (uint32_t)(AUX_RADIO_CONTROL_REG_DWORD_OFST * AUX_RADIO_FORMAT);
	radio->control.reset_n 				= (uint32_t)AUX_RADIO_CONTROL_RESET_N;
	radio->control.enable 				= (uint32_t)AUX_RADIO_CONTROL_ENABLE;

	// Status
	radio->status.offset 					= (uint32_t)(AUX_RADIO_STATUS_REG_DWORD_OFST * AUX_RADIO_FORMAT);
	radio->status.flag_reset 				= (uint32_t)AUX_RADIO_STATUS_FLAG_RESET;
	radio->status.flag_enable 			= (uint32_t)AUX_RADIO_STATUS_FLAG_ENABLE;
	radio->status.flag_init 				= (uint32_t)AUX_RADIO_STATUS_FLAG_INIT;

	// Hps2aux
	radio->hps2aux.offset					= (uint32_t)(AUX_RADIO_DATA_HPS2AUX_0_DWORD_OFST * AUX_RADIO_FORMAT);
	radio->hps2aux.length 				= (uint32_t)(AUX_RADIO_DATA_HPS2AUX_LENGTH_DWORD * AUX_RADIO_FORMAT);

	// Aux2hps
	radio->aux2hps.offset					= (uint32_t)(AUX_RADIO_DATA_AUX2HPS_0_DWORD_OFST * AUX_RADIO_FORMAT);
	radio->aux2hps.length 				= (uint32_t)(AUX_RADIO_DATA_AUX2HPS_LENGTH_DWORD * AUX_RADIO_FORMAT);

	// Routine
	radio->routine.count 					= (uint32_t)AUX_RADIO_NUMBER_OF_ROUTINES;

	// Routine - Control
	radio->routine.control.offset 		= (uint32_t)(AUX_RADIO_ROUTINE_CONTROL_REG_DWORD_OFST * AUX_RADIO_FORMAT);
	radio->routine.control.enable[0]		= (uint32_t)AUX_RADIO_ROUTINE_0_ENABLE;
	radio->routine.control.enable[1]		= (uint32_t)AUX_RADIO_ROUTINE_1_ENABLE;

	// Routine - Status
	radio->routine.status.offset 			= (uint32_t)(AUX_RADIO_ROUTINE_STATUS_REG_DWORD_OFST * AUX_RADIO_FORMAT);
	radio->routine.status.flag_enable[0]	= (uint32_t)AUX_RADIO_ROUTINE_0_FLAG_ENABLE;
	radio->routine.status.flag_enable[1]	= (uint32_t)AUX_RADIO_ROUTINE_1_FLAG_ENABLE;
	radio->routine.status.flag_pkt_rdy[0]	= (uint32_t)AUX_RADIO_ROUTINE_0_FLAG_PKT_RDY;
	radio->routine.status.flag_pkt_rdy[1]	= (uint32_t)AUX_RADIO_ROUTINE_1_FLAG_PKT_RDY;
	radio->routine.status.flag_valid[0]	= (uint32_t)AUX_RADIO_ROUTINE_0_FLAG_VALID;
	radio->routine.status.flag_valid[1]	= (uint32_t)AUX_RADIO_ROUTINE_1_FLAG_VALID;

	// Packet
	radio->routine.packet.offset[0]		= (uint32_t)(AUX_RADIO_DATA_PACKET_0_DWORD_OFST * AUX_RADIO_FORMAT);
	radio->routine.packet.offset[1]		= (uint32_t)(AUX_RADIO_DATA_PACKET_1_DWORD_OFST * AUX_RADIO_FORMAT);
};
#endif



#ifdef AUX_PLATFORM
// Control register
int32_t get_control_reg(aux_radio_mem radio){
	return GET_32REGISTER(radio.base_address, radio.control.offset);
};
// Status register
uint8_t  set_status_reg(aux_radio_mem radio, int32_t value){
	SET_32REGISTER(radio.base_address, radio.status.offset, value);
	return _SUCCESS_;
};
uint8_t  set_status_flag_reset(aux_radio_mem radio){
	SET_BIT(radio.base_address, radio.status.offset, radio.status.flag_reset);
	return _SUCCESS_;
};
uint8_t  set_status_flag_enable(aux_radio_mem radio){
	SET_BIT(radio.base_address, radio.status.offset, radio.status.flag_enable);
	return _SUCCESS_;
};
uint8_t  set_status_flag_init(aux_radio_mem radio){
	SET_BIT(radio.base_address, radio.status.offset, radio.status.flag_init);
	return _SUCCESS_;
};
uint8_t  clear_status_reg(aux_radio_mem radio){
	SET_32REGISTER(radio.base_address, radio.status.offset, 0x00000000);
	return _SUCCESS_;
};
uint8_t  clear_status_flag_reset(aux_radio_mem radio){
	CLEAR_BIT(radio.base_address, radio.status.offset, radio.status.flag_reset);
	return _SUCCESS_;
};
uint8_t  clear_status_flag_enable(aux_radio_mem radio){
	CLEAR_BIT(radio.base_address, radio.status.offset, radio.status.flag_enable);
	return _SUCCESS_;
};
uint8_t  clear_status_flag_init(aux_radio_mem radio){
	CLEAR_BIT(radio.base_address, radio.status.offset, radio.status.flag_init);
	return _SUCCESS_;
};
// hps2aux
int32_t get_hps2aux_reg(aux_radio_mem radio, uint8_t which){
	if(which < radio.hps2aux.length){
		return GET_32REGISTER(radio.base_address, radio.hps2aux.offset + (which * AUX_RADIO_FORMAT));
	}else{
		return _ERROR_;
	}
};
// aux2hps
uint8_t set_aux2hps_reg(aux_radio_mem radio, uint8_t which, int32_t data){
	SET_32REGISTER(radio.base_address, radio.hps2aux.offset + (which * AUX_RADIO_FORMAT), data);
	return _SUCCESS_;
};
uint8_t clear_aux2hps_reg(aux_radio_mem radio, uint8_t which){
	SET_32REGISTER(radio.base_address, radio.hps2aux.offset + (which * AUX_RADIO_FORMAT), 0x00000000);
	return _SUCCESS_;
};

// Routine - Control
int32_t get_routine_control_reg(aux_radio_mem radio){
	return GET_32REGISTER(radio.base_address, radio.routine.control.offset);
};
// Routine - Status
uint8_t  set_routine_status_reg(aux_radio_mem radio, int32_t value){
	SET_32REGISTER(radio.base_address, radio.routine.status.offset, value);
	return _SUCCESS_;
};
uint8_t  clear_routine_status_reg(aux_radio_mem radio){
	SET_32REGISTER(radio.base_address, radio.routine.status.offset, 0x00000000);
	return _SUCCESS_;
};
uint8_t  set_routine_status_flag_enable(aux_radio_mem radio, uint8_t which){
	SET_BIT(radio.base_address, radio.routine.status.offset, radio.routine.status.flag_enable[which]);
	return _SUCCESS_;
};
uint8_t set_routine_status_flag_pkt_rdy(aux_radio_mem radio, uint8_t which){
	SET_BIT(radio.base_address, radio.routine.status.offset, radio.routine.status.flag_pkt_rdy[which]);
	return _SUCCESS_;
};
uint8_t set_routine_status_flag_valid(aux_radio_mem radio, uint8_t which){
	SET_BIT(radio.base_address, radio.routine.status.offset, radio.routine.status.flag_valid[which]);
	return _SUCCESS_;
};


uint8_t  clear_routine_status_flag_enable(aux_radio_mem radio, uint8_t which){
	CLEAR_BIT(radio.base_address, radio.routine.status.offset, radio.routine.status.flag_enable[which]);
	return _SUCCESS_;
};
uint8_t clear_routine_status_flag_pkt_rdy(aux_radio_mem radio, uint8_t which){
	CLEAR_BIT(radio.base_address, radio.routine.status.offset, radio.routine.status.flag_pkt_rdy[which]);
	return _SUCCESS_;
};
uint8_t clear_routine_status_flag_valid(aux_radio_mem radio, uint8_t which){
	CLEAR_BIT(radio.base_address, radio.routine.status.offset, radio.routine.status.flag_valid[which]);
	return _SUCCESS_;
};
// Routine - Packet
uint8_t  store_packet8(aux_radio_mem radio, uint8_t which, uint16_t length, int8_t* data){
	STORE_8BIT_DATA	(radio.base_address, radio.routine.packet.offset[which], length, data);
	return _SUCCESS_;
};
uint8_t store_packet16(aux_radio_mem radio, uint8_t which, uint16_t length, int16_t* data){
	STORE_16BIT_DATA	(radio.base_address, radio.routine.packet.offset[which], length, data);
	return _SUCCESS_;
};
uint8_t store_packet32(aux_radio_mem radio, uint8_t which, uint16_t length, int32_t* data){
	STORE_32BIT_DATA	(radio.base_address, radio.routine.packet.offset[which], length, data);
	return _SUCCESS_;
};
#endif

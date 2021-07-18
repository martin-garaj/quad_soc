//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//


//==============================================================================================//
//											INCLUDES											//
//==============================================================================================//
#include "AUX_RADIO_mem.hpp"	// header
#include "AUX_RADIO.h"	// header
#include <stdint.h>			// uint8_t
#include "return_values.h"	// _SUCCES_, _ERROR_

#include <iostream>			// std::cout
#include <iomanip>			// std::cout << std::setw(8)



//==============================================================================================//
//												CLASS											//
//==============================================================================================//
//																								//
//	AUX_IMU_mem represents specific access to AUX_IMU memory, to read/set registers and bits.	//
//						                                                      					//
//==============================================================================================//

//constructor and destructor
AUX_RADIO_mem::AUX_RADIO_mem(uint32_t * aux_radio_memory_virtual_address, int aux_radio_memory_length_dword):API_mem(aux_radio_memory_virtual_address){
	AUX_RADIO_mem::memory_length = aux_radio_memory_length_dword;
	init_aux_radio(&(AUX_RADIO_mem::memory_map), 0x00000000);
	std::cout << "memory_map reset bit" << memory_map.control.reset_n << std::endl;
};


AUX_RADIO_mem::~AUX_RADIO_mem(){};

// extra
int AUX_RADIO_mem::hard_reset(){
	// clears both aux and routine, control and status reg
	uint32_t p_value = 0x00000000;
	API_mem::write_dword(&p_value, memory_map.control.offset, 1);
	API_mem::write_dword(&p_value, memory_map.status.offset, 1);
	API_mem::write_dword(&p_value, memory_map.routine.status.offset, 1);
	API_mem::write_dword(&p_value, memory_map.routine.status.offset, 1);
	return _SUCCESS_;
};

int AUX_RADIO_mem::deadbeef(){
	uint32_t deadbeef = 0xdeadbeef;
	for(int i = 0; i<memory_length; i++){
		API_mem::write_dword(&deadbeef, i, 1);
	}
	return _SUCCESS_;
};

int AUX_RADIO_mem::print(int start, int end){
	if( (start >= 0) or (start < memory_length) ){
		if( (end >= start) or (end < memory_length) ){
			uint32_t value;
			for(int i = start; i<end; i++){
				API_mem::read_dword(&value, i, 1);
				std::cout << "\t"  << std::setw(3) << std::dec << i << std::setw(10) << std::hex << value << std::dec << std::endl;
			}
			return _SUCCESS_;
		}else{
			std::cout << "AUX_RADIO_mem::print(int , int) > 2nd argument wrong, min value(1st argument)/argument/max value : " << start << "/" << end << "/" << memory_length << std::endl;
		}
	}else{
		std::cout << "AUX_RADIO_mem::print(int , int) > 1st argument wrong, min value/argument/max value = " << 0 << "/" << start << "/" << memory_length << std::endl;
	}
	return _ERROR_;
};


// control register
int AUX_RADIO_mem::set_control_reg(uint32_t value){
	uint32_t p_value = value;
	API_mem::write_dword(&p_value, memory_map.control.offset, 1);
	return _SUCCESS_;
};
int AUX_RADIO_mem::clear_control_reg(){
	uint32_t p_value = 0x00000000;
	API_mem::write_dword(&p_value, memory_map.control.offset, 1);
	return _SUCCESS_;
};
// control register bits
int AUX_RADIO_mem::set_control_reset(){
	// reversed logic
	API_mem::clear_bit(memory_map.control.offset, memory_map.control.reset_n);
	return _SUCCESS_;
};
int AUX_RADIO_mem::clear_control_reset(){
	// reversed logic
	API_mem::set_bit(memory_map.control.offset, memory_map.control.reset_n);
	return _SUCCESS_;
};
int AUX_RADIO_mem::set_control_enable(){
	API_mem::set_bit(memory_map.control.offset, memory_map.control.enable);
	return _SUCCESS_;
};
int AUX_RADIO_mem::clear_control_enable(){
	API_mem::clear_bit(memory_map.control.offset, memory_map.control.enable);
	return _SUCCESS_;
};

// status register
uint32_t AUX_RADIO_mem::get_status_reg(){
	uint32_t value;
	API_mem::read_dword(&value, memory_map.status.offset, 1);
	return value;
};
// bits
bool AUX_RADIO_mem::check_status_flag_reset(){
	return API_mem::check_bit(memory_map.status.offset, memory_map.status.flag_reset);
};
bool AUX_RADIO_mem::check_status_flag_enable(){
	return API_mem::check_bit(memory_map.status.offset, memory_map.status.flag_enable);
};
bool AUX_RADIO_mem::check_status_flag_init(){
	return API_mem::check_bit(memory_map.status.offset, memory_map.status.flag_init);
};

// hsp2aux
int32_t AUX_RADIO_mem::get_hps2aux_length(){
	// returns max size of hps2aux memory in dword
	return memory_map.hps2aux.length;
};
int AUX_RADIO_mem::store_hps2aux_data(int32_t * data, int length){
	if(length <= (int)memory_map.hps2aux.length){
		API_mem::write_dword((uint32_t*)data, (uint32_t)memory_map.hps2aux.offset, (uint32_t)length);
	}else{
		API_mem::write_dword((uint32_t*)data, (uint32_t)memory_map.hps2aux.offset, memory_map.hps2aux.length);
		return _ERROR_;
	}
	return _SUCCESS_;
};

// hsp2aux
int32_t AUX_RADIO_mem::get_aux2hps_length(){
	// returns max size of aux2hps memory in dword
	return memory_map.aux2hps.length;
};
int AUX_RADIO_mem::load_aux2hps_data(int32_t * data, int length){
	if(length <= (int)memory_map.aux2hps.length){
		API_mem::read_dword((uint32_t*)data, (uint32_t)memory_map.aux2hps.offset, (uint32_t)length);
	}else{
		API_mem::read_dword((uint32_t*)data, (uint32_t)memory_map.aux2hps.offset, memory_map.aux2hps.length);
		return _ERROR_;
	}
	return _SUCCESS_;
};

// routine control reg
int AUX_RADIO_mem::get_routine_count(){
	return memory_map.routine.count;
};
int AUX_RADIO_mem::set_routine_control_reg(uint32_t value){
	uint32_t p_value = value;
	API_mem::write_dword(&p_value, memory_map.routine.control.offset, 1);
	return _SUCCESS_;
};
int AUX_RADIO_mem::clear_routine_control_reg(){
	uint32_t p_value = 0x00000000;
	API_mem::write_dword(&p_value, memory_map.routine.control.offset, 1);
	return _SUCCESS_;
};
// bits
int AUX_RADIO_mem::set_routine_control_enable(int which){
	if( (which >= 0) or (which < (int)memory_map.routine.count) ){
		API_mem::set_bit(memory_map.routine.control.offset, memory_map.routine.control.enable[which]);
	}else{
		return _ERROR_;
	}
	return _SUCCESS_;
};
int AUX_RADIO_mem::clear_routine_control_enable(int which){
	if(which < (int)memory_map.routine.count){
		API_mem::clear_bit(memory_map.routine.control.offset, memory_map.routine.control.enable[which]);
	}else{
		return _ERROR_;
	}
	return _SUCCESS_;
};

// routine status reg
uint32_t AUX_RADIO_mem::get_routine_status_reg(){
	uint32_t p_value;
	API_mem::read_dword(&p_value, memory_map.routine.status.offset, 1);
	return p_value;
};

// bits
	// check
	bool AUX_RADIO_mem::check_routine_status_flag_enable(int which){
		return API_mem::check_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_enable[which]);
	};
	bool AUX_RADIO_mem::check_routine_status_flag_enable(int which, uint32_t * error){
		if( (which >= 0) or (which < (int)memory_map.routine.count) ){
			return API_mem::check_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_enable[which]);
			*error = 0x00000000;
		}
		*error = which;
		return false;
	};



	bool AUX_RADIO_mem::check_routine_status_flag_pkt_rdy(int which){
		return API_mem::check_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_pkt_rdy[which]);
	};
	bool AUX_RADIO_mem::check_routine_status_flag_pkt_rdy(int which, uint32_t * error){
		if( (which >= 0) or (which < (int)memory_map.routine.count) ){
			return API_mem::check_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_pkt_rdy[which]);
			*error = 0x00000000;
		}
		*error = which;
		return false;
	};



	bool AUX_RADIO_mem::check_routine_status_flag_valid(int which){
		return API_mem::check_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_valid[which]);
	};
	bool AUX_RADIO_mem::check_routine_status_flag_valid(int which, uint32_t * error){
		if( (which >= 0) or (which < (int)memory_map.routine.count) ){
			return API_mem::check_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_valid[which]);
			*error = 0x00000000;
		}
		*error = which;
		return false;
	};



	// clear
	int AUX_RADIO_mem::clear_routine_status_flag_enable(int which){
		API_mem::clear_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_enable[which]);
		return _SUCCESS_;
	}
	int AUX_RADIO_mem::clear_routine_status_flag_enable(int which, uint32_t * error){
		if( (which >= 0) or (which < (int)memory_map.routine.count) ){
			return API_mem::clear_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_enable[which]);
			*error = 0x00000000;
		}
		*error = which;
		return false;
	};



	int AUX_RADIO_mem::clear_routine_status_flag_pkt_rdy(int which){
		API_mem::clear_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_pkt_rdy[which]);
		return _SUCCESS_;
	}
	int AUX_RADIO_mem::clear_routine_status_flag_pkt_rdy(int which, uint32_t * error){
		if( (which >= 0) or (which < (int)memory_map.routine.count) ){
			return API_mem::clear_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_pkt_rdy[which]);
			*error = 0x00000000;
		}
		*error = which;
		return false;
	};

	int AUX_RADIO_mem::clear_routine_status_flag_valid(int which){
		API_mem::clear_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_valid[which]);
		return _SUCCESS_;
	};
	int AUX_RADIO_mem::clear_routine_status_flag_valid(int which, uint32_t * error){
		if( (which >= 0) or (which < (int)memory_map.routine.count) ){
			return API_mem::clear_bit(memory_map.routine.status.offset, memory_map.routine.status.flag_valid[which]);
			*error = 0x00000000;
		}
		*error = which;
		return false;
	};


// routine packet
int AUX_RADIO_mem::get_routine_packet_length(){
	// returns max size of packet memory in dword
	return memory_map.routine.packet.length;
};


int AUX_RADIO_mem::get_routine_packet(int which, int32_t * packet, int length){
	API_mem::read_dword((uint32_t*) packet, (uint32_t)memory_map.routine.packet.offset[which], (uint32_t)length);
	return _SUCCESS_;
};


// int AUX_RADIO_mem::get_routine_packet(int which, int32_t * packet, int length, uint32_t * error);











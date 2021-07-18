//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//
#ifndef AUX_IMU_MEM_HPP_
#define AUX_IMU_MEM_HPP_


//==============================================================================================//
//											INCLUDES											//
//==============================================================================================//
#include <stdint.h>		// uint8_t
#include "AUX_IMU.h"	// aux_imu_mem
#include <API_mem.hpp>	// API_mem base class

//==============================================================================================//
//												CLASS											//
//==============================================================================================//
//																								//
//	AUX_IMU_mem represents specific access to AUX_IMU memory, to read/set registers and bits.	//
//						                                                      					//
//==============================================================================================//
class AUX_IMU_mem : private API_mem {
// VARIABLES //
public:

// FUNCTIONS //
public:
	//constructor and destructor
	AUX_IMU_mem(uint32_t * aux_imu_memory_virtual_address, int aux_imu_memory_length_dword);
	~AUX_IMU_mem();

	// extra
	int hard_reset(); // clears both control and status reg
	int deadbeef();
	int print(int start, int end);

	// control register
	int set_control_reg(uint32_t value);
	int clear_control_reg();
	// control register bits
	int set_control_reset();
	int clear_control_reset();
	int set_control_enable();
	int clear_control_enable();

	// status register
	uint32_t get_status_reg();
	// bits
	bool check_status_flag_reset();
	bool check_status_flag_enable();
	bool check_status_flag_init();

	// hsp2aux
	int32_t get_hps2aux_length();
	int store_hps2aux_data(int32_t * data, int length);

	// hsp2aux
	int32_t get_aux2hps_length();
	int load_aux2hps_data(int32_t * data, int length);

	// routine control reg
	int get_routine_count();
	int set_routine_control_reg(uint32_t value);
	int clear_routine_control_reg();
	// bits
	int set_routine_control_enable(int which);
	int clear_routine_control_enable(int which);

	// routine status reg
	uint32_t get_routine_status_reg();
	// bits
		// check
		bool check_routine_status_flag_enable(int which);
		bool check_routine_status_flag_enable(int which, uint32_t * error);
		bool check_routine_status_flag_pkt_rdy(int which);
		bool check_routine_status_flag_pkt_rdy(int which, uint32_t * error);
		bool check_routine_status_flag_valid(int which);
		bool check_routine_status_flag_valid(int which, uint32_t * error);
		// clear
		int clear_routine_status_flag_enable(int which);
		int clear_routine_status_flag_enable(int which, uint32_t * error);
		int clear_routine_status_flag_pkt_rdy(int which);
		int clear_routine_status_flag_pkt_rdy(int which, uint32_t * error);
		int clear_routine_status_flag_valid(int which);
		int clear_routine_status_flag_valid(int which, uint32_t * error);
		// check_clear
		bool check_clear_routine_status_flag_enable(int which);
		bool check_clear_routine_status_flag_enable(int which, uint32_t * error);
		bool check_clear_routine_status_flag_pkt_rdy(int which);
		bool check_clear_routine_status_flag_pkt_rdy(int which, uint32_t * error);
		bool check_clear_routine_status_flag_valid(int which);
		bool check_clear_routine_status_flag_valid(int which, uint32_t * error);

	// routine packet
	int get_routine_packet_length();
	int get_routine_packet(int which, int32_t * packet, int length);
	int get_routine_packet(int which, int32_t * packet, int length, uint32_t * error);

// VARIABLES
private:
	aux_imu_memory_map memory_map;
	int memory_length;

};



#endif /* AUX_IMU_MEM_HPP_ */

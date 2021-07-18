//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//

//==============================================================================================================================================//
//											#INCLUDEs																							//
//==============================================================================================================================================//
#include <iostream>		// std::cout
#include <iomanip>		// std::cout << std::setprecision(1), std::cout << std::setw(8)
#include <stdint.h>		// uint8_t
#include <chrono>		// nanoseconds, system_clock, seconds
#include <thread>		// sleep_for, sleep_until
#include <fstream>      // std::fstream, file operations

// testing
#include <chrono>		// nanoseconds, system_clock, seconds

// system
#include "system_const.h"	// return values, type size

// fpga
#include "hps_0.h"
#include "API_I2C_module.hpp"
#include "API_Interconnect.hpp"
#include "API_PWM_module.hpp"
#include "API_PIO_module.hpp"
#include "AUX_IMU_mem.hpp"
#include "AUX_RADIO_mem.hpp"

// Configuration
#include "API_Configuration.hpp"
#include "XML_external_config.hpp"

//Control
#include "PID_Controller.hpp"

// RC
#include "API_RC_PWM.hpp"

//#define MPU6050_INCLUDE_DMP_MOTIONAPPS41
#include "q_MPU6050.h"
#include "config.h"

// memory mapping for AUX_IMU
#include "AUX_IMU.h"
#include "AUX_RADIO.h"

//==============================================================================================================================================//
//											MAIN FUNCTION																						//
//==============================================================================================================================================//
int main() {
//==============================================================================================//
//											VARIABLES											//
//==============================================================================================//
	//------------------------------------------------------------------------------------//
	//--------------------------------PERFORMANCE TESTING---------------------------------//
	//------------------------------------------------------------------------------------//
	#ifdef PERFORMANCE
		int aux_imu_packet_counter[AUX_IMU_ROUTINE];

		for(int i = 0; i<AUX_IMU_ROUTINE; i++){
			aux_imu_packet_counter[i] = 0;
		};

		int aux_radio_valid_packet_counter = 0;
		int aux_radio_INvalid_packet_counter = 0;
		int main_loop_counter = 0;

		auto begin = std::chrono::high_resolution_clock::now();
		auto end = std::chrono::high_resolution_clock::now();
		auto total_time_begin = std::chrono::high_resolution_clock::now();
		auto total_time_end = std::chrono::high_resolution_clock::now();
		int total_time;
		int hours;
		int minutes;
		int seconds;
	#endif
//==============================================================================================//
//											AUX RADIO DECLARATIONS								//
//==============================================================================================//

//==============================================================================================//
//											AUX IMU DECLARATIONS								//
//==============================================================================================//
#ifdef AUX_IMU
	//------------------------------------------------------------------------------------//
	//--------------------------------HPS2AUX DATA----------------------------------------//
	//------------------------------------------------------------------------------------//
	// imu i2c address, when active
	uint32_t imu_i2c_address = MPU6050_ADDRESS_AD0_HIGH;
	// select_0                    4th*  3rd   2nd   1st    IMU on i2c bus
	uint32_t imu_i2c_active[4] = { 0x01, 0x02, 0x04, 0x08 };
	// variable to set the timeout between aux_imu i2c access
	uint32_t sleep_length = 2500; // in microseconds [us]
	//------------------------------------------------------------------------------------//
	//--------------------------------ROUTINE VARIABLES-----------------------------------//
	//------------------------------------------------------------------------------------//
	int32_t imu_data[6]; // this should be the size of data packet passed from AUX_IMU
	VectorInt16 accel[AUX_IMU_ROUTINE];
	VectorInt16 gyro[AUX_IMU_ROUTINE];
#endif

//==============================================================================================================================================//
//											INITIALIZATION																						//
//==============================================================================================================================================//

//==============================================================================================//
//											HPS INITIALIZATION									//
//==============================================================================================//
	std::cout<<"//============================================================================//" << std::endl;
	std::cout<<"//	HPS INITIALIZATION                                                        //" << std::endl;
	std::cout<<"//============================================================================//" << std::endl;
	//------------------------------------------------------------------------------------//
	//--------------------------------HARDWARE TO VIRTUAL ADDRESS SPACE-------------------//
	//------------------------------------------------------------------------------------//
	API_Interconnect * interconnect = new API_Interconnect();
	uint32_t* i2c_0_virtual_address 			= interconnect->convert_address(I2C_0_BASE);
	uint32_t* select_0_virtual_address 			= interconnect->convert_address(SELECT_0_BASE);
	uint32_t* reset_0_virtual_address 			= interconnect->convert_address(SELECT_0_BASE);
	uint32_t* aux_radio_0_mem_virtual_address 	= interconnect->convert_address(AUX_RADIO_0_MEM_BASE);
	uint32_t* aux_imu_0_mem_virtual_address 	= interconnect->convert_address(AUX_IMU_0_MEM_BASE);
	uint32_t* pwm_0_virtual_address				= interconnect->convert_address(PWM_0_BASE);
	uint32_t* pwm_1_virtual_address 			= interconnect->convert_address(PWM_1_BASE);
	uint32_t* hps_0_led_virtual_address 		= interconnect->convert_address(HPS_0_LED_BASE);
	//------------------------------------------------------------------------------------//
	//--------------------------------OBJECT INITIALIZATION-------------------------------//
	//------------------------------------------------------------------------------------//
	// external config XML parser
	XML_external_config parser;
	// external config XML
	Configuration_object external_config;
	parser.parse("external_config.xml", &external_config);
	external_config.print();

#ifdef AUX_IMU
	// auxiliary controller
	AUX_IMU_mem aux_imu(aux_imu_0_mem_virtual_address, AUX_IMU_0_MEM_SPAN/4);
	const int aux_imu_routine_count = aux_imu.get_routine_count();
	// other components related to AUX_IMU
	API_I2C_module aux_imu_i2c(i2c_0_virtual_address);
	API_PIO_module aux_imu_select(select_0_virtual_address);
	API_PIO_module aux_imu_reset(reset_0_virtual_address);
	// initialization
	aux_imu_i2c.initialize(FREQ_400kHz, FPGA_FREQ, 50000000);
	aux_imu_select.set(0x00);
	// hps2aux data
	uint32_t aux_imu_hps2aux[aux_imu.get_hps2aux_length()];
	aux_imu_hps2aux[0] = imu_i2c_address;
	aux_imu_hps2aux[1] = imu_i2c_active[0];
	aux_imu_hps2aux[2] = imu_i2c_active[1];
	aux_imu_hps2aux[3] = imu_i2c_active[2];
	aux_imu_hps2aux[4] = imu_i2c_active[3];
	aux_imu_hps2aux[5] = sleep_length;
	// read the status register
	uint32_t imu_routine_status_reg;
#endif

#ifdef AUX_RADIO
	// auxiliary controller
	AUX_RADIO_mem aux_radio(aux_radio_0_mem_virtual_address, AUX_RADIO_0_MEM_SPAN/4);
	const int aux_radio_routine_count = aux_radio.get_routine_count();
	// constants to transform raw channel values
	const int CHANNEL_CENTER = 1500; 	// 0x05DC
	const int CHANNEL_LOW = 1000; 		// 0x03E8
	const int CHANNEL_HIGH = 2000; 		// 0x07D0
	const int CHANNEL_RANGE = CHANNEL_HIGH - CHANNEL_LOW;
	const int NUMBER_OF_CHANNELS = 10;
	// channel variable
	float channel_value;
	// array to store channel values
	uint16_t channels[NUMBER_OF_CHANNELS];
	// hps2aux data
	uint32_t aux_radio_hps2aux[aux_radio.get_hps2aux_length()];
	// predefined iBus UART baud rate
	aux_radio_hps2aux[0] = 115200;
	// predefined iBus checksum initial value
	aux_radio_hps2aux[1] = 0xDF;
	// number of channels
	aux_radio_hps2aux[2] = NUMBER_OF_CHANNELS;
#endif
	// PWM & SERVO objects
	API_PWM_module pwm_0(pwm_0_virtual_address, FPGA_FREQ, HPS_FREQ);
	API_PWM_module pwm_1(pwm_1_virtual_address, FPGA_FREQ, HPS_FREQ);
	API_PIO_module led(hps_0_led_virtual_address);
/*
	// PID
	PID_Controller PID_roll;
	PID_Controller PID_pitch;
	PID_Controller PID_yaw;

	PID_struct temp_pid;
	// yaw
	external_config.get_pid(0, &temp_pid);
	PID_roll.set_constants(temp_pid.P, temp_pid.I, temp_pid.D, temp_pid.T_d);
	// pitch
	external_config.get_pid(1, &temp_pid);
	PID_pitch.set_constants(temp_pid.P, temp_pid.I, temp_pid.D, temp_pid.T_d);
	// roll
	external_config.get_pid(2, &temp_pid);
	PID_yaw.set_constants(temp_pid.P, temp_pid.I, temp_pid.D, temp_pid.T_d);
*/
	//------------------------------------------------------------------------------------//
	//--------------------------------PERIPHERAL INITIALIZATION---------------------------//
	//------------------------------------------------------------------------------------//
	// PWM peripheral
	pwm_0.reset();
	pwm_0.prescaler_setup(0.0020, 5000);
	// PWM_0 - servo
	pwm_0.enable(PWM_0, true);
	pwm_0.set_period(PWM_0, 0.0020);
	pwm_0.set_blank(PWM_0, 1);
	// PWM_0 - servo
	pwm_0.enable(PWM_1, true);
	pwm_0.set_period(PWM_1, 0.0020);
	pwm_0.set_blank(PWM_1, 1);
	// PWM_2 - servo
	pwm_0.enable(PWM_2, true);
	pwm_0.set_period(PWM_2, 0.0020);
	pwm_0.set_blank(PWM_2, 1);
	// PWM_3 - servo
	pwm_0.enable(PWM_3, true);
	pwm_0.set_period(PWM_3, 0.0020);
	pwm_0.set_blank(PWM_3, 1);
	// PWM_4 - servo
	pwm_0.enable(PWM_4, true);
	pwm_0.set_period(PWM_4, 0.0020);
	pwm_0.set_blank(PWM_4, 0);

	// PWM peripheral
	pwm_1.reset();
	pwm_1.prescaler_setup(0.0020, 5000);
	// PWM_0 - servo
	pwm_1.enable(PWM_0, false);
	pwm_1.set_period(PWM_0, 0.0025);
	pwm_1.set_blank(PWM_0, 1);
	// PWM_1 - servo
	pwm_1.enable(PWM_1, false);
	pwm_1.set_period(PWM_1, 0.0025);
	pwm_1.set_blank(PWM_1, 1);
	// PWM_2 - servo
	pwm_1.enable(PWM_2, false);
	pwm_1.set_period(PWM_2, 0.0025);
	pwm_1.set_blank(PWM_2, 1);
	// PWM_3 - servo
	pwm_1.enable(PWM_3, false);
	pwm_1.set_period(PWM_3, 0.0025);
	pwm_1.set_blank(PWM_3, 1);
	// PWM_3 - servo
	pwm_1.enable(PWM_4, false);
	pwm_1.set_period(PWM_4, 0.0025);
	pwm_1.set_blank(PWM_4, 1);

//==============================================================================================//
//											AUX RADIO INITIALIZATION							//
//==============================================================================================//
#ifdef AUX_RADIO
	//--------------------------------1.! FORCED INITIALIZATION---------------------------//
	// in case the FPGA is not reprogrammed (FPGA is not in initial state)
	// clearing both CONTROL and STATUS register will force the AUX to reset
	std::cout<<"HPS : 1.! clear AUX_RADIO registers     " << std::flush;
	aux_radio.hard_reset();
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.1 WAIT FOR AUX RESET STATE------------------------//
	std::cout<<"HPS : 1.1 AUX_RADIO reset flag          " << std::flush;
	while(aux_radio.check_status_flag_reset() == false){
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	};
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.2 PASS INIT DATA TO AUX---------------------------//
	// pass initialization data to AUX_RADIO
	std::cout<<"HPS : 1.2 hps2aux data to AUX_RADIO     " << std::flush;
	aux_radio.store_hps2aux_data((int32_t *)aux_radio_hps2aux, aux_radio.get_hps2aux_length());
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.3 SET ENABLE BIT----------------------------------//
	std::cout<<"HPS : 1.3 enable AUX_RADIO              " << std::flush;
	aux_radio.clear_control_reset();
	aux_radio.set_control_enable();
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.4 AUX_RADIO IS IN INITIALIZATION------------------//
	std::cout<<"HPS : 1.4 AUX_RADIO initialization      " << std::flush;
	while(aux_radio.check_status_flag_init() == false){
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
	};
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.5 AUX_RADIO IS EXECUTION STATE--------------------//
	std::cout<<"HPS : 1.5 AUX_RADIO in execution state  " << std::flush;
	while(aux_radio.check_status_flag_init() == false);
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.5a AUX_RADIO ENABLE SPECIFIC ROUTINE--------------//
	std::cout<<"HPS : 1.6 AUX_RADIO enable radio routine" << std::flush;
	aux_radio.set_routine_control_enable(0);
	std::cout<<"[-]" << std::endl << std::flush;
#endif

//==============================================================================================//
//											AUX IMU INITIALIZATION								//
//==============================================================================================//
#ifdef AUX_IMU
	//--------------------------------1.! FORCED INITIALIZATION---------------------------//
	// in case the FPGA is not reprogrammed (FPGA is not in initial state)
	// clearing both CONTROL and STATUS register will force the AUX to reset
	std::cout<<"HPS : 1.! clear AUX_IMU registers       " << std::flush;
	aux_imu.hard_reset();
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.1 WAIT FOR AUX RESET STATE------------------------//
	std::cout<<"HPS : 1.1 AUX_IMU assert reset          " << std::flush;
	while( !aux_imu.check_status_flag_reset() );
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.2 PASS INIT DATA TO AUX---------------------------//
	std::cout<<"HPS : 1.2 pass init data to AUX_IMU     " << std::flush;
	aux_imu.store_hps2aux_data((int32_t*)aux_imu_hps2aux, 6);
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.3 SET ENABLE BIT----------------------------------//
	std::cout<<"HPS : 1.3 enable AUX_IMU                " << std::flush;
	aux_imu.clear_control_reset();
	aux_imu.set_control_enable();
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.4 AUX_IMU IS IN INITIALIZATION--------------------//
	std::cout<<"HPS : 1.4 AUX_IMU initialization        " << std::flush;
	while( !aux_imu.check_status_flag_enable() );
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.5 AUX_IMU IS EXECUTION STATE----------------------//
	std::cout<<"HPS : 1.5 AUX_IMU in execution state    " << std::flush;
	while( !aux_imu.check_status_flag_init() );
	std::cout<<"[-]" << std::endl << std::flush;
	//--------------------------------1.6 AUX_IMU ENABLE SPECIFIC ROUTINE-----------------//
	// take a little break
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	// MPU objects
	MPU6050 * mpu[AUX_IMU_ROUTINE];

	//AUX_IMU_MAX
	for(int i = 0; i<AUX_IMU_ROUTINE; i++){
		// MPU object
		mpu[i] = new MPU6050(imu_i2c_address, &aux_imu_i2c);
		aux_imu_select.set(imu_i2c_active[i]);
		mpu[i]->initialize();
		// set offsets
		mpu[i]->setXGyroOffset(220);
		mpu[i]->setYGyroOffset(76);
		mpu[i]->setZGyroOffset(-85);
		mpu[i]->setZAccelOffset(1788); // 1688 factory default for my test chip
		// have a small break, to let the I2C finish
		std::this_thread::sleep_for(std::chrono::milliseconds(8));
	}

	// enable
	for(int i = 0; i<AUX_IMU_ROUTINE; i++){
		aux_imu.set_routine_control_enable(i);
	}

	// disable
	for(int i = 0; i<(aux_imu_routine_count-AUX_IMU_ROUTINE); i++){
		aux_imu.set_routine_control_enable(AUX_IMU_ROUTINE+i);
	}

#endif

//==============================================================================================================================================//
//											MAIN LOOP																							//
//==============================================================================================================================================//
	while(true){
		//------------------------------------------------------------------------------------//
		//--------------------------------AUX_RADIO DATA--------------------------------------//
		//------------------------------------------------------------------------------------//
		#ifdef AUX_RADIO_ROUTINE
			// check for new packet
			if( aux_radio.check_routine_status_flag_pkt_rdy(0)){
				#ifdef PERFORMANCE
					aux_radio_valid_packet_counter++;
				#endif

				if( aux_radio.check_routine_status_flag_valid(0) ){
					// clear routine status flag : pkt_rdy, valid
					aux_radio.clear_routine_status_flag_pkt_rdy(0);
					aux_radio.clear_routine_status_flag_valid(0);

					// retrieve new information
					aux_radio.get_routine_packet(0, (int32_t*)channels, NUMBER_OF_CHANNELS);

					// RC PWM
					channel_value = 100 * ( ((float) ( ((int)channels[0]) - CHANNEL_LOW)) / CHANNEL_RANGE );
					pwm_0.set_duty_cycle(PWM_0, 50+(channel_value/2));
					channel_value = 100 * ( ((float) ( ((int)channels[1]) - CHANNEL_LOW)) / CHANNEL_RANGE );
					pwm_0.set_duty_cycle(PWM_1, 50+(channel_value/2));
					channel_value = 100 * ( ((float) ( ((int)channels[2]) - CHANNEL_LOW)) / CHANNEL_RANGE );
					pwm_0.set_duty_cycle(PWM_2, 50+(channel_value/2));
					channel_value = 100 * ( ((float) ( ((int)channels[3]) - CHANNEL_LOW)) / CHANNEL_RANGE );
					pwm_0.set_duty_cycle(PWM_3, 50+(channel_value/2));

					// LED
					channel_value = 100 * ( ((float) ( ((int)channels[0]) - CHANNEL_LOW)) / CHANNEL_RANGE );
					pwm_0.set_duty_cycle(PWM_4, channel_value);

				}else{
					// data have incorrect check-sum
					// clear routine status flag : pkt_rdy
					aux_radio.clear_routine_status_flag_pkt_rdy(0);

					#ifdef PERFORMANCE
						aux_radio_INvalid_packet_counter++;
					#endif
				}
			}
		#endif
		//------------------------------------------------------------------------------------//
		//--------------------------------AUX_IMU DATA----------------------------------------//
		//------------------------------------------------------------------------------------//
		#ifdef AUX_IMU
			// check for new data packets
			for(int i = 0; i<AUX_IMU_ROUTINE; i++){
				// new data available
				if( aux_imu.check_routine_status_flag_pkt_rdy(i) ){
					// fetch the data packet
					aux_imu.get_routine_packet(i, imu_data, 6);
					accel[i].x = imu_data[0];
					accel[i].y = imu_data[1];
					accel[i].z = imu_data[2];
					gyro[i].x  = imu_data[3];
					gyro[i].y  = imu_data[4];
					gyro[i].z  = imu_data[5];
					// clear routine status flag : pkt_rdy
					aux_imu.clear_routine_status_flag_pkt_rdy(i);

					#ifdef PERFORMANCE
						aux_imu_packet_counter[i]++;
					#endif
				}
			}
			// further data processing here
		#endif
		//------------------------------------------------------------------------------------//
		//--------------------------------PERFORMANCE TESTING---------------------------------//
		//------------------------------------------------------------------------------------//
#ifdef PERFORMANCE
			main_loop_counter++;

			end = std::chrono::high_resolution_clock::now();
			// print
			if( (float)(std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count()) > 500) {
				// restart timer
				begin = std::chrono::high_resolution_clock::now();
				end = begin;

				// console print format
				//    hh:mm:ss(000)   radio(00/00){ 000, 000 | 000, 000 | 000, 000 }   imu(00/00/00){ 000 , 000 , 000 | 000 , 000 , 000 | 000 , 000 , 000 }
				total_time_end = std::chrono::high_resolution_clock::now();
				total_time = (int)(std::chrono::duration_cast<std::chrono::seconds>(total_time_end-total_time_begin).count());
				seconds = total_time%60;
				minutes = total_time/60;
				hours = minutes/60;
				minutes = minutes%60;

				// print formated time
				std::cout << std::fixed;
				std::cout << std::setprecision(2);
				std::cout << "   " << std::setw(2) << hours << ":" << std::setw(2) << minutes << ":" << std::setw(2) << seconds << "(" << main_loop_counter / 1000 << "k)";
#ifdef AUX_RADIO
				std::cout << std::setprecision(3);
				std::cout << "   radio(" << std::setw(3) << aux_radio_valid_packet_counter << "/" << std::setw(3) << aux_radio_INvalid_packet_counter << ")";
				std::cout << "{" << std::setw(4) << (int)( (channels[0]-CHANNEL_CENTER) ) << ",";
				std::cout <<        std::setw(4) << (int)( (channels[1]-CHANNEL_CENTER) ) << "|";
				std::cout <<        std::setw(4) << (int)( (channels[2]-CHANNEL_CENTER) ) << ",";
				std::cout <<        std::setw(4) << (int)( (channels[3]-CHANNEL_CENTER) ) << "|";
				std::cout <<        std::setw(4) << (int)( (channels[4]-CHANNEL_CENTER) ) << ",";
				std::cout <<        std::setw(4) << (int)( (channels[5]-CHANNEL_CENTER) ) << "}";
#endif
#ifdef AUX_IMU
				std::cout << std::setprecision(2);
				std::cout << "   imu(" << std::setw(3) << aux_imu_packet_counter[0] << "/" << std::setw(3) << aux_imu_packet_counter[1] << "/" << std::setw(3) << aux_imu_packet_counter[2] << ")";
				int divider = 1024;
				std::cout << "{" << std::setw(4) << (int)(accel[0].x/divider) << "," << std::setw(4) << (int)(accel[0].y/divider) << "," << std::setw(4) << (int)(accel[0].z/divider) << "|";
				std::cout <<        std::setw(4) << (int)(accel[1].x/divider) << "," << std::setw(4) << (int)(accel[1].y/divider) << "," << std::setw(4) << (int)(accel[1].z/divider) << "|";
				std::cout <<        std::setw(4) << (int)(accel[2].x/divider) << "," << std::setw(4) << (int)(accel[2].y/divider) << "," << std::setw(4) << (int)(accel[2].z/divider) << "}";
#endif
				std::cout << std::endl;

				// reset counters
				main_loop_counter = 0;
				aux_radio_valid_packet_counter = 0;
				aux_radio_INvalid_packet_counter = 0;
				for(int i = 0; i<AUX_IMU_ROUTINE; i++){
					aux_imu_packet_counter[i] = 0;
				};
			}
#endif
	}

	return 0;
}

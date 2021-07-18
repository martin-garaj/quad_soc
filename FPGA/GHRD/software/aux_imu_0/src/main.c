//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//
//==============================================================================================//
//											COMMENTS											//
//==============================================================================================//
// LEDs

//==============================================================================================//
//											#LEDs												//
//==============================================================================================//
// LED Patterns

// | ON | ON | ON | ON | - Reset state
#define RESET_STATE			(0b00001111)
// | OFF| ON | ON | OFF| - Initialization done
#define INITIALIZATION		(0b00000110)

// | OFF| OFF| OFF| ON | - Packet received
#define PACKET_READY		(0b00000001)
// | ON | ON | OFF| OFF| - Packet not received
#define PACKET_NOT_READY	(0b00001100)

// | ON | OFF| OFF| OFF| - Stalling
#define STALLING			(0b00001000)
//==============================================================================================//
//											#DATA_TO_SLAVE										//
//==============================================================================================//
// address AUX_IMU_DATA_0_TO_SLAVE, expects the following data
//		address of MPU6050 module when it is selected (address, when the AD0 pin is HIGH) = 0x69
//
// address AUX_IMU_DATA_1_TO_SLAVE, expects the following data
//		position of pin, which selects IMU
//		example in binary = 1000 0100 0010 0001
//		example in uint32_t HEX = 0x00008421
//		example in uint32_t DEC = 33825
//
// address AUX_IMU_DATA_2_TO_SLAVE, expects the following data
//		number of cycles to "sleep" in between the cycles of general routine
//		example in uint32_t DEC = 1000 // 1000 cycles of sleep
//
//==============================================================================================//
//											#INCLUDEs											//
//==============================================================================================//
#include <stdio.h>
#include <string.h>
#include <unistd.h>			// usleep()
//#include <alt_types.h>	// alt_8, alt_u8, alt_16, alt_8

#include "sys/alt_irq.h"	// interrupt NIOS II functions
#include "sys/alt_stdio.h"	// print to console alt_putstr(...)

#include <io.h>				// low-level Avalon MM access : IOWR_8DIRECT(), ...
#include "system.h"			// FPGA memory mapping

#include "config.h"						// configuration file
#include "MPU6050_DMP_arbiter.h" 	// sensor function declarations

#include "AUX_IMU.h" 	// sensor function declarations

//==============================================================================================//
//											#DEFINEs											//
//==============================================================================================//
// timeout for i2c communication
#define TIMEOUT 			(1000)
#define IMU_BEFFER_LEN		(6) // [accel.x, accel.y, accel.z, gyro.x, gyro.y, gyro.z]
#define I2C_BEFFER_LEN		(14) // i2c buffer

int main(){
	//==============================================================================================//
	//											DECLARATIONS										//
	//==============================================================================================//
	// CONSTANTS
	aux_imu_memory_map imu;
	init_aux_imu(&imu, AUX_IMU_0_MEM_BASE);
	const uint32_t i2c_base = I2C_0_BASE;

	// VARIABLES
	// IMU Routine variables
	uint32_t imu_buffer[IMU_BEFFER_LEN];
	uint8_t i2c_buffer[I2C_BEFFER_LEN];
	// IMU I2C address (7 bits long)
	uint32_t imu_i2c_address;
	// IMU I2C active condition
	uint8_t imu_i2c_active[imu.routine.count];
	// control_reg value
	uint32_t control_reg = 0;
	uint32_t routine_control_reg = 0;
	// initialization bool (do initialization only once)
	bool init_done = false;
	// i2c function return value flag
	uint8_t i2c_flag = 0;
	// number of cycles to sleep in between on general routine cycles
	uint32_t sleep = 0;

#ifdef _DEBUG_
	alt_printf("AUX_IMU\n");
#endif

	//==============================================================================================//
	//											MAIN LOOP											//
	//==============================================================================================//
	while(true){
		// GET CURRENT VALUE OF CONTROL_REG------------------------------------//
		control_reg = get_control_reg(imu);

		//==============================================================================================//
		//												RESET STATE										//
		//==============================================================================================//
		if(	!((control_reg & imu.control.reset_n)==imu.control.reset_n) ){

			// initialization bool
			init_done = false;
			routine_control_reg = 0;
			// IMU variables
			imu_i2c_address = 0;
			i2c_flag = 0;
			sleep = 0;
			// set status flag : reset
			set_status_flag_reset(imu);
			// Reset state
			SET_32REGISTER(AUX_IMU_0_LED_BASE, 0x00, RESET_STATE);

#ifdef _DEBUG_
			alt_printf("R");
#endif

		//==============================================================================================//
		//											EXECUTION STATE										//
		//==============================================================================================//
		}else if( (control_reg & imu.control.enable)==imu.control.enable ){
			// set status flag : enable
			set_status_flag_enable(imu);
#ifdef _DEBUG_
			alt_printf("E");
#endif
			//==============================================================================================//
			//											INITIALIZATION										//
			//==============================================================================================//
			if( !init_done ){
#ifdef _DEBUG_
			alt_printf("Initialization ...");
#endif
				// read hps2aux data
				imu_i2c_address = get_hps2aux_reg(imu, 0);		// active i2c address of IMUs
				imu_i2c_active[0] = get_hps2aux_reg(imu, 1);	// status of 1st IMU
				imu_i2c_active[1] = get_hps2aux_reg(imu, 2);	// status of 2nd IMU
				imu_i2c_active[2] = get_hps2aux_reg(imu, 3);	// status of 3rd IMU
				imu_i2c_active[3] = get_hps2aux_reg(imu, 4);	// status of 4th IMU
				sleep = get_hps2aux_reg(imu, 5);
#ifdef _DEBUG_
				alt_printf("\n");
				alt_printf("addr  : %x\n", imu_i2c_address);
				alt_printf("ac[0] : %x\n", imu_i2c_active[0]);
				alt_printf("ac[1] : %x\n", imu_i2c_active[1]);
				alt_printf("ac[2] : %x\n", imu_i2c_active[2]);
				alt_printf("ac[3] : %x\n", imu_i2c_active[3]);
				alt_printf("sleep : %d\n", sleep);
#endif

				// set initialization bool to stop looping
				init_done = true;
				// set status flag : init
				set_status_flag_init(imu);
				// Initialization
				SET_32REGISTER(AUX_IMU_0_LED_BASE, 0x00, INITIALIZATION);

#ifdef _DEBUG_
			alt_printf("Initialization DONE");
#endif
			//==============================================================================================//
			//												ROUTINES										//
			//==============================================================================================//
			}else{
				routine_control_reg = get_routine_control_reg(imu);
				//==============================================================================================//
				//										GENERAL ROUTINE											//
				//==============================================================================================//
				// this routine is executed if AUX is not in reset and enabled
#ifdef _DEBUG_
			alt_printf("GR");
#endif
				//
				// ENTER ROUTINE HERE
				//

				//==============================================================================================//
				//										ENABLE-SPECIFIC ROUTINE									//
				//==============================================================================================//
				// routines executed only when specific enable-bit is set
					// IMU ROUTINE---------------------------------------------------------//
					for(uint8_t i = 0; i<imu.routine.count; i++){
						if( (routine_control_reg & imu.routine.control.enable[i]) == imu.routine.control.enable[i] ){
							set_routine_status_flag_enable(imu, i);

							// reset variables so one i2c doesn't interfere with other i2c
							for(int k = 0; k<IMU_BEFFER_LEN; k++){
								imu_buffer[k] = 0x00000000;
							}
							for(int k = 0; k<I2C_BEFFER_LEN; k++){
								i2c_buffer[k] = 0x00;
							}
							// select IMU (change the i2c address)
							SET_32REGISTER(SELECT_0_BASE, 0x00, imu_i2c_active[i]);
							// read the data from the selected IMU
							i2c_flag = getMotion6(i2c_base, imu_i2c_address, &imu_buffer[0], &imu_buffer[1], &imu_buffer[2], &imu_buffer[3], &imu_buffer[4], &imu_buffer[5], i2c_buffer, TIMEOUT);
							// de-select IMU (change the i2c address)
							SET_32REGISTER(SELECT_0_BASE, 0x00, 0x00);

							// handle new data
							if(i2c_flag > 0){
#ifdef _DEBUG_
			alt_printf("| +  %x  %x"  , i, imu_i2c_active[i]);
#endif

								// store received data
								store_packet32(imu, i, IMU_BEFFER_LEN, imu_buffer);
								// set routine status flag : pkt_rdy
								set_routine_status_flag_pkt_rdy(imu, i);
								// packet ready
								SET_32REGISTER(AUX_IMU_0_LED_BASE, 0x00, PACKET_READY);

							// I2C communication error
							}else{
#ifdef _DEBUG_
			alt_printf("| -  %x  %x"  , i, imu_i2c_active[i]);
#endif

								// Properly handle re-initialization
								//alt_printf("-");
								//alt_printf("RE-INITIALIZATION ------|");
								//setClockSource(i2c_base, imu_i2c_address,  MPU6050_CLOCK_PLL_XGYRO, TIMEOUT);
								//setFullScaleGyroRange(i2c_base, imu_i2c_address,  MPU6050_GYRO_FS_250, TIMEOUT);
								//setFullScaleAccelRange(i2c_base, imu_i2c_address,  MPU6050_ACCEL_FS_2, TIMEOUT);
								//setSleepEnabled(i2c_base, imu_i2c_address,  false, TIMEOUT);
								// REQUIRES HARDWARE RESET OF THE SENSOR

								// clear routine status flag : pkt_rdy
								clear_routine_status_flag_pkt_rdy(imu, i);
								// packet not ready
								SET_32REGISTER(AUX_IMU_0_LED_BASE, 0x00, PACKET_NOT_READY);
							}

						}else{
							// clear routine status flag : enable
							clear_routine_status_flag_enable(imu, i);
						}// if routine == enable
					} // for every routine

					// OTHER ROUTINE-------------------------------------------------------//
					// other routine goes below this line

					//
					// ENTER ROUTINE HERE
					//

					//==============================================================================================//
					//										SLEEP													//
					//==============================================================================================//
					// postpone the next cycle of general routine
					usleep(sleep);

			}
			// NO CODE BELOW THIS LINE---------------------------------------------//

		//==============================================================================================//
		//												STALLING										//
		//==============================================================================================//
		}else{
			// clear status flag : enable
			clear_status_flag_enable(imu);
			// stalling
			SET_32REGISTER(AUX_IMU_0_LED_BASE, 0x00, STALLING);
		};
#ifdef _DEBUG_
			alt_printf("\n");
#endif
	}// while

	return 0;
}// main

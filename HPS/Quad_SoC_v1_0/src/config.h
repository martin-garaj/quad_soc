//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//

#ifndef _CONFIG_H_
#define _CONFIG_H_

//==============================================================================================//
//											CONFIGURATION										//
//==============================================================================================//
//																								//
//	The following file configures the project before the compilation.							//
//																								//
//==============================================================================================//
//--------------------------------------------------------------//
//							PLATFORM							//
//--------------------------------------------------------------//
	#define ARM_HPS		// certain functions use C++ features
	//#define NIOS_II		// certain C functions use Nios II macros

//--------------------------------------------------------------//
//							CONSTANTS							//
//--------------------------------------------------------------//
#define FPGA_FREQ			100*1000*1000 			// in Hz (1 MHz = 1000*1000 Hz)
#define HPS_FREQ			900*1000*1000			// in Hz (1 MHz = 1000*1000 Hz)

//--------------------------------------------------------------//
//							FUNCTIONALITY						//
//--------------------------------------------------------------//
#define PERFORMANCE							// uncomment to enable performance testing console print
#define DEBUG								// uncomment to enable debugging console print
#define LOG_FILE			"log_file.txt"

//--------------------------------------------------------------//
//							ROUTINE								//
//--------------------------------------------------------------//
	#define AUX_RADIO
	#ifdef AUX_RADIO
		#define AUX_RADIO_ROUTINE
	#endif

	#define AUX_IMU
	#ifdef AUX_RADIO
		#define AUX_IMU_ROUTINE		3
	#endif


#endif /* _CONFIG_H_ */

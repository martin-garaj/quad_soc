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
//							DEBUG								//
//--------------------------------------------------------------//
	//#define _DEBUG_		// aux_imu uses console print


//--------------------------------------------------------------//
//							PLATFORM							//
//--------------------------------------------------------------//
	//#define ARM_HPS		// certain functions use C++ features
	#define NIOS_II		// certain C functions use Nios II macros


//--------------------------------------------------------------//
//							ROUTINE								//
//--------------------------------------------------------------//
	// maximum number of IMU routines serviced by AUX_IMU
	#define MAX_IMUs	3		// testing showed the AUX_IMU can service at max 2 IMUs


#endif /* _CONFIG_H_ */

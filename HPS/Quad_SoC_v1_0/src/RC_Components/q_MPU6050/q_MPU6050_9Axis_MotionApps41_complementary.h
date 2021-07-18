//================================================================//
//                                                                //
//	AUTHOR 	: Martin Garaj <garaj.martin@gmail.com>               //
//	PROJECT : Cyclone V Quadcopter (2017)                         //
//                                                                //
//================================================================//

//==============================================================================================//
//										FILE DESCRIPTION										//
//==============================================================================================//
//																								//
//	The MPU6050_9Axis_MotionApps41_complementary.h is a complementary file to the 				//
//	MPU6050_9Axis_MotionApps41.h file, which is originally written for platform, which natively //
//	includes #define and/or functions, otherwise not present in current platform/compiler.		//
//	Simply said, this file includes the missing stuff, which is not automatically available on	//
//	Linux, but it is automatically present on platforms such as Arduino.						//
//																								//
//==============================================================================================//

#ifndef _Q_MPU6050_9AXIS_MOTIONAPPS41_COMPLEMENTARY_H_
#define _Q_MPU6050_9AXIS_MOTIONAPPS41_COMPLEMENTARY_H_


/* this definition is required for q_MPU6050.h file,
 * which enabled the declaration of MotionApp 4.1 functions
 * defined in q_MPU6050_9Axis_MotionApps41.h
 */
//#define MPU6050_INCLUDE_DMP_MOTIONAPPS41

// type defined within Arduino platform
#define prog_uchar uint8_t

// function defined within Arduino framework (https://www.arduino.cc/en/reference/min)
int min(int val0, int val1){
	if(val0<=val1){
		return val0;
	}
	return val1;
}

#endif /* _Q_MPU6050_9AXIS_MOTIONAPPS41_COMPLEMENTARY_H_ */

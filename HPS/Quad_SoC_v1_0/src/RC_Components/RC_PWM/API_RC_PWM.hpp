/*
//============================================================================
// Name        : MPSO_PVP.cpp
// Authors     : Roberto Fernandez Molanes 	(robertofem@gmail.com)
//				 Martin Garaj 				(garaj.martin@gmail.com)
//				 Jianfeng Zhou				(599102934@qq.com)
// Version     : 1.0 (February 2017)
// Copyright   : -
// Description : This files define a class to easily control the UART_module
//				 component in the FPGA. The UART_module is capable of transmitting
//				 and receiving 1 byte at the time. No FIFO buffer is present, as
//				 for the current application.
//				 The communication is based on POLLING, the flags for receiving and
//				 transmitting needs to be checked prior to reading/writing to RX
//				 and TX registers.
//============================================================================

#ifndef _API_RC_PWM_HPP_
#define _API_RC_PWM_HPP_

// DEBUG
#include <iostream>
//#include "diagnostics.hpp"

#include <stdio.h> 	// memcpy
#include <stdint.h> // uint8_t, uint16_t
#include <cmath> // pow()
#include "system_const.h"
#include "API_PWM_module.hpp"

// basic frequencies for RC components (ESC, servo)
#define freq_50_Hz			50
#define freq_300_Hz			300

#define maximum_PWM_modules	16

#define range_0_to_1_		1
#define range_-1_to_1_		2
#define range_0_to_100_		3

 CLASS DECLARATION
class API_RC_PWM
{
// VARIABLES //
private:
	API_PWM_module * rc_pwm;
	int PWM_params[maximum_PWM_modules][3]; // 16 PWMs [PWM_0, PWM_1, PWM_2, ... PWM_15], 3 parameters [offset_us, range_us, -1.0-to-1.0 / 0.0-to-1.0]
public:


// FUNCTIONS //
public:
	//constructor and destructor
	API_RC_PWM(API_PWM_module* pwm_module);
	~API_RC_PWM();

	int set_freq(uint32_t which_pwm, int freq_HZ);
	int set_param(uint32_t which_pwm, int offset_us, int range_us, int type);
	int set_value(uint32_t which_pwm, float duty_percent);


};

// DEFINITIONS //
* Constructor
 * @param - takes API_PWM_module pointer, the PWM need to be ENABLED

API_RC_PWM::API_RC_PWM(API_PWM_module* pwm_module){
	rc_pwm = pwm_module;

	// reset parameters
	for(int i = 0; i<maximum_PWM_modules; i++){
		for(int j = 0; j<3; j++){
			PWM_params[i][j] = 0;
		}
	}

}

* Destructor

API_RC_PWM::~API_RC_PWM(){

}



* Set the frequency of PWM
 * @param - which PWM (outputs are numbered sequentially, starting at 0, named PWM_X)
 * @param - frequency in Hz
 * @return - RETURN_ERROR / RETURN_SUCCESS

int API_RC_PWM::set_freq(uint32_t which_pwm, int freq_HZ){

	// period in us
	uint32_t period_microsecond = (1.0/freq_HZ)*1000*1000;

	// register value update
	return rc_pwm->set_period_us(which_pwm, period_microsecond);

};


* Set parameters of the RC PWM
 * @param - which PWM (outputs are numbered sequentially, starting at 0, named PWM_X)
 * @param - minimum length of a PWM signal (lower limit) in micro-seconds
 * @param - range of the PWM signal (upper limit = lower limit + range) in micro_seconds
 * @param - type of value that set_value() takes
 *           0 == un-initialized
 *           1 == range 0.0 to 1.0
 *           2 == range -1.0 to 1.0
 *           3 == range 0.0 to 100.0
 * @return - RETURN_ERROR / RETURN_SUCCESS

int API_RC_PWM::set_param(uint32_t which_pwm, int offset_us, int range_us, int type){
	if(which_pwm > (rc_pwm->quantity-1)){
		std::cout << "ERROR : API_RC_PWM::set_param, WRONG PWM" << std::endl;
		return RETURN_ERROR;
	}

	uint32_t microsecond_cycles = rc_pwm->fpga_frequency / (1000*1000);

	if(type == 1){
		// range 0.0 to 1.0
		PWM_params[which_pwm][0] = microsecond_cycles*offset_us;
		PWM_params[which_pwm][1] = microsecond_cycles*range_us;
		PWM_params[which_pwm][2] = type;
	}else if(type == 2){
		// range -1.0 to 1.0
		PWM_params[which_pwm][0] = microsecond_cycles*offset_us;
		PWM_params[which_pwm][1] = microsecond_cycles*range_us / 2;
		PWM_params[which_pwm][2] = type;
	}else if(type == 3){
		// range 0.0 to 100.0
		PWM_params[which_pwm][0] = microsecond_cycles*offset_us;
		PWM_params[which_pwm][1] = microsecond_cycles*range_us / 100;
		PWM_params[which_pwm][2] = type;
	}else{
		std::cout << "ERROR : API_RC_PWM::set_pwm, PARAM[2] = " << type << " is unexpected value" << std::endl;
	}

	return RETURN_SUCCESS;
};


* Set parameters of the RC PWM
 * @param - which PWM (outputs are numbered sequentially, starting at 0, named PWM_X)
 * @param - value of PWM according to pre-set range
 *           1 == range 0.0 to 1.0
 *           2 == range -1.0 to 1.0
 *           3 == range 0.0 to 100.0
 * @return - RETURN_ERROR / RETURN_SUCCESS

int API_RC_PWM::set_value(uint32_t which_pwm, float value){
	uint32_t cycles = PWM_params[which_pwm][0] + (PWM_params[which_pwm][1]*value);

	return rc_pwm->set_duty_cycle_reg(which_pwm, cycles);

};


#endif  _API_RC_PWM_HPP_
*/

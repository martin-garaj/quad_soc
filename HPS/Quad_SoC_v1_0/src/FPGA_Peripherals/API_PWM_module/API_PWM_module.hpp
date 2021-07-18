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

#ifndef _API_PWM_MODULE_HPP_
#define _API_PWM_MODULE_HPP_

// DEBUG
#include <iostream>
//#include "diagnostics.hpp"

#include <stdio.h> 	// memcpy
#include <stdint.h> // uint8_t, uint16_t
#include <cmath> // pow() log
#include "system_const.h"

#define CONTROL_REG			0x00
#define STATUS_REG			0x01
#define PRESCALER_REG		0x02
#define INFO_0_REG			0x03
#define INFO_1_REG			0x04
#define INFO_2_REG			0x05
#define INFO_3_REG			0x06
#define BLANK_REG_OFFSET	0x10
#define PERIOD_REG_OFFSET	0x20
#define DUTY_REG_OFFSET		0x30

// CONTROL_REG bits
// reset bit mask
#define RESET_BIT			0x00000001
#define PWM_ENABLE_OFFSET	1

// PWM MODULE MEMORY MAP
// QUANTITY - Qsys parameter
//											| PERIOD_REG_OFFSET --------------------> | DUTY_REG_OFFSET ------------->
//---------------------------------------------------------------------------------------------------------------------
// ADDRESS 	: 	|     0x00    |     0x01    |     0x02    |     0x03    |     ....    |0x02+QUANTITY|0x03+QUANTITY| ...
// REGISTER :	| CONTROL_REG | STATUS_REG  |PWM_PERIOD_0 |PWM_PERIOD_1 |     ....    | PWM_DUTY_0  | PWM_DUTY_1  | ...
//
//

#define PWM_0				0
#define PWM_1				1
#define PWM_2				2
#define PWM_3				3
#define PWM_4				4
#define PWM_5				5
#define PWM_6				6
#define PWM_7				7
#define PWM_8				8
#define PWM_9				9
#define PWM_10				10
#define PWM_11				11
#define PWM_12				12
#define PWM_13				13
#define PWM_14				14
#define PWM_15				15

#define PWM_ALL				0x11111111


/* CLASS DECLARATION */
class API_PWM_module
{
// VARIABLES //
private:
	uint32_t* pwm_module_addr; //base addresses of UART_module
protected:
	uint32_t pwm_counter_max_len;
	uint32_t prescaler_counter_max_len;
	uint32_t blank_counter_max_len;
	uint32_t pwm_counter_max_val;
	uint32_t prescaler_counter_max_val;
	uint32_t blank_counter_max_val;
	int hps_frequency;
	int pwm_module_frequency;
public:
	uint32_t quantity;
	int fpga_frequency;
	static void *virtual_base;
	static int instance_counter;

// FUNCTIONS //
public:
	//constructor and destructor
	API_PWM_module(uint32_t * pwm_module_address, int long fpga_freq, int long hps_freq);
	~API_PWM_module();

// PWM functions
	int prescaler_setup(float desired_period_length, int desired_period_resolution);

	int reset(void);
	int set_period(uint32_t which_pwm, float period_second);
	int set_duty_cycle(uint32_t which_pwm, float duty_percent);
	int set_blank(uint32_t which_pwm, int blank_periods);
	int enable(uint32_t which_PWM, bool enable);
	int set_duty_cycle_reg(uint32_t which_pwm, uint32_t reg_val);

};

void * API_PWM_module::virtual_base = NULL;
int API_PWM_module::instance_counter = 0;

// DEFINITIONS //

API_PWM_module::API_PWM_module(uint32_t * pwm_module_address, int long fpga_freq, int long hps_freq){
	pwm_module_addr = pwm_module_address;
	quantity = pwm_module_addr[INFO_0_REG];
	prescaler_counter_max_len = pwm_module_addr[INFO_1_REG];
	pwm_counter_max_len = pwm_module_addr[INFO_2_REG];
	blank_counter_max_len = pwm_module_addr[INFO_3_REG];
	fpga_frequency = fpga_freq;
	hps_frequency = hps_freq;

	prescaler_counter_max_val = (uint32_t)std::pow(2,prescaler_counter_max_len);
	pwm_counter_max_val = (uint32_t)std::pow(2,pwm_counter_max_len);
	blank_counter_max_val = (uint32_t)std::pow(2,blank_counter_max_len);

}


API_PWM_module::~API_PWM_module(){
	pwm_module_addr[CONTROL_REG] = 0x00000000; // shut-down the PWM module
}

/** Sets up the prescaler value
 * @param - desired minimum length of 1 period (in seconds) for all PWM outputs
 * @param - desired maximum resolution (number of step-wise adjustments of duty cycle within 1 period) of period with minimum length
 */
int API_PWM_module::prescaler_setup(float desired_period_min_length, int desired_period_max_resolution){

	float resolution = desired_period_max_resolution;
	float length = desired_period_min_length;
	float fpga_freq = (float)fpga_frequency;
	float prescaler;

	prescaler = (fpga_freq/(2.0 * resolution))*length - 1.0;

	if( ceil(log2(prescaler)) > prescaler_counter_max_len ){
		std::cout << "ERROR : API_PWM_module::prescaler_setup, prescaler counter is not long enough, required : " << ceil(log2(prescaler)) << " [bits], available : " << prescaler_counter_max_len << " [bits]" << std::endl;
		return RETURN_ERROR;
	}else{
		pwm_module_addr[PRESCALER_REG] = (int)prescaler;
		pwm_module_frequency = (float)(fpga_freq / ((floor(prescaler)+1.0)*2.0) );
		std::cout << "DEBUG : API_PWM_module::prescaler_setup, prescaler set to : " << (int)prescaler << std::endl;
	}
	return RETURN_SUCCESS;
};


/** Resets the PWM module (the reset is enabled, then the function waits, then disables the reset)
 */
int API_PWM_module::reset(void){
	// reset procedure
	pwm_module_addr[CONTROL_REG] = 0x00000000; // reset is when RESET_BIT == 0
	usleep(25000); // sleep 0.25 second
	pwm_module_addr[CONTROL_REG] = RESET_BIT;	// module is enabled when RESET_BIT == 1

	return RETURN_SUCCESS;
}

/** Sets the period
 * @param - which PWM (outputs are numbered sequentially, starting at 0, named PWM_X)
 * @param - 0-100% value of duty cycle
 */
int API_PWM_module::set_period(uint32_t which_pwm, float period_second){
	if(which_pwm > (quantity-1)){
		std::cout << "ERROR : API_PWM_module::set_period, WRONG PWM" << std::endl;
		return RETURN_ERROR;
	}

	float cycles_per_second = (float)pwm_module_frequency;


	int period_cycles = (cycles_per_second * period_second);
	if( period_cycles >= pwm_counter_max_val ){
		// prevent overflow
		pwm_module_addr[PERIOD_REG_OFFSET+which_pwm] = pwm_counter_max_val-1; // period is from 0 to 2^(COUNTER_SIZE)-1
		std::cout << "ERROR : API_PWM_module::set_period_ms, OVERFLOW, pwm_counter_max_val < value (" << pwm_counter_max_val << " < " << period_cycles << "), value set to "<< pwm_counter_max_val << std::endl;
		return RETURN_ERROR;
	}else if( period_cycles < 0){
		pwm_module_addr[PERIOD_REG_OFFSET+which_pwm] = 0;
		std::cout << "ERROR : API_PWM_module::set_period_ms, UNDERFLOW, 0 > value (" << 0 << " > " << period_cycles << "), value set to 0 instead" << std::endl;
		return RETURN_ERROR;
	}else{
		pwm_module_addr[PERIOD_REG_OFFSET+which_pwm] = period_cycles; // period is from 0 to 2^(COUNTER_SIZE)-1
	}


	return RETURN_SUCCESS;
}


/** Sets the duty cycle (PERIOD HAS TO BE SET PRIOR TO DUTY CYCLE)
 * @param - which PWM (outputs are numbered sequentially, starting at 0, named PWM_X)
 * @param - 0-100% value of duty cycle
 */
int API_PWM_module::set_duty_cycle(uint32_t which_pwm, float duty_percent){
	if(which_pwm > (quantity-1)){
		std::cout << "ERROR : API_PWM_module::set_duty_cycle, WRONG PWM" << std::endl;
		return RETURN_ERROR;
	}

	// period is from 0 to 2^(COUNTER_SIZE)-1
	// duty cycle is from 0 to max(period)+1 (e.g. period is 3, then I need values 1, 2, 3 (to cover whole range when duty cycle != 0) and 0, when duty_cycle == 0)
	int long period = pwm_module_addr[PERIOD_REG_OFFSET+which_pwm];
	if(duty_percent <= 0){
		pwm_module_addr[DUTY_REG_OFFSET+which_pwm] = 0;	// minimum
	}else if(duty_percent >= 100){
		pwm_module_addr[DUTY_REG_OFFSET+which_pwm] = period + 1;	// maximum
	}else{
		// duty_cycle = (duty_percent/100)*(period);
		pwm_module_addr[DUTY_REG_OFFSET+which_pwm] = (int)( ((period * duty_percent)+1) / 100 ); // +1 = round up
	}

	return RETURN_SUCCESS;
}





/** Add blank periods, between the "active" periods, these "passive" periods dont have any duty cycle
 * @param - which PWM (outputs are numbered sequentially, starting at 0, named PWM_X)
 * @param - number of blank periods added, after the "active" period with duty cycle
 */
int API_PWM_module::set_blank(uint32_t which_pwm, int blank_periods){
	if(which_pwm > (quantity-1)){
		std::cout << "ERROR : API_PWM_module::set_blank, WRONG PWM" << std::endl;
		return RETURN_ERROR;
	}

	if(blank_periods <= 0){
		pwm_module_addr[BLANK_REG_OFFSET+which_pwm] = 0;	// minimum
		return RETURN_ERROR;
	}else if(blank_periods >= blank_counter_max_val){
		pwm_module_addr[BLANK_REG_OFFSET+which_pwm] = blank_counter_max_val - 1;	// maximum
		return RETURN_ERROR;
	}else{
		// duty_cycle = (duty_percent/100)*(period);
		pwm_module_addr[BLANK_REG_OFFSET+which_pwm] = blank_periods;
	}

	return RETURN_SUCCESS;
}




/** Enables/disabled the PWM output
 * @param - which PWM (outputs are numbered sequentially, starting at 0, named PWM_X)
 * @param - TRUE (enables PWM output) / FALSE (disables PWM output)
 */
int API_PWM_module::enable(uint32_t which_pwm, bool enable){
	if(which_pwm > (quantity-1)){
		std::cout << "ERROR : API_PWM_module::enable, WRONG PWM" << std::endl;
		return RETURN_ERROR;
	}

	// get the current value of CONTROL_REG
	uint32_t control_reg = pwm_module_addr[CONTROL_REG];
	// go from decimal representation to one_hot bit representation
	int one_hot = 1<<which_pwm;

	// the if...else...then statement does :
	// a) avoid deleting the first bit in control register (RESET_BIT)
	// b) update the bit representing specific PWM in CONTROL_REG
	// c) keep the value of RESET_BIT
	if(enable){
		// set the PWM bit HIGH
		pwm_module_addr[CONTROL_REG] = ( control_reg | (one_hot<<PWM_ENABLE_OFFSET) ) | (control_reg & RESET_BIT);		// shifted to the left because the first bit is RESET_BIT
	}else{
		// set the PWM bit LOW
		pwm_module_addr[CONTROL_REG] = ( control_reg & (~(one_hot<<PWM_ENABLE_OFFSET)) ) | (control_reg & RESET_BIT);		// shifted to the left because the first bit is RESET_BIT
	}

	return RETURN_SUCCESS;

};


int API_PWM_module::set_duty_cycle_reg(uint32_t which_pwm, uint32_t reg_val){
	if(which_pwm > (quantity-1)){
		std::cout << "ERROR : API_PWM_module::set_duty_cycle_reg, WRONG PWM" << std::endl;
		return RETURN_ERROR;
	}

	// raw access to duty_cycle register
	pwm_module_addr[DUTY_REG_OFFSET+which_pwm] = reg_val;

	return RETURN_SUCCESS;
};


#endif /* _API_PWM_MODULE_HPP_ */

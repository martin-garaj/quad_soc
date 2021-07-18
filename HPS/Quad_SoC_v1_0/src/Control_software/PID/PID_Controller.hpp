

#ifndef _PID_CONTROLLER_HPP_
#define _PID_CONTROLLER_HPP_


#include "system_const.h"




class PID_Controller
{
// VARIABLES //
public:

private:

	// PID Controller constants
	double K_p;
	double K_i;
	double K_d;
	// PID constants
	// at the moment I am not clear on why to keep these constants,
	// in the future I will probably keep only one set
	double K_P;
	double K_I;
	double K_D;
	// output limits
	double upper_limit;
	double lower_limit;

	// internal Controller variables
	double prev_state;
	double internal_i_state; // this is integral term (this holds the "memory" of previous internal states)

	// update time interval - time difference between consecutive samples
	double d_T;

	// Controller mode (DIRECT = +input == +output, REVERSE = +input == -output)
	bool reverse_mode;

// FUNCTIONS //
public:
	//constructor and destructor
	PID_Controller();
	PID_Controller(bool reverse);
	~PID_Controller();

	int initialize(double current_state, double current_i_state, double upper, double lower);
	int set_constants(double P, double I, double D, double sampling_period_ms);
	int set_output_limits(double upper, double lower);
	int calculate(double current_state, double set_point, double * output);
private:
	inline double limit(double value);

};

PID_Controller::PID_Controller(){
	K_p = 0.0;
	K_i = 0.0;
	K_d = 0.0;
	K_P = 0.0;
	K_I = 0.0;
	K_D = 0.0;
	upper_limit = 0.0;
	lower_limit = 0.0;
	prev_state = 0.0;
	reverse_mode = false;
	d_T = 0.0;
}


PID_Controller::PID_Controller(bool reverse){
	K_p = 0.0;
	K_i = 0.0;
	K_d = 0.0;
	K_P = 0.0;
	K_I = 0.0;
	K_D = 0.0;
	upper_limit = 0.0;
	lower_limit = 0.0;
	prev_state = 0.0;
	reverse_mode = reverse;
	d_T = 0.0;
}

PID_Controller::~PID_Controller(){

}


int PID_Controller::initialize(double current_state, double current_i_state, double upper, double lower){
	prev_state = current_state;
	internal_i_state = current_i_state;
	upper_limit = upper;
	lower_limit = lower;

	return RETURN_SUCCESS;
}


int PID_Controller::set_constants(double P, double I, double D, double sampling_period_ms){
	d_T = sampling_period_ms;
	K_P = P;
	K_I = I;
	K_D = D;
	K_p = P;
	K_i = I * d_T;
	K_d = D / d_T;

	// reverse mode
	if(reverse_mode == true){
		K_p = -K_p;
		K_i = -K_i;
		K_d = -K_d;
	}

	return RETURN_SUCCESS;
};


int PID_Controller::set_output_limits(double upper, double lower){
	upper_limit = upper;
	lower_limit = lower;

	return RETURN_SUCCESS;
};


int PID_Controller::calculate(double current_state, double set_point, double * output){

	// define variables
	double error = set_point - current_state;
	double d_state = current_state - prev_state;

	// calculate
	internal_i_state = internal_i_state + (K_i * error); // internal integration state
	internal_i_state = limit(internal_i_state);
	double current_output = (K_p * error) + (internal_i_state) - (K_d * d_state); // PID Controller output
	// reasoning for this PID structure (the "D" part negative), can be found at :
	// http://controlguru.com/pid-control-and-derivative-on-measurement/

	// check output for limits
	current_output = limit(current_output);

	// set output pointer
	*output = current_output;

	// update memory
	prev_state = current_state;

	return RETURN_SUCCESS;
};

inline double PID_Controller::limit(double value){
	// check for limits and correct
	if(value > upper_limit){
		return upper_limit;
	}else if(value < lower_limit){
		return lower_limit;
	}else{
		return value;
	}
};

#endif /* _PID_CONTROLLER_HPP_ */


#ifndef _API_CONFIGURATION_HPP_
#define _API_CONFIGURATION_HPP_

#include "system_const.h"
#include <iostream>		// std::cout
#include <iomanip>	 	// setprecision()

// MAXIMUM COUNTS
#define CONFIG_CONTROL_PID_COUNT		3
#define CONFIG_RADIO_CHANNEL_COUNT		10
#define CONFIG_SENSOR_IMU_COUNT			3

// structures

struct PID_struct{
	bool init_done;
	double T_d;
	double P;
	double I;
	double D;
};

struct Channel_struct{
	bool init_done;
	int limit_min;
	int center_value;
	int limit_max;
};

struct Control_struct{
	bool init_done;
	PID_struct * pid;
};

struct Radio_struct{
	bool init_done;
	int protocol;
	Channel_struct * channel;
};

struct IMU_struct{
	bool init_done;
	int component;
	int version;
	int state;
	uint8_t i2c_address;
};

struct Sensor_struct{
	bool init_done;
	IMU_struct * imu;
};

// overal structure
struct Configuration_struct{
	Control_struct * control;
	Radio_struct * radio;
	Sensor_struct * sensor;

};



// object representing structure with configuration variables
class Configuration_object{
public:
	Configuration_object();
	~Configuration_object();

	// setters
	int set_protocol(int protocol);
	int set_pid(int which_pid, double T_d, double P, double I, double D);
	int set_channel(int which_channel, int limit_min, int center_value, int limit_max);
	int set_imu(int which_mpu, int component, int version, int state, uint8_t i2c_address);
	// getters
	int get_protocol(int * protocol);
	int get_pid(int which_pid, PID_struct * return_struct);
	int get_channel(int which_channel, Channel_struct * return_struct);
	int get_imu(int which_imu, IMU_struct * return_struct);

	// other
	int print();

private:
	Configuration_struct _config;
	Control_struct _control;
	PID_struct _pid[CONFIG_CONTROL_PID_COUNT];
	Radio_struct _radio;
	Channel_struct _channel[CONFIG_RADIO_CHANNEL_COUNT];
	Sensor_struct _sensor;
	IMU_struct _imu[CONFIG_SENSOR_IMU_COUNT];
};



/** Constructor
 */
Configuration_object::Configuration_object(){
	// fill the pointers inside structures
	_config.control = &_control;
	_control.pid = _pid;
	_config.radio = &_radio;
	_radio.channel = _channel;
	_config.sensor = &_sensor;
	_sensor.imu = _imu;

	// set all final nodes to NOT initialize state (to prevent passing random memory values)
	_config.control->init_done = false;
	for(int i = 0; i<CONFIG_CONTROL_PID_COUNT; i++){
		_config.control->pid[i].init_done = false;
	}

	_config.radio->init_done = false;
	for(int i = 0; i<CONFIG_RADIO_CHANNEL_COUNT; i++){
		_config.radio->channel[i].init_done = false;
	}

	_config.sensor->init_done = false;
	for(int i = 0; i<CONFIG_SENSOR_IMU_COUNT; i++){
		_config.sensor->imu[i].init_done = false;
	}

}

/** Destructor
 */
Configuration_object::~Configuration_object(){};

int Configuration_object::set_pid(int which_pid, double T_d, double P, double I, double D){
	if(which_pid < CONFIG_CONTROL_PID_COUNT){
		_config.control->pid[which_pid].T_d = T_d;
		_config.control->pid[which_pid].P = P;
		_config.control->pid[which_pid].I = I;
		_config.control->pid[which_pid].D = D;

		_config.control->pid[which_pid].init_done = true;

		_config.control->init_done = true;

		return RETURN_SUCCESS;
	}

	return RETURN_ERROR;
};


int Configuration_object::set_protocol(int protocol){

		_config.radio->protocol = protocol;
		_config.radio->init_done = true;
		return RETURN_SUCCESS;
};

/** Set channel values
 */
int Configuration_object::set_channel(int which_channel, int limit_min, int center_value, int limit_max){
	if(which_channel < CONFIG_RADIO_CHANNEL_COUNT){
		_config.radio->channel[which_channel].limit_min = limit_min;
		_config.radio->channel[which_channel].center_value = center_value;
		_config.radio->channel[which_channel].limit_max = limit_max;

		_config.radio->channel[which_channel].init_done = true;

		_config.radio->init_done = true;

		return RETURN_SUCCESS;
	}

	return RETURN_ERROR;
}


int Configuration_object::get_pid(int which_pid, PID_struct * return_struct){
	if(which_pid < CONFIG_CONTROL_PID_COUNT){
		if(_config.control->pid[which_pid].init_done){
			// copy the structure, DONT provide pointer !
			*return_struct = _config.control->pid[which_pid];

			return RETURN_SUCCESS;
		}
	}

	return RETURN_ERROR;
};


int Configuration_object::get_protocol(int * protocol){
	if(_config.radio->init_done){
		// copy the structure, DONT provide pointer !
		*protocol = _config.radio->protocol;

		return RETURN_SUCCESS;
	}
	return RETURN_ERROR;
};


/** Get channel values
 */
int Configuration_object::get_channel(int which_channel, Channel_struct * return_struct){
	if(which_channel < CONFIG_RADIO_CHANNEL_COUNT){
		if(_config.radio->channel[which_channel].init_done){
			// copy the structure, DONT provide pointer !
			*return_struct = _config.radio->channel[which_channel];

			return RETURN_SUCCESS;
		}
	}

	return RETURN_ERROR;
}

/** Set MPU values
 */
int Configuration_object::set_imu(int which_mpu, int component, int version, int state, uint8_t i2c_address){
	if(which_mpu < CONFIG_SENSOR_IMU_COUNT){
		_config.sensor->imu[which_mpu].version = version;
		_config.sensor->imu[which_mpu].state = state;
		_config.sensor->imu[which_mpu].i2c_address = i2c_address;

		_config.sensor->imu[which_mpu].init_done = true;

		_config.sensor->init_done = true;

		return RETURN_SUCCESS;
	}

	return RETURN_ERROR;
}



/** Get MPU values
 */
int Configuration_object::get_imu(int which_imu, IMU_struct * return_struct){
	if(which_imu < CONFIG_SENSOR_IMU_COUNT){
		if(_config.sensor->imu[which_imu].init_done){
			// copy the structure, DONT provide pointer !
			*return_struct = _config.sensor->imu[which_imu];

			return RETURN_SUCCESS;
		}
	}

	return RETURN_ERROR;
}


int Configuration_object::print(){
	// the PuTTY console is by default 80 characters wide
	std::cout<<"//============================================================================//" << std::endl;
	std::cout<<"//   external_config.xml                                                      //" << std::endl;
	std::cout<<"//============================================================================//" << std::endl;
	std::cout<<"//  configuration                                                             " << std::endl;
	if(_config.control->init_done){
		std::cout<<"//      |-->control                                                             " << std::endl;
		std::cout<<"//      |   |-->pid       T_d            P         I         D" << std::endl;
	}
	for(int i = 0; i<CONFIG_CONTROL_PID_COUNT; i++){
		if(_config.control->pid[i].init_done){
			std::cout << std::fixed;
			std::cout<<"//      |        "<< i <<"         " << std::setprecision(4) << _config.control->pid[i].T_d <<"         " << std::setprecision(2) << _config.control->pid[i].P <<"      " << std::setprecision(2) << _config.control->pid[i].I <<"      " << std::setprecision(2) << _config.control->pid[i].D <<" " << std::endl;

		}
	}
	std::cout << std::setprecision(3);

	if(_config.radio->init_done){
		std::cout<<"//      |-->radio                                                             " << std::endl;
		std::cout<<"//      |   |-->protocol : "<< _config.radio->protocol <<" " << std::endl;
		std::cout<<"//      |   |-->channel   limit_min      center_value   limit_max              " << std::endl;
	}


	for(int i = 0; i<CONFIG_RADIO_CHANNEL_COUNT; i++){
		if(_config.radio->channel[i].init_done){
			std::cout<<"//      |        "<< i <<"         "<< _config.radio->channel[i].limit_min <<"           "<< _config.radio->channel[i].center_value <<"           "<< _config.radio->channel[i].limit_max <<" " << std::endl;
		}
	}

	if(_config.sensor->init_done){
		std::cout<<"//      |-->sensor                                                            " << std::endl;
		std::cout<<"//          |-->MPU       state          version        i2c address            " << std::endl;
	}
	for(int i = 0; i<CONFIG_SENSOR_IMU_COUNT; i++){
		if(_config.sensor->imu[i].init_done){
			std::cout<<"//               "<< i <<"         "<< _config.sensor->imu[i].state <<"              "<< _config.sensor->imu[i].version <<"              "<< std::hex << "0x" <<(int)_config.sensor->imu[i].i2c_address << std::dec <<" " << std::endl;
		}
	}
	std::cout<<"//============================================================================//" << std::endl;


	return RETURN_SUCCESS;
};

#endif // _API_CONFIGURATION_HPP_

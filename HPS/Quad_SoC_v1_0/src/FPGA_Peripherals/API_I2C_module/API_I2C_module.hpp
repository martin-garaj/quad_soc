
#ifndef _I2C_MODULE_API_HPP_
#define _I2C_MODULE_API_HPP_

#include <stdio.h> 	// memcpy
#include <stdint.h> // uint8_t, uint16_t
#include "system_const.h"

#include "i2c_opencores_regs.h"
#include "oc_i2c_master.h"


// avalable frequency
#define FREQ_100kHz		100000
#define FREQ_400kHz		400000

#define I2C_READ			1
#define I2C_WRITE			0
#define RETURN_I2C_ACK		1
#define RETURN_I2C_NO_ACK	0



//----------------------------------------------------------------CLASS----------------------------------------------------------------//
class API_I2C_module{
//----------------VARIABLES----------------//
private:
	uint32_t* i2c_module; //base addresses of UART_module
	uint32_t prescaler;
public:
	static int instance_counter;
	static uint16_t read_timeout;
//----------------FUNCTIONS----------------//
private:
	int init(int freq_scl, int freq_fpga);
	int start(uint8_t slave_addres, uint8_t transmission_type, uint16_t timeout=API_I2C_module::read_timeout);
	uint8_t read(bool last_transmission, uint16_t timeout=API_I2C_module::read_timeout);
	int write(uint8_t data, bool last_transmission, uint16_t timeout=API_I2C_module::read_timeout);

public:
	//constructor & destructor
	API_I2C_module(uint32_t * i2c_module_address);
	~API_I2C_module();

    void initialize(int freq_scl, int freq_fpga, int default_timeout);
    void enable(bool is_enabled);

    int read_bit(uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_num, uint8_t *data, uint16_t timeout=API_I2C_module::read_timeout);
    int read_bits(uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_start, uint8_t length, uint8_t *data, uint16_t timeout=API_I2C_module::read_timeout);
    int read_byte(uint8_t addr_slave, uint8_t addr_reg, uint8_t *data, uint16_t timeout=API_I2C_module::read_timeout);
    int read_bytes(uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint8_t *data, uint16_t timeout=API_I2C_module::read_timeout);

    int write_bit(uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_num, uint8_t data);
    int write_bits(uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_start, uint8_t length, uint8_t data);
    int write_byte(uint8_t addr_slave, uint8_t addr_reg, uint8_t data);
    int write_bytes(uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint8_t *data);
    int write_word(uint8_t addr_slave, uint8_t addr_reg, uint16_t data);
    int write_words(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);

//----------------COMPATIBILITY----------------//
    int8_t readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout=API_I2C_module::read_timeout);
    //TODO static int8_t readBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout=this->read_timeout);
    int8_t readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout=API_I2C_module::read_timeout);
    //TODO static int8_t readBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data, uint16_t timeout=this->read_timeout);
    int8_t readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout=API_I2C_module::read_timeout);
    //TODO static int8_t readWord(uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout=this->read_timeout);
    int8_t readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout=API_I2C_module::read_timeout);
    //TODO static int8_t readWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout=this->read_timeout);

    bool writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
    //TODO static bool writeBitW(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
    bool writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
    //TODO static bool writeBitsW(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
    bool writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data);
    bool writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data);
    bool writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
    bool writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);
};


#endif /* _I2C_MODULE_API_HPP_ */

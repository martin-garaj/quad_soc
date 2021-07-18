#ifndef API_I2C_MODULE_H_
#define API_I2C_MODULE_H_

#include <stdint.h> 	// uint8_t, uint16_t
#include "stdbool.h"	// bool type


// avalable frequency
#define FREQ_100kHz		100000
#define FREQ_400kHz		400000

#define I2C_READ			1
#define I2C_WRITE			0
#define RETURN_I2C_ACK		1
#define RETURN_I2C_NO_ACK	0



//----------------------------------------------------------------DECLARATION : LOW LEVEL FUNCTIONS----------------------------------------------------------------//
	int i2c_init(uint32_t i2c_base_address, int freq_scl, int freq_fpga);
	int i2c_start(uint32_t i2c_base_address, uint8_t slave_addres, uint8_t transmission_type, uint16_t timeout);
	uint8_t i2c_read(uint32_t i2c_base_address, bool last_transmission, uint16_t timeout);
	int i2c_write(uint32_t i2c_base_address, uint8_t data, bool last_transmission, uint16_t timeout);

//----------------------------------------------------------------DECLARATION : HIGH LEVEL FUNCTIONS----------------------------------------------------------------//
	void i2c_initialize(uint32_t i2c_base_address, int freq_scl, int freq_fpga);
	void i2c_enable(uint32_t i2c_base_address, bool is_enabled);

	int i2c_read_bit(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_num, uint8_t *data, uint16_t timeout);
	int i2c_read_bits(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_start, uint8_t length, uint8_t *data, uint16_t timeout);
	int i2c_read_byte(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t *data, uint16_t timeout);
	int i2c_read_bytes(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint8_t *data, uint16_t timeout);

	int i2c_write_bit(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_num, uint8_t data, uint16_t timeout);
	int i2c_write_bits(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_start, uint8_t length, uint8_t data, uint16_t timeout);
	int i2c_write_byte(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t data, uint16_t timeout);
	int i2c_write_bytes(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint8_t *data, uint16_t timeout);
	int i2c_write_word(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint16_t data, uint16_t timeout);
	int i2c_write_words(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout);

//----------------COMPATIBILITY----------------//
	int8_t i2c_readBit(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout);
    //TODO int8_t readBitW(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t *data, uint16_t timeout);
	int8_t i2c_readBits(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout);
    //TODO int8_t readBitsW(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t *data, uint16_t timeout);
	int8_t i2c_readByte(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout);
    //TODO int8_t readWord(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint16_t *data, uint16_t timeout);
	int8_t i2c_readBytes(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout);
    //TODO int8_t readWords(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout);

	bool writeBit(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data);
    //TODO bool writeBitW(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint16_t data);
	bool writeBits(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data);
    //TODO bool writeBitsW(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint16_t data);
	bool writeByte(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t data);
	bool writeWord(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint16_t data);
	bool writeBytes(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data);
	bool writeWords(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data);


#endif /* API_I2C_MODULE_H_ */

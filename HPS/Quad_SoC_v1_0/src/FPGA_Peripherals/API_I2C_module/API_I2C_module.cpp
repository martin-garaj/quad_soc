/*
 * API_I2C_module_API.cpp
 *
 *  Created on: 12.6.2017
 *      Author: gartin
 */

#include "API_I2C_module.hpp"
#include <iostream>		// std::cout

#ifndef Q_I2C_I2C_MODULE_API_CPP_
#define Q_I2C_I2C_MODULE_API_CPP_


int API_I2C_module::instance_counter = 0;
uint16_t API_I2C_module::read_timeout = 0;

//----------------------------------------------------------------DEFINITIONS : PUBLIC----------------------------------------------------------------//
// DEFINITIONS //
API_I2C_module::API_I2C_module(uint32_t * i2c_module_address){
	this->i2c_module = i2c_module_address;
	this->instance_counter++;
}


API_I2C_module::~API_I2C_module(){
	this->instance_counter--;
}

void API_I2C_module::initialize(int freq_scl, int freq_fpga, int default_timeout) {
	this->init(freq_scl,freq_fpga);
	this->read_timeout = default_timeout;
}

void API_I2C_module::enable(bool is_enabled) {
/** Enable or Disable I2C peripheral
 * @param is_enabled true = enable, false = disable
 */
	if(is_enabled){
		// disable i2c IP core
		i2c_module[OC_I2C_CTR] = 0x00;
	}else{
		// enable i2c IP core
		i2c_module[OC_I2C_CTR] = I2C_OPENCORES_CTR_EN_MSK;
	}
}

int API_I2C_module::read_bit(uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_num, uint8_t *data, uint16_t timeout) {
/** Read a single bit from an 8-bit device register.
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to read from
 * @param bit_num Bit position to read (0-7)
 * @param data Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in API_I2C_module::readTimeout)
 * $return Status of read operation (true = success)
 */
    uint8_t b;
    uint8_t count = read_byte(addr_slave, addr_reg, &b, timeout);
    *data = b & (1 << bit_num);
    return count;
}

int API_I2C_module::read_bits(uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_start, uint8_t length, uint8_t *data, uint16_t timeout) {
/** Read multiple bits from an 8-bit device register.
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to read from
 * @param bit_start First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bit_start position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in API_I2C_module::readTimeout)
 * @return Status of read operation (true = success)
 */
    // 01101001 read byte
    // 76543210 bit numbers
    //    xxx   args: bit_start=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count, b;
    if ((count = read_byte(addr_slave, addr_reg, &b, timeout)) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        b &= mask;
        b >>= (bit_start - length + 1);
        *data = b;
    }
    return count;
}

int API_I2C_module::read_byte(uint8_t addr_slave, uint8_t addr_reg, uint8_t *data, uint16_t timeout) {
/** Read single byte from an 8-bit device register.
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to read from
 * @param data Container for byte value read from device
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in API_I2C_module::readTimeout)
 * @return Status of read operation (true = success)
 */
    return read_bytes(addr_slave, addr_reg, 1, data, timeout);
}

int API_I2C_module::read_bytes(uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint8_t *data, uint16_t timeout) {
/** Read multiple bytes from an 8-bit device register.
 * @param addr_slave I2C slave device address
 * @param addr_reg First register addr_reg to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in API_I2C_module::readTimeout)
 * @return @param length on success, 0 otherwise
 */
	if( this->start(addr_slave, I2C_WRITE) 	== RETURN_ERROR){		return 0;	};
	if( this->write(addr_reg, true) 		== RETURN_ERROR){		return 0;	};

	if( this->start(addr_slave, I2C_READ) 	== RETURN_ERROR){		return 0;	};

	for(int i = 0; i < length-1; i++){
		data[i] = this->read(false);
	}

	data[length-1] = this->read(true);

	return length;


}

int API_I2C_module::write_bit(uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_num, uint8_t data) {
/** write a single bit in an 8-bit device register.
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to write to
 * @param bit_num Bit position to write (0-7)
 * @param value New bit value to write
 * @return Status of operation (true = success)
 */
    uint8_t b;
    read_byte(addr_slave, addr_reg, &b);
    b = (data != 0) ? (b | (1 << bit_num)) : (b & ~(1 << bit_num));
    return write_byte(addr_slave, addr_reg, b);
}

int API_I2C_module::write_bits(uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_start, uint8_t length, uint8_t data) {
/** Write multiple bits in an 8-bit device register.
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to write to
 * @param bit_start First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @return Status of operation (true = success)
 */
    //      010 value to write
    // 76543210 bit numbers
    //    xxx   args: bit_start=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b;
    if (read_byte(addr_slave, addr_reg, &b) != 0) {
        uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        data <<= (bit_start - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return write_byte(addr_slave, addr_reg, b);
    } else {
        return false;
    }
}

int API_I2C_module::write_byte(uint8_t addr_slave, uint8_t addr_reg, uint8_t data) {
/** Write single byte to an 8-bit device register.
 * @param addr_slave I2C slave device address
 * @param addr_reg Register address to write to
 * @param data New byte value to write
 * @return Status of operation (true = success)
 */
	write_bytes(addr_slave, addr_reg, 1, &data);
}

int API_I2C_module::write_bytes(uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint8_t *data){
/** Write multiple bytes to an 8-bit device register.
 * @param addr_slave I2C slave device address
 * @param addr_reg Register address to write to
 * @param length Length of byte array with data
 * @param data Byte array with data
 * @return Status of operation (true = success)
 */
	if( this->start(addr_slave, I2C_WRITE) 	== RETURN_ERROR){		return false;	};
	if( this->write(addr_reg, false)		== RETURN_ERROR){		return false;	};

	for(int i = 0; i < length-1; i++){
		if( this->write(data[i], false)		== RETURN_ERROR){		return false;	};
	}

	this->write(data[length-1], true);

	return true;
};


int API_I2C_module::write_word(uint8_t addr_slave, uint8_t addr_reg, uint16_t data){
	return write_words(addr_slave, addr_reg, 1, &data);
};

int API_I2C_module::write_words(uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint16_t *data){
	//std::cout << "I2C WRITE WORDS : " << (int)length << "|" << std::endl << std::flush;
	if( this->start(addr_slave, I2C_WRITE) 	== RETURN_ERROR){		return false;	};
	if( this->write(addr_reg, false)		== RETURN_ERROR){		return false;	};

	for (uint8_t i = 0; i < length-1; i++) {
		if( this->write((uint8_t)(data[i] >> 8), false)		== RETURN_ERROR){		return false;	}; // send MSB
		if( this->write((uint8_t)(0xff & data[i] ), false)	== RETURN_ERROR){		return false;	}; // send LSB
	}
	if( this->write((uint8_t)(data[length-1] >> 8), false)		== RETURN_ERROR){		return false;	}; // send MSB
	if( this->write((uint8_t)(0xff & data[length-1] ), true)	== RETURN_ERROR){		return false;	}; // send LSB

	return true;
};


//----------------COMPATIBILITY----------------//
int8_t API_I2C_module::readBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout){
	if(read_bit(devAddr, regAddr, bitNum, data, timeout) == RETURN_SUCCESS){
		return 1;
	}else{
		return 0;
	}
};

int8_t API_I2C_module::readBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout){
	if(read_bits(devAddr, regAddr, bitStart, length, data, timeout) == RETURN_SUCCESS){
		return 1;
	}else{
		return 0;
	}
};

int8_t API_I2C_module::readByte(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout){
	if(read_byte(devAddr, regAddr, data, timeout) == RETURN_SUCCESS){
		return 1;
	}else{
		return 0;
	}
};
int8_t API_I2C_module::readBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout){
	if(read_bytes(devAddr, regAddr, length, data, timeout) == RETURN_SUCCESS){
		return 1;
	}else{
		return 0;
	}
};

bool API_I2C_module::writeBit(uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data){
	if(write_bit(devAddr, regAddr, bitNum, data) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};
bool API_I2C_module::writeBits(uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data){
	if(write_bits(devAddr, regAddr, bitStart, length, data) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};

bool API_I2C_module::writeByte(uint8_t devAddr, uint8_t regAddr, uint8_t data){
	if(write_byte(devAddr, regAddr, data) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};

bool API_I2C_module::writeBytes(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data){
	if(write_bytes(devAddr, regAddr, length, data) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};

bool API_I2C_module::writeWord(uint8_t devAddr, uint8_t regAddr, uint16_t data){
	if(write_word(devAddr, regAddr, data) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};

bool API_I2C_module::writeWords(uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data){
	if(write_words(devAddr, regAddr, length, data) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	};
}




//----------------------------------------------------------------DEFINITIONS : PRIVATE----------------------------------------------------------------//
int API_I2C_module::init(int freq_scl, int freq_fpga){
/* description 	: initializes i2c IP core,
 * 				  calculates and loads the prescaler registers,
 * 				  cleans pending interrupts
 * parameters  	: freq_fpga - input clock frequency of i2c IP core
 * 				  freq_comm	- desired frequency of SCL i2c line
 * return		: RETURN_SUCCESS/RETURN_NO_PROBLEM
 */
	// calculate prescaler value
	prescaler = (freq_fpga/( 5 * freq_scl))-1;
	// disable i2c IP core
	i2c_module[OC_I2C_CTR] = 0x00;
	// clear any pending interrupt
	i2c_module[OC_I2C_CR] = I2C_OPENCORES_CR_IACK_MSK;
	// load prescaler low bits
	i2c_module[OC_I2C_PRER_LO] = (0xff & prescaler);
	// load prescaler high bits
	i2c_module[OC_I2C_PRER_HI] = (0xff & (prescaler>>8));
	// enable i2c IP core
	i2c_module[OC_I2C_CTR] = I2C_OPENCORES_CTR_EN_MSK;

	return RETURN_SUCCESS;
}

int API_I2C_module::start(uint8_t slave_addres, uint8_t transmission_type, uint16_t timeout){
/* description 	: loads the TX register with slave address and read/write bit
 * 						read	transmission_type = 1
 * 						write	transmission_type = 0
 * 				  sets the STA bit in Command Register
 * parameters  	: uint8_t slave_addres
 * 				  uint8_t operation_mode
 * return		: RETURN_SUCCESS/RETURN_NO_PROBLEM = slave ACKnowledged the transaction
 * 				  RETURN_ERROR = slave NOT ACKnowledged the transaction
 */
	uint16_t counter = 0;
	// load the TX register with slave address and read/write bit
	i2c_module[OC_I2C_TXR] = ((slave_addres<<1) + (0x01 & transmission_type));
	// set STA and WR bits in Command Register
	i2c_module[OC_I2C_CR] = I2C_OPENCORES_CR_STA_MSK | I2C_OPENCORES_CR_WR_MSK;
	// wait until the Transaction In Progress bit is low
	while( i2c_module[OC_I2C_SR] & I2C_OPENCORES_SR_TIP_MSK){
		counter++;
		if(counter>timeout){
			std::cout << "i2c start ERROR" << std::endl;
			return RETURN_ERROR;
		}
	};
	// check acknowledgment bit
	if( i2c_module[OC_I2C_SR] & I2C_OPENCORES_SR_RXNACK_MSK){
		return RETURN_I2C_NO_ACK;
	}

	return RETURN_I2C_ACK;
}

uint8_t API_I2C_module::read(bool last_transmission, uint16_t timeout){
/* description 	: reads a byte from slave,
 * 				  if this is the last transmission, the stop bit
 * 				  is asserted and ack is not checked
 * parameters  	: bool last_transmission
 * return		: returns a byte read from the slave
 */
	uint16_t counter = 0;
	if( last_transmission ){
		// start read, no ack, stop bit
		i2c_module[OC_I2C_CR] = (I2C_OPENCORES_CR_RD_MSK | I2C_OPENCORES_CR_NACK_MSK | I2C_OPENCORES_CR_STO_MSK);
	}else{
		// start read
		i2c_module[OC_I2C_CR] = I2C_OPENCORES_CR_RD_MSK;
	}

	// wait for the transmission to finish
	while( i2c_module[OC_I2C_SR] & I2C_OPENCORES_SR_TIP_MSK){
		counter++;
		if(counter>timeout){
			std::cout << "i2c start ERROR" << std::endl;
			return RETURN_ERROR;
		}
	};

	// read the data
	return (i2c_module[OC_I2C_RXR]);
}

int API_I2C_module::write(uint8_t data, bool last_transmission, uint16_t timeout){
/* description 	: writes a byte to slave,
 * 				  if this is the last transmission, the stop bit
 * 				  is asserted and ack is not checked
 * parameters  	: uint8_t data - data to be sent to the slave
 * 				  bool last_transmission
 * return		: returns a byte read from the slave
 */
	uint16_t counter = 0;
    // fill the TX register with data
	i2c_module[OC_I2C_TXR] = data;

	if( last_transmission ){
		// assert STA bit and WR bit
		i2c_module[OC_I2C_CR] = I2C_OPENCORES_CR_WR_MSK | I2C_OPENCORES_CR_STO_MSK;
	}else{
		// assert STA bit
		i2c_module[OC_I2C_CR] =  I2C_OPENCORES_CR_WR_MSK;
	}

	// wait for the transmission to finish
	while( i2c_module[OC_I2C_SR] & I2C_OPENCORES_SR_TIP_MSK){
		counter++;
		if(counter>timeout){
			std::cout << "i2c start ERROR" << std::endl;
			return RETURN_ERROR;
		}
	};

	// check whether the transaction was acknowledged
	if( i2c_module[OC_I2C_SR] & I2C_OPENCORES_SR_RXNACK_MSK){
		return RETURN_I2C_NO_ACK;
	}

	return RETURN_I2C_ACK;
}



#endif /* Q_I2C_I2C_MODULE_API_CPP_ */

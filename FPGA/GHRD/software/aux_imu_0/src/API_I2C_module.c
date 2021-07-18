
// header with declarations
#include "API_I2C_module.h"

// other includes
#include "i2c_opencores_regs.h" 	// Nios II Avalon MM macros
#include "stdbool.h"				// bool type
#include "system_const.h"			// RETURN_ERROR, RETURN_SUCCESS

//----------------------------------------------------------------DEFINITIONS : HIGH LEVEL FUNCTIONS----------------------------------------------------------------//

void i2c_initialize(uint32_t i2c_base_address, int freq_scl, int freq_fpga) {
	i2c_init(i2c_base_address, freq_scl,freq_fpga);
}

void i2c_enable(uint32_t i2c_base_address, bool is_enabled) {
/** Enable or Disable I2C peripheral
 * @param base address of I2C module
 * @param is_enabled true = enable, false = disable
 */
	if(is_enabled){
		// disable i2c IP core
		IOWR_I2C_OPENCORES_CTR(i2c_base_address, 0x00);
	}else{
		// enable i2c IP core
		IOWR_I2C_OPENCORES_CTR(i2c_base_address, I2C_OPENCORES_CTR_EN_MSK);
	}
}

int i2c_read_bit(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_num, uint8_t *data, uint16_t timeout) {
/** Read a single bit from an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to read from
 * @param bit_num Bit position to read (0-7)
 * @param data Container for single bit value
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * $return Status of read operation (true = success)
 */
    uint8_t b;
    uint8_t count = i2c_read_byte(i2c_base_address, addr_slave, addr_reg, &b, timeout);
    *data = b & (1 << bit_num);
    return count;
}

int i2c_read_bits(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_start, uint8_t length, uint8_t *data, uint16_t timeout) {
/** Read multiple bits from an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to read from
 * @param bit_start First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bit_start position will equal 0x05)
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return Status of read operation (true = success)
 */
    // 01101001 read byte
    // 76543210 bit numbers
    //    XXX   args: bit_start=4, length=3
    //    010   masked
    //   -> 010 shifted
    uint8_t count, b;
    if ((count = i2c_read_byte(i2c_base_address, addr_slave, addr_reg, &b, timeout)) > 0) {
        uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        b &= mask;
        b >>= (bit_start - length + 1);
        *data = b;
    }
    return count;
}

int i2c_read_byte(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t *data, uint16_t timeout) {
/** Read single byte from an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to read from
 * @param data Container for byte value read from device
 * @param @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return Status of read operation (true = success)
 */
    return i2c_read_bytes(i2c_base_address, addr_slave, addr_reg, 1, data, timeout);
}

int i2c_read_bytes(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint8_t *data, uint16_t timeout) {
/** Read multiple bytes from an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg First register addr_reg to read from
 * @param length Number of bytes to read
 * @param data Buffer to store read data in
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return number of bytes read on success, RETURN_ERROR otherwise
 */
	if( i2c_start(i2c_base_address, addr_slave, I2C_WRITE, timeout) == RETURN_ERROR){		return 0;	};
	if( i2c_write(i2c_base_address, addr_reg, true, timeout) 		== RETURN_ERROR){		return 0;	};

	if( i2c_start(i2c_base_address, addr_slave, I2C_READ, timeout) 	== RETURN_ERROR){		return 0;	};

	for(int i = 0; i < length-1; i++){
		data[i] = i2c_read(i2c_base_address, false, timeout);
	}

	data[length-1] = i2c_read(i2c_base_address, true, timeout);

	return length;
}

int i2c_write_bit(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_num, uint8_t data, uint16_t timeout) {
/** write a single bit in an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to write to
 * @param bit_num Bit position to write (0-7)
 * @param value New bit value to write
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return Status of operation (true = success)
 */
    uint8_t b;
    i2c_read_byte(i2c_base_address, addr_slave, addr_reg, &b, timeout);
    b = (data != 0) ? (b | (1 << bit_num)) : (b & ~(1 << bit_num));
    return i2c_write_byte(i2c_base_address, addr_slave, addr_reg, b, timeout);
}

int i2c_write_bits(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t bit_start, uint8_t length, uint8_t data, uint16_t timeout) {
/** Write multiple bits in an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg Register addr_reg to write to
 * @param bit_start First bit position to write (0-7)
 * @param length Number of bits to write (not more than 8)
 * @param data Right-aligned value to write
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return Status of operation (true = success)
 */
    //      010 value to write
    // 76543210 bit numbers
    //    XXX   args: bit_start=4, length=3
    // 00011100 mask byte
    // 10101111 original value (sample)
    // 10100011 original & ~mask
    // 10101011 masked | value
    uint8_t b;
    if (i2c_read_byte(i2c_base_address, addr_slave, addr_reg, &b, timeout) > 0) {
        uint8_t mask = ((1 << length) - 1) << (bit_start - length + 1);
        data <<= (bit_start - length + 1); // shift data into correct position
        data &= mask; // zero all non-important bits in data
        b &= ~(mask); // zero all important bits in existing byte
        b |= data; // combine data with existing byte
        return i2c_write_byte(i2c_base_address, addr_slave, addr_reg, b, timeout);
    } else {
        return RETURN_ERROR;
    }
}

int i2c_write_byte(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t data, uint16_t timeout) {
/** Write single byte to an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg Register address to write to
 * @param data New byte value to write
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return RETURN_SUCCESS / RETURN_ERROR
 */
	return i2c_write_bytes(i2c_base_address, addr_slave, addr_reg, 1, &data, timeout);
}

int i2c_write_bytes(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint8_t *data, uint16_t timeout){
/** Write multiple bytes to an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg Register address to write to
 * @param length Length of byte array with data
 * @param data Byte array with data
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return RETURN_SUCCESS / RETURN_ERROR
 */
	if( i2c_start(i2c_base_address, addr_slave, I2C_WRITE, timeout) == RETURN_ERROR){		return RETURN_ERROR;	};
	if( i2c_write(i2c_base_address, addr_reg, false, timeout)		== RETURN_ERROR){		return RETURN_ERROR;	};

	for(int i = 0; i < length-1; i++){
		if( i2c_write(i2c_base_address, data[i], false, timeout)	== RETURN_ERROR){		return RETURN_ERROR;	};
	}

	if( i2c_write(i2c_base_address, data[length-1], true, timeout) 	== RETURN_ERROR){		return RETURN_ERROR;	};

	return RETURN_SUCCESS;
};


int i2c_write_word(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint16_t data, uint16_t timeout){
/** Write a word (2 bytes) to an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg Register address to write to
 * @param data Byte array with data
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return RETURN_SUCCESS / RETURN_ERROR
 */
	return i2c_write_words(i2c_base_address, addr_slave, addr_reg, 1, &data, timeout);
};

int i2c_write_words(uint32_t i2c_base_address, uint8_t addr_slave, uint8_t addr_reg, uint8_t length, uint16_t *data, uint16_t timeout){
/** Write a word (2 bytes) to an 8-bit device register.
 * @param base address of I2C module
 * @param addr_slave I2C slave device address
 * @param addr_reg Register address to write to
 * @param length Length of byte array with data
 * @param data Byte array with data
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return RETURN_SUCCESS / RETURN_ERROR
 */
	//std::cout << "I2C WRITE WORDS : " << (int)length << "|" << std::endl << std::flush;
	if( i2c_start(i2c_base_address, addr_slave, I2C_WRITE, timeout ) 	== RETURN_ERROR){		return RETURN_ERROR;	};
	if( i2c_write(i2c_base_address, addr_reg, false, timeout )			== RETURN_ERROR){		return RETURN_ERROR;	};

	for (uint8_t i = 0; i < length-1; i++) {
		if( i2c_write(i2c_base_address, (uint8_t)(data[i] >> 8), false, timeout )		== RETURN_ERROR){		return RETURN_ERROR;	}; // send MSB
		if( i2c_write(i2c_base_address, (uint8_t)(0xff & data[i] ), false, timeout )	== RETURN_ERROR){		return RETURN_ERROR;	}; // send LSB
	}
	if( i2c_write(i2c_base_address, (uint8_t)(data[length-1] >> 8), false, timeout )	== RETURN_ERROR){		return RETURN_ERROR;	}; // send MSB
	if( i2c_write(i2c_base_address, (uint8_t)(0xff & data[length-1] ), true, timeout )	== RETURN_ERROR){		return RETURN_ERROR;	}; // send LSB

	return RETURN_SUCCESS;
};


//----------------COMPATIBILITY----------------//
int8_t i2c_readBit(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t *data, uint16_t timeout){
	if(i2c_read_bit(i2c_base_address, devAddr, regAddr, bitNum, data, timeout) == RETURN_SUCCESS){
		return 1;
	}else{
		return 0;
	}
};

int8_t i2c_readBits(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data, uint16_t timeout){
	if(i2c_read_bits(i2c_base_address, devAddr, regAddr, bitStart, length, data, timeout) == RETURN_SUCCESS){
		return 1;
	}else{
		return 0;
	}
};

int8_t i2c_readByte(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t timeout){
	if(i2c_read_byte(i2c_base_address, devAddr, regAddr, data, timeout) == RETURN_SUCCESS){
		return 1;
	}else{
		return 0;
	}
};

// ERROR
// == RETURN_SUCCESS, the function returns 0, when fails, otherwise the length of the data sent
int8_t i2c_readBytes(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout){
	if(i2c_read_bytes(i2c_base_address, devAddr, regAddr, length, data, timeout) > 0){
		return 1;
	}else{
		return 0;
	}
};

bool i2c_writeBit(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitNum, uint8_t data, uint16_t timeout){
	if(i2c_write_bit(i2c_base_address, devAddr, regAddr, bitNum, data, timeout) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};
bool i2c_writeBits(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data, uint16_t timeout){
	if(i2c_write_bits(i2c_base_address, devAddr, regAddr, bitStart, length, data, timeout) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};

bool i2c_writeByte(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t data, uint16_t timeout){
	if(i2c_write_byte(i2c_base_address, devAddr, regAddr, data, timeout) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};

bool i2c_writeBytes(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data, uint16_t timeout){
	if(i2c_write_bytes(i2c_base_address, devAddr, regAddr, length, data, timeout) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};

bool i2c_writeWord(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint16_t data, uint16_t timeout){
	if(i2c_write_word(i2c_base_address, devAddr, regAddr, data, timeout) == RETURN_SUCCESS){
		return true;
	}else{
		return false;
	}
};

bool i2c_writeWords(uint32_t i2c_base_address, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint16_t *data, uint16_t timeout){
	if(i2c_write_words(i2c_base_address, devAddr, regAddr, length, data, timeout) == RETURN_SUCCESS){
		return true;
	}
	return false;
}




//----------------------------------------------------------------DEFINITIONS : LOW LEVEL FUNCTIONS----------------------------------------------------------------//
int i2c_init(uint32_t i2c_base_address, int freq_scl, int freq_fpga){
/** initializes i2c IP core, calculates and loads the prescaler registers, cleans pending interrupts
 * @param base address of I2C IP core
 * @param desired frequency of SCL i2c line
 * @param input clock frequency of i2c IP core
 * @return RETURN_SUCCESS/RETURN_NO_PROBLEM
 */
	uint16_t prescaler;
	// calculate prescaler value
	prescaler = (freq_fpga/( 5 * freq_scl))-1;
	// disable i2c IP core
	IOWR_I2C_OPENCORES_CTR(i2c_base_address, 0x00);
	// clear any pending interrupt
	IOWR_I2C_OPENCORES_CR(i2c_base_address ,I2C_OPENCORES_CR_IACK_MSK);
	// load prescaler low bits
	IOWR_I2C_OPENCORES_PRERLO(i2c_base_address, (0xff & prescaler) );;
	// load prescaler high bits
	IOWR_I2C_OPENCORES_PRERHI(i2c_base_address ,(0xff & (prescaler>>8)) );
	// enable i2c IP core
	IOWR_I2C_OPENCORES_CTR(i2c_base_address ,I2C_OPENCORES_CTR_EN_MSK);

	return RETURN_SUCCESS;
}

int i2c_start(uint32_t i2c_base_address, uint8_t slave_addres, uint8_t transmission_type, uint16_t timeout){
/** loads the TX register with slave address and read/write bit, sets the STA bit in Command Register
 * @param base address of I2C IP core
 * @param slaves address present on the i2c bus
 * @param mode : read	transmission_type = 1
 * 				 write	transmission_type = 0
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return RETURN_SUCCESS/RETURN_NO_PROBLEM = slave ACKnowledged the transaction
 * 		   RETURN_ERROR = slave NOT ACKnowledged the transaction
 */
	int timeout_counter = 0;
	// load the TX register with slave address and read/write bit
	IOWR_I2C_OPENCORES_TXR( i2c_base_address ,((slave_addres<<1) + (0x01 & transmission_type)) );
	// set STA and WR bits in Command Register
	IOWR_I2C_OPENCORES_CR( i2c_base_address ,(I2C_OPENCORES_CR_STA_MSK | I2C_OPENCORES_CR_WR_MSK) );
	// wait until the Transaction In Progress bit is low
	while( IORD_I2C_OPENCORES_SR(i2c_base_address) & I2C_OPENCORES_SR_TIP_MSK){
		if(timeout_counter > timeout){ return RETURN_ERROR; }
		timeout_counter++;
	};
	// check acknowledgment bit
	if( IORD_I2C_OPENCORES_SR(i2c_base_address) & I2C_OPENCORES_SR_RXNACK_MSK){
		return RETURN_I2C_NO_ACK;
	}

	return RETURN_I2C_ACK;
}

uint8_t i2c_read(uint32_t i2c_base_address, bool last_transmission, uint16_t timeout){
/** reads a byte from slave, if this is the last transmission, the stop bit is asserted and ack is not checked
 * @param base address of I2C IP core
 * @param slaves address present on the i2c bus
 * @param last transmission (yes = true, no = false)
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return returns a byte read from the slave
 */
	int timeout_counter = 0;
	if( last_transmission ){
		// start read, no ack, stop bit
		IOWR_I2C_OPENCORES_CR( i2c_base_address, (I2C_OPENCORES_CR_RD_MSK | I2C_OPENCORES_CR_NACK_MSK | I2C_OPENCORES_CR_STO_MSK ) );
	}else{
		// start read
		IOWR_I2C_OPENCORES_CR( i2c_base_address, I2C_OPENCORES_CR_RD_MSK );
	}

	// wait for the transmission to finish
	while( IORD_I2C_OPENCORES_SR(i2c_base_address) & I2C_OPENCORES_SR_TIP_MSK){
		if(timeout_counter > timeout){ return RETURN_ERROR; }
		timeout_counter++;
	};

	// read the data
	return (IORD_I2C_OPENCORES_RXR(i2c_base_address));
}

int i2c_write(uint32_t i2c_base_address, uint8_t data, bool last_transmission, uint16_t timeout){
/** writes a byte to slave, if this is the last transmission, the stop bit is asserted and ack is not checked
 * @param base address of I2C IP core
 * @param byte to be sent to the slave
 * @param last transmission (yes = true, no = false)
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return returns RETURN_I2C_ACK if the slave acknowledged the transmission,
 * 				   RETURN_I2C_NO_ACK if the slave didnt acknowledged the transmission,
 * 				   RETURN_ERROR if the i2c IP core reached the timeout limit
 */
	int timeout_counter = 0;
    // fill the TX register with data
	IOWR_I2C_OPENCORES_TXR(i2c_base_address, data);

	if( last_transmission ){
		// assert STA bit and WR bit
		IOWR_I2C_OPENCORES_CR(i2c_base_address , I2C_OPENCORES_CR_WR_MSK | I2C_OPENCORES_CR_STO_MSK );
	}else{
		// assert STA bit
		IOWR_I2C_OPENCORES_CR(i2c_base_address , I2C_OPENCORES_CR_WR_MSK);
	}

	// wait for the transmission to finish
	while( IORD_I2C_OPENCORES_SR(i2c_base_address) & I2C_OPENCORES_SR_TIP_MSK){
		if(timeout_counter > timeout){ return RETURN_ERROR; }
		timeout_counter++;
	};

	// check whether the transaction was acknowledged
	if( IORD_I2C_OPENCORES_SR(i2c_base_address) & I2C_OPENCORES_SR_RXNACK_MSK){
		return RETURN_I2C_NO_ACK;
	}

	return RETURN_I2C_ACK;
}

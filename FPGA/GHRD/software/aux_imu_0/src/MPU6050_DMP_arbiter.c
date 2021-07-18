

#include "MPU6050_DMP_arbiter.h" 	// function declarations

// other includes
#include <stdint.h> 		// uint8_t, uint16_t, uint32_t
#include "system_const.h"	// RETURN_ERROR, RETURN_SUCCESS


// below functions are copied and modified (to fit C style) from q_MPU6050.hpp

/** Set clock source setting.
 * An internal 8MHz oscillator, gyroscope based clock, or external sources can
 * be selected as the MPU-60X0 clock source. When the internal 8 MHz oscillator
 * or an external source is chosen as the clock source, the MPU-60X0 can operate
 * in low power modes with the gyroscopes disabled.
 *
 * Upon power up, the MPU-60X0 clock source defaults to the internal oscillator.
 * However, it is highly recommended that the device be configured to use one of
 * the gyroscopes (or an external clock source) as the clock reference for
 * improved stability. The clock source can be selected according to the following table:
 *
 * <pre>
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
 * </pre>
 *
 * @param source New clock source setting
 * @see getClockSource()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_CLKSEL_BIT
 * @see MPU6050_PWR1_CLKSEL_LENGTH
 */
void setClockSource(uint32_t i2c_base_address, uint8_t devAddr,  uint8_t source, uint16_t timeout) {
    i2c_writeBits(i2c_base_address, devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source, timeout);
}

/** Set full-scale gyroscope range.
 * @param range New full-scale gyroscope range value
 * @see getFullScaleRange()
 * @see MPU6050_GYRO_FS_250
 * @see MPU6050_RA_GYRO_CONFIG
 * @see MPU6050_GCONFIG_FS_SEL_BIT
 * @see MPU6050_GCONFIG_FS_SEL_LENGTH
 */
void setFullScaleGyroRange(uint32_t i2c_base_address, uint8_t devAddr,  uint8_t range, uint16_t timeout) {
    i2c_writeBits(i2c_base_address, devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range, timeout);
}


/** Set full-scale accelerometer range.
 * @param range New full-scale accelerometer range setting
 * @see getFullScaleAccelRange()
 */
void setFullScaleAccelRange(uint32_t i2c_base_address, uint8_t devAddr,  uint8_t range, uint16_t timeout) {
    i2c_writeBits(i2c_base_address, devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range, timeout);
}


/** Set sleep mode status.
 * @param enabled New sleep mode enabled status
 * @see getSleepEnabled()
 * @see MPU6050_RA_PWR_MGMT_1
 * @see MPU6050_PWR1_SLEEP_BIT
 */
void setSleepEnabled(uint32_t i2c_base_address, uint8_t devAddr,  bool enabled, uint16_t timeout) {
    i2c_writeBit(i2c_base_address, devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled, timeout);
}



uint16_t getFIFOCount(uint32_t i2c_base_address, uint8_t addr_slave, int8_t* buffer, uint32_t timeout) {
/* Get current FIFO buffer size.
 * This value indicates the number of bytes stored in the FIFO buffer. This
 * number is in turn the number of bytes that can be read from the FIFO buffer
 * and it is directly proportional to the number of samples available given the
 * set of sensor data bound to be stored in the FIFO (register 35 and 36).
 * @param i2c IP core base address, e.g. I2C_0_BASE (defined in hps_0.h)
 * @param address of MPU6050 sensor, e.g MPU6050_DEFAULT_ADDRESS (defined in q_MPU6050)
 * @param buffer is 14 byte long array of uint8_t
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return current FIFO buffer size
 */
	i2c_read_bytes(i2c_base_address, addr_slave, MPU6050_RA_FIFO_COUNTH, 2, buffer, timeout);
	return (((uint16_t)buffer[0]) << 8) | buffer[1];
}


uint8_t getIntStatus(uint32_t i2c_base_address, uint8_t addr_slave, int8_t* buffer, uint32_t timeout) {
/** Get full set of interrupt status bits.
 * These bits clear to 0 after the register has been read. Very useful
 * for getting multiple INT statuses, since each single bit read clears
 * all of them because it has to read the whole byte.
 * @param i2c IP core base address, e.g. I2C_0_BASE (defined in hps_0.h)
 * @param address of MPU6050 sensor, e.g MPU6050_DEFAULT_ADDRESS (defined in q_MPU6050)
 * @param buffer is 14 byte long array of uint8_t
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return Current interrupt status
 * @see MPU6050_RA_INT_STATUS
 */
	i2c_read_byte(i2c_base_address, addr_slave, MPU6050_RA_INT_STATUS, buffer, timeout);
	return buffer[0];
}



/** Get byte from FIFO buffer.
 * This register is used to read and write data from the FIFO buffer. Data is
 * written to the FIFO in order of register number (from lowest to highest). If
 * all the FIFO enable flags (see below) are enabled and all External Sensor
 * Data registers (Registers 73 to 96) are associated with a Slave device, the
 * contents of registers 59 through 96 will be written in order at the Sample
 * Rate.
 *
 * The contents of the sensor data registers (Registers 59 to 96) are written
 * into the FIFO buffer when their corresponding FIFO enable flags are set to 1
 * in FIFO_EN (Register 35). An additional flag for the sensor data registers
 * associated with I2C Slave 3 can be found in I2C_MST_CTRL (Register 36).
 *
 * If the FIFO buffer has overflowed, the status bit FIFO_OFLOW_INT is
 * automatically set to 1. This bit is located in INT_STATUS (Register 58).
 * When the FIFO buffer has overflowed, the oldest data will be lost and new
 * data will be written to the FIFO.
 *
 * If the FIFO buffer is empty, reading this register will return the last byte
 * that was previously read from the FIFO until new data is available. The user
 * should check FIFO_COUNT to ensure that the FIFO buffer is not read when
 * empty.
 *
 * @return Byte from FIFO buffer
 */
//uint8_t MPU6050::getFIFOByte() {
    //i2c->readByte(devAddr, MPU6050_RA_FIFO_R_W, buffer);
    //return buffer[0];
//}
void getFIFOBytes(uint32_t i2c_base_address, uint8_t addr_slave, uint16_t packet_size, uint8_t *data, uint32_t timeout) {
/** Get byte from FIFO buffer.
 * This register is used to read and write data from the FIFO buffer. Data is
 * written to the FIFO in order of register number (from lowest to highest). If
 * all the FIFO enable flags (see below) are enabled and all External Sensor
 * Data registers (Registers 73 to 96) are associated with a Slave device, the
 * contents of registers 59 through 96 will be written in order at the Sample
 * Rate.
 *
 * The contents of the sensor data registers (Registers 59 to 96) are written
 * into the FIFO buffer when their corresponding FIFO enable flags are set to 1
 * in FIFO_EN (Register 35). An additional flag for the sensor data registers
 * associated with I2C Slave 3 can be found in I2C_MST_CTRL (Register 36).
 *
 * If the FIFO buffer has overflowed, the status bit FIFO_OFLOW_INT is
 * automatically set to 1. This bit is located in INT_STATUS (Register 58).
 * When the FIFO buffer has overflowed, the oldest data will be lost and new
 * data will be written to the FIFO.
 *
 * If the FIFO buffer is empty, reading this register will return the last byte
 * that was previously read from the FIFO until new data is available. The user
 * should check FIFO_COUNT to ensure that the FIFO buffer is not read when
 * empty.
 *
 * @param i2c IP core base address, e.g. I2C_0_BASE (defined in hps_0.h)
 * @param address of MPU6050 sensor, e.g MPU6050_DEFAULT_ADDRESS (defined in q_MPU6050)
 * @param packet is 'dmpPacketSize' (defined in MPU6050_DMP_arbiter.h) byte long array of uint8_t
 * @param number of cycles after which the function automatically returns RETURN_ERROR
 * @return Byte from FIFO buffer
 */
    if(packet_size > 0){
        i2c_read_bytes(i2c_base_address, addr_slave, MPU6050_RA_FIFO_R_W, packet_size, data, timeout);
    } else {
    	*data = 0;
    }
}



void resetFIFO(uint32_t i2c_base_address, uint8_t addr_slave, uint32_t timeout) {
/** Reset the FIFO.
 * This bit resets the FIFO buffer when set to 1 while FIFO_EN equals 0. This
 * bit automatically clears to 0 after the reset has been triggered.
 * @see MPU6050_RA_USER_CTRL
 * @see MPU6050_USERCTRL_FIFO_RESET_BIT
 */
    i2c_write_bit(i2c_base_address, addr_slave, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_FIFO_RESET_BIT, true, timeout);
}




/** Get raw 9-axis motion sensor readings (accel/gyro/compass).
 * FUNCTION NOT FULLY IMPLEMENTED YET.
 * @param i2c IP core base address, e.g. I2C_0_BASE (defined in hps_0.h)
 * @param address of MPU6050 sensor, e.g MPU6050_DEFAULT_ADDRESS (defined in q_MPU6050)
 * @param ax 16-bit signed integer container for accelerometer X-axis value
 * @param ay 16-bit signed integer container for accelerometer Y-axis value
 * @param az 16-bit signed integer container for accelerometer Z-axis value
 * @param gx 16-bit signed integer container for gyroscope X-axis value
 * @param gy 16-bit signed integer container for gyroscope Y-axis value
 * @param gz 16-bit signed integer container for gyroscope Z-axis value
 * @param mx 16-bit signed integer container for magnetometer X-axis value
 * @param my 16-bit signed integer container for magnetometer Y-axis value
 * @param mz 16-bit signed integer container for magnetometer Z-axis value
 * @see getMotion6()
 * @see getAcceleration()
 * @see getRotation()
 * @see MPU6050_RA_ACCEL_XOUT_H
 */
/* I AM NOT USING THIS FUNCTION AT THE MOMENT
void getMotion9(uint32_t i2c_base_address, uint8_t addr_slave, int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, int16_t* mx, int16_t* my, int16_t* mz, uint8_t *buffer, uint32_t timeout) {
    getMotion6(i2c_base_address, addr_slave, ax, ay, az, gx, gy, gz, buffer, timeout);
    // TODO: magnetometer integration
}
*/



/** Get raw 6-axis motion sensor readings (accel/gyro).
 * Retrieves all currently available motion sensor values.
 * @param i2c IP core base address, e.g. I2C_0_BASE (defined in hps_0.h)
 * @param address of MPU6050 sensor, e.g MPU6050_DEFAULT_ADDRESS (defined in q_MPU6050)
 * @param ax 16-bit signed integer container for accelerometer X-axis value
 * @param ay 16-bit signed integer container for accelerometer Y-axis value
 * @param az 16-bit signed integer container for accelerometer Z-axis value
 * @param gx 16-bit signed integer container for gyroscope X-axis value
 * @param gy 16-bit signed integer container for gyroscope Y-axis value
 * @param gz 16-bit signed integer container for gyroscope Z-axis value
 * @see getAcceleration()
 * @see getRotation()
 * @see MPU6050_RA_ACCEL_XOUT_H
 */
uint8_t getMotion6(uint32_t i2c_base_address, uint8_t addr_slave, int16_t* ax, int16_t* ay, int16_t* az, int16_t* gx, int16_t* gy, int16_t* gz, uint8_t *buffer, uint32_t timeout) {
    uint8_t return_val = i2c_readBytes(i2c_base_address, addr_slave, MPU6050_RA_ACCEL_XOUT_H, 14, buffer, timeout);
    *ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    *ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    *az = (((int16_t)buffer[4]) << 8) | buffer[5];
    *gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    *gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    *gz = (((int16_t)buffer[12]) << 8) | buffer[13];
    return return_val;
}


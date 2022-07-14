// Siehe auch: https://github.com/Sylaina/oled-display

#include "i2c.h"

uint8_t I2C_ErrorCode = 0x00;

/**********************************************
 Public Function: i2c_init

 Purpose: Initialise I2C interface
 **********************************************/
void i2c_init(void)
{
    // SET GPIO IOF for I2C
    ACCESS(GPIO_IOF_EN) |= (0x01UL << 0x0CUL) | (0x01UL << 0x0DUL);

    // Init I2C (BAUDRATE, ENABLE)
    ACCESS(I2C_PRERLO) = 0x1FUL;
    ACCESS(I2C_PRERHI) = 0x00UL;
    ACCESS(I2C_CONTROL) = 0x80UL;
}

/**********************************************
 Public Function: i2c_start

 Purpose: Start I2C interface

 Input Parameter:
 - uint8_t i2c_addr: Adress of receiver
 **********************************************/
void i2c_start(uint8_t i2c_addr)
{
    // Declare and reset timer
    time_t timeout = clock() + I2C_TIMEOUT;

    // Check if any transfer is in progress
    while (ACCESS(I2C_CMD_STAT) & (0x01UL << 0x01UL))
    {
        if (clock() > timeout)
        {
            I2C_ErrorCode |= 1 << I2C_START;
            return;
        }
    }

    // Set transmit register to slave address with write flag
    ACCESS(I2C_TX_RX) = (i2c_addr & 0x7FUL) << 0x01UL;

    // Set start and write to slave flag to trigger the address transfer
    ACCESS(I2C_CMD_STAT) = (0x01UL << 0x04UL) | (0x01UL << 0x07UL);

    // Reset timeout
    timeout = clock() + I2C_TIMEOUT;

    // Check for transmit completion
    while (ACCESS(I2C_CMD_STAT) & (0x01UL << 0x01UL))
    {
        if (clock() > timeout)
        {
            I2C_ErrorCode |= 1 << I2C_START;
            return;
        }
    }

    // Check for ACK from slave
    if ((ACCESS(I2C_CMD_STAT) & (0x01UL << 0x07UL)))
    {
        /* No ACK, return error */
        I2C_ErrorCode |= 1 << I2C_SENDADRESS;
        return;
    }
}

/**********************************************
 Public Function: i2c_stop

 Purpose: Stop I2C interface
 **********************************************/
void i2c_stop(void)
{
    // i2c stop
    ACCESS(I2C_TX_RX) = 0x00;
    ACCESS(I2C_CMD_STAT) = (1 << 4) | (1 << 6);
}

/**********************************************
 Public Function: i2c_byte

 Purpose: Send byte at TWI/I2C interface

 Input Parameter:
 - uint8_t byte: Byte to send to reciever
 **********************************************/
void i2c_byte(uint8_t byte)
{
    // Copy into transmit register
    ACCESS(I2C_TX_RX) = byte;

    uint8_t command = (1 << 4);

    // Write command register
    ACCESS(I2C_CMD_STAT) = command;

    // Reset timeout
    time_t timeout = clock() + I2C_TIMEOUT;

    // Check for transfer completion
    while (ACCESS(I2C_CMD_STAT) & (0x01UL << 0x01UL))
    {
        if (clock() > timeout)
        {
            I2C_ErrorCode |= 1 << I2C_BYTE;
            return;
        }
    }

    // Check for ACK from slave
    if ((ACCESS(I2C_CMD_STAT) & (0x01UL << 0x07UL)))
    {
        // No ACK, return error
        I2C_ErrorCode |= 1 << I2C_READACK;
        return;
    }
}
// Siehe auch: https://github.com/Sylaina/oled-display

#include <stdio.h>
#include <time.h>

extern uint8_t I2C_ErrorCode;		// variable for communication error
									// ckeck it in your code
									// 0 means no error
									// define bits in I2C-ErrorCode:
#define I2C_START		0			// bit 0: timeout start-condition
#define I2C_SENDADRESS	1			// bit 1: timeout device-adress
#define I2C_BYTE		2			// bit 2: timeout byte-transmission
#define I2C_READACK		3			// bit 3: timeout read acknowledge

#define I2C_TIMEOUT 1000000 // ~1s

#define GPIO_BASE_ADDRESS 0x10012000
#define GPIO_IOF_EN GPIO_BASE_ADDRESS + 0x38UL

#define I2C_BASE_ADDRESS 0x10016000
#define I2C_PRERLO I2C_BASE_ADDRESS
#define I2C_PRERHI I2C_BASE_ADDRESS + 0x04UL
#define I2C_TX_RX I2C_BASE_ADDRESS + 0x0CUL
#define I2C_CONTROL I2C_BASE_ADDRESS + 0x08UL
#define I2C_CMD_STAT I2C_BASE_ADDRESS + 0x10UL

#define ACCESS(address) (*(uint32_t volatile *)((uint8_t *)address))

void i2c_init(void);				// init hw-i2c
void i2c_start(uint8_t i2c_addr);	// send i2c_start_condition
void i2c_stop(void);				// send i2c_stop_condition
void i2c_byte(uint8_t byte);		// send data_byte

uint8_t i2c_readAck(void);          // read byte with ACK
uint8_t i2c_readNAck(void);         // read byte with NACK

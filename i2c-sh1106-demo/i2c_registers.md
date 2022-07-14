### I2C Registers

#### Base address: 

0x10016000

#### Offsets:

|  R/W/RW  |   OFFSET   |    SHORT_NAME    |          LONG_NAME           |
|----|------|--------|---------------------|
| RW | 0x00 | PRERlo | Clock Prescale low  |
| RW | 0x04 | PRERhi | Clock Prescale high |
| RW | 0x08 | CTR    | Control register    |
| W  | 0x0C | TXR    | Transmit register   |
| R  | 0x0C | RXR    | Receive register    |
| W  | 0x10 | CR     | Command register    |
| R  | 0x10 | SR     | Status register     |

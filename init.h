
#include "./info/crc32_info.h"

#define ZERO 	0x0

#define ONES8	0xFF
#define ONES16	0xFFFF
#define ONES64	0xFFFFFFFFFFFFFFFF

//8bit checksums
#define DARC 	(uint8_t)0x39
#define CRC_8	(uint8_t)0x07

//16bit checksums
#define CCITT	(uint16_t)0x1021  

uint8_t CRC8;
uint16_t CRC16;

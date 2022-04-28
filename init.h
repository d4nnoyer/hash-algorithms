#define ZERO 	0x0
#define ONES8	0xFF
#define ONES16	0xFFFF
#define ONES32	0xFFFFFFFF
#define ONES64	0xFFFFFFFFFFFFFFFF

//8bit checksums
#define DARC 	(uint8_t)0x39
#define CRC_8	(uint8_t)0x07

//16bit checksums
#define CCITT	(uint16_t)0x1021   

//32bit checksums
#define POSIX 	(uint32_t)0x04C11DB7
#define XFER 	(uint32_t)0x000000AF
#define CRC_32	(uint32_t)0x04C11DB7 //default crc32, for example, used by 7-zip

//64bit checksums


uint8_t CRC8;
uint16_t CRC16;
uint32_t CRC32;
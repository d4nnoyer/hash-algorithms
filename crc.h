#include <iostream>
#include <fstream>
#include <iomanip>


#define ZERO 	0x0
#define ONES8	0xFF
#define ONES16	0xFFFF
#define ONES32	0xFFFFFFFF
#define ONES64	0xFFFFFFFFFFFFFFFF

#define DARC 	(uint8_t)0x39
#define CRC_8	(uint8_t)0x07

#define CCITT	(uint16_t)0x1021   

#define POSIX 	(uint32_t)0x04C11DB7
#define XFER 	(uint32_t)0x000000AF
#define CRC_32	(uint32_t)0x04C11DB7 //default crc32, for example, used by 7-zip


uint8_t CRC8;
uint16_t CRC16;
uint32_t CRC32;

template<typename T>
T reflect(T value)
{
    T reflected = 0;
    for (int i = 0; i < (8 * sizeof(value)); ++i, value >>= 1){
        reflected = (reflected << 1) | (value & 1);
    }
    return reflected;
}

template<typename T>
void stringCRC( const std::string &FILE, 
				T crc,  
				T POLY, 
				const int INIT, 
				const int XOROUT, 
				const bool refIn, 
				const bool refOut)
{
	const uint8_t OFFSET = (sizeof(T)*8 - 8);		//Calculate offset, needed to compare MSB of checksum and current bit of data
	const T MSB = (ZERO | 1) << (sizeof(T)*8 - 1); 	//Turn most significant bit to 1, all other to 0
	crc = INIT;

	if(refIn){
		POLY = reflect(POLY);
		for (size_t pos = 0; pos < FILE.size(); pos++)
		{
			uint8_t byte = FILE[pos];
			for (uint8_t i = 0; i < 8; ++i, byte >>= 1) {
				(crc & 1) ^ (byte & 1) ?  crc = (crc >> 1) ^ POLY : crc >>= 1;
			}
		}		
	}

	else
	{
		for (size_t pos = 0; pos < FILE.size(); pos++)
		{
			uint8_t byte = FILE[pos];
			for (uint8_t i = 0; i < 8; ++i, byte <<= 1) {
				((crc & MSB) >> OFFSET) ^ (byte & 0x80) ?  crc = (crc << 1) ^ POLY : crc <<= 1;
			}
		}
		if (refOut) crc = reflect(crc);
	}

	std::cout << "0x" <<  std::uppercase << std::hex <<  (crc^XOROUT) << std::endl;
}

template<typename T>
std::string fileCRC( 	const std::string filename, 
						T crc,  
						T POLY, 
						const int INIT, 
						const int XOROUT, 
						const bool refIn, 
						const bool refOut)
{
	const uint8_t OFFSET = (sizeof(T)*8 - 8);		//Calculate offset, needed to compare MSB of checksum and current bit of data
	const T MSB = (ZERO | 1) << (sizeof(T)*8 - 1); 	//Turn most significant bit to 1, all other to 0
	crc = INIT;

	std::ifstream fread(filename, std::fstream::in|std::fstream::binary );
	if (!fread) return "...unknown. Failed to open " + filename;
	char chunk[256];

	if(refIn){
		POLY = reflect(POLY);

		while (fread)
		{
			fread.read( chunk, sizeof(chunk) );
			for (size_t pos = 0; pos < fread.gcount(); pos++)
			{
				uint8_t byte = chunk[pos];
				for (uint8_t i = 0; i < 8; ++i, byte >>= 1) {
					(crc & 1) ^ (byte & 1) ?  crc = (crc >> 1) ^ POLY : crc >>= 1;
				}
			}		
			if (!refOut) crc = reflect(crc);
		}
	}

	else
	{
		while (fread)
		{
			fread.read( chunk, sizeof(chunk) );
			for (size_t pos = 0; pos < fread.gcount(); pos++)
			{
				uint8_t byte = chunk[pos];
				for (uint8_t i = 0; i < 8; ++i, byte <<= 1) {
					((crc & MSB) >> OFFSET) ^ (byte & 0x80) ?  crc = (crc << 1) ^ POLY : crc <<= 1;
				}
			}
			if (refOut) crc = reflect(crc);
		}
	}

	std::stringstream stream;
	stream << std::uppercase << std::hex  << (crc^XOROUT);

	return "0x" + stream.str();	
}
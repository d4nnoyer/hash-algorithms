#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "init.h"
#include "./info/crc_info.h"

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
T leftBitShift(T &value)
{
	return value <<= 1;
}

template<typename T>
T rightBitShift(T &value)
{
	return value >>= 1;
}

template<typename T>
std::string numericToHexString(T number)
{
	std::stringstream stream;
	stream << "0x" <<  std::uppercase << std::hex <<  number;

	return stream.str();	
}

template<typename T>
T getMessageCRC( const std::string &FILE, 
				T crc,  
				T POLYNOME, 
				const int INIT, 
				const T XOROUT, 
				const bool isInputReflected, 
				const bool isOutputReflected)
{
	const uint8_t OFFSET = (sizeof(T)*8 - 8);		//Calculate offset, needed to compare MSB of checksum and current bit of data
	const T MSB = (ZERO | 1) << (sizeof(T)*8 - 1); 	//Turn most significant bit to 1, all other to 0
	crc = INIT;

	if (isInputReflected)
		POLYNOME = reflect(POLYNOME);

	if(isInputReflected){

		for (size_t pos = 0; pos < FILE.size(); pos++)
		{
			uint8_t byte = FILE[pos];
			for (uint8_t i = 0; i < 8; ++i, rightBitShift(byte)) {
				(crc & 1) ^ (byte & 1) 
				?  
				crc = rightBitShift(crc) ^ POLYNOME 
				: 
				rightBitShift(crc);
			}
		}	
	}
	else
	{
		for (size_t pos = 0; pos < FILE.size(); pos++)
		{
			uint8_t byte = FILE[pos];
			for (uint8_t i = 0; i < 8; ++i, leftBitShift(byte)) {
				((crc & MSB) >> OFFSET) ^ (byte & 0x80) 
				?  
				crc = leftBitShift(crc) ^ POLYNOME 
				: 
				leftBitShift(crc);
			}
		}
	}

	if (isInputReflected ^ isOutputReflected)
		crc = reflect(crc);

	return  crc^XOROUT;
}

template<typename T>
bool isMostSignificantBitTrue(T value,  uint8_t byte)
{
	uint8_t OFFSET = (sizeof(T)*8 - 8); //Calculate offset, needed to compare MSB of checksum and current bit of data
	T MSB = (ZERO | 1) << (sizeof(T)*8 - 1); 	//Turn most significant bit to 1, all other to 0
	return ((value & MSB) >> OFFSET) ^ (byte & 0x80); 
}

template<typename T>
bool isLessSignificantBitTrue(T value,  uint8_t byte)
{
	uint8_t LSB = 0x01;
	return (value & LSB) ^ (byte & LSB);
}


template<typename T>
T getMessageCRC( const std::string &message, crc_info<T> info)
{
	T crc = info.init;

	if (info.isInputReflected)
		info.polynome = reflect(info.polynome);

	for (size_t pos = 0; pos < message.size(); pos++)
	{
		uint8_t byte = message[pos];
		
		for (uint8_t i = 0; i < 8; ++i, info.isInputReflected ? rightBitShift(byte) : leftBitShift(byte)) 
		{
			if (info.isInputReflected)
			{
				isLessSignificantBitTrue(crc, byte)
				?  crc = rightBitShift(crc) ^ info.polynome 
				: rightBitShift(crc);
			}
			else
			{
				isMostSignificantBitTrue(crc, byte)
				?  crc = leftBitShift(crc) ^ info.polynome 
				: leftBitShift(crc);
			}
		}
	}

	if (info.isInputReflected ^ info.isOutputReflected)
		crc = reflect(crc);

	return  crc^ info.xorout;
}







template<typename T>
T getFileCRC( 	const std::string filePath, 
						T crc,  
						T POLYNOME, 
						const int INIT, 
						const int XOROUT, 
						const bool isInputReflected, 
						const bool isOutputReflected)
{
	const uint8_t OFFSET = (sizeof(T)*8 - 8);		
	const T MSB = (ZERO | 1) << (sizeof(T)*8 - 1); 	
	crc = INIT;

    uint8_t i;
    uint8_t byte;
    size_t pos;

    size_t bufsize = 1048576;
    std::vector<char> buffer;
    buffer.resize(bufsize);

    size_t chunksize = 65536;
    std::vector<char> chunk;
    chunk.resize(chunksize);

	std::ifstream fread;

    fread.rdbuf()->pubsetbuf(&buffer[0], bufsize);

    fread.open(filePath, std::ios::in|std::ios::binary);

	if (!fread) throw  std::runtime_error("...unknown. Failed to open " + filePath);

	if (isInputReflected)
		POLYNOME = reflect(POLYNOME);

	if(isInputReflected){

		while (fread)
		{
            fread.read( &chunk[0], chunksize );
			for ( pos = 0; pos < fread.gcount(); ++pos)
			{
				byte = chunk[pos];
				for ( i = 0; i < 8; ++i, byte >>= 1) {
					(crc & 1) ^ (byte & 1) ?  crc = (crc >> 1) ^ POLYNOME : crc >>= 1;
				}
			}		
			if (!isOutputReflected) crc = reflect(crc);
		}
	}
	else
	{
		while (fread)
		{
            fread.read( &chunk[0], chunksize );
			for ( pos = 0; pos < fread.gcount(); ++pos)
			{
				byte = chunk[pos];
				for ( i = 0; i < 8; ++i, byte <<= 1) {
					((crc & MSB) >> OFFSET) ^ (byte & 0x80) ?  crc = (crc << 1) ^ POLYNOME : crc <<= 1;
				}
			}
			if (isOutputReflected) crc = reflect(crc);
		}
	}

	return crc^XOROUT;	
}



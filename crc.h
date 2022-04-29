#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "init.h"
#include "./info/crc_info.h"

template<typename T>
T reflect(T value)
{
    T reflected = 0x0;
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
bool isMostSignificantBitDiffer(T value,  uint8_t byte)
{
	uint8_t OFFSET = (sizeof(T)*8 - 8); //Calculate offset, needed to compare MSB of checksum and current bit of data
	T MSB = (ZERO | 0x01) << (sizeof(T)*8 - 1); 	//Turn most significant bit to 1, all other to 0
	return ((value & MSB) >> OFFSET) ^ (byte & 0x80); 
}

template<typename T>
bool isLessSignificantBitDiffer(T value,  uint8_t byte)
{
	uint8_t LSB = 0x01;
	return (value & LSB) ^ (byte & LSB);
}

template<typename T>
void updateCRCperLSB(T &crc, uint8_t byte, const T &polynome)
{
	if (isLessSignificantBitDiffer(crc, byte))
	{
		rightBitShift(crc);
		crc ^= polynome;
	}
	else
	{
		rightBitShift(crc);
	}
}

template<typename T>
void updateCRCperMSB(T &crc, uint8_t byte, const T &polynome)
{
	if (isMostSignificantBitDiffer(crc, byte))
	{
		leftBitShift(crc);
		crc ^= polynome;
	}
	else
	{
		leftBitShift(crc);
	}
}



template<typename T>
T getCRCofStringMessage(const std::string &message, crc_info<T> crcInfo)
{
	std::vector<char> chunk(message.begin(), message.end());

	T crc = crcInfo.init;

	void (*updateCRC)(T&, uint8_t , const T&);

	if (crcInfo.isInputReflected){
		updateCRC = &updateCRCperLSB;
		crcInfo.polynome = reflect(crcInfo.polynome);
	}
	else{
		updateCRC = &updateCRCperMSB;
	}

	updateCRCperChunk(crc, chunk.size(), chunk, crcInfo, updateCRC);

	crc = getFinalCrcValue(crc, crcInfo);
	return  crc;
}



template<typename T>
T getFinalCrcValue(T &crc, const crc_info<T> &info)
{
	if (info.isInputReflected ^ info.isOutputReflected)
		crc = reflect(crc);

	crc ^= info.xorout;

	return crc;
}


template<typename T>
T updateCRCperChunk(T &crc, size_t chunkSize, const std::vector<char> &chunk, const crc_info<T> &crcInfo, void (*updateCRC)(T&, uint8_t , const T&))
{
	for (size_t currentPosition = 0; currentPosition < chunkSize; currentPosition++)
	{
		uint8_t byte = chunk[currentPosition];
		for (uint8_t i = 0; i < 8; ++i, crcInfo.isInputReflected ? rightBitShift(byte) : leftBitShift(byte)) 
		{
			updateCRC(crc, byte, crcInfo.polynome);
		}
	}

	return crc;
}



template<typename T>
T getCRCofFile(const std::string filePath, crc_info<T> crcInfo)
{
    size_t bufsize = 1048576;
    std::vector<char> buffer;
    buffer.resize(bufsize);

    size_t chunksize = 65536;
    std::vector<char> chunk;
    chunk.resize(chunksize);

	std::ifstream fread;
    fread.rdbuf()->pubsetbuf(&buffer[0], bufsize);
    fread.open(filePath, std::ios::in | std::ios::binary);

	if (!fread) throw  std::runtime_error("Failed to open " + filePath);

	T crc = crcInfo.init;

	void (*updateCRC)(T&, uint8_t byte, const T&);

	if (crcInfo.isInputReflected){
		updateCRC = &updateCRCperLSB;
		crcInfo.polynome = reflect(crcInfo.polynome);
	}
	else{
		updateCRC = &updateCRCperMSB;
	}

	while (fread)
	{
		fread.read( &chunk[0], chunksize );

		updateCRCperChunk(crc, fread.gcount(), chunk, crcInfo, updateCRC);	
	}

	crc = getFinalCrcValue(crc, crcInfo);
	return  crc;	
}


template<typename T>
T getCRCofFile(std::string filePath, T POLYNOME, T INIT, T XOROUT, bool isInputReflected, bool isOutputReflected)
{
	crc_info<typeof(T)> info = 
	{
		POLYNOME, 
		INIT, 
		XOROUT, 
		isInputReflected, 
		isOutputReflected
	};

	auto crc = getCRCofFile(filePath, info);

	return crc;
}


template<typename T>
T getCRCofStringMessage(const std::string &FILE, T POLYNOME, T INIT, T XOROUT, bool isInputReflected, bool isOutputReflected)
{
	crc_info<typeof(T)> info = 
	{
		POLYNOME, 
		INIT, 
		XOROUT, 
		isInputReflected, 
		isOutputReflected
	};

	auto crc = getCRCofStringMessage(FILE, info);

	return crc;
}




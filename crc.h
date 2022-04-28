#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "init.h"




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
        if (!refOut) crc = reflect(crc);
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
	std::ios_base::sync_with_stdio(false);

	const uint8_t OFFSET = (sizeof(T)*8 - 8);		
	const T MSB = (ZERO | 1) << (sizeof(T)*8 - 1); 	
	crc = INIT;

    uint8_t i;
    uint8_t byte;
    size_t pos;

    size_t bufsize = 1048576;
    std::vector<char> buffer;
    buffer.resize(bufsize);

    // char chunk[8192];
    size_t chunksize = 65536;
    std::vector<char> chunk;
    chunk.resize(chunksize);

	std::ifstream fread;

    fread.rdbuf()->pubsetbuf(&buffer[0], bufsize);

    fread.open(filename, std::ios::in|std::ios::binary);

	if (!fread) return "...unknown. Failed to open " + filename;


	if(refIn){
		POLY = reflect(POLY);

		while (fread)
		{
            fread.read( &chunk[0], chunksize );
			for ( pos = 0; pos < fread.gcount(); ++pos)
			{
				byte = chunk[pos];
				for ( i = 0; i < 8; ++i, byte >>= 1) {
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
            fread.read( &chunk[0], chunksize );
			for ( pos = 0; pos < fread.gcount(); ++pos)
			{
				byte = chunk[pos];
				for ( i = 0; i < 8; ++i, byte <<= 1) {
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
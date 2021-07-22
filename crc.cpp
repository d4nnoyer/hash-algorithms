#include <iostream>
#include <fstream>
#include <iomanip>

typedef unsigned short ushort;

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
#define CRC_32	(uint32_t)0x04C11DB7 //default crc32, for example, used by 7z


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

void hexDump(const char* filename){

	std::ifstream fread( filename, std::fstream::in|std::fstream::binary );
    std::cout << std::setfill( '0' ) << std::hex << std::uppercase;

    char chunk[16];
    size_t addrOffset = 0;

    while (fread)
    {
        size_t i;

        std::cout << "0x" << std::setw( 18 )  <<(addrOffset & 0xFFFFFFFFFFFFFFFF) << "  ";
        addrOffset += sizeof(chunk);
        fread.read( chunk, sizeof(chunk) );

        for (i = 0; i < fread.gcount(); i++)
            std::cout << std::setw(2) << ((ushort)chunk[ i ] % 0xFF00)<< " ";

        std::cout << "  ";

        for (i = 0; i < fread.gcount(); i++) 
            std::isprint( chunk[ i ] ) ? std::cout << chunk[i] :  std::cout << ".";

        std::cout << "\n";
    }
}

int main(int argc, char const *argv[]) {

	if (argc < 2)
	{
		std::cout << "Enter message: ";
		std::string message;
		std::cin >> message;
		stringCRC(message, CRC32, CRC_32, ONES32, ONES32, true, true );
	}
	else 
	{
		for (int n = 1; n < argc; n++){
			std::cout << "CRC32 of " << argv[n] << " is: " << fileCRC(argv[ n ], CRC32, CRC_32, ONES32, ONES32, true, true ) << std::endl;
		}
	}
	
	// hexDump();
	return 0;
}


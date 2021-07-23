#include <iostream>
#include "crc.h"
#include "hexdump.h"


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


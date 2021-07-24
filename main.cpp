#include "crc.h"
#include "hexdump.h"
#include "timer.h"


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
		Timer _;	
		std::cout << "CRC32 of " << argv[1] << " is: " << fileCRC(argv[1], CRC32, CRC_32, ONES32, ONES32, true, true ) << std::endl;
	}
	
	// hexDump();
	return 0;
}


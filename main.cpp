#include "crc.h"
#include "hexdump.h"
#include "timer.h"


using std::cout;
using std::endl;
using std::boolalpha;
using std::string;


void printCrcTestResult(const string crcName, long value, long expectedValue)
{
	cout << crcName << ": \t" << numericToHexString(value) << "\t" <<  boolalpha << (value == expectedValue) <<  endl;
}


int main(int argc, char const *argv[]) {

	if (argc < 2)
	{
		// std::cout << "Enter message: ";
		string message = "message";
		// std::cin >> message;

		auto crc = getMessageCRC(message, CRC_32_INFO);
		printCrcTestResult("CRC_32", crc, 0xB6BD307F);

		crc = getMessageCRC(message, CRC_32_BZIP2_INFO);
		printCrcTestResult("CRC_32/BZIP2", crc, 0xB8B548C0);

		crc = getMessageCRC(message, CRC_32C_INFO );
		printCrcTestResult("CRC_32C", crc, 0x98A214D0);
		
		crc = getMessageCRC(message, CRC_32D_INFO );
		printCrcTestResult("CRC_32D", crc, 0x2CB6DF28);

		crc = getMessageCRC(message, CRC_32Q_INFO );
		printCrcTestResult("CRC-CRC_32Q", crc, 0x2F795398);

		crc = getMessageCRC(message, CRC_32_JAMCRC_INFO );
		printCrcTestResult("CRC_32/JAMCRC", crc, 0x4942CF80);

		crc = getMessageCRC(message, CRC_32_MPEG2_INFO );
		printCrcTestResult("CRC-32/MPEG-2", crc, 0x474AB73F);

		crc = getMessageCRC(message, CRC_32_POSIX_INFO );
		printCrcTestResult("CRC-32/POSIX", crc, 0x39B18186);

		crc = getMessageCRC(message, CRC_32_XFER_INFO );
		printCrcTestResult("CRC-32/XFER", crc, 0xEF2CB5A9);
	}
	else 
	{
		Timer _;	
		cout << "CRC32 of " << argv[1] << " is: " << getFileCRC(argv[1], CRC32, CRC_32, ONES32, ONES32, true, true ) << endl;

		hexDump(argv[1]);
	}
	
	return 0;
}



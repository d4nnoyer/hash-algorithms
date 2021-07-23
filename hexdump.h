#include <iostream>
#include <fstream>
#include <iomanip>

typedef unsigned short ushort;


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
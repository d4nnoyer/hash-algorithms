
#include "crc_info.h"
#define ONES32	0xFFFFFFFF

//32bit CRC polynomes
#define CRC_32	(uint32_t)0x04C11DB7 //default crc32 used, for example, by 7-zip
#define CRC_32C	(uint32_t)0x1EDC6F41 
#define CRC_32D	(uint32_t)0xA833982B 
#define CRC_32Q	(uint32_t)0x814141AB 
#define CRC_32_XFER 	(uint32_t)0x000000AF

uint32_t CRC32;

const crc_info<typeof(CRC32)> CRC_32_INFO = {
    CRC_32,
    ONES32,
    ONES32,
    true,
    true
};

const crc_info<typeof(CRC32)> CRC_32_BZIP2_INFO = {
    CRC_32,
    ONES32,
    ONES32,
    false,
    false
};

const crc_info<typeof(CRC32)> CRC_32C_INFO = {
    CRC_32C,
    ONES32,
    ONES32,
    true,
    true
};

const crc_info<typeof(CRC32)> CRC_32D_INFO = {
    CRC_32D,
    ONES32,
    ONES32,
    true,
    true
};

const crc_info<typeof(CRC32)> CRC_32Q_INFO = {
    CRC_32Q,
    ~ONES32,
    ~ONES32,
    false,
    false
};

const crc_info<typeof(CRC32)> CRC_32_JAMCRC_INFO = {
    CRC_32,
    ONES32,
    ~ONES32,
    true,
    true
};

const crc_info<typeof(CRC32)> CRC_32_MPEG2_INFO = {
    CRC_32,
    ONES32,
    ~ONES32,
    false,
    false
};

const crc_info<typeof(CRC32)> CRC_32_POSIX_INFO = {
    CRC_32,
    ~ONES32,
    ONES32,
    false,
    false
};

const crc_info<typeof(CRC32)> CRC_32_XFER_INFO = {
    CRC_32_XFER,
    ~ONES32,
    ~ONES32,
    false,
    false
};
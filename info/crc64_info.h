#ifndef HASH_ALGORITHMS_CRC64_INFO_H
#define HASH_ALGORITHMS_CRC64_INFO_H

#include <cstdint>
#include "crc_info.h"

#define ONES64	UINT64_MAX

//Polynomials for 64bit CRC's
#define CRC_64_ECMA_182	        (uint64_t)0x42F0E1EBA9EA3693
#define CRC_64_GO_ISO	        (uint64_t)0x000000000000001B
#define CRC_64_WE	            (uint64_t)0x42F0E1EBA9EA3693
#define CRC_64_XZ	            (uint64_t)0x42F0E1EBA9EA3693

uint64_t CRC64;

const crc_info<typeof(CRC64)> CRC_64_ECMA_182_INFO = {
        .generating_polynomial =  CRC_64_ECMA_182,
        .initial_polynomial =  ~ONES64,
        .xorout_polynomial =  ~ONES64,
        .isInputReflected =  false,
        .isOutputReflected =  false
};

const crc_info<typeof(CRC64)> CRC_64_GO_ISO_INFO = {
        .generating_polynomial =  CRC_64_GO_ISO,
        .initial_polynomial =  ONES64,
        .xorout_polynomial =  ONES64,
        .isInputReflected =  true,
        .isOutputReflected =  true
};

const crc_info<typeof(CRC64)> CRC_64_WE_INFO = {
        .generating_polynomial =  CRC_64_WE,
        .initial_polynomial =  ONES64,
        .xorout_polynomial =  ONES64,
        .isInputReflected =  false,
        .isOutputReflected =  false
};

const crc_info<typeof(CRC64)> CRC_64_XZ_INFO = {
        .generating_polynomial =  CRC_64_XZ,
        .initial_polynomial =  ONES64,
        .xorout_polynomial =  ONES64,
        .isInputReflected =  true,
        .isOutputReflected =  true
};


#endif //HASH_ALGORITHMS_CRC64_INFO_H

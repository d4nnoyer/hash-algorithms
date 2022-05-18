cmake_minimum_required(VERSION 3.22)
project(hash_algorithms)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(crc/info)
include_directories(utils)

add_executable(hash_algorithms
        crc/info/crc32_info.h
        crc/info/crc_info.h
        utils/hexdump.h
        utils/timer.h
        crc/crc.h
        crc/init.h
        main.cpp crc/info/crc64_info.h)

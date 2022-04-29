cmake_minimum_required(VERSION 3.22)
project(hash_algorithms)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(info)
include_directories(utils)

add_executable(hash_algorithms
        info/crc32_info.h
        info/crc_info.h
        utils/hexdump.h
        utils/timer.h
        crc.h
        init.h
        main.cpp)

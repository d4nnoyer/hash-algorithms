
#ifndef CRC_INFO_H
#define CRC_INFO_H

template<typename T>
struct crc_info
{
    T generating_polynomial;
    T initial_polynomial;
    T xorout_polynomial;
    bool isInputReflected;
    bool isOutputReflected;
};

#endif /* CRC_INFO_H */
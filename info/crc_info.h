
#ifndef CRC_INFO_H
#define CRC_INFO_H

template<typename T>
struct crc_info
{
    T polynome;
    T init;
    T xorout;
    bool isInputReflected;
    bool isOutputReflected;
};

#endif /* CRC_INFO_H */
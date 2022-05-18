# hash-algorithms

This repository contains c++ implementation of popular hashing and checksum-calculating algorithms.

Current state: generalized CRC-calculation algorithm (with no precalculated values array) and info for popular 64 and 32 bit CRC types. 
CRC is described with 6 values: size in bits, initial polynomial, generating polynomial, polynomial to xor result, is input reflected, is is output reflected.
In this way it is easy to add CRC type in header file or call crc calculating method that takes all the above described values as arguments.
[Catalogue of parametrised CRC algorithms](https://reveng.sourceforge.io/crc-catalogue/all.htm)

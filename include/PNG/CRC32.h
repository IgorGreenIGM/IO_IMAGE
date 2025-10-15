#ifndef _CRC_32_H_INCLUDED_
#define _CRC_32_H_INCLUDED_

#include <iostream>
#include <cstdint>

/**
 * @brief CRC32 class, for CRC32 algorithm. 
 * 
 */
class CRC32
{
    public :
        CRC32();
        ~CRC32();
        void crc_table_compute();
        static uint32_t getCRC32(uint8_t *chunkDatas, int chunkLen);
        static uint32_t CRC32_update(uint32_t crc, uint8_t *dataCHUNK, int len);
        static void CRC32_table_compute(void);

        static uint32_t crc_table[256];
        static bool crc_table_computed;
};

#endif //_CRC_32_H_INCLUDED_
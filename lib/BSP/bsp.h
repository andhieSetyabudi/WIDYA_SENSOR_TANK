/**
 * 
 *  Purpose for Board Support Package
 * 
 *  Andhie Setyabudi
 *  Andhie.13@gmail.com
 *  https://github.com/andhieSetyabudi
 */

#ifndef _BSP_H_
#define _BSP_H_

#include "stdio.h"
#include "Arduino.h"
#include <avr/boot.h>
#include "EEPROM.h"

#define DEV_INFO_ADDR       10
#define MAX_UUID_LENGTH     8

#define DEFAULT_MODEL           "TANK-SENSOR"
#define DEFAULT_MODEL_VERSION   "0.0.1"
#define DEFAULT_SN              "000000"

typedef enum
{
    DEV_OK      = 0,
    DEV_ERROR   = 1,
    DEV_TIMEOUT = 2,
    DEV_UNKNOWN = 3,
}DEVICE_STATUS;


typedef struct
{
    char MODEL [12];
    char version [12];
    char firm_ver [12];
    char UUID [MAX_UUID_LENGTH+2];
    char SN[24];
}DEVICE_INFO;


typedef struct
{
    float heigth;
    float volume;
    float r_tank;
    uint32_t raw_data;
    byte status_flag;
}DEVICE_PARAM;

template <typename T, size_t size_>
float getSum(T (&series)[size_])
{
    float sum = 0;
    for (size_t i = 0; i < size_; i++)
    {
        sum += series[i];
    }
    return sum;
}

template <typename T, size_t size_>
float getAverage(T (&series)[size_])
{
    float sum = 0;
    for (size_t i = 0; i < size_; i++)
    {
        sum += series[i];
    }
    sum = sum / size_;
    return sum;
}


template <typename T, size_t size_>
float getDeviasion(T (&series)[size_])
{
    float sum = getSum(series) / size_;
    float std = 0;
    for (size_t id = 0; id < size_; id++)
    {
        std += pow((series[id] - sum), 2);
    }
    return (sqrt(std / (size_ - 1)));
}


class BSP{
    protected :
        static DEVICE_INFO info;
        static void (*resetFunc)(void);
    private :
        static DEVICE_STATUS BOARD_INIT_INFO(DEVICE_INFO *dev_info);
        static void BOARD_RESET_INFO(void);
    public :
        static DEVICE_STATUS initialize(void);
        static char* BOARD_GET_UUID(char* UUID, uint8_t len);
        
        static void uint32To4bytes(uint32_t res, uint8_t* dest)
        {
            // MSB first
            memset(dest,'\0',4);
            dest[0] = ( res >> 24 ) & 0xff;
            dest[1] = ( res >> 16 ) & 0xff;
            dest[2] = ( res >> 8 )  & 0xff;
            dest[3] = res & 0xff;
        };
        static void fourBytesToUint32(uint8_t* res, uint32_t *dest)
        {
            // MSB first
            uint32_t tmp_ = 0;
            tmp_ = res[0];
            tmp_ = (tmp_ << 8) | res[1];
            tmp_ = (tmp_ << 8) | res[2];
            tmp_ = (tmp_ << 8) | res[3];
            *dest = tmp_;
        };

        //=============== memory for load and store the parameter
        /**
         *  Read data from memory ( EEPROM )
         *  with CRC32 ( 4bytes ) as default method for checksum
         *  CRC MSB-first
         */
        static bool readMemory(uint8_t address, uint8_t *pData, size_t len);
        /**
         *  Write data from memory ( EEPROM )
         *  with CRC32 ( 4bytes ) as default method for checksum
         *  CRC MSB-first
         */
        static bool writeMemory(uint8_t address, uint8_t *pData, size_t len);


        static void print_device_info(void);
};

#endif
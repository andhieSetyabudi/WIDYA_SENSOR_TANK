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
#include "pin_info.h"
#include "HX710B.h"

#define DEV_INFO_ADDR       10
#define DEV_TANK_ADDR       
#define DEV_CAL_PARAM       


#define MAX_UUID_LENGTH     8

#define DEFAULT_MODEL           "TANK-SENSOR"
#define DEFAULT_MODEL_VERSION   "0.0.1"
#define DEFAULT_SN              "000000"

#define WATER_LEVEL_TS          2000UL      // update every 2seconds

typedef enum
{
    DEV_OK      = 0,
    DEV_ERROR   = 1,
    DEV_TIMEOUT = 2,
    DEV_UNKNOWN = 3,
}DEVICE_STATUS;


typedef struct
{
    char MODEL [12];                    // device model
    char version [12];                  // versi model 
    char firm_ver [12];                 // versi firmware
    char UUID [MAX_UUID_LENGTH+2];      // UUID
    char SN[24];                        // Serial Number
}DEVICE_INFO;


typedef struct
{
    float heigth;                       // Heigth of water-level
    float volume;                       // Volume of water in tank
    float r_tank;                       // Radius of tank ( assume as tube )
    uint32_t raw_data;                  // Raw data of adc to read height of water level
    byte status_flag;                   // Status flag of sensor
    byte motorFlag;                     // Flag of Motor-pump. 
}DEVICE_PARAM;

typedef struct 
{
    float const_base_area;
    float const_height;
}TANK_PARAM;

typedef struct 
{
    float slope,
          gain;
    /* data */
}CAL_PARAM;


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
template <typename T, size_t size_>
uint64_t str2uint64(T(&series)[size_])
{
    uint64_t tp=0;
    for(uint8_t id = 0; id< size_; id++)
    {
        if( id>0 )
            tp = tp << 8;
        tp |= (uint8_t)series[id];
    }
    return tp;
}

class BSP{
    protected :
        static uint32_t waterLevel_millis;
        static DEVICE_INFO info;
        static DEVICE_PARAM param;
        static void (*resetFunc)(void);
    private :
        static DEVICE_STATUS BOARD_INIT_INFO(DEVICE_INFO *dev_info);
        static void BOARD_RESET_INFO(void);

        static HX710B waterLevel;
        static uint8_t waterLevel_flag;
        static uint8_t waterLevel_error_count;
    public :
        static DEVICE_STATUS initialize(void);
        static void loop(void);
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
        static uint32_t crc32(const char *s,size_t n) 
        {
            uint32_t crc=0xFFFFFFFF;
            
            for(size_t i=0;i<n;i++) {
                char ch=s[i];
                for(size_t j=0;j<8;j++) {
                    uint32_t b=(ch^crc)&1;
                    crc>>=1;
                    if(b) crc=crc^0xEDB88320;
                    ch>>=1;
                }
            }
            return ~crc;
        }

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

        static const char* getUUID(void) {return BSP::info.UUID;}

        static void print_device_info(void);
};

#endif
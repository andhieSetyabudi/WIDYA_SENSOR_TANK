#include "com.h"
#include "pin_info.h"

RF24 com::radio;
uint8_t com::radio_flag = 0;

RADIO_INFO com::initialize(void)
{
    SPI.begin();
    if (!radio.begin(&SPI, CE_PIN,CSN_PIN)) 
        radio_flag = (uint8_t) RADIO_ERROR; 
    else
    {
        radio_flag = (uint8_t) RADIO_OK;
        radio.enableDynamicPayloads(); 
        radio.enableAckPayload();
    }
        

    return (RADIO_INFO)radio_flag;       
}
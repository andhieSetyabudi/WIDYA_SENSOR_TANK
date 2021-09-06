#ifndef _com_h_
#define _com_h_

#include "Arduino.h"
#include "RF24.h"
#include "pin_info.h"
#include "SPI.h"

typedef enum 
{
    RADIO_OK    =   0,
    RADIO_ERROR =   1,
    RADIO_TIMEOUT=  2,
    RADIO_UNKNOWN=  3,
}RADIO_INFO;

class com
{
    protected : 
        static RF24 radio;
    private :
        static uint8_t radio_flag;
    public :
        static RADIO_INFO initialize(void);
        static void loop(void);

};

#endif
#include "bsp.h"
#include <Version.h>
#include "CRC32.h"


DEVICE_INFO BSP::info={0};

void (*BSP::resetFunc)(void) = 0;
// uint16_t updateCRC(uint16_t crc,uint8_t data)
// {
//     crc = ((crc >> 8) & 0xff) | ((crc << 8) & 0xff00);  // swap byte
//     crc ^= data;
//     crc ^= (crc & 0x00ff) >> 4;
//     crc ^= (crc << 12);
//     crc ^= (crc & 0x00ff) << 5;
//     return (crc);
// }

char* BSP::BOARD_GET_UUID(char* UUID, uint8_t len)
{
    if( len > MAX_UUID_LENGTH )
        len = MAX_UUID_LENGTH;
    memset(UUID, '\0', len);
    for (size_t i = 0; i < len; i++)
    {
        UUID[i] = boot_signature_byte_get(0x0E + i + (len == 9 && i > 5 ? 1 : 0));
    }
    if( !isAlphaNumeric(UUID[len-1]))
        UUID[len-1]='\0';
    return UUID;
}

void BSP::BOARD_RESET_INFO(void)
{
    BSP::info={0};
    BOARD_GET_UUID(info.UUID, MAX_UUID_LENGTH+2);
    sprintf(info.firm_ver,VERSION);
    sprintf_P(info.MODEL,(const char*)F(DEFAULT_MODEL));
    sprintf_P(info.version,(const char*)F(DEFAULT_MODEL_VERSION));
    sprintf_P(info.SN,(const char*)F("WIT-TS%s"),DEFAULT_SN);
}

//=============== memory for load and store the parameter
bool BSP::readMemory(uint8_t address, uint8_t *pData, size_t len)
{
    CRC32 crc_count;
    crc_count.reset();
    uint8_t *crc_data = (uint8_t *)malloc(4 * sizeof(*crc_data));
    memset(crc_data, 0, 4);
    // load the data
    for (size_t i = 0; i < len; i++)
    {
        *pData = EEPROM.read(address + i);
        crc_count.update(*pData);
        pData++;
    };
    // load checksum
    for(uint8_t i = 0; i < 4; i++)
    {
        crc_data[i] = EEPROM.read(address+len+i);
    }
    uint32_t chk1, chk2;
    chk1 = crc_count.finalize();
    BSP::fourBytesToUint32(crc_data,&chk2);
    free(crc_data);
    if( chk1 == chk2 )
        return true;
    else
        return false;
}

bool BSP::writeMemory(uint8_t address, uint8_t *pData, size_t len)
{
    CRC32 crc_count;
    crc_count.reset();
    uint8_t *crc_data = (uint8_t *)malloc(4 * sizeof(*crc_data));
    memset(crc_data, 0, 4);
    // write the data
    for (size_t i = 0; i < len; i++)
    {
        EEPROM.write(address+i, *pData);
        crc_count.update(*pData);
        pData++;
    };
    // checksum check
    uint32_t chk1 = crc_count.finalize();
    BSP::uint32To4bytes(chk1, crc_data);
    // write checksum
    for (uint8_t i = 0; i < 4; i++)
    {
        EEPROM.write(address+len+i, crc_data[i]);
    }
    free(crc_data);
    return true;
}

DEVICE_STATUS BSP::initialize(void)
{
    if(!readMemory(DEV_INFO_ADDR, (uint8_t*)&BSP::info,sizeof(DEVICE_INFO)))
    {
        BOARD_RESET_INFO();
        writeMemory(DEV_INFO_ADDR,(uint8_t*)&BSP::info,sizeof(DEVICE_INFO));
        return DEV_ERROR;
    }
    return DEV_OK;
}


void BSP::print_device_info(void)
{
    Serial.print(F("MODEL NAME : "));
    Serial.println(info.MODEL);
    Serial.print(F("version : "));
    Serial.println(info.version);
    Serial.print(F("Firmware : "));
    Serial.println(info.firm_ver);
    Serial.print(F("UUID : "));
    Serial.println(info.UUID);
    Serial.print(F("SN : "));
    Serial.println(info.SN);
}
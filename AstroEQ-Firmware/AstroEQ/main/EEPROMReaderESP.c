#ifdef __ESP32__

#include "EEPROMReader.h"
#include "nvs_flash.h"
#include "nvs.h"

nvs_handle_t my_handle;

void initEEPROM(){
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err = ESP_ERR_NVS_NEW_VERSION_FOUND) 
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open("astroeq", NVS_READWRITE, my_handle);
    ESP_ERROR_CHECK(err);

}

char* stringify(unsigned int address) 
{
    char str[(int)((ceil(log10(address))+1))*sizeof(char)];
    sprintf(str, "%d", address);
    return *str[0];
}

byte EEPROM_readByte(unsigned int address)
{
    uint8_t b = 0;
    if(nvs_get_u8(my_handle, stringify(address), &b) != ESP_OK)
    {
        b = 0; // default value
    }
    return b;
}
unsigned int EEPROM_readInt(unsigned int address)
{
    uint32_t b = 0;
    if(nvs_get_u32(my_handle, stringify(address), &b) != ESP_OK)
    {
        b = 0; // default value
    }
    return (unsigned int)b;
}
unsigned long EEPROM_readLong(unsigned int address)
{
    uint64_t b = 0;
    if(nvs_get_u64(my_handle, stringify(address), &b) != ESP_OK)
    {
        b = 0; // default value
    }
    return (unsigned long)b;
}
void EEPROM_readString(char* string, byte len, unsigned int address)
{

    if(nvs_get_str(my_handle, stringify(address), string, len) != ESP_OK)
    {
        // default value
    }
}

void EEPROM_writeByte(byte val,unsigned int address)
{
    ESP_ERROR_CHECK(nvs_set_u8(my_handle, stringify(address), val));
}
void EEPROM_writeInt(unsigned int val,unsigned int address)
{
    ESP_ERROR_CHECK(nvs_set_u32(my_handle, stringify(address), val));
}
void EEPROM_writeLong(unsigned long val,unsigned int address)
{
    ESP_ERROR_CHECK(nvs_set_u64(my_handle, stringify(address), val));
}
void EEPROM_writeString(const char* string, byte len, unsigned int address)
{
    ESP_ERROR_CHECK(nvs_set_str(my_handle, stringify(address), string));
}

#endif
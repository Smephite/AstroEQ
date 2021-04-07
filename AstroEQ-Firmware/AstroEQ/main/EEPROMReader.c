#ifdef __ESP32__
#include <EEPROMReaderESP.c>
#else
#include <EEPROMReaderAVR.c>
#endif

void EEPROM_readAccelTable(AccelTableStruct* table, byte elements, unsigned int address){
    for(byte i = 0; i < elements; i++) {
        table[i].speed = EEPROM_readInt(address);
        address = address + sizeof(unsigned int);
        table[i].repeats = EEPROM_readInt(address);
        address = address + sizeof(byte);
    }
}

void EEPROM_writeAccelTable(AccelTableStruct* table, byte elements, unsigned int address){
    for(byte i = 0; i < elements; i++) {
        EEPROM_writeInt(table[i].speed,address);
        address = address + sizeof(unsigned int);
        EEPROM_writeByte(table[i].repeats,address);
        address = address + sizeof(byte);
    }
}
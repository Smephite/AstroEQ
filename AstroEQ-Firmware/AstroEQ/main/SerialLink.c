#ifdef __ESP32__

#include <SerialLinkESP.c>
#else
#include <SerialLinkAVR.c>
#endif

//Convert string to bytes
void Serial_writeStr(char* str) {
    while (*str) {
        Serial_write(*str++);
    }
}

//Convert array to bytes
void Serial_writeArr(char* arr, byte len) {
    while (len--) {
        Serial_write(*arr++);
    }
}
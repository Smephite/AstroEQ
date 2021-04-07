#ifdef __ESP32__
#include "SerialLink.h"
#include <driver/uart.h>
#include <soc/uart_struct.h>
#include "driver/spi_master.h"

#define CUSTOM_PIN false
#if CUSTOM_PIN
    #define CUSTOM_RX 0
    #define CUSTOM_TX 0
#endif

#define UART_BUFFER_SIZE 2048

#define UART true

bool softSPI = false;
spi_device_handle_t spi;

//Serial Functions

//Initialise the hardware UART port and set baud rate.
void Serial_initialise(const unsigned long baud)
{
    uart_config_t uart_config = {
        .baud_rate = baud,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 112,
    };
    // Configure SERIALn Port
    uart_param_config(SERIALn, &uart_config);
    #if CUSTOM_PIN
        uart_set_pin(Serialn, CUSTOM_TX, CUSTOM_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    #else
        uart_set_pin(Serialn, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    #endif

    QueueHandle_t uart_queue;

    uart_driver_install(Serialn, UART_BUFFER_SIZE, UART_BUFFER_SIZE, 10, &uart_queue, 0);
}

//Disable the hardware UART port
void Serial_disable()
{
    Serial_clear();
    uart_driver_delete(Serialn);
}

//SPI Functions

//Initialise the Software SPI by setting ports to correct direction and state.
void SPI_initialise()
{
    spi_bus_config_t buscfg={
        .miso_io_num=SPIMISOPin_Define,
        .mosi_io_num=SPIMOSIPin_Define,
        .sclk_io_num=SPIClockPin_Define,
        .quadhd_io_num=-1,
        .quadhd_io_num=-1
    };
    spi_device_interface_config_t devcfg={
        .clock_speed_hz=10*1000*1000,
        .mode=0,
        .spics_io_num=SPISSnPin_Define,
        .queue_size=7
    };
    spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    Serial_clear();

    softSPI = true;
}
//Disable the Software SPI by setting all ports back to input pull-up
void SPI_disable()
{
    spi_bus_remove_device(spi);
    spi_bus_free(HSPI_HOST);
    Serial_clear();
    softSPI = false;
}
//Software SPI Mode 3 Transfer
//One byte of data is sent and at the same time a byte is received.
byte SPI_transfer(byte data)
{
    nop(); //TODO
}
//Performs an SPI read request and stores the data in the RX buffer.
// - If there is no space in the buffer, a read request will *not* be performed
//   The buffer should be first emptied by using Serial_read()
void SPI_read(void)
{
    nop(); //TODO
}
//Performs an SPI write request.
void SPI_write(byte data)
{
    nop(); //TODO
}

//Common Functions

//Checks if there is any data available in the RX buffer.
// - If in SPI mode, this will also perform an SPI read transfer to see if there is any valid data.
byte Serial_available(void)
{
    int length = 0;
    if (softSPIEnabled) {
        //If SPI is enabled, we do a read to check if there is any data.
        SPI_read();
        //TODO set length
    } else {
        uart_get_buffered_data_len(Serialn, (size_t*)&length);
    }
    return (byte)length;
}
//Clear the serial buffers
void Serial_clear(void)
{
    if(!softSPI)
    {
        uart_flush(Serialn);
    } else {
        //TODO
    }
}
//Returns the next available data byte in the buffer
// - If there is nothing there, -1 is returned.
char Serial_read(void)
{
    if(softSPI)
    {
        return 'a'; //TODO
    } else {
        byte length = Serial_available();

        if(length == 0)
        {
            return -1;
        }

        unit8_t data[1];

        int code = uart_read_bytes(Serialn, data, 1, 100);

        if(code != 1) // code == -1 is err state , else should be read length (== 1)
        {
            // ERROR STATE
            return code;
        }

        return (char)data[0];   
    }
}

//Flushes data from TX buffer
void Serial_flush(void){
    if(!softSPI)
    {
        uart_wait_tx_done(Serialn, 1000); // todo: tick count
    }
}
//Write a byte of data
// - If in UART mode, the byte is stored into the TX buffer when there is space.
// - If in SPI mode, a write transfer is performed.
void Serial_write(char ch)
{
    if(softSPI)
    { // SPI
        SPI_write(ch);
    } else {
        //UART
        uart_write_bytes(SERIALn, ch, 1);
    }
}


#else
#error Board must be ESP32
#endif
#include "pico/stdlib.h"
#include "mcp2515.h"
#include "MS5837.h"
#include <cstdio>

#define I2C_PORT    (PICO_DEFAULT_I2C_INSTANCE) // i2c0
#define SDA_PIN     (PICO_DEFAULT_I2C_SDA_PIN)  // gpio 4
#define SCL_PIN     (PICO_DEFAULT_I2C_SCL_PIN)  // gpio 5

#define SPI_PORT    (PICO_DEFAULT_SPI_INSTANCE) // spi0
#define PIN_CS      (PICO_DEFAULT_SPI_CSN_PIN)  // gpio 17
#define PIN_MISO    (PICO_DEFAULT_SPI_RX_PIN)   // gpio 16
#define PIN_MOSI    (PICO_DEFAULT_SPI_TX_PIN)   // gpio 19
#define PIN_SCK     (PICO_DEFAULT_SPI_SCK_PIN)  // gpio 18

MCP2515 can(SPI_PORT, PIN_CS, PIN_MISO, PIN_MOSI, PIN_SCK);
struct can_frame canMsg;

MS5837 bar30;


bool init() {
    stdio_init_all();
    can.reset();
    can.setBitrate(CAN_250KBPS, MCP_12MHZ);
    can.setNormalMode();

    return bar30.init(I2C_PORT, SDA_PIN, SCL_PIN);
}

int main()
{
    int rate_ms = 200;
    int can_id  = 0x696;
    int error   = 0;

    if (!init()) { // bar30 init failed
        // printf("Bar30 init failed!\n");
        error = 1;
    }
    while (true) {
        bar30.read();

        float pressure = bar30.pressure();
        float depth = bar30.depth();
        float temp = bar30.temperature();

        // check for out of range measurements
        if (pressure < 0 || pressure > 30000.0f || depth < 0.0f || depth > 300.0f || temp < -25.0f || temp > 85.0f) {
            error = 2;
        }

        // printf("Pressure: %.1f mbar\n", pressure);
        // printf("Temp: %.0f deg C\n", temp);
        // printf("Depth: %.2f m\n\n", depth);
        
        uint32_t tx_pressure = (uint32_t) (pressure * 10);
        uint16_t tx_depth = (uint16_t) (depth * 100);
        int8_t tx_temp = (int8_t) temp;

        canMsg.can_id   = can_id;
        canMsg.can_dlc  = 8;
        canMsg.data[0]  = (tx_depth >> 0) & 0xFF;
        canMsg.data[1]  = (tx_depth >> 8) & 0xFF;
        canMsg.data[2]  = tx_temp;
        canMsg.data[3]  = (tx_pressure >> 0) & 0xFF;
        canMsg.data[4]  = (tx_pressure >> 8) & 0xFF;
        canMsg.data[5]  = (tx_pressure >> 16) & 0xFF;
        canMsg.data[6]  = (tx_pressure >> 24) & 0xFF;
        canMsg.data[7]  = error;

        can.sendMessage(&canMsg);
        sleep_ms(rate_ms);
    }
}


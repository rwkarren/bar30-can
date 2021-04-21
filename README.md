# bar30-can
Simple code to read pressure, depth, and seawater temperature from a Blue Robotics Bar30 pressure sensor, then package it and transmit it via CAN, through a board using a MCP2515 CAN transceiver.

This is implemented on a Raspberry Pi Pico and requires a Blue Robotics Bar30 pressure sensor as well as a CAN transceiver board that includes a Microchip MCP2515 CAN Controller chip. Find original repos for the Bar30 library (implemented on Pico [here](https://github.com/rwkarren/pico-ms5837) or originally from Blue Robotics implemented on Arduino [here](https://github.com/bluerobotics/BlueRobotics_MS5837_Library)) and the MCP2515 library (implemented on Pico [here](https://github.com/rwkarren/pico-mcp2515) or originally from seeed studio implemented on Arduino [here](https://github.com/autowp/arduino-mcp2515))

The CAN message ID is set by default as 0x696, but can be changed.

By default, the microcontroller will read and transmit values at 5Hz (or every 200ms), but that can be changed. The Bar30 pressure sensor cannot read more frequently than every 40ms.

The CAN message structure is as follows:

| CAN ID      | DLC         | Byte 7      | Byte 6      | Byte 5      | Byte 4      | Byte 3      | Byte 2      | Byte 1      | Byte 0      |
| ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- | ----------- |
| 0x696       | 0x08        | 0x00        | 0x00        | 0x02        | 0x20        | 0x34        | 0x15        | 0x32        | 0x02        |
| --          | 8 data bytes| Error       | Pressure B3 | Pressure B2 | Pressure B1 | Pressure B0 | Temp        | Depth MSB   | Depth LSB   |

| Data        | Type        | Scale       | Range       | Unit        | Example (hex) | Example (dec) | Example (scaled)        |
| ----------- | ----------- | ----------- | ----------- | ----------- | ------------- | ------------- | ----------------------- |
| Pressure    | uint32      | 10          | 0...30000.0 | mbar        | 0x00022034    | 139316        | 13931.6 mbar (absolute) |
| Temperature |  int8       | 1           | -20...85    | deg C       | 0x15          | 21            | 21 deg C                |
| Depth       | uint16      | 100         | 0...300.00  | m           | 0x3202        | 12802         | 128.02 m                |
| Error       | uint8       | n/a         | n/a         | n/a         | n/a           | n/a           | Valid Data              |

Error codes and their meanings:

| Error code  | Meaning     |
| ----------- | ----------- |
| 0           | Valid data  |

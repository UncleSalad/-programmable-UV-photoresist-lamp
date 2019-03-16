Programmable UV lamp
===
UV lamp for exposure of the photoresist with three presets that you can configure.

Connection:
---
*if the settings are standard*
1. Encoder:
* clk - 2
* dat - 3
* sw - 4
2. LCD 1602:
* sda - A4
* scl - A5
3. Relay/MOSFET/UV Lamp:
* out - 13

Settings:
---
1. Encoder:
* 9.#define clk 2
* 10.#define dt 3
* 11.#define sw 4
2. Relay/MOSFET/UV Lamp:
* 17.#define lamp 13
3. LCD 1602:
* 33.LiquidCrystal_I2C lsd(0x3f(addr), 16, 2);
4. Others:
* 26.#define presetLimit 360 *(maximum time for presets)* 

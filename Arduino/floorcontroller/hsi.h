//
// hsi.h -- hardware-software interface for floor controller
//
// BSla, 7 jul 2024
#ifndef _HSI_FC_H
#define _HSI_FC_H

// IO pin definitions

#define GPIO_ID_FLOOR_CONTROLLER    35
#define GPIO_ID_WHICH_FLOOR         36
#define GPIO_SCREEN                  2
#define GPIO_BUZZER                 33
#define GPIO_BTN_LED_RED            16
#define GPIO_BTN_LED_GREEN          15
#define GPIO_BTN_LED_BLUE           14
#define GPIO_BTN_PUSH                4
#define GPIO_BTN_EMO                 5

#define SDA_1306                    32     // I2C info for accessing the SSD1306 display
#define SCL_1306                    13
#define ADDRESS_1306              0x3C     // in range 0..0x7F  2 * 0x3C = 0x78


#endif


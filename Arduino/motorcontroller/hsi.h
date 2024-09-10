//
// hsi.h -- hardware-software interface for floor controller
//
// BSla, 7 jul 2024
#ifndef _HSI_H
#define _HSI_H

// IO pin definitions (for floor controller)

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

// IO pin definitions for motor controller

#define GPIO_LOWER_ENDSTOP  2  // GPIO 2
#define GPIO_UPPER_ENDSTOP  5  // GPIO 5

#define GPIO_ENC_A         33  // GPIO 33
#define GPIO_ENC_B         32  // GPIO 32

#define GPIO_VFD_POWERED    4  // GPIO 4 is powered input. active low
#define GPIO_VFD_FORWARD   16  // GPIO16 is forward, active high
#define GPIO_VFD_REVERSE   15  // GPIO15 is reverse, active high
#define GPIO_VFD_RESET     14  // GPIO14 is reset, active high
#define GPIO_VFD_SPEED     13  // GPIO13 is PWM output analog

// geometry definitions



#define BELOW_LOWER_HOME (float (0.03))  // maximum position below lower endstop

#define PAST_UPPER_HOME  (0.03)    // meters past home switching point
#define PAST_LOWER_HOME  (0.03)
#define PAST_HOME_SWITCH (double (0.03)) // meters past home switching point during homing

#define BEFORE_PHYSICAL_END  (double (0.02)) // stop this distance before physical end position

// lift move times
#define LOW_SPEED_TIME      120000   // milliseconds per meter
#define MEDIUM_SPEED_TIME    40000   // milliseconds per meter
#define HIGH_SPEED_TIME      20000   // milliseconds per meter


#endif

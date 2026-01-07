#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include "adc.h" 
#include "car_black_box.h"
#include "digital_keypad.h" 
#include "clcd.h"
#include "ds1307.h" 
#include "i2c.h"
#include "timers.h"
#include "string.h"
#include "EEPROM.h"
#include "car_black_box.h"

#define DASHBOARD_SCREEN      0X01
#define LOGIN_SCREEN          0X02
#define MAIN_MENU_SCREEN      0X03

#define VIEW_LOG_SCREEN       0x04
#define CLEAR_LOG_SCREEN      0x05
#define DOWNLOAD_LOG          0x06
#define CHANGE_PASSWORD       0x07
#define SET_TIME              0x08

#define RESET_PASSWORD        0x11
#define RESET_NOTHING         0x22
#define RETURN_BACK           0x33
#define LOGIN_SUCCESS         0x44
#define RESET_MENU            0x55
#define LONG_PRESS_THRESHOLD  1000
#endif

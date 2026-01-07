/*
 * File:   main.c
 * Author: MRFR
 *
 * Created on 21 April, 2025, 10:13 PM
 */


#include <xc.h>
#include "main.h"
#define _XTAL_FREQ                  20000000
#include "car_black_box.h"
#include "timers.h"
#pragma config WDTE=OFF

void init_config()
{
    //initialise i2c
    init_i2c(1000000);
    //initialise rtc
    init_ds1307();
    //initialise clcd
    init_clcd();
    //initialisse adc
    init_adc();
    //initialise dkp
    init_digital_keypad();
    //initialise timer 2
    init_timer2();
    
    PEIE=1;
    GIE=1;
    
}
void main(void) 
{
    init_config();
    unsigned char control_flag=DASHBOARD_SCREEN, key;
    unsigned char event[3]="ON";
    unsigned char reset_flag;
    
    unsigned char *gear[]={"GN","GR","G1","G2","G3","G4"};
    unsigned char gr=0;
    unsigned char speed=0;
    log_event(event, speed);
    ext_eeprom_24C02_str_write(0X00, "1010");
    extern unsigned char menu_pos;
    while(1)
    {
        speed=read_adc(1)/10.3;
        key=read_digital_keypad(STATE);
        if(key==SW1)
        {
            strcpy(event,"CO");
            log_event(event, speed);
        }
        else if(key==SW2 && gr<6)
        {
            strcpy(event, gear[gr]);
            gr++;
            log_event(event, speed);
        }
        else if(key==SW3 && gr>0)
        {
            gr--;
            strcpy(event, gear[gr]);
            
        }
        
        else if((key==SW4 || key==SW5) && control_flag==DASHBOARD_SCREEN)
        {
            clear_screen();
            control_flag=LOGIN_SCREEN;
            
            clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
            __delay_us(500);
            
            clcd_print("Enter Password", LINE1(1));
            clcd_write(LINE2(4), INST_MODE);
            
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            
            reset_flag=RESET_PASSWORD;
           //TMR2ON = 1;
            
        }
        
        else if (control_flag == MAIN_MENU_SCREEN && long_press(key))
        {
            enter_selected_menu(menu_pos, &control_flag);
        }
        switch(control_flag)
        {
            case DASHBOARD_SCREEN :
                display_dashboard(event, speed);
                break ;          
            case LOGIN_SCREEN :
                switch(login(key, reset_flag))
                {
                    case RETURN_BACK:
                        control_flag=DASHBOARD_SCREEN;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        TMR2ON = 0;
                        break;
                    case LOGIN_SUCCESS:
                        control_flag=MAIN_MENU_SCREEN;
                        clear_screen();
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        reset_flag=RESET_MENU;
                        continue;
                }
                break;
            case MAIN_MENU_SCREEN:
                menu_screen(key, reset_flag);
                break;
        }
        reset_flag=RESET_NOTHING;
        
    }
    return;
}

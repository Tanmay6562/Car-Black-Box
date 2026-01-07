/*
 * File:   car_black_box_def.c
 * Author: MRFR
 *
 * Created on 21 April, 2025, 10:16 PM
 */


#include <xc.h>
#include "main.h"
#include "clcd.h"
#include "timers.h"
#include "EEPROM.h"

#include <stdio.h> // for puts() and putch()

// EEPROM function prototypes
void ext_eeprom_24C02_str_read(unsigned char address, char *data, unsigned char length);
void ext_eeprom_24C02_str_write(unsigned char address, char *data);
void ext_eeprom_24C02_write(unsigned char address, char data);



unsigned char clock_reg[3];
char time[7];
char log[11];
char log_pos =0;
unsigned char sec;
unsigned char return_time;
unsigned char *menu[]={"View Log","Clear Log","Download Log","Change Password","Set Time"};
unsigned char menu_pos;


void clear_screen()
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}

static void get_time()
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
    
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
    // MM 
    time[2]= ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3]= (clock_reg[1] & 0x0F) + '0';
    
    // SS
    time[4]= ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5]= (clock_reg[2] & 0x0F) + '0';
    time[6]= '\0';
}
static void display_time()
{
    get_time();
     // HH:MM:SS 
    clcd_putch(time[0] , LINE2(2));
    clcd_putch(time[1] , LINE2(3));
    clcd_print(":", LINE2(4));
    clcd_putch(time[2] , LINE2(5));
    clcd_putch(time[3] , LINE2(6));
    clcd_print(":", LINE2(7));
    clcd_putch(time[4] , LINE2(8));
    clcd_putch(time[5] , LINE2(9));
}



void display_dashboard(unsigned char event[], unsigned char speed)
{
    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
    display_time();
    clcd_print("Time     E  Sp", LINE1(2));
    clcd_print(event, LINE2(11));
    clcd_putch((speed/10)+'0', LINE2(14));
    clcd_putch((speed%10)+'0', LINE2(15));
    
    
}

void store_event()
{
    char addr;
    if(log_pos==10)
    {
        log_pos=0;
    }
    addr=0x05+log_pos*10;
    ext_eeprom_24C02_str_write(addr,log);
    log_pos++;
}
void log_event(unsigned char event[], unsigned char speed)
{
    get_time();
    strncpy(log , time , 6 );
    strncpy(&log[6] , event , 2);
    
    
    log[8]=speed/10 + '0';
    log[9]=speed%10 + '0';
    log[10]='\0';
    
    store_event();
}
unsigned char login(unsigned char key, unsigned char reset_flag)
{
    static char user_password[4];
    static unsigned char attempts_left;
    static unsigned char i;
    
    if(reset_flag==RESET_PASSWORD)
    {
        i=0;
        attempts_left=3;
        user_password[0]='\0';
        user_password[1]='\0';
        user_password[2]='\0';
        user_password[3]='\0';
        key=ALL_RELEASED;
        return_time=5;
    }
    if(return_time==0)
    {
        return RETURN_BACK;
    }
    if(key==SW4&&i<4)
    {
        clcd_putch('*', LINE2(4+i));
        user_password[i]='1';
        i++;
        return_time=5;
    }
    else if(key==SW5&&i<4)
    {
        clcd_putch('*', LINE2(4+i));
        user_password[i]='0';
        i++;
        return_time=5;
    }
    if(i==4)
    {
        char s_password[4];
        //read the stored password from external EEPROM
        for(int j=0; j<4; j++)
        {
           s_password[j] = ext_eeprom_24C02_read(j);
        }
        //Compare the stored password
        if(strncmp(user_password, s_password, 4 ) ==0)
        {
            clear_screen();
            clcd_print("Login Success", LINE1(1));
            __delay_ms(5000);
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            
            //Change to Menu Screen
            return LOGIN_SUCCESS;
        }
        else
        {
            attempts_left--;
            if(attempts_left==0)
            {
                clear_screen();
                //display block screen
                clcd_print("You are Blocked", LINE1(1));
                clcd_print("Wait For", LINE2(1));
                sec=60;
                while(sec)
                {
                    clcd_putch((sec/10)+'0', LINE2(9));
                    clcd_putch((sec%10)+'0', LINE2(10));    
                }
                clcd_print("secs",LINE2(12));
                __delay_ms(3000);
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                attempts_left=3;
            }
            else
            {
                clear_screen();
                clcd_print("Wrong Password", LINE1(0));
                clcd_print("Attempts_left", LINE2(2));
                clcd_putch(attempts_left+'0',LINE2(0));
                __delay_ms(3000);   
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
            }
            clear_screen();
            clcd_print("Enter Password", LINE1(1));
            clcd_write(LINE2(4), INST_MODE);
            
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            i=0;
        }
               
    }
    
}
unsigned char menu_screen(unsigned char key, unsigned char reset_flag)
{
    read_digital_keypad(LEVEL) == key;
    if(reset_flag==RESET_MENU)
    {
        return_time=5;
        menu_pos=0;
    }
    if(key==SW4 && menu_pos>0)
    {
        clear_screen();
        //INCREMENT THE menu
        menu_pos--;
        
    }
    else if(key==SW5 && menu_pos<4)
    {
        clear_screen();
        //DECREMENT THE menu
        menu_pos++;
    }
    if(menu_pos==4)
    {
        clcd_putch('*', LINE2(0));
        clcd_print(menu[menu_pos-1],LINE1(1));
        clcd_print(menu[menu_pos],LINE2(1));
    }
    else{
    clcd_putch('*', LINE1(0));
    clcd_print(menu[menu_pos],LINE1(1));
    clcd_print(menu[menu_pos+1],LINE2(1));
    }
    return_time=5;
    return menu_pos;
}

void view_log_screen(unsigned char key)
{
    static unsigned char index = 0;
    char read_log[11];
    char addr = 0x05 + (index * 10);
    ext_eeprom_24C02_str_read(addr, read_log, 10);

    clcd_print("Ev# Time  Ev Sp", LINE1(0));
    clcd_putch(index + '0', LINE2(0));
    clcd_putch(' ', LINE2(1));
    for (char i = 0; i < 10; i++)
    {
        clcd_putch(read_log[i], LINE2(2 + i));
    }

    if (key == SW4)
    {
        index = (index == 0) ? 9 : index - 1;
    }
    else if (key == SW5)
    {
        index = (index + 1) % 10;
    }
    else if (long_press(key))
    {
        clear_screen();
        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
        __delay_us(100);
    }
}

void clear_log_data()
{
    for (char i = 0; i < 100; i += 10)
    {
        ext_eeprom_24C02_str_write(0x05 + i, "          ");
    }
    log_pos = 0;
    clcd_print("Log Cleared", LINE1(1));
    __delay_ms(1000);
}

void download_log_to_pc()
{
    char read_log[11];
    for (char i = 0; i < 10; i++)
    {
        ext_eeprom_24C02_str_read(0x05 + (i * 10), read_log, 10);
        puts(read_log);
        putch('\n');
    }
}

unsigned char change_password(unsigned char key)
{
    static char new_pass[4];
    static char confirm_pass[4];
    static unsigned char i = 0, stage = 0;

    if (key == SW4 && i < 4)
    {
        clcd_putch('*', LINE2(4 + i));
        (stage == 0 ? new_pass : confirm_pass)[i++] = '1';
    }
    else if (key == SW5 && i < 4)
    {
        clcd_putch('*', LINE2(4 + i));
        (stage == 0 ? new_pass : confirm_pass)[i++] = '0';
    }

    if (i == 4)
    {
        if (stage == 0)
        {
            stage = 1;
            i = 0;
            clear_screen();
            clcd_print("Re-enter Pass", LINE1(1));
        }
        else
        {
            if (strncmp(new_pass, confirm_pass, 4) == 0)
            {
                for (char j = 0; j < 4; j++)
                {
                    ext_eeprom_24C02_write(j, new_pass[j]);
                }
                clcd_print("Pass Updated", LINE1(1));
                __delay_ms(1000);
                return 1;
            }
            else
            {
                clcd_print("Mismatch", LINE1(1));
                __delay_ms(1000);
            }
            i = 0;
            stage = 0;
            clear_screen();
            clcd_print("Enter Password", LINE1(1));
        }
    }
    return 0;
}

unsigned char set_time_screen(unsigned char key)
{
    static unsigned char time[3];
    static unsigned char field = 0; // 0: hour, 1: min, 2: sec

    time[0] = read_ds1307(HOUR_ADDR);
    time[1] = read_ds1307(MIN_ADDR);
    time[2] = read_ds1307(SEC_ADDR);

    char disp[9];
    get_time();
    clcd_print("Set Time", LINE1(3));
    clcd_print(disp, LINE2(4));
    clcd_putch('^', LINE2(4 + (field * 3)));

    if (key == SW4)
    {
        time[field] += 1;
        if ((field == 0 && time[0] > 0x23) ||
            (field == 1 && time[1] > 0x59) ||
            (field == 2 && time[2] > 0x59))
        {
            time[field] = 0;
        }
        write_ds1307(HOUR_ADDR + field, time[field]);
    }
    else if (key == SW5)
    {
        field = (field + 1) % 3;
    }
    else if (long_press(key))
    {
        return 1;
    }
    return 0;
}
//detect switch is long pressed 
unsigned char long_press(unsigned char key)
{
    static unsigned short press_count = 0;

    if (key == SW4)
    {
        press_count++;
        if (press_count > 100) // Adjust this threshold if needed (based on how often your loop runs)
        {
            press_count = 0;
            return 1; // Long press detected
        }
    }
    else
    {
        press_count = 0;
    }

    return 0;
}



void enter_selected_menu(unsigned char menu_pos, unsigned char *control_flag)
{
    switch(menu_pos)
    {
        case 0:
            *control_flag = VIEW_LOG_SCREEN;
            break;
        case 1:
            *control_flag = CLEAR_LOG_SCREEN;
            break;
        case 2:
            *control_flag = DOWNLOAD_LOG;
            break;
        case 3:
            *control_flag = CHANGE_PASSWORD;
            break;
        case 4:
            *control_flag = SET_TIME;
            break;
    }
}

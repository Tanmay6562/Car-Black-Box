#include <xc.h>
#include "digital_keypad.h"
/*
void init_digital_keypad(void)
{
    // Set Keypad Port as input 
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    
    if (mode == LEVEL_DETECTION)
    {
        return KEYPAD_PORT & INPUT_LINES;
    }
    else
    {
        if (((KEYPAD_PORT & INPUT_LINES) != ALL_RELEASED) && once)
        {
            once = 0;
            
            return KEYPAD_PORT & INPUT_LINES;
        }
        else if ((KEYPAD_PORT & INPUT_LINES) == ALL_RELEASED)
        {
            once = 1;
        }
    }
    
    return ALL_RELEASED;
}*/
void init_digital_keypad(void)
{
    /* Set keypad port pins (RB0 to RB5) as input */
    KEYPAD_PORT_DDR |= INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode)
{
    static unsigned char once = 1;
    unsigned char key_value = KEYPAD_PORT & INPUT_LINES;

    if (mode == LEVEL_DETECTION)
    {
        return key_value;
    }
    else // STATE_DETECTION
    {
        if ((key_value != ALL_RELEASED) && once)
        {
            once = 0;
            return key_value;
        }
        else if (key_value == ALL_RELEASED)
        {
            once = 1;
        }
    }

    return ALL_RELEASED;
}

#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>
#include <stdbool.h>        
#include "board.h"      
#include "LCD_Driver.h"     

typedef enum {
    DIR_NONE  = 0,
    DIR_LEFT  = 1,
    DIR_RIGHT = 2
} TouchDirection_t;

bool TouchButtonTap(uint16_t x, uint16_t y, uint16_t w, uint16_t h);  
TouchDirection_t GetTouchDirection(void);
uint8_t IsDropPressed(void);

#endif

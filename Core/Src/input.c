#include <stdbool.h>    
#include "stmpe811.h"    
#include "board.h"       
#include "input.h"      
#include "board.h"      
#include <stdbool.h>    

#define SCREEN_WIDTH 240

bool TouchButtonTap(uint16_t x,uint16_t y,uint16_t w,uint16_t h)
 {
     STMPE811_TouchData t;
     bool pressedInside = false;
 
     while (returnTouchStateAndLocation(&t) != STMPE811_State_Pressed);
 
     t.x = SCREEN_WIDTH - 1 - t.x;
 
     if (t.x >= x && t.x < x + w && t.y >= y && t.y < y + h)
         pressedInside = true;
 
     while (returnTouchStateAndLocation(&t) == STMPE811_State_Pressed);
 
     return pressedInside;
 }

 TouchDirection_t GetTouchDirection(void)
 {
     STMPE811_TouchData touch;
     if (returnTouchStateAndLocation(&touch) == STMPE811_State_Pressed)
     {
         uint16_t x = SCREEN_WIDTH - 1 - touch.x;
         return (x < SCREEN_WIDTH/2) ? DIR_LEFT : DIR_RIGHT;
     }
     return DIR_NONE;
 }

 uint8_t IsDropPressed(void)
{
    return (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET);
}

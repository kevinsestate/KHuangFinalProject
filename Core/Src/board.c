#include "board.h"
#include "LCD_Driver.h"
#include <string.h>

#define SCREEN_WIDTH      240
#define Y_SHIFT           10

#define LCD_COLOR_WHITE   0xFFFF
#define LCD_COLOR_BLACK   0x0000
#define LCD_COLOR_RED     0xF800
#define LCD_COLOR_YELLOW  0xFFE0
#define LCD_COLOR_BLUE    0x001F

#define CELL_WIDTH        30
#define CELL_HEIGHT       30
#define CELL_RADIUS       14
#define GRID_X_OFFSET     15
#define GRID_Y_OFFSET     30

#define MENU_BG 0x0010    

void FillRect(int x,int y,int w,int h,uint16_t color)
{
    for (int yy = y; yy < y + h; yy++)
        for (int xx = x; xx < x + w; xx++)
            LCD_Draw_Pixel(xx, yy, color);
}

void DrawRect(int x,int y,int w,int h,uint16_t color)
{
    for (int xx = x; xx < x + w; xx++) {
        LCD_Draw_Pixel(xx, y,       color);
        LCD_Draw_Pixel(xx, y + h-1, color);
    }
    for (int yy = y; yy < y + h; yy++) {
        LCD_Draw_Pixel(x,       yy, color);
        LCD_Draw_Pixel(x + w-1, yy, color);
    }
}

void CellCenterXY(uint8_t row, uint8_t col, uint16_t *x, uint16_t *y)
{
    *x = GRID_X_OFFSET + col * CELL_WIDTH  + CELL_WIDTH  / 2;
    *y = GRID_Y_OFFSET + row * CELL_HEIGHT + CELL_HEIGHT / 2;
}

void DrawBoard(void)
{
    LCD_Clear(0, MENU_BG);
    for (uint8_t r = 0; r < ROWS; ++r)
        for (uint8_t c = 0; c < COLS; ++c) {
            uint16_t cx, cy;
            CellCenterXY(r, c, &cx, &cy);
            LCD_Draw_Circle_Fill(cx, cy, CELL_RADIUS, LCD_COLOR_WHITE);
        }
}

void DrawEmptyCell(uint8_t row, uint8_t col)
{
    uint16_t cx, cy;
    CellCenterXY(row, col, &cx, &cy);
    LCD_Draw_Circle_Fill(cx, cy, CELL_RADIUS, LCD_COLOR_WHITE);
}

void DrawToken(uint8_t row,uint8_t col,uint8_t player)
{
    if (row >= ROWS || col >= COLS || player == 0) return;
    uint16_t cx, cy;
    CellCenterXY(row, col, &cx, &cy);
    uint16_t color = (player == 1) ? LCD_COLOR_RED : LCD_COLOR_YELLOW;
    LCD_Draw_Circle_Fill(cx, cy, CELL_RADIUS, color);
}

void DrawHoverToken(uint8_t col, uint8_t player)
{
    uint16_t cx, cy;
    CellCenterXY(0, col, &cx, &cy);
    cy -= CELL_HEIGHT;                       
    uint16_t color = (player == 1) ? LCD_COLOR_RED : LCD_COLOR_YELLOW;
    LCD_Draw_Circle_Fill(cx, cy, CELL_RADIUS, color);
}

void ClearHoverToken(uint8_t col)
{
    uint16_t cx, cy;
    CellCenterXY(0, col, &cx, &cy);
    cy -= CELL_HEIGHT;
    LCD_Draw_Circle_Fill(cx, cy, CELL_RADIUS, MENU_BG);
}
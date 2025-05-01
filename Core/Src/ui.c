#include "ui.h"
#include "board.h"          
#include "game.h"
#include "stm32f4xx_hal.h"
#include "LCD_Driver.h"
#include <string.h>
#include <stdio.h>
#include "input.h"


/* ── UI constants (colours, positions) ───────────────────────── */
#define MENU_BG        0x0010
#define BUTTON_BG      LCD_COLOR_WHITE
#define BUTTON_FG      LCD_COLOR_BLUE
#define TITLE_COLOR    LCD_COLOR_WHITE

#define TOKEN_Y        80
#define TOKEN_R        12
#define BUTTON_W       180
#define BUTTON_H       40
#define BUTTON_SP      20
#define DETECT_SHIFT_Y 20

#define BTN_W   160
#define BTN_H    40
#define BTN_X   ((LCD_PIXEL_WIDTH - BTN_W) / 2)
#define BTN_Y   (LCD_PIXEL_HEIGHT / 2 + 30)
#define DETECT_BTN_Y  (BTN_Y - 150)

#define Y_SHIFT           10
#define TOKEN_RADIUS      12
#define MODE_DETECT_OFFSET_Y  20


/* ── low-level pixel helpers ─────────────────────────────────── */


void DrawTextCenterShifted(const char *s,int y,uint16_t color)
{
    uint16_t len = strlen(s);
    uint16_t w   = Font16x24.Width * len;
    uint16_t x   = (LCD_PIXEL_WIDTH - w) / 2;

    LCD_SetFont(&Font16x24);
    LCD_SetTextColor(color);
    for (uint16_t i=0;i<len;i++)
        LCD_DisplayChar(x + i*Font16x24.Width, y + Y_SHIFT, s[i]);
}

void DrawCenteredMessageAtY(const char *msg,uint16_t y,uint16_t color)
{
    LCD_SetFont(&Font16x24);
    LCD_SetTextColor(color);

    uint16_t len = strlen(msg);
    uint16_t x   = (LCD_PIXEL_WIDTH - Font16x24.Width * len) / 2;

    for (uint16_t i=0;i<len;i++)
        LCD_DisplayChar(x + i*Font16x24.Width, y, msg[i]);
}


void ShowStartScreen(void)
{
    LCD_Clear(0, MENU_BG);

    // title 
    DrawTextCenterShifted("CONNECT", 10,  TITLE_COLOR);
    DrawTextCenterShifted("   FOUR", 40,  TITLE_COLOR);

    // logo tokens
    int cols = 4;
    int startX = (LCD_PIXEL_WIDTH - (cols*2*TOKEN_RADIUS + (cols-1)*10)) / 2;
    for (int i=0;i<cols;i++)
    {
        uint16_t c = (i==3) ? LCD_COLOR_RED : LCD_COLOR_YELLOW;
        int x = startX + i*(2*TOKEN_RADIUS+10) + TOKEN_RADIUS;
        LCD_Draw_Circle_Fill(x, TOKEN_Y + Y_SHIFT, TOKEN_RADIUS, c);
    }

    /* buttons */
    int btnX  = (LCD_PIXEL_WIDTH - BUTTON_W)/2;
    int btnY1 = TOKEN_Y + Y_SHIFT + 2*TOKEN_RADIUS + 30;
    int btnY2 = btnY1 + BUTTON_H + BUTTON_SP;

    FillRect(btnX, btnY1, BUTTON_W, BUTTON_H, BUTTON_BG);
    DrawRect(btnX, btnY1, BUTTON_W, BUTTON_H, BUTTON_FG);
    DrawTextCenterShifted("1 PLAYER", btnY1 + 8 - Y_SHIFT, BUTTON_FG);

    FillRect(btnX, btnY2, BUTTON_W, BUTTON_H, BUTTON_BG);
    DrawRect(btnX, btnY2, BUTTON_W, BUTTON_H, BUTTON_FG);
    DrawTextCenterShifted("2 PLAYERS", btnY2 + 8 - Y_SHIFT, BUTTON_FG);
}

void WaitForModeSelection(void)
{
    ShowStartScreen();

    const int btnX  = (LCD_PIXEL_WIDTH - BUTTON_W) / 2;
    const int btnY1 = TOKEN_Y + Y_SHIFT + 2*TOKEN_RADIUS + 30;
    const int btnY2 = btnY1 + BUTTON_H + BUTTON_SP;

    const int detY1 = btnY1 + MODE_DETECT_OFFSET_Y;
    const int detY2 = btnY2 + MODE_DETECT_OFFSET_Y;

    while (gameMode == 0)
    {
        STMPE811_TouchData t;
        if (returnTouchStateAndLocation(&t) == STMPE811_State_Pressed)
        {
            t.x = LCD_PIXEL_WIDTH - 1 - t.x;
            int x = t.x;
            int y = t.y;

            if (x >= btnX && x < btnX + BUTTON_W)
            {
                if (y >= detY1 && y < detY1 + BUTTON_H)
                {
                    gameMode = 1;  // 1-PLAYER tapped
                }
                else if (y >= detY2 && y < detY2 + BUTTON_H)
                {
                    gameMode = 2;  // 2-PLAYERS tapped
                }
            }
        }
        HAL_Delay(200);
    }

    LCD_Clear(0, MENU_BG);
    DrawBoard();
}

void DrawStatusBar(void)
{
    int barHeight = Font16x24.Height * 2 + 12;
    int barY      = LCD_PIXEL_HEIGHT - barHeight;


    FillRect(0, barY, LCD_PIXEL_WIDTH, barHeight, LCD_COLOR_WHITE);

    //line 1: Score
    char buf[32];
    sprintf(buf, "P1:%d  P2:%d", winsP1, winsP2);
    int len    = strlen(buf);
    int textW  = len * Font16x24.Width;
    int startX = (LCD_PIXEL_WIDTH - textW) / 2;
    int y1     = barY + 4;

    LCD_SetFont(&Font16x24);
    LCD_SetTextColor(LCD_COLOR_BLACK);
    for (int i = 0; i < len; i++)
        LCD_DisplayChar(startX + i*Font16x24.Width, y1, buf[i]);

    // line 2: Time 
    sprintf(buf, "Time: %lus", GetElapsedTimeSeconds());
    len    = strlen(buf);
    textW  = len * Font16x24.Width;
    startX = (LCD_PIXEL_WIDTH - textW) / 2;
    int y2 = y1 + Font16x24.Height + 4;

    for (int i = 0; i < len; i++)
        LCD_DisplayChar(startX + i*Font16x24.Width, y2, buf[i]);
}


void DrawFinalScreen(uint32_t roundTime)
{
    char buf[32];

    LCD_Clear(0, MENU_BG);
    DrawTextCenterShifted("CONNECT", 10, TITLE_COLOR);
    DrawTextCenterShifted("   FOUR", 40, TITLE_COLOR);

    const int cols = 4, gap = 10;
    int totalW = cols*2*TOKEN_RADIUS + (cols-1)*gap;
    int startX = (LCD_PIXEL_WIDTH - totalW) / 2;
    for (int i = 0; i < cols; i++)
    {
        uint16_t c = (i == 3) ? LCD_COLOR_RED : LCD_COLOR_YELLOW;
        int x = startX + i*(2*TOKEN_RADIUS + gap) + TOKEN_RADIUS;
        LCD_Draw_Circle_Fill(x, TOKEN_Y + 5, TOKEN_RADIUS, c);
    }

    const char *msg =
        (lastRoundResult == 1) ? "P1  WINS!" :
        (lastRoundResult == 2) ? "P2  WINS!" :
                                 "DRAW!";
    DrawCenteredMessageAtY(msg, (LCD_PIXEL_HEIGHT/2) - 60, LCD_COLOR_WHITE);

    snprintf(buf, sizeof(buf), "Score R:%d  Y:%d", winsP1, winsP2);
    DrawCenteredMessageAtY(buf, (LCD_PIXEL_HEIGHT/2) - 20, LCD_COLOR_WHITE);

    snprintf(buf, sizeof(buf), "Time: %lus", roundTime);
    DrawCenteredMessageAtY(buf, (LCD_PIXEL_HEIGHT/2) +  0, LCD_COLOR_WHITE);

    FillRect(BTN_X, BTN_Y, BTN_W, BTN_H, BUTTON_BG);
    DrawRect(BTN_X, BTN_Y, BTN_W, BTN_H, BUTTON_FG);
    DrawCenteredMessageAtY("RESTART",
        BTN_Y + (BTN_H - Font16x24.Height)/2, BUTTON_FG);
}

void WaitForRestartTap(void)
{
    STMPE811_TouchData t;

    DrawFinalScreen(GetElapsedTimeSeconds());

    while (1)
    {
        if (returnTouchStateAndLocation(&t) == STMPE811_State_Pressed)
        {
            uint16_t touchX = LCD_PIXEL_WIDTH  - 1 - t.x;
            uint16_t touchY = LCD_PIXEL_HEIGHT - 1 - t.y;

            if (touchX >= BTN_X && touchX <  BTN_X + BTN_W &&
                touchY >= DETECT_BTN_Y && touchY <  DETECT_BTN_Y + BTN_H)
            {
                while (returnTouchStateAndLocation(&t) == STMPE811_State_Pressed)
                    HAL_Delay(5);

                ResetGame();
                DrawBoard();
                return;
            }
        }
        HAL_Delay(50);
    }
}

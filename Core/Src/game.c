#include "game.h"
#include "stm32f4xx_hal.h"
#include "board.h"          /* DrawToken */
#include <string.h>   


/* definitions for the externs */
uint8_t  board[ROWS][COLS] = {0};
uint8_t  currentPlayer     = 1;
uint8_t  selectedCol       = COLS / 2;
uint8_t  winsP1            = 0;
uint8_t  winsP2            = 0;
uint8_t  gameOver          = 0;
uint32_t startTime         = 0;
uint8_t  lastRoundResult   = 0;
uint8_t gameMode = 0;

void ResetGame(void)
{
    memset(board, 0, sizeof(board));
    startTime     = 0;
    currentPlayer = 1;
    selectedCol   = COLS / 2;
    gameOver      = 0;
}

int8_t DropToken(uint8_t col)
{
    if (col >= COLS) return -1;
    for (int8_t row = ROWS - 1; row >= 0; --row)
        if (board[row][col] == 0) {
            board[row][col] = currentPlayer;
            DrawToken(row, col, currentPlayer);
            return row;
        }
    return -1;
}

uint8_t CheckWin(uint8_t row, uint8_t col)
{
    uint8_t player = board[row][col];
    if (player == 0) return 0;

    const int dir[4][2] = { {0,1}, {1,0}, {1,1}, {1,-1} };
    for (int d = 0; d < 4; ++d) {
        int cnt = 1;
        int r = row + dir[d][0], c = col + dir[d][1];
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == player) {
            ++cnt; r += dir[d][0]; c += dir[d][1];
        }
        r = row - dir[d][0]; c = col - dir[d][1];
        while (r >= 0 && r < ROWS && c >= 0 && c < COLS && board[r][c] == player) {
            ++cnt; r -= dir[d][0]; c -= dir[d][1];
        }
        if (cnt >= 4) return 1;
    }
    return 0;
}

uint8_t CheckDraw(void)
{
    for (uint8_t r = 0; r < ROWS; ++r)
        for (uint8_t c = 0; c < COLS; ++c)
            if (board[r][c] == 0) return 0;
    return 1;
}

uint32_t GetElapsedTimeSeconds(void)
{
    return (HAL_GetTick() - startTime) / 1000;
}

void SetRoundResult(uint8_t result)
{
    lastRoundResult = result;
}

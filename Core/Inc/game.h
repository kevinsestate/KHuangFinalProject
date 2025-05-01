#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "board.h"                  /* ROWS, COLS, DrawToken prototype */

/* global state (defined in game.c) */
extern uint8_t gameMode;	
extern uint8_t  board[ROWS][COLS];
extern uint8_t  currentPlayer;
extern uint8_t  selectedCol;
extern uint8_t  winsP1;
extern uint8_t  winsP2;
extern uint8_t  gameOver;
extern uint32_t startTime;
extern uint8_t  lastRoundResult;

/* API */
void     ResetGame(void);
int8_t   DropToken(uint8_t col);
uint8_t  CheckWin(uint8_t row, uint8_t col);
uint8_t  CheckDraw(void);
uint32_t GetElapsedTimeSeconds(void);
void     SetRoundResult(uint8_t result);

#endif /* GAME_H */

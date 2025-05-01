#ifndef AI_H
#define AI_H

 
#include <stdint.h>
#include "board.h"
#include "game.h"	

int8_t  GetSmartAIMove(void);                        

int8_t  SimDrop(uint8_t b[ROWS][COLS], uint8_t col, uint8_t player);

uint8_t CheckWinFast(uint8_t b[ROWS][COLS], int8_t row,  uint8_t col);

int EvaluateBoard(uint8_t b[ROWS][COLS]);
 
int Minimax(uint8_t b[ROWS][COLS], uint8_t depth, int alpha, int beta, uint8_t maximizing);
 
 #endif /* AI_H */
 

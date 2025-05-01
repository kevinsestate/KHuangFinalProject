#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

/* grid size */
#define ROWS 6
#define COLS 7

/* geometry (pixels) */

#define CELL_WIDTH     30
#define CELL_HEIGHT    30
#define CELL_RADIUS    14
#define GRID_X_OFFSET  15
#define GRID_Y_OFFSET  30



void FillRect(int x, int y, int w, int h, uint16_t color);
void DrawRect(int x, int y, int w, int h, uint16_t color);


/* drawing API */
void DrawBoard(void);
void CellCenterXY(uint8_t row, uint8_t col, uint16_t *x, uint16_t *y);
void DrawToken(uint8_t row, uint8_t col, uint8_t player);
void DrawEmptyCell(uint8_t row, uint8_t col);
void DrawHoverToken(uint8_t col, uint8_t player);
void ClearHoverToken(uint8_t col);

#endif /* BOARD_H */

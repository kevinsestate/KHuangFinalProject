#include "ai.h"
#include <string.h>
#include <stdint.h>

#define SEARCH_DEPTH   4      // full plies (Max+Min = 1 ply each) 
#define WIN_SCORE      100000 // must be >> any heuristic sum      
#define P2             2      // AI is always player-2             
#define P1             1

static const int posScore[ROWS][COLS] = {
    { 3,  4,  5,  7,  5,  4,  3},
    { 4,  6,  8, 10,  8,  6,  4},
    { 5,  8, 11, 13, 11,  8,  5},
    { 5,  8, 11, 13, 11,  8,  5},
    { 4,  6,  8, 10,  8,  6,  4},
    { 3,  4,  5,  7,  5,  4,  3}
};


int8_t GetSmartAIMove(void)
{
    int bestScore = -0x7FFFFFFF;
    int8_t bestCol = -1;

    const uint8_t order[COLS] = {3, 2, 4, 1, 5, 0, 6};

    uint8_t temp[ROWS][COLS];

    for (int k = 0; k < COLS; ++k)
    {
        uint8_t col = order[k];
        if (board[0][col] != 0) continue;      /* column full */

        memcpy(temp, board, sizeof(board));
        int8_t row = SimDrop(temp, col, P2);
        if (row < 0) continue;

        /* immediate win? take it */
        if (CheckWinFast(temp, row, col))
            return col;

        int score = Minimax(temp, SEARCH_DEPTH - 1, -0x7FFFFFFF,
                            0x7FFFFFFF, 0);    /* opponent turn */

        if (score > bestScore)
        {
            bestScore = score;
            bestCol   = col;
        }
    }
    return bestCol;
}

/* ---------------------------------------------------------------
 *  Minimax + alpha–beta (depth in plies)
 * -------------------------------------------------------------*/
int Minimax(uint8_t b[ROWS][COLS], uint8_t depth, int alpha, int beta, uint8_t maximizing)
{
    /* quick heuristic at leaves */
    if (depth == 0) return EvaluateBoard(b);

    const uint8_t order[COLS] = {3, 2, 4, 1, 5, 0, 6};
    uint8_t temp[ROWS][COLS];

    if (maximizing)          /* AI turn */
    {
        int best = -0x7FFFFFFF;
        for (int k = 0; k < COLS; ++k)
        {
            uint8_t col = order[k];
            if (b[0][col] != 0) continue;

            memcpy(temp, b, sizeof(temp));
            int8_t row = SimDrop(temp, col, P2);
            if (row < 0) continue;

            if (CheckWinFast(temp, row, col))
                return WIN_SCORE;          /* quickest win */

            int val = Minimax(temp, depth - 1, alpha, beta, 0);
            if (val > best) best = val;
            if (best > alpha) alpha = best;
            if (beta <= alpha) break;      /* beta cut-off     */
        }
        return best;
    }
    else                    /* Opponent turn */
    {
        int best = 0x7FFFFFFF;
        for (int k = 0; k < COLS; ++k)
        {
            uint8_t col = order[k];
            if (b[0][col] != 0) continue;

            memcpy(temp, b, sizeof(temp));
            int8_t row = SimDrop(temp, col, P1);
            if (row < 0) continue;

            if (CheckWinFast(temp, row, col))
                return -WIN_SCORE;         /* avoid loss */

            int val = Minimax(temp, depth - 1, alpha, beta, 1);
            if (val < best) best = val;
            if (best < beta) beta = best;
            if (beta <= alpha) break;      /* alpha cut-off     */
        }
        return best;
    }
}


int EvaluateBoard(uint8_t b[ROWS][COLS])
{
    int score = 0;

    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            if (b[r][c] == P2)      score += posScore[r][c];
            else if (b[r][c] == P1) score -= posScore[r][c];

    static const int dir[4][2] = {{0,1},{1,0},{1,1},{1,-1}};
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
        {
            uint8_t player = b[r][c];
            if (!player) continue;

            for (int d = 0; d < 4; ++d)
            {
                int dr = dir[d][0], dc = dir[d][1];
                int count = 0, openEnds = 0;

                /* look forward */
                int rr = r, cc = c;
                for (int k = 0; k < 4 && rr < ROWS && cc >= 0 && cc < COLS; ++k)
                {
                    if (b[rr][cc] == player) count++;
                    else if (b[rr][cc] == 0) { openEnds++; break; }
                    else break;
                    rr += dr; cc += dc;
                }
                /* look backward */
                rr = r - dr; cc = c - dc;
                for (int k = count; k < 4 && rr >= 0 && cc >= 0 && cc < COLS; ++k)
                {
                    if (b[rr][cc] == player) { count++; }
                    else if (b[rr][cc] == 0) { openEnds++; break; }
                    else break;
                    rr -= dr; cc -= dc;
                }

                if (count >= 4) score += (player == P2) ? WIN_SCORE : -WIN_SCORE;
                else if (count == 3 && openEnds == 2)
                    score += (player == P2) ? 300 : -300;
                else if (count == 2 && openEnds == 2)
                    score += (player == P2) ? 40 : -40;
            }
        }
    return score;
}


//Helpers

int8_t SimDrop(uint8_t b[ROWS][COLS], uint8_t col, uint8_t player)
{
    for (int8_t r = ROWS - 1; r >= 0; --r)
        if (b[r][col] == 0) { b[r][col] = player; return r; }
    return -1;
}

int8_t inBounds(int r, int c) { return r>=0 && r<ROWS && c>=0 && c<COLS; }

uint8_t CheckWinFast(uint8_t b[ROWS][COLS], int8_t row, uint8_t col)
{
    uint8_t p = b[row][col];
    const int dir[4][2] = {{0,1},{1,0},{1,1},{1,-1}};

    for (int d=0; d<4; ++d)
    {
        int cnt = 1;
        int dr = dir[d][0], dc = dir[d][1];
        int r=row+dr, c=col+dc;
        while(inBounds(r,c) && b[r][c]==p){cnt++; r+=dr; c+=dc;}
        r=row-dr; c=col-dc;
        while(inBounds(r,c) && b[r][c]==p){cnt++; r-=dr; c-=dc;}
        if (cnt>=4) return 1;
    }
    return 0;
}

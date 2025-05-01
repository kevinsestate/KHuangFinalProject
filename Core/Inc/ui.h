#ifndef UI_H
#define UI_H

#include <stdint.h>

/* ---------- high-level screens ---------- */
void ShowStartScreen(void);
void WaitForModeSelection(void);
void DrawStatusBar(void);
void DrawFinalScreen(uint32_t roundTime);
void WaitForRestartTap(void);

/* ---------- misc draw helpers ---------- */
void DrawTextCenterShifted(const char *s, int y, uint16_t color);
void DrawCenteredMessageAtY(const char *msg, uint16_t y, uint16_t color);

#endif /* UI_H */

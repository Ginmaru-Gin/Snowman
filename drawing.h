#pragma once

#include <Windows.h>

#include "variables.h"

VOID renderGameFrame(HDC hdcRenderer, size_t frameWidth, size_t frameHeight);
VOID renderPaneFrame(HDC hdcRenderer, size_t frameWidth, size_t frameHeight);
VOID drawFrame(HDC hdcWindow, size_t widthGameWnd, size_t heightGameWnd, HDC hdcRenderer);
VOID updateGameInfoPane(VOID);
VOID updateScoreCounter(HDC hdc, PAINTSTRUCT *ps, HFONT hFont);
VOID switchPane(HWND source, HWND destination);

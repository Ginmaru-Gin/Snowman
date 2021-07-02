#pragma once
#include <stdlib.h>

#include "mechanics.h"
#include "objects.h"
#include "variables.h"

#include <Windows.h>

VOID initGame(
    HINSTANCE hInstance,
    INT windowWidth,
    UINT giftCount,
    UINT lifes,
    DOUBLE gravity,
    DOUBLE difficultyMultiplierScore,
    UINT AbilityP1,
    UINT AbilityP2,
    UINT AbilityA1,
    UINT AbilityA2,
    UINT AbilityA3
);

VOID reinitGame(
    HINSTANCE hInstance,
    INT windowWidth,
    UINT giftCount,
    UINT lifes,
    DOUBLE gravity,
    DOUBLE difficultyMultiplierScore,
    UINT AbilityP1,
    UINT AbilityP2,
    UINT AbilityA1,
    UINT AbilityA2,
    UINT AbilityA3
);

VOID clearGame();

VOID gameProc(VOID);

VOID startGame(VOID);
VOID restartGame(VOID);
VOID pauseGame(VOID);
VOID continueGame(VOID);
VOID finishGame(VOID);
VOID gameOver(VOID);

VOID respawnResource(Resource *resource);

VOID giftsFalling(VOID);
VOID abilityResourcesFalling(VOID);
VOID resourceLifeFalling(VOID);

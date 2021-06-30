#include "objects.h"

#include <stdlib.h>

#include "mechanics.h"
#include "resources.h"
#include "abilities.h"

VOID respawnResource(Resource *resource);

VOID giftsFalling(VOID);
VOID abilityResourcesFalling(VOID);
VOID resourceLifeFalling(VOID);

VOID loadSkin(UINT ID);

VOID resizeObjectsSizeBasedOnSkin();

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

VOID clearGame();

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

VOID gameProc(VOID);

VOID restartGame(VOID);

VOID updateGameInfoPane(VOID);

VOID updateScoreCounter(HDC hdc, PAINTSTRUCT *ps, HFONT hFont);

VOID gameOver(VOID);

VOID switchPane(HWND source, HWND destination);

VOID startGame(VOID);
VOID pauseGame(VOID);
VOID continueGame(VOID);
VOID finishGame(VOID);

VOID playMusic(UINT music);

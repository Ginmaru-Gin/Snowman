#include "game.h"

#define MAX_SCORE_COUNTER_LENGTH 20
#define MAX_LIVES_COUNTER_LENGTH 3

VOID respawnResource(Resource *resource) {
    resource->coordinates.X = rand() % (game.gameField->width - resource->width);
    resource->coordinates.Y = -(rand() % (game.gameField->height) * (rand() % resource->rarity + 1));
}

VOID loadSkin(UINT ID) {
    if (ID == skin.ID) return;
    switch (ID) {
    case SKIN_SNOWMAN:
        skin.gameBackground = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_BKG_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.paneBackground = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_PANE_BKG_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.character = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_SNOWMAN_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.gift = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE_GIFT_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.abilityResources[0] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE1_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.abilityResources[1] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE2_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.abilityResources[2] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE3_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.resourceLife = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE_LIFE_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.gameMusic = WAVE_SNOWMAN_GAME_MUSIC;
        skin.menuMusic = WAVE_SNOWMAN_MENU_MUSIC;
        break;
    case SKIN_EVANGELION:
        skin.gameBackground = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_BKG_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.paneBackground = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_PANE_BKG_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.character = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_SNOWMAN_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.gift = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE_GIFT_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.abilityResources[0] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE1_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.abilityResources[1] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE2_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.abilityResources[2] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE3_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.resourceLife = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE_LIFE_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.gameMusic = WAVE_EVANGELION_GAME_MUSIC;
        skin.menuMusic = WAVE_EVANGELION_MENU_MUSIC;
        break;
    }

    resizeObjectsSizeBasedOnSkin();
    InvalidateRect(hCharacterListPaneWnd, NULL, TRUE);
    playMusic(skin.menuMusic);
}

VOID resizeObjectsSizeBasedOnSkin() {
    BITMAP bmp;
    DOUBLE k;

    GetObject(skin.character, sizeof(bmp), &bmp);
    k = bmp.bmWidth > bmp.bmHeight ? (DOUBLE)MAX_SNOWMAN_WIDTH / bmp.bmWidth : (DOUBLE)MAX_SNOWMAN_HEIGHT / bmp.bmHeight;
    game.player->width = bmp.bmWidth * k;
    game.player->height = bmp.bmHeight * k;

    GetObject(skin.resourceLife, sizeof(bmp), &bmp);
    k = bmp.bmWidth > bmp.bmHeight ? (DOUBLE)MAX_FALLER_WIDTH / bmp.bmWidth : (DOUBLE)MAX_FALLER_HEIGHT / bmp.bmHeight;
    game.lifeResource.width = bmp.bmWidth * k;
    game.lifeResource.height = bmp.bmHeight * k;

    GetObject(skin.gift, sizeof(bmp), &bmp);
    k = bmp.bmWidth > bmp.bmHeight ? (DOUBLE)MAX_FALLER_WIDTH / bmp.bmWidth : (DOUBLE)MAX_FALLER_HEIGHT / bmp.bmHeight;
    for (int i = 0; i < game.giftCount; ++i) {
    game.gifts[i]->width = bmp.bmWidth * k;
    game.gifts[i]->height = bmp.bmHeight * k;
    }

    for (int i = 0; i < ACTIVE_ABILITY_COUNT; ++i) {
        GetObject(skin.abilityResources[i], sizeof(bmp), &bmp);
        k = bmp.bmWidth > bmp.bmHeight ? (DOUBLE)MAX_FALLER_WIDTH / bmp.bmWidth : (DOUBLE)MAX_FALLER_HEIGHT / bmp.bmHeight;
        game.abilityResources[i].width = bmp.bmWidth * k;
        game.abilityResources[i].height = bmp.bmHeight * k;
    }
}

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
) {

    game.giftCount = giftCount;
    game.maxResources = DEFAULT_MAX_RESOURCES;
    game.maxLifes = game.lifes = lifes;
    game.gravity = gravity;
    game.difficultyMultiplierScore = difficultyMultiplierScore;
    game.score = 0;

    game.gameField = newGameField(
        GAME_FIELD_WIDTH,
        GAME_FIELD_HEIGHT,
        (DOUBLE)windowWidth / GAME_FIELD_WIDTH
    );

    game.ground = newEnviroment(
        (Coordinate) { 0, GAME_FIELD_HEIGHT - 1 },
        GAME_FIELD_WIDTH,
            1
            );

    game.player = newCharacter(
        (Coordinate) { GAME_FIELD_WIDTH / 2, GAME_FIELD_HEIGHT - MAX_SNOWMAN_HEIGHT },
        100,
        MAX_SNOWMAN_WIDTH,
        MAX_SNOWMAN_HEIGHT
    );

    game.gifts = (Resource **)malloc(sizeof(Resource *) * giftCount);
    if (game.gifts) {
        for (int i = 0; i < giftCount; ++i) {
            game.gifts[i] = newGift(
                (Coordinate) {
                0, 0
            },
                10,
                    1,
                    1,
                    MAX_FALLER_WIDTH,
                    MAX_FALLER_HEIGHT
                    );
            respawnResource(game.gifts[i]);
        }
    }
    else giftCount = 0;

    for (int i = 0; i < ACTIVE_ABILITY_COUNT; ++i) {
        game.abilityResourceCounters[i] = 0;
        initFaller(
            (Faller *)&game.abilityResources[i],
            (Coordinate) {
            0, 0
        },
            10,
                MAX_FALLER_WIDTH,
                MAX_FALLER_HEIGHT,
                0,
                10
                );
        respawnResource(&game.abilityResources[i]);
    }

    initFaller(
        (Faller *)&game.lifeResource,
        (Coordinate) {
        0, 0
    },
        10,
            MAX_FALLER_WIDTH,
            MAX_FALLER_HEIGHT,
            0,
            10
            );
    respawnResource(&game.lifeResource);

    game.abilities[PASSIVE_ABILITY_1] = (Ability){ AbilityP1, ABILITY_DISABLED };
    game.abilities[PASSIVE_ABILITY_2] = (Ability){ AbilityP2, ABILITY_DISABLED };
    game.abilities[ACTIVE_ABILITY_1] = (Ability){ AbilityA1, ABILITY_DISABLED };
    game.abilities[ACTIVE_ABILITY_2] = (Ability){ AbilityA2, ABILITY_DISABLED };
    game.abilities[ACTIVE_ABILITY_3] = (Ability){ AbilityA3, ABILITY_DISABLED };

    resizeObjectsSizeBasedOnSkin();

    game.status = INITIALIZED;
}

VOID clearGame() {
	deleteGameField(game.gameField);
	deleteCharacter(game.player);
	deleteEnviroment(game.ground);
	for (int i = 0; i < game.giftCount; ++i)
		deleteGift(game.gifts[i]);
	free(game.gifts);
}

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
) {
    clearGame();
    initGame(
        hInstance,
        windowWidth,
        giftCount,
        lifes,
        gravity,
        difficultyMultiplierScore,
        AbilityP1,
        AbilityP2,
        AbilityA1,
        AbilityA2,
        AbilityA3
    );
}

VOID gameProc(VOID) {
    giftsFalling();
    abilityResourcesFalling();
    resourceLifeFalling();
}

VOID giftsFalling(VOID) {
    for (int i = 0; i < game.giftCount; ++i) {
        if (move((Object *)game.gifts[i], FALLING)) {
            if (game.status != GAME_IN_BACKGROUND_MODE && collision((Object *)game.gifts[i], (Object *)game.ground)) {
                game.lifes -= 1;
                if (!game.lifes) gameOver();
                updateGameInfoPane();
            }
            else if (collision((Object *)game.gifts[i], (Object *)game.player)) {
                game.score += game.gifts[i]->price * game.difficultyMultiplierScore;
                respawnResource(game.gifts[i]);

                updateGameInfoPane();
            }
        }
        else respawnResource(game.gifts[i]);
    }
}

VOID abilityResourcesFalling(VOID) {
    for (int i = 0; i < ACTIVE_ABILITY_COUNT; ++i) {
        if (move((Object *)&game.abilityResources[i], FALLING)) {
            if (game.status != GAME_IN_BACKGROUND_MODE && collision((Object *)&game.abilityResources[i], (Object *)game.ground)) {
                respawnResource(&game.abilityResources[i]);
            }
            else if (collision((Object *)&game.abilityResources[i], (Object *)game.player)) {
                if (game.abilityResourceCounters[i] < game.maxResources) {
                    game.abilityResourceCounters[i] += 1;
                    updateGameInfoPane();
                }
                respawnResource(&game.abilityResources[i]);
            }
        }
        else respawnResource(&game.abilityResources[i]);
    }
}

VOID resourceLifeFalling(VOID) {
        if (move((Object *)&game.lifeResource, FALLING)) {
            if (game.status != GAME_IN_BACKGROUND_MODE && collision((Object *)&game.lifeResource, (Object *)game.ground)) {
                respawnResource(&game.lifeResource);
            }
            else if (collision((Object *)&game.lifeResource, (Object *)game.player)) {
                if (game.lifes < game.maxLifes) {
                    game.lifes += 1;
                    updateGameInfoPane();
                }
                respawnResource(&game.lifeResource);
            }
        }
        else respawnResource(&game.lifeResource);
}

VOID restartGame(VOID) {
    useAbility(ACTIVE_ABILITY_1, ABILITY_DEACTIVATE);
    useAbility(ACTIVE_ABILITY_2, ABILITY_DEACTIVATE);
    useAbility(ACTIVE_ABILITY_3, ABILITY_DEACTIVATE);
    useAbility(PASSIVE_ABILITY_1, ABILITY_DEACTIVATE);
    useAbility(PASSIVE_ABILITY_2, ABILITY_DEACTIVATE);
    reinitGame(
        hInstance,
        widthGameWnd,
        game.giftCount,
        game.maxLifes,
        1,
        1,
        game.abilities[PASSIVE_ABILITY_1].ID,
        game.abilities[PASSIVE_ABILITY_2].ID,
        game.abilities[ACTIVE_ABILITY_1].ID,
        game.abilities[ACTIVE_ABILITY_2].ID,
        game.abilities[ACTIVE_ABILITY_3].ID
    );
    startGame();
}

VOID updateGameInfoPane(VOID) {
    InvalidateRect(hGameInfoPaneWnd, NULL, FALSE);
}

VOID updateScoreCounter(HDC hdc, PAINTSTRUCT *ps, HFONT hFont) {
    SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, 0xFFFFFF);
    WCHAR wScore[MAX_SCORE_COUNTER_LENGTH];
    _ui64tow_s(game.score, wScore, MAX_SCORE_COUNTER_LENGTH, 10);
    //SetWindowTextW(hScoreCounterLabel, wScore);
    TextOutW(hdc, LEFT_PANE_FIELD, RIGHT_PANE_FIELD, wScore, lstrlenW(wScore));
}

VOID gameOver(VOID) {
    pauseGame();
    if (recordList.amount < 10 || game.score > recordList.records[recordList.amount - 1].score) {
        switchPane(hGameInfoPaneWnd, hNewRecordPaneWnd);
    }
    else {
        switchPane(hGameInfoPaneWnd, hGameOverPaneWnd);
    }
    playMusic(skin.menuMusic);
}

VOID switchPane(HWND source, HWND destination) {
    ShowWindow(source, SW_HIDE);
    ShowWindow(destination, SW_SHOW);
    activePane = destination;
    SetFocus(destination);
    updateGameInfoPane();
}

VOID startGame(VOID) {
    useAbility(PASSIVE_ABILITY_1, ABILITY_ACTIVATE);
    useAbility(PASSIVE_ABILITY_2, ABILITY_ACTIVATE);
    updateGameInfoPane();
	game.status = game.status | STARTED;
    playMusic(skin.gameMusic);
}

VOID pauseGame(VOID) {
	game.status = game.status | PAUSED;
    playMusic(skin.menuMusic);
}

VOID continueGame(VOID) {
	game.status = game.status & ~PAUSED;
    playMusic(skin.gameMusic);
}

VOID finishGame(VOID) {
	game.status = game.status & ~(STARTED | PAUSED);
    playMusic(skin.menuMusic);
} 

VOID playMusic(UINT music) {
    PlaySoundW(MAKEINTRESOURCEW(music), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP | SND_ASYNC);
}

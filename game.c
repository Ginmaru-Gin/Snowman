#include "game.h"

VOID respawnResource(Resource *resource) {
    resource->coordinates.X = rand() % (game.gameField->width - resource->width);
    resource->coordinates.Y = -(rand() % (game.gameField->height) * (rand() % resource->rarity + 1));
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

VOID clearGame() {
	deleteGameField(game.gameField);
	deleteCharacter(game.player);
	deleteEnviroment(game.ground);
	for (int i = 0; i < game.giftCount; ++i)
		deleteGift(game.gifts[i]);
	free(game.gifts);
}

VOID gameProc(VOID) {
    giftsFalling();
    abilityResourcesFalling();
    resourceLifeFalling();
}

VOID startGame(VOID) {
    useAbility(PASSIVE_ABILITY_1, ABILITY_ACTIVATE);
    useAbility(PASSIVE_ABILITY_2, ABILITY_ACTIVATE);
    updateGameInfoPane();
	game.status = game.status | STARTED;
    playMusic(skin.gameMusic);
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







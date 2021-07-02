#include "abilities.h"

#include "variables.h"

#define INCREASED_BY_PERCENTS(VAR, PERCENTS)( VAR * (1 + (DOUBLE)PERCENTS / 100) )
#define DECREASED_BY_PERCENTS(VAR, PERCENTS)( VAR / (1 + (DOUBLE)PERCENTS / 100) )
#define INCREASED_BY_VALUE(VAR, VALUE)( VAR + VALUE)
#define DECREASED_BY_VALUE(VAR, VALUE)( VAR - VALUE)

static VOID useAbilityP1(INT action);
static VOID useAbilityP2(INT action);
static VOID useAbilityP3(INT action);
static VOID useAbilityP4(INT action);
static VOID useAbilityA1(INT action);
static VOID useAbilityA2(INT action);
static VOID useAbilityA3(INT action);
static VOID useAbilityA4(INT action);
static VOID useAbilityA5(INT action);
static VOID useAbilityA6(INT action);

VOID useAbility(UINT abilityIndex, INT action) {
    if (game.abilities[abilityIndex].status ^ action) {
        if (abilityIndex < ACTIVE_ABILITY_COUNT) {
            if (action == ABILITY_ACTIVATE) {
                if (game.abilityResourceCounters[abilityIndex] >= RESOURCE_COUNT_TO_USE_ABILITY) {
                    game.abilityResourceCounters[abilityIndex] -= RESOURCE_COUNT_TO_USE_ABILITY;
                    updateGameInfoPane();
                }
                else {
                    return;
                }
            }
            switch (game.abilities[abilityIndex].ID) {

            case ABILITY_A1_ID:
                game.abilities[abilityIndex].status = action;
                useAbilityA1(action);
                break;
            case ABILITY_A2_ID:
                game.abilities[abilityIndex].status = action;
                useAbilityA2(action);
                break;
            case ABILITY_A3_ID:
                useAbilityA3(action);
                break;
            case ABILITY_A4_ID:
                useAbilityA4(action);
                break;
            case ABILITY_A5_ID:
                game.abilities[abilityIndex].status = action;
                useAbilityA5(action);
                break;
            case ABILITY_A6_ID:
                useAbilityA6(action);
                break;
            }
        }
        else {

            game.abilities[abilityIndex].status = action;
            switch (game.abilities[abilityIndex].ID) {
            case ABILITY_P1_ID:
                useAbilityP1(action);
                break;
            case ABILITY_P2_ID:
                game.abilities[abilityIndex].status = action;
                useAbilityP2(action);
                break;
            case ABILITY_P3_ID:
                game.abilities[abilityIndex].status = action;
                useAbilityP3(action);
                break;
            case ABILITY_P4_ID:
                game.abilities[abilityIndex].status = action;
                useAbilityP4(action);
                break;
            }
        }
    }
}

VOID useAbilityP1(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        game.player->speed = INCREASED_BY_PERCENTS(game.player->speed, ABILITY_P1_STRENGTH);
        break;
    case ABILITY_DEACTIVATE:
        game.player->speed = DECREASED_BY_PERCENTS(game.player->speed, ABILITY_P1_STRENGTH);
        break;
    }
}

VOID useAbilityP2(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        game.maxLifes = INCREASED_BY_VALUE(game.maxLifes, ABILITY_P2_STRENGTH);
        game.lifes = game.maxLifes;
        break;
    case ABILITY_DEACTIVATE:
        game.maxLifes = DECREASED_BY_VALUE(game.maxLifes, ABILITY_P2_STRENGTH);
        game.lifes = game.maxLifes;
        break;
    }
}

VOID useAbilityP3(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        resizeObject(
            (Object *)game.player,
            INCREASED_BY_PERCENTS(game.player->width, ABILITY_P3_STRENGTH),
            INCREASED_BY_PERCENTS(game.player->height, ABILITY_P3_STRENGTH)
        );
        break;
    case ABILITY_DEACTIVATE:
        resizeObject(
            (Object *)game.player,
            DECREASED_BY_PERCENTS(game.player->width, ABILITY_P3_STRENGTH),
            DECREASED_BY_PERCENTS(game.player->width, ABILITY_P3_STRENGTH)
        );
        break;
    }
}

VOID useAbilityP4(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        game.maxResources = INCREASED_BY_VALUE(game.maxResources, ABILITY_P4_STRENGTH);
        break;
    case ABILITY_DEACTIVATE:
        game.maxResources = DECREASED_BY_VALUE(game.maxResources, ABILITY_P4_STRENGTH);
        break;
    }
}

VOID useAbilityA1(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        resizeObject(
            (Object *)game.player,
            INCREASED_BY_PERCENTS(game.player->width, ABILITY_A1_STRENGTH),
            INCREASED_BY_PERCENTS(game.player->height, ABILITY_A1_STRENGTH)
        );
        SetTimer(
            hGameWnd,
            ABILITY_SLOT_A1_TIMER_ID,
            ABILITY_A1_TIME,
            (TIMERPROC)NULL
        );
        break;
    case ABILITY_DEACTIVATE:
        resizeObject(
            (Object *)game.player,
            DECREASED_BY_PERCENTS(game.player->width, ABILITY_A1_STRENGTH),
            DECREASED_BY_PERCENTS(game.player->height, ABILITY_A1_STRENGTH)
        );
        break;
    }
}

VOID useAbilityA2(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        game.player->speed = INCREASED_BY_PERCENTS(game.player->speed, ABILITY_A2_STRENGTH);
        SetTimer(
            hGameWnd,
            ABILITY_SLOT_A2_TIMER_ID,
            ABILITY_A2_TIME,
            (TIMERPROC)NULL
        );
        break;
    case ABILITY_DEACTIVATE:
        game.player->speed = DECREASED_BY_PERCENTS(game.player->speed, ABILITY_A2_STRENGTH);
        break;
    }
}

VOID useAbilityA3(INT action) {
    for (int i = 0; i < game.giftCount; ++i) {
        if (game.gifts[i]->coordinates.Y > -game.gifts[0]->height) {
            game.score += game.gifts[i]->price;
            respawnResource(game.gifts[i]);
        }
    }
}

VOID useAbilityA4(INT action) {
    game.lifes = INCREASED_BY_VALUE(game.lifes, ABILITY_A4_STRENGTH);
    if (game.lifes > game.maxLifes) {
        game.lifes = game.maxLifes;
    }
}

VOID useAbilityA5(INT action){
    switch (action) {
    case ABILITY_ACTIVATE:
        game.difficultyMultiplierScore = INCREASED_BY_PERCENTS(
            game.difficultyMultiplierScore,
            ABILITY_A5_STRENGTH
        );
        SetTimer(
            hGameWnd,
            ABILITY_SLOT_A2_TIMER_ID,
            ABILITY_A5_TIME,
            (TIMERPROC)NULL
        );
        break;
    case ABILITY_DEACTIVATE:
        game.difficultyMultiplierScore = DECREASED_BY_PERCENTS(
        game.difficultyMultiplierScore,
        ABILITY_A5_STRENGTH
        );
        break;
    }
}

VOID useAbilityA6(INT action) {
    game.gravity = DECREASED_BY_PERCENTS(game.gravity, ABILITY_A6_STRENGTH);
}

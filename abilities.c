#include "abilities.h"

VOID useAbilityP1(INT action);
VOID useAbilityP2(INT action);
VOID useAbilityP3(INT action);
VOID useAbilityP4(INT action);
VOID useAbilityA1(INT action);
VOID useAbilityA2(INT action);
VOID useAbilityA3(INT action);
VOID useAbilityA4(INT action);
VOID useAbilityA5(INT action);
VOID useAbilityA6(INT action);

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
        game.player->speed *= 1.2;
        break;
    case ABILITY_DEACTIVATE:
        game.player->speed /= 1.2;
        break;
    }
}

VOID useAbilityP2(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        game.maxLifes += 2;
        game.lifes = game.maxLifes;
        break;
    case ABILITY_DEACTIVATE:
        game.maxLifes -= 2;
        game.lifes = game.maxLifes;
        break;
    }
}

VOID useAbilityP3(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        resizeObject((Object *)game.player, game.player->width * 1.2, game.player->height * 1.2);
        break;
    case ABILITY_DEACTIVATE:
        resizeObject((Object *)game.player, game.player->width / 1.2, game.player->height / 1.2);
        break;
    }
}

VOID useAbilityP4(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        game.maxResources += 1;
        break;
    case ABILITY_DEACTIVATE:
        game.maxResources -= 1;
        break;
    }
}

VOID useAbilityA1(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        resizeObject((Object *)game.player, game.player->width * 2, game.player->height * 2);
        SetTimer(
            hGameWnd,
            ABILITY_SLOT_A1_TIMER_ID,
            ABILITY_A1_TIMER,
            (TIMERPROC)NULL
        );
        break;
    case ABILITY_DEACTIVATE:
        resizeObject((Object *)game.player, game.player->width / 2, game.player->height / 2);
        break;
    }
}

VOID useAbilityA2(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        game.player->speed *= 2;
        SetTimer(
            hGameWnd,
            ABILITY_SLOT_A2_TIMER_ID,
            ABILITY_A2_TIMER,
            (TIMERPROC)NULL
        );
        break;
    case ABILITY_DEACTIVATE:
        game.player->speed /= 2;
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
    game.lifes += 3;
    if (game.lifes > game.maxLifes) {
        game.lifes = game.maxLifes;
    }
}

VOID useAbilityA5(INT action) {
    switch (action) {
    case ABILITY_ACTIVATE:
        game.difficultyMultiplierScore *= 2;
        SetTimer(
            hGameWnd,
            ABILITY_SLOT_A2_TIMER_ID,
            ABILITY_A5_TIMER,
            (TIMERPROC)NULL
        );
        break;
    case ABILITY_DEACTIVATE:
        game.difficultyMultiplierScore /= 2;
        break;
    }
}

VOID useAbilityA6(INT action) {
    game.gravity *= 0.9;
}

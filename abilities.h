#pragma once
#include "game.h"
#include "objects.h"
#include "variables.h"

#define ABILITY_ACTIVATE        0x1
#define ABILITY_DEACTIVATE      0x2
#define ABILITY_ENABLED         ABILITY_ACTIVATE
#define ABILITY_DISABLED        ABILITY_DEACTIVATE

VOID useAbility(UINT abilityIndex, INT action);

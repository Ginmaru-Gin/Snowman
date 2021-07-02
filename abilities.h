#pragma once

#include <Windows.h>

enum AbilityIndex {
	ACTIVE_ABILITY_1,
	ACTIVE_ABILITY_2,
	ACTIVE_ABILITY_3,
	PASSIVE_ABILITY_1,
	PASSIVE_ABILITY_2
};

typedef struct Ability {
	UINT ID;
	UINT status;
} Ability;


// COMMANDS FOR (DE)ACTIVATING ABILITIES
#define ABILITY_ACTIVATE        0x1
#define ABILITY_DEACTIVATE      0x2
// ABILITIES STATUSES
#define ABILITY_ENABLED         ABILITY_ACTIVATE
#define ABILITY_DISABLED        ABILITY_DEACTIVATE

VOID useAbility(UINT abilityIndex, INT action);

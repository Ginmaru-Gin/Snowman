#pragma once

#include "abilities.h"

#include <malloc.h>
#include <stdio.h>
#include <Windows.h>

typedef struct Coordinate {
	INT X, Y;
} Coordinate;

typedef struct GameField {
	INT width;
	INT height;
	DOUBLE screenMultiplier;
} GameField;

typedef struct Object {
	UINT speed;
	Coordinate coordinates;
	INT width;
	INT height;
} Object; // abstract

typedef struct Enviroment {
	Object;
} Enviroment;

typedef struct Character {
	Object;
} Character;

typedef struct Faller {
	Object;

	INT price;
	INT rarity;
} Faller; // abstract

typedef struct Resource {
	Faller;

} Resource;

typedef struct Game {
	GameField *gameField;
	Enviroment *ground;
	Character *player;

	Resource **gifts;
	UINT giftCount;
	Resource abilityResources[3];
	UINT abilityResourceCounters[3];
	Resource lifeResource;

	Ability abilities[5];
	UINT maxResources;
	UINT maxLifes;
	UINT lifes;
	DOUBLE gravity;
	UINT score;
	UINT difficultyMultiplierScore;

	DWORD status;
} Game;

VOID initObject(
	Object *object,
	Coordinate coordinates,
	UINT speed,
	UINT width,
	UINT height
);

VOID initFaller(
	Faller *faller,
	Coordinate coordinates,
	UINT speed,
	UINT width,
	UINT height,
	INT price,
	INT rarity
);

GameField *newGameField(
	UINT width,
    UINT height,
	DOUBLE screenMultiplier
);

Enviroment *newEnviroment(
	Coordinate coordinates,
	UINT width,
	UINT height
);

Character *newCharacter(
	Coordinate coordinates,
	UINT speed,
	UINT width,
	UINT height
);

Resource *newGift(
	Coordinate coordinates,
	UINT speed,
	INT price,
	INT rarity,
	UINT width,
	UINT height
);

VOID deleteGameField(GameField *gameField);
VOID deleteEnviroment(Enviroment *enviroment);
VOID deleteCharacter(Character *character);
VOID deleteGift(Resource *gift);
VOID deleteGame(Game *game);

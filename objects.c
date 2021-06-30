#include "objects.h"

VOID initObject(
	Object *object,
	Coordinate coordinates,
	UINT speed,
	UINT width,
	UINT height
) {
	object->coordinates = coordinates;
	object->speed = speed;
	object->width = width;
	object->height = height;
}

VOID initFaller(
	Faller *faller,
	Coordinate coordinates,
	UINT speed,
	UINT width,
	UINT height,
	INT price,
	INT rarity
) {
	initObject((Object*)faller,
		coordinates,
		speed,
		width,
		height);
	faller->price = price;
	faller->rarity = rarity;
}

GameField *newGameField(
	UINT width,
	UINT height,
	DOUBLE screenMultiplier
) {
	GameField *gameField = (GameField *)malloc(sizeof(GameField));
	if (!gameField) return NULL;

	gameField->width = width;
	gameField->height = height;
	gameField->screenMultiplier = screenMultiplier;

	return gameField;
}

Enviroment *newEnviroment(
	Coordinate coordinates,
	UINT width,
	UINT height
) {
	Enviroment *enviroment = (Enviroment *)malloc(sizeof(Enviroment));
	if (!enviroment) return NULL;

	initObject((Object *)enviroment,
		coordinates,
		0,
		width,
		height);

	return enviroment;
}

Character *newCharacter(
	Coordinate coordinates,
	UINT speed,
	UINT width,
	UINT height
) {
	Character *character = (Character *)malloc(sizeof(Character));
	if (!character) return NULL;

	initObject((Object *)character,
		coordinates,
		speed,
		width,
		height);

	return character;
}

Resource *newGift(
	Coordinate coordinates,
	UINT speed,
	INT rarity,
	INT price,
	UINT width,
	UINT height
) {
	Resource *gift = (Resource *)malloc(sizeof(Resource));
	if (!gift) return NULL;

	initFaller(
		(Faller *)gift,
		coordinates,
		speed,
		width,
		height,
		price,
		rarity);

	return gift;
}

VOID deleteGameField(GameField *gameField) {
	if (gameField) free(gameField);
}

VOID deleteEnviroment(Enviroment *enviroment) {
	if (enviroment) free(enviroment);
}

VOID deleteCharacter(Character *character) {
	if (character) free(character);
}

VOID deleteGift(Resource *gift) {
	if (gift) free(gift);
}

VOID deleteGame(Game *game) {
	deleteGameField(game->gameField);
	deleteCharacter(game->player);
	deleteEnviroment(game->ground);
	for (int i = 0; i < game->giftCount; ++i)
		deleteGift(game->gifts[i]);
	free(game->gifts);
	if (game) free(game);
}

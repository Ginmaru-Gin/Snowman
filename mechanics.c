#include "mechanics.h"


BOOL collision(Object *first, Object *second) {
	return (first->coordinates.Y  <= second->coordinates.Y + second->height) &&
		   (second->coordinates.X <= first->coordinates.X + first->width) &&
		   (first->coordinates.X  <= second->coordinates.X + second->width) &&
		   (second->coordinates.Y <= first->coordinates.Y + first->height);
}

INT move(Object* object, Direction direction) {
	int result = 0;
	switch (direction) {
	case LEFT:
		if (object->coordinates.X > object->speed) {
			object->coordinates.X -= object->speed;
		}
		else object->coordinates.X = 0;
        result = 1;
		break;
	case RIGHT:
		if (object->coordinates.X <= game.gameField->width - object->width - object->speed) {
			object->coordinates.X += object->speed;
		}
		else object->coordinates.X = game.gameField->width - object->width;
        result = 1;
		break;
	case FALLING:
		if ((object->coordinates.Y + object->height - 1) < game.gameField->height) {
			object->coordinates.Y += object->speed * game.gravity;
			result = 1;
		}
		break;
	}
	return result;
}

VOID increaseGravity(DOUBLE value) {
	if (game.status == GAME_IS_STARTED)
        game.gravity += value;
}

VOID resizeObject(Object *object, INT width, INT height) {
	object->coordinates.Y += object->height - height;
	object->coordinates.X += (object->width - width) / 2;
	object->width = width;
	object->height = height;
	if (object->coordinates.X < 0) {
		object->coordinates.X = 0;
	}
	if (object->coordinates.X + object->width > game.gameField->width) {
		object->coordinates.X = game.gameField->width - object->width;
	}
}


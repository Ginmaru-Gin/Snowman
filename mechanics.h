#pragma once

#include "objects.h"
#include "variables.h"

#include <Windows.h>

typedef enum Direction{
    LEFT,
    RIGHT,
    UP,
    FALLING
} Direction;

BOOL collision(Object* first, Object* second);

INT move(Object* object, Direction direction);

VOID increaseGravity(DOUBLE value);

VOID resizeObject(Object *object, INT width, INT height);


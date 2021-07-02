#pragma once

#include "resources.h"

#include <Windows.h>

typedef struct Skin {
	UINT ID;
	HBITMAP gameBackground;
	HBITMAP paneBackground;
	HBITMAP character;
	HBITMAP gift;
	HBITMAP resourceLife;
	HBITMAP abilityResources[3];
	UINT gameMusic;
	UINT menuMusic;
} Skin;

VOID loadSkin(UINT ID);

VOID resizeObjectsSizeBasedOnSkin();

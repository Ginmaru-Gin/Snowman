#pragma once

#include "resource.h"

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

typedef struct SkinsList {
	LPWSTR *names;
	UINT amount;
} SkinsList;

VOID loadDefaultSkin(VOID);

VOID loadCustomSkin(UINT ID);

VOID loadSkin(UINT ID);

VOID resizeObjectsSizeBasedOnSkin();

VOID loadSkinsList(VOID);

UINT getSkinID(LPCWSTR skinName);

HBITMAP loadBmpFromFile(LPCWSTR dir, LPCWSTR fileName);

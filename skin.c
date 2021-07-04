#include "skin.h"
#include "variables.h"

VOID loadDefaultSkin(VOID) {
    skin.gameBackground = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_BKG_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    skin.paneBackground = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_PANE_BKG_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    skin.character = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_SNOWMAN_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    skin.gift = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE_GIFT_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    skin.abilityResources[0] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE1_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    skin.abilityResources[1] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE2_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    skin.abilityResources[2] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE3_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    skin.resourceLife = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_SNOWMAN_RESOURCE_LIFE_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
    skin.gameMusic = WAVE_SNOWMAN_GAME_MUSIC;
    skin.menuMusic = WAVE_SNOWMAN_MENU_MUSIC;
}

VOID loadCustomSkin(UINT ID) {

    LPWSTR skinDir = (LPWSTR)calloc(sizeof(WCHAR), (ULONGLONG)lstrlenW(SKINS_DIR) + (ULONGLONG)lstrlenW(skinsList.names[ID]) + 5ULL);
    if (skinDir) {
        lstrcpyW(skinDir, SKINS_DIR);
        lstrcatW(skinDir, L"\\");
        lstrcatW(skinDir, skinsList.names[ID]);
        lstrcatW(skinDir, L"\\");
    }

    skin.gameBackground = loadBmpFromFile(skinDir, SKIN_FILE_GAME_BKG);
    skin.paneBackground = loadBmpFromFile(skinDir, SKIN_FILE_PANE_BKG);
    skin.character = loadBmpFromFile(skinDir, SKIN_FILE_CHARACTER);
    skin.gift = loadBmpFromFile(skinDir, SKIN_FILE_RESOURCE_GIFT);
    skin.abilityResources[0] = loadBmpFromFile(skinDir, SKIN_FILE_RESOURCE_1);
    skin.abilityResources[1] = loadBmpFromFile(skinDir, SKIN_FILE_RESOURCE_2);
    skin.abilityResources[2] = loadBmpFromFile(skinDir, SKIN_FILE_RESOURCE_3);
    skin.resourceLife = loadBmpFromFile(skinDir, SKIN_FILE_RESOURCE_LIFE);
}

VOID loadSkin(UINT ID) {
    if (ID == skin.ID) return;

    if (ID == SKIN_DEFAULT_ID) {
        loadDefaultSkin();
    }
    else {
        loadCustomSkin(ID);
    }
    skin.ID = ID;

    resizeObjectsSizeBasedOnSkin();
    InvalidateRect(hCharacterListPaneWnd, NULL, TRUE);
    playMusic(skin.menuMusic);
}

VOID resizeObjectsSizeBasedOnSkin() {
    BITMAP bmp;
    DOUBLE k;

    GetObject(skin.character, sizeof(bmp), &bmp);
    k = bmp.bmWidth > bmp.bmHeight ? (DOUBLE)MAX_SNOWMAN_WIDTH / bmp.bmWidth : (DOUBLE)MAX_SNOWMAN_HEIGHT / bmp.bmHeight;
    game.player->width = bmp.bmWidth * k;
    game.player->height = bmp.bmHeight * k;

    GetObject(skin.resourceLife, sizeof(bmp), &bmp);
    k = bmp.bmWidth > bmp.bmHeight ? (DOUBLE)MAX_FALLER_WIDTH / bmp.bmWidth : (DOUBLE)MAX_FALLER_HEIGHT / bmp.bmHeight;
    game.lifeResource.width = bmp.bmWidth * k;
    game.lifeResource.height = bmp.bmHeight * k;

    GetObject(skin.gift, sizeof(bmp), &bmp);
    k = bmp.bmWidth > bmp.bmHeight ? (DOUBLE)MAX_FALLER_WIDTH / bmp.bmWidth : (DOUBLE)MAX_FALLER_HEIGHT / bmp.bmHeight;
    for (INT i = 0; i < game.giftCount; ++i) {
    game.gifts[i]->width = bmp.bmWidth * k;
    game.gifts[i]->height = bmp.bmHeight * k;
    }

    for (INT i = 0; i < ACTIVE_ABILITY_COUNT; ++i) {
        GetObject(skin.abilityResources[i], sizeof(bmp), &bmp);
        k = bmp.bmWidth > bmp.bmHeight ? (DOUBLE)MAX_FALLER_WIDTH / bmp.bmWidth : (DOUBLE)MAX_FALLER_HEIGHT / bmp.bmHeight;
        game.abilityResources[i].width = bmp.bmWidth * k;
        game.abilityResources[i].height = bmp.bmHeight * k;
    }
}

VOID loadSkinsList(VOID) {
    WIN32_FIND_DATAW findData;
    HANDLE hFind;

    LPWSTR skinsDir = (LPWSTR)calloc(sizeof(WCHAR), lstrlenW(SKINS_DIR) + 3);
    if (skinsDir) {
        lstrcpyW(skinsDir, SKINS_DIR);
        lstrcatW(skinsDir, L"\\*");


        skinsList.amount = 1;   // default skin exists always
        hFind = FindFirstFileW(skinsDir, &findData);
        FindNextFileW(hFind, &findData);    // skip . dir
        while (FindNextFileW(hFind, &findData)) {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                ++skinsList.amount;
            }
        }
        FindClose(hFind);

        if (skinsList.names = (LPWSTR *)malloc(sizeof(LPWSTR) * ((ULONGLONG)skinsList.amount + 2ULL))) {
            skinsList.names[1] = L"default";
            hFind = FindFirstFileW(skinsDir, &findData);

            LPWSTR *ptr = skinsList.names + 2;

            FindNextFileW(hFind, &findData);
            while (FindNextFileW(hFind, &findData) && ptr < skinsList.names + skinsList.amount + 1) {
                if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    size_t nameLength = lstrlenW(findData.cFileName);
                    if (*ptr = (LPWSTR)malloc(sizeof(WCHAR) * (nameLength + 1))) {
                        lstrcpyW(*ptr, findData.cFileName);
                        ++ptr;
                    }
                }
            }
            FindClose(hFind);
        }
    }
}

UINT getSkinID(LPCWSTR skinName) {
    UINT index = SKIN_DEFAULT_ID;
    while (lstrcmpW(skinName, skinsList.names[index])) {
        ++index;
    }
    return index;
}

HBITMAP loadBmpFromFile(LPCWSTR dir, LPCWSTR fileName) {
    WCHAR strBuff[MAX_PATH + 1];
    lstrcpyW(strBuff, dir);
    lstrcatW(strBuff, fileName);
    return LoadImageW(hInstance, strBuff, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

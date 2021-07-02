#include "skin.h"
#include "variables.h"

VOID loadSkin(UINT ID) {
    if (ID == skin.ID) return;
    switch (ID) {
    case SKIN_SNOWMAN:
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
        break;
    case SKIN_EVANGELION:
        skin.gameBackground = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_BKG_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.paneBackground = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_PANE_BKG_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.character = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_SNOWMAN_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.gift = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE_GIFT_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.abilityResources[0] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE1_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.abilityResources[1] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE2_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.abilityResources[2] = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE3_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.resourceLife = LoadImageW(hInstance, MAKEINTRESOURCEW(IDB_EVANGELION_RESOURCE_LIFE_BMP), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
        skin.gameMusic = WAVE_EVANGELION_GAME_MUSIC;
        skin.menuMusic = WAVE_EVANGELION_MENU_MUSIC;
        break;
    }

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
    for (int i = 0; i < game.giftCount; ++i) {
    game.gifts[i]->width = bmp.bmWidth * k;
    game.gifts[i]->height = bmp.bmHeight * k;
    }

    for (int i = 0; i < ACTIVE_ABILITY_COUNT; ++i) {
        GetObject(skin.abilityResources[i], sizeof(bmp), &bmp);
        k = bmp.bmWidth > bmp.bmHeight ? (DOUBLE)MAX_FALLER_WIDTH / bmp.bmWidth : (DOUBLE)MAX_FALLER_HEIGHT / bmp.bmHeight;
        game.abilityResources[i].width = bmp.bmWidth * k;
        game.abilityResources[i].height = bmp.bmHeight * k;
    }
}
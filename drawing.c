#include "drawing.h"

VOID renderGameFrame(HDC hdcRenderer, size_t frameWidth, size_t frameHeight) {
    HBITMAP defaultMemoryBmp, frameBmp;
    HDC hdcMemory;
    BITMAP textureBitmap;
    size_t textureWidth, textureHeight;

    hdcMemory = CreateCompatibleDC(hdcRenderer);
    frameBmp = CreateCompatibleBitmap(hdcMemory, frameWidth, frameHeight);
    defaultMemoryBmp = (HBITMAP)SelectObject(hdcMemory, frameBmp);

    SelectObject(hdcMemory, skin.gameBackground);
    GetObject(skin.gameBackground, sizeof(textureBitmap), &textureBitmap);
    StretchBlt(hdcRenderer,
        0,
        0,
        frameWidth,
        frameHeight,
        hdcMemory,
        0,
        0,
        textureBitmap.bmWidth,
        textureBitmap.bmHeight,
        SRCCOPY);

    SelectObject(hdcMemory, skin.character);
    GetObject(skin.character, sizeof(textureBitmap), &textureBitmap);
    TransparentBlt(hdcRenderer,
        game.player->coordinates.X * game.gameField->screenMultiplier,
        game.player->coordinates.Y * game.gameField->screenMultiplier,
        game.player->width * game.gameField->screenMultiplier,
        game.player->height * game.gameField->screenMultiplier,
        hdcMemory,
        0,
        0,
        textureBitmap.bmWidth,
        textureBitmap.bmHeight,
        GetPixel(hdcMemory, 0, 0));

    SelectObject(hdcMemory, skin.gift);
    GetObject(skin.gift, sizeof(textureBitmap), &textureBitmap);
    for (size_t i = 0; i < game.giftCount; ++i) {
        TransparentBlt(hdcRenderer,
            game.gifts[i]->coordinates.X * game.gameField->screenMultiplier,
            game.gifts[i]->coordinates.Y * game.gameField->screenMultiplier,
            game.gifts[i]->width * game.gameField->screenMultiplier,
            game.gifts[i]->height * game.gameField->screenMultiplier,
            hdcMemory,
            0,
            0,
            textureBitmap.bmWidth,
            textureBitmap.bmHeight,
            GetPixel(hdcMemory, 0, 0));
    }

    for (size_t i = 0; i < ACTIVE_ABILITY_COUNT; ++i) {
        SelectObject(hdcMemory, skin.abilityResources[i]);
        GetObject(skin.abilityResources[i], sizeof(textureBitmap), &textureBitmap);
        TransparentBlt(hdcRenderer,
            game.abilityResources[i].coordinates.X * game.gameField->screenMultiplier,
            game.abilityResources[i].coordinates.Y * game.gameField->screenMultiplier,
            game.abilityResources[i].width * game.gameField->screenMultiplier,
            game.abilityResources[i].height * game.gameField->screenMultiplier,
            hdcMemory,
            0,
            0,
            textureBitmap.bmWidth,
            textureBitmap.bmHeight,
            GetPixel(hdcMemory, 0, 0));
    }

    SelectObject(hdcMemory, skin.resourceLife);
    GetObject(skin.resourceLife, sizeof(textureBitmap), &textureBitmap);
    TransparentBlt(hdcRenderer,
        game.lifeResource.coordinates.X * game.gameField->screenMultiplier,
        game.lifeResource.coordinates.Y * game.gameField->screenMultiplier,
        game.lifeResource.width * game.gameField->screenMultiplier,
        game.lifeResource.height * game.gameField->screenMultiplier,
        hdcMemory,
        0,
        0,
        textureBitmap.bmWidth,
        textureBitmap.bmHeight,
        GetPixel(hdcMemory, 0, 0));
    
    DeleteObject(frameBmp);
    SelectObject(hdcMemory, defaultMemoryBmp);
    DeleteDC(hdcMemory);
}

VOID renderPaneFrame(HDC hdcRenderer, size_t frameWidth, size_t frameHeight) {
    HBITMAP defaultMemoryBmp, frameBmp;
    HDC hdcMemory;
    BITMAP textureBitmap;
    size_t textureWidth, textureHeight;

    hdcMemory = CreateCompatibleDC(hdcRenderer);
    frameBmp = CreateCompatibleBitmap(hdcMemory, frameWidth, frameHeight);
    defaultMemoryBmp = (HBITMAP)SelectObject(hdcMemory, frameBmp);

    SelectObject(hdcMemory, skin.paneBackground);
    GetObject(skin.paneBackground, sizeof(textureBitmap), &textureBitmap);
    StretchBlt(
        hdcRenderer,
        0,
        0,
        frameWidth,
        frameHeight,
        hdcMemory,
        0,
        0,
        textureBitmap.bmWidth,
        textureBitmap.bmHeight,
        SRCCOPY);

    if (activePane == hGameInfoPaneWnd) {
        INT width, height;
        DOUBLE widthK, heightK, k;

        widthK = (DOUBLE)widthPaneRow / game.maxLifes / game.lifeResource.width;
        heightK = (DOUBLE)heightPaneRow / game.lifeResource.height;
        k = min(widthK, heightK);
        SelectObject(hdcMemory, skin.resourceLife);
        GetObject(skin.resourceLife, sizeof(textureBitmap), &textureBitmap);
        width = game.lifeResource.width * k;
        height = game.lifeResource.height * k;
        for (int i = 0; i < game.lifes; ++i) {
            TransparentBlt(
                hdcRenderer,
                LEFT_PANE_FIELD + i * width,
                heightPaneRow,
                width,
                height,
                hdcMemory,
                0,
                0,
                textureBitmap.bmWidth,
                textureBitmap.bmHeight,
                GetPixel(hdcMemory, 0, 0));
        }

        widthK = (DOUBLE)widthPaneRow / game.maxResources / game.abilityResources[ACTIVE_ABILITY_1].width;
        heightK = (DOUBLE)heightPaneRow / game.abilityResources[ACTIVE_ABILITY_1].height;
        k = min(widthK, heightK);
        SelectObject(hdcMemory, skin.abilityResources[ACTIVE_ABILITY_1]);
        GetObject(skin.abilityResources[ACTIVE_ABILITY_1], sizeof(textureBitmap), &textureBitmap);
        width = game.abilityResources[ACTIVE_ABILITY_1].width * k;
        height = game.abilityResources[ACTIVE_ABILITY_1].height * k;
        for (int i = 0; i < game.abilityResourceCounters[ACTIVE_ABILITY_1]; ++i) {
            TransparentBlt(
                hdcRenderer,
                LEFT_PANE_FIELD + i * width,
                heightPaneRow * 2,
                width,
                height,
                hdcMemory,
                0,
                0,
                textureBitmap.bmWidth,
                textureBitmap.bmHeight,
                GetPixel(hdcMemory, 0, 0));
        }

        widthK = (DOUBLE)widthPaneRow / game.maxResources / game.abilityResources[ACTIVE_ABILITY_2].width;
        heightK = (DOUBLE)heightPaneRow / game.abilityResources[ACTIVE_ABILITY_2].height;
        k = min(widthK, heightK);
        SelectObject(hdcMemory, skin.abilityResources[ACTIVE_ABILITY_2]);
        GetObject(skin.abilityResources[ACTIVE_ABILITY_2], sizeof(textureBitmap), &textureBitmap);
        width = game.abilityResources[ACTIVE_ABILITY_2].width * k;
        height = game.abilityResources[ACTIVE_ABILITY_2].height * k;
        for (int i = 0; i < game.abilityResourceCounters[ACTIVE_ABILITY_2]; ++i) {
            TransparentBlt(
                hdcRenderer,
                LEFT_PANE_FIELD + i * width,
                heightPaneRow * 3,
                width,
                height,
                hdcMemory,
                0,
                0,
                textureBitmap.bmWidth,
                textureBitmap.bmHeight,
                GetPixel(hdcMemory, 0, 0));
        }

        widthK = (DOUBLE)widthPaneRow / game.maxResources / game.abilityResources[ACTIVE_ABILITY_3].width;
        heightK = (DOUBLE)heightPaneRow / game.abilityResources[ACTIVE_ABILITY_3].height;
        k = min(widthK, heightK);
        SelectObject(hdcMemory, skin.abilityResources[ACTIVE_ABILITY_3]);
        GetObject(skin.abilityResources[ACTIVE_ABILITY_3], sizeof(textureBitmap), &textureBitmap);
        width = game.abilityResources[ACTIVE_ABILITY_3].width * k;
        height = game.abilityResources[ACTIVE_ABILITY_3].height * k;
        for (int i = 0; i < game.abilityResourceCounters[ACTIVE_ABILITY_3]; ++i) {
            TransparentBlt(
                hdcRenderer,
                LEFT_PANE_FIELD + i * width,
                heightPaneRow * 4,
                width,
                height,
                hdcMemory,
                0,
                0,
                textureBitmap.bmWidth,
                textureBitmap.bmHeight,
                GetPixel(hdcMemory, 0, 0));
        }
    }

    DeleteObject(frameBmp);
    SelectObject(hdcMemory, defaultMemoryBmp);
    DeleteDC(hdcMemory);
}

VOID drawFrame(HDC hdcWindow, size_t widthGameWnd, size_t heightGameWnd, HDC hdcRenderer) {
    BITMAP frameBitmap;

    GetObject((HBITMAP)GetCurrentObject(hdcRenderer, OBJ_BITMAP), sizeof(frameBitmap), &frameBitmap);
    StretchBlt(hdcWindow, 0, 0, (int)widthGameWnd, (int)heightGameWnd, hdcRenderer, 0, 0, frameBitmap.bmWidth, frameBitmap.bmHeight, SRCCOPY);
}

VOID updateGameInfoPane(VOID) {
    InvalidateRect(hGameInfoPaneWnd, NULL, FALSE);
}

VOID updateScoreCounter(HDC hdc, PAINTSTRUCT *ps, HFONT hFont) {
    SelectObject(hdc, hFont);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, 0xFFFFFF);
    WCHAR wScore[MAX_SCORE_COUNTER_LENGTH];
    _ui64tow_s(game.score, wScore, MAX_SCORE_COUNTER_LENGTH, 10);
    //SetWindowTextW(hScoreCounterLabel, wScore);
    TextOutW(hdc, LEFT_PANE_FIELD, RIGHT_PANE_FIELD, wScore, lstrlenW(wScore));
}

VOID switchPane(HWND source, HWND destination) {
    ShowWindow(source, SW_HIDE);
    ShowWindow(destination, SW_SHOW);
    activePane = destination;
    SetFocus(destination);
    updateGameInfoPane();
}


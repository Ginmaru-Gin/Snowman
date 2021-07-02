#include <Windows.h>
#include <time.h>

#include "abilities.h"
#include "drawing.h"
#include "game.h"
#include "mechanics.h"
#include "records.h"
#include "variables.h"

VOID registerClasses(VOID);
VOID createWindows(VOID);
LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK gameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK paneWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

INT WINAPI wWinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {

    DWORD errorCode = 0;
    srand(time(0));
    hInstance = hInst;

    MSG msg;
    registerClasses();
    createWindows();

    if (hMainWnd && hGameWnd && hPaneWnd) {
        ShowWindow(hMainWnd, nCmdShow);
        ShowWindow(hPaneWnd, SW_SHOW);
        GetWindowRect(hPaneWnd, &clipCursorRect);
        SetFocus(hMainWnd);

        while (GetMessageW(&msg, 0, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
    else errorCode = GetLastError();

    return errorCode;
}

LRESULT CALLBACK mainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    switch (msg) {

    case WM_SETFOCUS:

        ClipCursor(&clipCursorRect);
        SetFocus(activePane);
        break;

    case WM_CREATE:
        loadRecordList();
        break;

    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default: return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK gameWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hdcWindow, hdcRenderer;
    HBITMAP defaultRendererBmp, rendererFrame;
    RECT rectWnd; 

    if (game.status & ~STARTED && (game.status == GAME_IS_PAUSED)) return DefWindowProc(hwnd, msg, wParam, lParam);

    switch (msg) {

    case WM_PAINT:

        hdcWindow = BeginPaint(hwnd, &ps);

        hdcRenderer = CreateCompatibleDC(hdcWindow);
        rendererFrame = CreateCompatibleBitmap(hdcWindow, widthGameWnd, heightGameWnd);
        defaultRendererBmp = SelectObject(hdcRenderer, rendererFrame);

        renderGameFrame(hdcRenderer, widthGameWnd, heightGameWnd);
        drawFrame(hdcWindow, widthGameWnd, heightGameWnd, hdcRenderer);

        SelectObject(hdcRenderer, defaultRendererBmp);
        DeleteDC(hdcRenderer);
        DeleteObject(rendererFrame);

        EndPaint(hwnd, &ps);
        break;

    case WM_TIMER:

        switch ((UINT)wParam) {

        case DRAWING_TIMER_ID:
            InvalidateRect(hwnd, NULL, FALSE);
            break;

        case GAME_PROC_TIMER_ID:
            gameProc();
            break;

        case INPUT_TIMER_ID:
            if (GetKeyState((WCHAR)'D') < 0 || GetKeyState(VK_RIGHT) < 0) {
                move((Object *)game.player, RIGHT);
            }
            if (GetKeyState((WCHAR)'A') < 0 || GetKeyState(VK_LEFT) < 0) {
                move((Object *)game.player, LEFT);
            }
            break;

        case GRAVITY_INCREASING_TIMER_ID:
            increaseGravity(gravityIncreasing);
            break;

        case ABILITY_SLOT_A1_TIMER_ID:
            useAbility(ACTIVE_ABILITY_1, ABILITY_DEACTIVATE);
            break;
        case ABILITY_SLOT_A2_TIMER_ID:
            useAbility(ACTIVE_ABILITY_2, ABILITY_DEACTIVATE);
            break;
        case ABILITY_SLOT_A3_TIMER_ID:
            useAbility(ACTIVE_ABILITY_3, ABILITY_DEACTIVATE);
            break;
        }
        break;

    case WM_KEYDOWN:

        switch ((WCHAR)wParam) {

        case VK_ESCAPE:

            pauseGame();
            switchPane(hGameInfoPaneWnd, hPausePaneWnd);
            break;

        case VK_NUMPAD1:
        case L'1':
            useAbility(ACTIVE_ABILITY_1, ABILITY_ACTIVATE);
            break;

        case VK_NUMPAD2:
        case L'2':
            useAbility(ACTIVE_ABILITY_2, ABILITY_ACTIVATE);
            break;

        case VK_NUMPAD3:
        case L'3':
            useAbility(ACTIVE_ABILITY_3, ABILITY_ACTIVATE);
            break;
        }

        break;

    case WM_CREATE:

        if (GetWindowRect(hwnd, &rectWnd)) {
            widthGameWnd = rectWnd.right - rectWnd.left;
            heightGameWnd = rectWnd.bottom - rectWnd.top;
        }

        initGame(
            GetModuleHandle(NULL),
            widthGameWnd,
            GIFT_COUNT,
            0,
            1,
            0,
            DEFAULT_ABILITY_P1_ID,
            DEFAULT_ABILITY_P2_ID,
            DEFAULT_ABILITY_A1_ID,
            DEFAULT_ABILITY_A2_ID,
            DEFAULT_ABILITY_A3_ID
        );
        game.status = GAME_IN_BACKGROUND_MODE;

        loadSkin(SKIN_DEFAULT);
        playMusic(skin.menuMusic);

        SetTimer(
            hwnd,
            GAME_PROC_TIMER_ID,
            GAME_TACT_TIMER,
            (TIMERPROC)NULL
        );

        SetTimer(
            hwnd,
            DRAWING_TIMER_ID,
            1000 / FPS,
            (TIMERPROC)NULL
        );

        SetTimer(
            hwnd,
            INPUT_TIMER_ID,
            INPUT_TIMER,
            (TIMERPROC)NULL
        );

        SetTimer(
            hwnd,
            GRAVITY_INCREASING_TIMER_ID,
            GRAVITY_INCREASING_TIMER,
            (TIMERPROC)NULL
        );
        break;


    case WM_CLOSE:
    case WM_DESTROY:
        //deleteGame(&game);
        KillTimer(hwnd, GAME_PROC_TIMER_ID);
        KillTimer(hwnd, DRAWING_TIMER_ID);
        KillTimer(hwnd, INPUT_TIMER_ID);
        KillTimer(hwnd, GRAVITY_INCREASING_TIMER_ID);

        break;

        default:
            return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK paneWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdcWindow, hdcRenderer;
    HBITMAP defaultRendererBmp, rendererFrame;
    RECT rectWnd;
    BITMAP bmp;
    static HFONT hFont;
    PWSTR bufferStr[100];
    Record record;

    static UINT abilitiesForNewGame[ACTIVE_ABILITY_COUNT + PASSIVE_ABILITY_COUNT] = {
        DEFAULT_ABILITY_A1_ID,
        DEFAULT_ABILITY_A2_ID,
        DEFAULT_ABILITY_A3_ID,
        DEFAULT_ABILITY_P1_ID,
        DEFAULT_ABILITY_P2_ID
    };
    UINT difficultyScoreMultiplierForNewGame = EASY_DIFFICULTY_SCORE_MULTIPLIER;
    DOUBLE gravityIncreasingForNewGame = EASY_DIFFICULTY_GRAVITY_INCREASING;
    UINT maxLivesForNewGame = EASY_DIFFICULTY_LIVES;
    UINT giftsCountForNewGame = EASY_GIFTS_COUNT;


    switch (msg) {

    case WM_PAINT:
        if (activePane != hGameInfoPaneWnd)
            break;
    case WM_ERASEBKGND:

        hdcWindow = BeginPaint(hwnd, &ps);

        hdcRenderer = CreateCompatibleDC(hdcWindow);
        rendererFrame = CreateCompatibleBitmap(hdcWindow, widthPaneWnd, heightPaneWnd);
        defaultRendererBmp = SelectObject(hdcRenderer, rendererFrame);

        renderPaneFrame(hdcRenderer, widthPaneWnd, heightPaneWnd);
        drawFrame(hdcWindow, widthPaneWnd, heightPaneWnd, hdcRenderer);
        if (activePane == hGameInfoPaneWnd) {
            updateScoreCounter(hdcWindow, &ps, hFont);
        }

        SelectObject(hdcRenderer, defaultRendererBmp);
        DeleteDC(hdcRenderer);
        DeleteObject(rendererFrame);

        EndPaint(hwnd, &ps);
        return 1;

    case WM_CREATE:
        if (GetWindowRect(hwnd, &rectWnd)) {
            widthPaneWnd = rectWnd.right - rectWnd.left;
            heightPaneWnd = rectWnd.bottom - rectWnd.top;
            widthPaneRow = widthPaneWnd - LEFT_PANE_FIELD - RIGHT_PANE_FIELD;
            heightPaneRow = heightPaneWnd / (PANE_ROWS_COUNT + 1);
        }
        hFont = CreateFontW(
            50, 20,
            0, 0,
            FW_MEDIUM, TRUE,
            FALSE, FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            FF_DONTCARE,
            L"SCORE FONT"
        );

        break;

    case WM_SETFOCUS:
        if (activePane == hGameInfoPaneWnd) SetFocus(hGameWnd);
        break;

    case WM_KEYDOWN:
        if (hwnd == hPausePaneWnd && (WCHAR)wParam == VK_ESCAPE) {
            switchPane(hPausePaneWnd, hGameInfoPaneWnd);
            continueGame();
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {

            // MAIN MENU PANE
        case EXIT_BTN_ID:
            DestroyWindow(hMainWnd);
            break;

        case NEW_GAME_BTN_ID:
            switchPane(hMenuPaneWnd, hCharacterListPaneWnd);
            break;

        case RECORD_LIST_BTN_ID:
            updateRecordList();
            switchPane(hMenuPaneWnd, hRecordListPaneWnd);
            break;

            // RECORD LIST PANE
        case BACK_FROM_RECORD_LIST_PANE_BTN_ID:
            switchPane(hRecordListPaneWnd, hMenuPaneWnd);
            break;

            // SKIN LIST PANE
        case APPLY_CHOOSE_CHARACTER_PANE_BTN_ID:
            switchPane(hCharacterListPaneWnd, hChooseAbilitiesPaneWnd);
            break;

        case BACK_FROM_CHARACTER_LIST_PANE_BTN_ID:
            switchPane(hCharacterListPaneWnd, hMenuPaneWnd);
            break;

            // CHOOSE SKIN
        case CHARACTER_SNOWMAN_BTN_ID:
            loadSkin(SKIN_SNOWMAN);
            break;
        case CHARACTER_EVANGELION_BTN_ID:
            loadSkin(SKIN_EVANGELION);
            break;

            // ABILITY LIST PANE
        case CHOOSE_ABILITY_P1_BTN_ID:
            switchPane(hwnd, hChooseAbilityP1PaneWnd);
            break;
        case CHOOSE_ABILITY_P2_BTN_ID:
            switchPane(hwnd, hChooseAbilityP2PaneWnd);
            break;
        case CHOOSE_ABILITY_A1_BTN_ID:
            switchPane(hwnd, hChooseAbilityA1PaneWnd);
            break;
        case CHOOSE_ABILITY_A2_BTN_ID:
            switchPane(hwnd, hChooseAbilityA2PaneWnd);
            break;
        case CHOOSE_ABILITY_A3_BTN_ID:
            switchPane(hwnd, hChooseAbilityA3PaneWnd);
            break;
        case BACK_FROM_CHOOSE_ABILITIES_PANE_BTN_ID:
            switchPane(hwnd, hCharacterListPaneWnd);
            break;
        case APPLY_CHOOSE_ABILITIES_PANE_BTN_ID:
            switchPane(hwnd, hChooseDifficultyPaneWnd);
            break;

            // CHOOSE ACTIVE ABILITY 1
        case CHOOSE_ACTIVE_ABILITY1_BTN_ID:
            abilitiesForNewGame[ACTIVE_ABILITY_1] = ABILITY_A1_ID;
            GetWindowTextW(hChooseAbilityA1Btn, (LPWSTR)bufferStr, 100);
            SetWindowTextW(hListActiveAbility1BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;
            // CHOOSE ACTIVE ABILITY 2
        case CHOOSE_ACTIVE_ABILITY2_BTN_ID:
            abilitiesForNewGame[ACTIVE_ABILITY_2] = ABILITY_A2_ID;
            GetWindowTextW(hChooseAbilityA2Btn, (PWSTR)bufferStr, 100);
            SetWindowTextW(hListActiveAbility2BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;
            // CHOOSE ACTIVE ABILITY 3
        case CHOOSE_ACTIVE_ABILITY3_BTN_ID:
            abilitiesForNewGame[ACTIVE_ABILITY_3] = ABILITY_A3_ID;
            GetWindowTextW(hChooseAbilityA3Btn, (PWSTR)bufferStr, 100);
            SetWindowTextW(hListActiveAbility3BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;
            // CHOOSE ACTIVE ABILITY 4
        case CHOOSE_ACTIVE_ABILITY4_BTN_ID:
            abilitiesForNewGame[ACTIVE_ABILITY_1] = ABILITY_A4_ID;
            GetWindowTextW(hChooseAbilityA4Btn, (PWSTR)bufferStr, 100);
            SetWindowTextW(hListActiveAbility1BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;
            // CHOOSE ACTIVE ABILITY 5
        case CHOOSE_ACTIVE_ABILITY5_BTN_ID:
            abilitiesForNewGame[ACTIVE_ABILITY_2] = ABILITY_A5_ID;
            GetWindowTextW(hChooseAbilityA5Btn, (PWSTR)bufferStr, 100);
            SetWindowTextW(hListActiveAbility2BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;
            // CHOOSE ACTIVE ABILITY 6
        case CHOOSE_ACTIVE_ABILITY6_BTN_ID:
            abilitiesForNewGame[ACTIVE_ABILITY_3] = ABILITY_A6_ID;
            GetWindowTextW(hChooseAbilityA6Btn, (PWSTR)bufferStr, 100);
            SetWindowTextW(hListActiveAbility3BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;
            // CHOOSE PASSIVE ABILITY 1
        case CHOOSE_PASSIVE_ABILITY1_BTN_ID:
            abilitiesForNewGame[PASSIVE_ABILITY_1] = ABILITY_P1_ID;
            GetWindowTextW(hChooseAbilityP1Btn, (PWSTR)bufferStr, 100);
            SetWindowTextW(hListPassiveAbility1BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;
            // CHOOSE PASSIVE ABILITY 2
        case CHOOSE_PASSIVE_ABILITY2_BTN_ID:
            abilitiesForNewGame[PASSIVE_ABILITY_2] = ABILITY_P2_ID;
            GetWindowTextW(hChooseAbilityP2Btn, (PWSTR)bufferStr, 100);
            SetWindowTextW(hListPassiveAbility2BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;
            // CHOOSE PASSIVE ABILITY 3
        case CHOOSE_PASSIVE_ABILITY3_BTN_ID:
            abilitiesForNewGame[PASSIVE_ABILITY_1] = ABILITY_P3_ID;
            GetWindowTextW(hChooseAbilityP3Btn, (PWSTR)bufferStr, 100);
            SetWindowTextW(hListPassiveAbility1BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;
            // CHOOSE PASSIVE ABILITY 4
        case CHOOSE_PASSIVE_ABILITY4_BTN_ID:
            abilitiesForNewGame[PASSIVE_ABILITY_2] = ABILITY_P4_ID;
            GetWindowTextW(hChooseAbilityP4Btn, (PWSTR)bufferStr, 100);
            SetWindowTextW(hListPassiveAbility2BtnWnd, (LPCWSTR)bufferStr);
            switchPane(hwnd, hChooseAbilitiesPaneWnd);
            break;

            // CHOOSE DIFFICULTY PANE
        case HARD_DIFFICULTY_BTN_ID:
            difficultyScoreMultiplierForNewGame += 1;
            gravityIncreasingForNewGame *= 2;
            giftsCountForNewGame += 2;
            maxLivesForNewGame -= 2;
        case NORMAL_DIFFICULTY_BTN_ID:
            difficultyScoreMultiplierForNewGame += 1;
            gravityIncreasingForNewGame *= 2;
            giftsCountForNewGame += 2;
            maxLivesForNewGame -= 2;
        case EASY_DIFFICULTY_BTN_ID:
            reinitGame(
                hInstance,
                widthGameWnd,
                giftsCountForNewGame,
                maxLivesForNewGame,
                1,
                difficultyScoreMultiplierForNewGame,
                abilitiesForNewGame[PASSIVE_ABILITY_1],
                abilitiesForNewGame[PASSIVE_ABILITY_2],
                abilitiesForNewGame[ACTIVE_ABILITY_1],
                abilitiesForNewGame[ACTIVE_ABILITY_2],
                abilitiesForNewGame[ACTIVE_ABILITY_3]
            );
            gravityIncreasing = gravityIncreasingForNewGame;

            switchPane(hChooseDifficultyPaneWnd, hGameInfoPaneWnd);

            startGame();
            break;

        case BACK_FROM_CHOOSE_DIFFICULTY_PANE_BTN_ID:
            switchPane(hChooseDifficultyPaneWnd, hChooseAbilitiesPaneWnd);
            break;

            // PAUSE MENU PANE
        case CONTINUE_GAME_BTN_ID:
            switchPane(hPausePaneWnd, hGameInfoPaneWnd);
            continueGame();
            break;

        case RESTART_GAME_BTN_ID:
            switchPane(hPausePaneWnd, hGameInfoPaneWnd);
            restartGame();
            break;

        case MAIN_MENU_FROM_PAUSE_PANE_BTN_ID:
            finishGame();
            reinitGame(
                hInstance,
                widthGameWnd,
                GIFT_COUNT,
                0,
                1,
                0,
                DEFAULT_ABILITY_P1_ID,
                DEFAULT_ABILITY_P2_ID,
                DEFAULT_ABILITY_A1_ID,
                DEFAULT_ABILITY_A2_ID,
                DEFAULT_ABILITY_A3_ID
            );
            switchPane(hPausePaneWnd, hMenuPaneWnd);
            break;

            // GAME OVER PANE
        case MAIN_MENU_FROM_GAME_OVER_PANE_BTN_ID:
            switchPane(hGameOverPaneWnd, hMenuPaneWnd);

            reinitGame(
                GetModuleHandle(NULL),
                widthGameWnd,
                GIFT_COUNT,
                1,
                1,
                1,
                DEFAULT_ABILITY_P1_ID,
                DEFAULT_ABILITY_P2_ID,
                DEFAULT_ABILITY_A1_ID,
                DEFAULT_ABILITY_A2_ID,
                DEFAULT_ABILITY_A3_ID
            );
            game.status = GAME_IN_BACKGROUND_MODE;
            break;

        case TRY_AGAIN_BTN_ID:
            switchPane(hwnd, hGameInfoPaneWnd);
            restartGame();
            break;

        case APPLY_NEW_RECORD_PANE_BTN_ID:
            GetWindowTextW(hNameForNewRecordTextBox, record.name, MAX_NAME_FOR_RECORD_LENGTH);
            record.score = game.score;
            addNewRecord(&record);
            switchPane(hNewRecordPaneWnd, hGameOverPaneWnd);
            break;
        }
        break;

    default: return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

VOID registerClasses(VOID) {
    WNDCLASSEXW mainWC;
    mainWC.cbSize = sizeof(WNDCLASSEXW);
    mainWC.lpszClassName = MAIN_WC_NAME;
    mainWC.lpfnWndProc = mainWndProc;
    mainWC.hInstance = hInstance;
    mainWC.hCursor = LoadCursor(NULL, IDC_ARROW);
    mainWC.hIcon = NULL;
    mainWC.hIconSm = NULL;
    mainWC.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    mainWC.lpszMenuName = NULL;
    mainWC.style = CS_PARENTDC;
    mainWC.cbClsExtra = 0;
    mainWC.cbWndExtra = 0;

    WNDCLASSEXW gameWC;
    gameWC.cbSize = sizeof(WNDCLASSEXW);
    gameWC.lpszClassName = GAME_WC_NAME;
    gameWC.lpfnWndProc = gameWndProc;
    gameWC.hInstance = hInstance;
    gameWC.hCursor = LoadCursor(NULL, IDC_ARROW);
    gameWC.hIcon = NULL;
    gameWC.hIconSm = NULL;
    gameWC.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    gameWC.lpszMenuName = NULL;
    gameWC.style = 0;
    gameWC.cbClsExtra = 0;
    gameWC.cbWndExtra = 0;

    WNDCLASSEXW paneWC;
    paneWC.cbSize = sizeof(WNDCLASSEXW);
    paneWC.lpszClassName = PANE_WC_NAME;
    paneWC.lpfnWndProc = paneWndProc;
    paneWC.hInstance = hInstance;
    paneWC.hCursor = LoadCursor(NULL, IDC_ARROW);
    paneWC.hIcon = NULL;
    paneWC.hIconSm = NULL;
    paneWC.hbrBackground = (HBRUSH)(COLOR_WINDOW);
    paneWC.lpszMenuName = NULL;
    paneWC.style = 0;
    paneWC.cbClsExtra = 0;
    paneWC.cbWndExtra = 0;

    RegisterClassExW(&mainWC);
    RegisterClassExW(&gameWC);
    RegisterClassExW(&paneWC);
}

VOID createWindows(VOID) {
    const INT MONITOR_WIDTH = GetSystemMetrics(SM_CXSCREEN);
    const INT MONITOR_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
    const INT GAME_SCREEN_WIDTH = MONITOR_WIDTH * GAME_WINDOW_WIDTH_MULTIPLIER;
    const INT GAME_SCREEN_HEIGHT = MONITOR_HEIGHT;
    const INT PANE_SCREEN_WIDTH = MONITOR_WIDTH - GAME_SCREEN_WIDTH;
    const INT PANE_SCREEN_HEIGHT = MONITOR_HEIGHT;

    // GENERAL WINDOWS
    hMainWnd = CreateWindowW(
        MAIN_WC_NAME,
        L"Snowman",
        WS_POPUP | WS_CLIPCHILDREN,
        0, 0,
        MONITOR_WIDTH, MONITOR_HEIGHT,
        0,
        0,
        hInstance,
        NULL
    );

    hGameWnd = CreateWindowW(
        GAME_WC_NAME,
        L"Game",
        WS_CHILD | WS_VISIBLE,
        0, 0,
        GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT,
        hMainWnd,
        (HMENU)GAME_WND_ID,
        hInstance,
        NULL
    );

    hPaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"Pane",
        WS_CHILD,
        GAME_SCREEN_WIDTH, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hMainWnd,
        (HMENU)PANE_WND_ID,
        hInstance,
        NULL
    );

    // PANE WINDOWS
    hMenuPaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"Menu",
        WS_CHILD | WS_VISIBLE,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)MENU_PANE_WND_ID,
        hInstance,
        NULL
    );

    hPausePaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"Pause",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)PAUSE_PANE_WND_ID,
        hInstance,
        NULL
    );

    hGameInfoPaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"GameInfo",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)GAME_INFO_PANE_WND_ID,
        hInstance,
        NULL
    );

    hCharacterListPaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"CharacterList",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)CHARACTER_LIST_PANE_WND_ID,
        hInstance,
        NULL
    );

    hChooseDifficultyPaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"ChooseDificulty",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)CHOOSE_DIFFICULTY_PANE_WND_ID,
        hInstance,
        NULL
    );

    hGameOverPaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"GameOver",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)GAME_OVER_PANE_WND_ID,
        hInstance,
        NULL
    );

    hChooseAbilitiesPaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"ChooseAbilities",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)CHOOSE_ABILITIES_PANE_WND_ID,
        hInstance,
        NULL
    );

    hChooseAbilityP1PaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"ChoosePassiveAbility1",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)CHOOSE_ABILITY_P1_PANE_WND_ID,
        hInstance,
        NULL
    );

    hChooseAbilityP2PaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"ChoosePassiveAbility2",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)CHOOSE_ABILITY_P2_PANE_WND_ID,
        hInstance,
        NULL
    );

    hChooseAbilityA1PaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"ChooseActiveAbility1",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)CHOOSE_ABILITY_A1_PANE_WND_ID,
        hInstance,
        NULL
    );

    hChooseAbilityA2PaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"ChooseActiveAbility2",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)CHOOSE_ABILITY_A2_PANE_WND_ID,
        hInstance,
        NULL
    );

    hChooseAbilityA3PaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"ChooseActiveAbility3",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)CHOOSE_ABILITY_A3_PANE_WND_ID,
        hInstance,
        NULL
    );

    hNewRecordPaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"NewRecord",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)NEW_RECORD_PANE_WND_ID,
        hInstance,
        NULL
    );

    hRecordListPaneWnd = CreateWindowW(
        PANE_WC_NAME,
        L"RecordList",
        WS_CHILD,
        0, 0,
        PANE_SCREEN_WIDTH, PANE_SCREEN_HEIGHT,
        hPaneWnd,
        (HMENU)RECORD_LIST_PANE_WND_ID,
        hInstance,
        NULL
    );

    // BUTTON
    hNewGameBtn = CreateWindowW(
        L"BUTTON",
        L"Новая игра",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hMenuPaneWnd,
        (HMENU)NEW_GAME_BTN_ID,
        hInstance,
        0
    );

    hExitBtn = CreateWindowW(
        L"BUTTON",
        L"Выход",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 250,
        200, 50,
        hMenuPaneWnd,
        (HMENU)EXIT_BTN_ID,
        hInstance,
        0
    );

    hContinueGameBtn = CreateWindowW(
        L"BUTTON",
        L"Продолжить",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hPausePaneWnd,
        (HMENU)CONTINUE_GAME_BTN_ID,
        hInstance,
        0
    );

    hMainMenuFromPausePaneBtn = CreateWindowW(
        L"BUTTON",
        L"В главное меню",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 250,
        200, 50,
        hPausePaneWnd,
        (HMENU)MAIN_MENU_FROM_PAUSE_PANE_BTN_ID,
        hInstance,
        0
    );

    hMainMenuFromGameOverPaneBtn = CreateWindowW(
        L"BUTTON",
        L"В главное меню",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 250,
        200, 50,
        hGameOverPaneWnd,
        (HMENU)MAIN_MENU_FROM_GAME_OVER_PANE_BTN_ID,
        hInstance,
        0
    );

    hApplyChooseCharacterPaneBtn = CreateWindowW(
        L"BUTTON",
        L"Далее",
        WS_CHILD |WS_VISIBLE | BS_CENTER,
        PANE_SCREEN_WIDTH - 200, PANE_SCREEN_HEIGHT - 100,
        200, 50,
        hCharacterListPaneWnd,
        (HMENU)APPLY_CHOOSE_CHARACTER_PANE_BTN_ID,
        hInstance,
        NULL
    );

    hEasyDifficultyBtn = CreateWindowW(
        L"BUTTON",
        L"Легко",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hChooseDifficultyPaneWnd,
        (HMENU)EASY_DIFFICULTY_BTN_ID,
        hInstance,
        NULL
    );

    hNormalDifficultyBtn = CreateWindowW(
        L"BUTTON",
        L"Обычно",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hChooseDifficultyPaneWnd,
        (HMENU)NORMAL_DIFFICULTY_BTN_ID,
        hInstance,
        NULL
    );

    hHardDifficultyBtn = CreateWindowW(
        L"BUTTON",
        L"Сложно",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 250,
        200, 50,
        hChooseDifficultyPaneWnd,
        (HMENU)HARD_DIFFICULTY_BTN_ID,
        hInstance,
        NULL
    );

    hCharacterBtnSnowman = CreateWindowW(
        L"BUTTON",
        L"Снежок",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hCharacterListPaneWnd,
        (HMENU)CHARACTER_SNOWMAN_BTN_ID,
        hInstance,
        NULL
    );

    hCharacterBtnEvangelion = CreateWindowW(
        L"BUTTON",
        L"Evangelion",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hCharacterListPaneWnd,
        (HMENU)CHARACTER_EVANGELION_BTN_ID,
        hInstance,
        NULL
    );

    hBackFromListCharacterPaneBtn = CreateWindowW(
        L"BUTTON",
        L"Назад",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, GAME_SCREEN_HEIGHT - 100,
        200, 50,
        hCharacterListPaneWnd,
        (HMENU)BACK_FROM_CHARACTER_LIST_PANE_BTN_ID,
        hInstance,
        NULL
    );

    hBackFromChooseDifficultyPaneBtn = CreateWindowW(
        L"BUTTON",
        L"Назад",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, GAME_SCREEN_HEIGHT - 100,
        200, 50,
        hChooseDifficultyPaneWnd,
        (HMENU)BACK_FROM_CHOOSE_DIFFICULTY_PANE_BTN_ID,
        hInstance,
        NULL
    );

    hRestartGameBtn = CreateWindowW(
        L"BUTTON",
        L"Заново",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hPausePaneWnd,
        (HMENU)RESTART_GAME_BTN_ID,
        hInstance,
        NULL
    );

    hTryAgainBtn = CreateWindowW(
        L"BUTTON",
        L"Заново",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hGameOverPaneWnd,
        (HMENU)TRY_AGAIN_BTN_ID,
        hInstance,
        NULL
    );

    hListPassiveAbility1BtnWnd  = CreateWindowW(
        L"BUTTON",
        L"Пассивная способность 1",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hChooseAbilitiesPaneWnd,
        (HMENU)CHOOSE_ABILITY_P1_BTN_ID,
        hInstance,
        NULL
    );

    hListPassiveAbility2BtnWnd = CreateWindowW(
        L"BUTTON",
        L"Пассивная способность 2",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hChooseAbilitiesPaneWnd,
        (HMENU)CHOOSE_ABILITY_P2_BTN_ID,
        hInstance,
        NULL
    );

    hListActiveAbility1BtnWnd   = CreateWindowW(
        L"BUTTON",
        L"Активная способность 1",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 250,
        200, 50,
        hChooseAbilitiesPaneWnd,
        (HMENU)CHOOSE_ABILITY_A1_BTN_ID,
        hInstance,
        NULL
    );

    hListActiveAbility2BtnWnd = CreateWindowW(
        L"BUTTON",
        L"Активная способность 2",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 350,
        200, 50,
        hChooseAbilitiesPaneWnd,
        (HMENU)CHOOSE_ABILITY_A2_BTN_ID,
        hInstance,
        NULL
    );

    hListActiveAbility3BtnWnd = CreateWindowW(
        L"BUTTON",
        L"Активная способность 3",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 450,
        200, 50,
        hChooseAbilitiesPaneWnd,
        (HMENU)CHOOSE_ABILITY_A3_BTN_ID,
        hInstance,
        NULL
    );

    hBackFromChooseAbilitiesPaneBtn = CreateWindowW(
        L"BUTTON",
        L"Назад",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 550,
        200, 50,
        hChooseAbilitiesPaneWnd,
        (HMENU)BACK_FROM_CHOOSE_ABILITIES_PANE_BTN_ID,
        hInstance,
        NULL
    );

    hApplyChooseAbilitiesPaneBtn = CreateWindowW(
        L"BUTTON",
        L"Далее",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 650,
        200, 50,
        hChooseAbilitiesPaneWnd,
        (HMENU)APPLY_CHOOSE_ABILITIES_PANE_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityP1Btn = CreateWindowW(
        L"BUTTON",
        L"+20% скорость",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hChooseAbilityP1PaneWnd,
        (HMENU)CHOOSE_PASSIVE_ABILITY1_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityP2Btn = CreateWindowW(
        L"BUTTON",
        L"+2 к макс. жизни",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hChooseAbilityP2PaneWnd,
        (HMENU)CHOOSE_PASSIVE_ABILITY2_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityP3Btn = CreateWindowW(
        L"BUTTON",
        L"+20% размер",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hChooseAbilityP1PaneWnd,
        (HMENU)CHOOSE_PASSIVE_ABILITY3_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityP4Btn = CreateWindowW(
        L"BUTTON",
        L"+1 к макс. ресурсам",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hChooseAbilityP2PaneWnd,
        (HMENU)CHOOSE_PASSIVE_ABILITY4_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityA1Btn = CreateWindowW(
        L"BUTTON",
        L"x2 размер на 10с",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hChooseAbilityA1PaneWnd,
        (HMENU)CHOOSE_ACTIVE_ABILITY1_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityA2Btn = CreateWindowW(
        L"BUTTON",
        L"x2 скорость на 10с",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hChooseAbilityA2PaneWnd,
        (HMENU)CHOOSE_ACTIVE_ABILITY2_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityA3Btn = CreateWindowW(
        L"BUTTON",
        L"сбор всех подарков на экране",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 50,
        200, 50,
        hChooseAbilityA3PaneWnd,
        (HMENU)CHOOSE_ACTIVE_ABILITY3_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityA4Btn = CreateWindowW(
        L"BUTTON",
        L"+3 жизни",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hChooseAbilityA1PaneWnd,
        (HMENU)CHOOSE_ACTIVE_ABILITY4_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityA5Btn = CreateWindowW(
        L"BUTTON",
        L"x2 получаемые очки на 10с",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hChooseAbilityA2PaneWnd,
        (HMENU)CHOOSE_ACTIVE_ABILITY5_BTN_ID,
        hInstance,
        NULL
    );

    hChooseAbilityA6Btn = CreateWindowW(
        L"BUTTON",
        L"-10% гравитация",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hChooseAbilityA3PaneWnd,
        (HMENU)CHOOSE_ACTIVE_ABILITY6_BTN_ID,
        hInstance,
        NULL
    );

    hApplyNewRecordPaneBtn = CreateWindowW(
        L"BUTTON",
        L"Сохранить рекорд",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 250,
        200, 50,
        hNewRecordPaneWnd,
        (HMENU)APPLY_NEW_RECORD_PANE_BTN_ID,
        hInstance,
        NULL
    );
    
    hBackFromRecordListPaneBtn = CreateWindowW(
        L"BUTTON",
        L"Назад",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, PANE_SCREEN_HEIGHT - 100,
        200, 50,
        hRecordListPaneWnd,
        (HMENU)BACK_FROM_RECORD_LIST_PANE_BTN_ID,
        hInstance,
        NULL
    );

    hRecordListBtn = CreateWindowW(
        L"BUTTON",
        L"Рекорды",
        WS_CHILD | WS_VISIBLE | BS_CENTER,
        50, 150,
        200, 50,
        hMenuPaneWnd,
        (HMENU)RECORD_LIST_BTN_ID,
        hInstance,
        NULL
    );

    // TEXT BOX
    hNameForNewRecordTextBox = CreateWindowW(
        L"EDIT",
        L"Игрок",
        WS_CHILD | WS_VISIBLE | ES_CENTER,
        50, 150,
        200, 50,
        hNewRecordPaneWnd,
        (HMENU)NAME_FOR_NEW_RECORD_TEX_BOX_ID,
        hInstance,
        NULL
    );

    // LABEL
    for (INT i = 0; i < MAX_RECORDS_AMOUNT; ++i) {
        hRecordListLabels[i] = CreateWindowW(
            L"STATIC",
            L"",
            WS_CHILD | WS_VISIBLE,
            50, 50 + i * ((PANE_SCREEN_HEIGHT - 150) / MAX_RECORDS_AMOUNT),
            200, 50,
            hRecordListPaneWnd,
            (HMENU)RECORD_LIST_LABELS_ID + i,
            hInstance,
            NULL
        );
    }
}

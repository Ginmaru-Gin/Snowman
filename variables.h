#pragma once

#include "objects.h"
#include "records.h"
#include "skin.h"

HINSTANCE hInstance;
RECT clipCursorRect;
Game game;
RecordList recordList;
Skin skin;
SkinsList skinsList;
INT widthGameWnd;
INT heightGameWnd;
INT widthPaneWnd;
INT heightPaneWnd;
INT widthPaneRow;
INT heightPaneRow;
DOUBLE gravityIncreasing;

// WINDOW HANDLE
HWND activePane;
HWND hMainWnd;
HWND hGameWnd;
HWND hPaneWnd;

// PANE HANDLE
HWND hMenuPaneWnd;
HWND hGameInfoPaneWnd;
HWND hPausePaneWnd;
HWND hCharacterListPaneWnd;
HWND hChooseDifficultyPaneWnd;
HWND hGameOverPaneWnd;
HWND hChooseAbilitiesPaneWnd;
HWND hChooseAbilityP1PaneWnd;
HWND hChooseAbilityP2PaneWnd;
HWND hChooseAbilityA1PaneWnd;
HWND hChooseAbilityA2PaneWnd;
HWND hChooseAbilityA3PaneWnd;
HWND hNewRecordPaneWnd;
HWND hRecordListPaneWnd;

// CHOOSE ABILITY BUTTON
HWND hListPassiveAbility1BtnWnd;
HWND hListPassiveAbility2BtnWnd;
HWND hListActiveAbility1BtnWnd;
HWND hListActiveAbility2BtnWnd;
HWND hListActiveAbility3BtnWnd;

// TEXT BOX
HWND hNameForNewRecordTextBox;

// BUTTON HANDLE
HWND hNewGameBtn;
HWND hExitBtn;
HWND hContinueGameBtn;
HWND hMainMenuFromPausePaneBtn;
HWND hMainMenuFromGameOverPaneBtn;
HWND hApplyChooseCharacterPaneBtn;
HWND hBackFromChooseCharacterPaneBtn;
HWND hBackFromListCharacterPaneBtn;
HWND hBackFromChooseDifficultyPaneBtn;
HWND hRestartGameBtn;
HWND hTryAgainBtn;
HWND hBackFromChooseAbilitiesPaneBtn;
HWND hApplyChooseAbilitiesPaneBtn;
HWND hApplyNewRecordPaneBtn;
HWND hBackFromRecordListPaneBtn;
HWND hRecordListBtn;

// ABILITY HANDLE
// PASSIVE
HWND hChooseAbilityP1Btn;
HWND hChooseAbilityP2Btn;
HWND hChooseAbilityP3Btn;
HWND hChooseAbilityP4Btn;
// ACTIVE
HWND hChooseAbilityA1Btn;
HWND hChooseAbilityA2Btn;
HWND hChooseAbilityA3Btn;
HWND hChooseAbilityA4Btn;
HWND hChooseAbilityA5Btn;
HWND hChooseAbilityA6Btn;


// DIFFICULTY BUTTON HANDLE
HWND hEasyDifficultyBtn;
HWND hNormalDifficultyBtn;
HWND hHardDifficultyBtn;

// SKIN BUTTON HANDLE
HWND *hSkinsButtonsHandles;

// CHARACTER BUTTON HANDLE
HWND hCharacterBtnSnowman;
HWND hCharacterBtnEvangelion;

// LABEL HANDLE
HWND hRecordListLabels[MAX_RECORDS_AMOUNT];

#define GAME_WINDOW_WIDTH_MULTIPLIER    2 / 3

// CHILD WINDOW ID : 1XX
#define GAME_WND_ID                     101
#define PANE_WND_ID                     102
#define MENU_PANE_WND_ID                103
#define GAME_INFO_PANE_WND_ID           104
#define PAUSE_PANE_WND_ID               105
#define CHOOSE_CHARACTER_PANE_WND_ID    106
#define CHARACTER_LIST_PANE_WND_ID      107
#define CHOOSE_DIFFICULTY_PANE_WND_ID   108
#define GAME_OVER_PANE_WND_ID           109
#define CHOOSE_ABILITIES_PANE_WND_ID    110
#define CHOOSE_ABILITY_P1_PANE_WND_ID   111
#define CHOOSE_ABILITY_P2_PANE_WND_ID   112
#define CHOOSE_ABILITY_A1_PANE_WND_ID   113
#define CHOOSE_ABILITY_A2_PANE_WND_ID   114
#define CHOOSE_ABILITY_A3_PANE_WND_ID   115
#define NEW_RECORD_PANE_WND_ID          116
#define RECORD_LIST_PANE_WND_ID         117

// BUTTON ID : 2XX
#define NEW_GAME_BTN_ID                 201
#define EXIT_BTN_ID                     202
#define CONTINUE_GAME_BTN_ID            204
#define MAIN_MENU_FROM_PAUSE_PANE_BTN_ID        205
#define MAIN_MENU_FROM_GAME_OVER_PANE_BTN_ID    206
#define APPLY_CHOOSE_CHARACTER_PANE_BTN_ID      207
#define BACK_FROM_CHARACTER_LIST_PANE_BTN_ID    209
#define BACK_FROM_CHOOSE_DIFFICULTY_PANE_BTN_ID 210
#define RESTART_GAME_BTN_ID                     211
#define TRY_AGAIN_BTN_ID                        212
#define CHOOSE_ABILITY_P1_BTN_ID                213
#define CHOOSE_ABILITY_P2_BTN_ID                214
#define CHOOSE_ABILITY_A1_BTN_ID                215
#define CHOOSE_ABILITY_A2_BTN_ID                216
#define CHOOSE_ABILITY_A3_BTN_ID                217
#define BACK_FROM_CHOOSE_ABILITIES_PANE_BTN_ID  218
#define APPLY_CHOOSE_ABILITIES_PANE_BTN_ID      219
#define APPLY_NEW_RECORD_PANE_BTN_ID            220
#define BACK_FROM_RECORD_LIST_PANE_BTN_ID       221
#define RECORD_LIST_BTN_ID                      222
#define SKIN_BTN_ID                             223

// DIFFICULTY BUTTON ID : 3XX
#define EASY_DIFFICULTY_BTN_ID           301
#define NORMAL_DIFFICULTY_BTN_ID         302
#define HARD_DIFFICULTY_BTN_ID           303

// LABEL ID : 4XX
#define RECORD_LIST_LABELS_ID           401 // RESERVS 10 VALUES, the next one should be 412
//#define                                 412

// CHOOSE PASSIVE ABILITY BUTTON ID : 5XX
#define CHOOSE_PASSIVE_ABILITY1_BTN_ID          601
#define CHOOSE_PASSIVE_ABILITY2_BTN_ID          602
#define CHOOSE_PASSIVE_ABILITY3_BTN_ID          603
#define CHOOSE_PASSIVE_ABILITY4_BTN_ID          604

// CHOOSE ACTIVE ABILITY BUTTON ID : 6XX
#define CHOOSE_ACTIVE_ABILITY1_BTN_ID           701
#define CHOOSE_ACTIVE_ABILITY2_BTN_ID           702
#define CHOOSE_ACTIVE_ABILITY3_BTN_ID           703
#define CHOOSE_ACTIVE_ABILITY4_BTN_ID           704
#define CHOOSE_ACTIVE_ABILITY5_BTN_ID           705
#define CHOOSE_ACTIVE_ABILITY6_BTN_ID           706

// TEXT BOX : 7XX
#define NAME_FOR_NEW_RECORD_TEX_BOX_ID          801

// WINDOW CLASS NAME
#define MAIN_WC_NAME                    L"Snowman Main Window"
#define GAME_WC_NAME                    L"Snowman Game Window"
#define PANE_WC_NAME                    L"Snowman Pane Window"

// TIMER VALUE
#define GAME_TACT_TIMER                 10
#define INPUT_TIMER                     30
#define GRAVITY_INCREASING_TIMER        500
#define FPS                             120

// TIMER ID
#define GAME_PROC_TIMER_ID              1
#define DRAWING_TIMER_ID                2
#define INPUT_TIMER_ID                  3
#define GRAVITY_INCREASING_TIMER_ID     4
#define ABILITY_SLOT_A1_TIMER_ID        5
#define ABILITY_SLOT_A2_TIMER_ID        6
#define ABILITY_SLOT_A3_TIMER_ID        7

//GAME STATUS FLAG
#define INITIALIZED                     0x0
#define GAME_IN_BACKGROUND_MODE			0x1
#define STARTED                         0x2
#define PAUSED                          0x4
#define GAME_IS_STARTED					STARTED
#define GAME_IS_PAUSED					(STARTED | PAUSED)
#define GAME_IN_PROCESS					(STARTED & ~PAUSED)

// SUPPORT VARIABLE
#define MAX_SCORE_COUNTER_LENGTH        20
#define MAX_LIVES_COUNTER_LENGTH        3
#define MAX_SCORE_STR_LEN               10

// GAME FIELD AND GAME PROCESS VARIABLE
#define GIFT_COUNT                      7
#define ACTIVE_ABILITY_COUNT            3
#define PASSIVE_ABILITY_COUNT           2
#define RESOURCE_COUNT_TO_USE_ABILITY   3
#define DEFAULT_MAX_RESOURCES           3
#define GAME_FIELD_WIDTH                5120
#define GAME_FIELD_HEIGHT               4320
#define PANE_ROWS_COUNT                 7
#define LEFT_PANE_FIELD                 50
#define RIGHT_PANE_FIELD                50
#define MAX_SNOWMAN_HEIGHT              1080
#define MAX_SNOWMAN_WIDTH               MAX_SNOWMAN_HEIGHT
#define MAX_FALLER_HEIGHT               540
#define MAX_FALLER_WIDTH                MAX_FALLER_HEIGHT
// DEFAULT VALUE
#define EASY_DIFFICULTY_SCORE_MULTIPLIER    1
#define EASY_DIFFICULTY_GRAVITY_INCREASING  0.005
#define EASY_DIFFICULTY_LIVES               7
#define EASY_GIFTS_COUNT                    5

// SKINS VARIABLES
#define SKINS_DIR                       L"skins"
#define SKIN_DEFAULT_ID                 1
// SKIN TEXTURES FILES
#define SKIN_FILE_GAME_BKG              L"background.data.bmp"
#define SKIN_FILE_PANE_BKG              L"paneBkg.data.bmp"
#define SKIN_FILE_CHARACTER             L"snowman.data.bmp"
#define SKIN_FILE_RESOURCE_1            L"resource1.data.bmp"
#define SKIN_FILE_RESOURCE_2            L"resource2.data.bmp"
#define SKIN_FILE_RESOURCE_3            L"resource3.data.bmp"
#define SKIN_FILE_RESOURCE_GIFT         L"resourceGift.data.bmp"
#define SKIN_FILE_RESOURCE_LIFE         L"resourceLife.data.bmp"
// SKIN AUDIO FILES
#define SKIN_FILE_GAME_MUSIC            L"gameMusic.data.wav"
#define SKIN_FILE_MENU_MUSIC            L"menuMusic.data.wav"

// ABILITIES
#define WM_USE_ABILITY                  (WM_USER + 1)
// PASSIVE ABILITY (WM_USER + 1XXX)
#define DEFAULT_ABILITY_P1_ID           ABILITY_P1_ID
#define DEFAULT_ABILITY_P2_ID           ABILITY_P2_ID

#define ABILITY_P1_ID                   1001 // +[ABILITY_P1_STRENGTH]% speed
#define ABILITY_P1_STRENGTH             20

#define ABILITY_P2_ID                   1002 // +[ABILITY_P2_STRENGTH] max lifes
#define ABILITY_P2_STRENGTH             2

#define ABILITY_P3_ID                   1003 // +[ABILITY_P3_STRENGTH]% size
#define ABILITY_P3_STRENGTH             20

#define ABILITY_P4_ID                   1004 // +[ABILITY_P4_STRENGTH] max resources
#define ABILITY_P4_STRENGTH             1

// ACTIVE ABILITY (WM_USER + 2XXX)
#define DEFAULT_ABILITY_A1_ID           ABILITY_A1_ID
#define DEFAULT_ABILITY_A2_ID           ABILITY_A2_ID
#define DEFAULT_ABILITY_A3_ID           ABILITY_A3_ID

#define ABILITY_A1_ID                   2001 // +[ABILITY_A1_STRENGTH]% character size for [ABILITY_A1_TIME]ms
#define ABILITY_A1_STRENGTH             100
#define ABILITY_A1_TIME                 10000

#define ABILITY_A2_ID                   2002 // +[ABILITY_A2_STRENGTH]% speed for [ABILITY_A2_TIME]ms
#define ABILITY_A2_STRENGTH             100
#define ABILITY_A2_TIME                 10000

#define ABILITY_A3_ID                   2003 // collects all gifts on the screen

#define ABILITY_A4_ID                   2004 // heals [ABILITY_A4_STRENGTH] lifes
#define ABILITY_A4_STRENGTH             3

#define ABILITY_A5_ID                   2005 // +[ABILITY_A5_STRENGTH]% income score for [ABILITY_A5_TIME]s
#define ABILITY_A5_STRENGTH             100
#define ABILITY_A5_TIME                 10000

#define ABILITY_A6_ID                   2006 // -[ABILITY_A6_STRENGTH]% current gravity
#define ABILITY_A6_STRENGTH             10


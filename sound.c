#include "sound.h"

VOID playMusic(UINT music) {
    if (skin.ID == SKIN_DEFAULT_ID) {
        PlaySoundW(MAKEINTRESOURCEW(music), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP | SND_ASYNC);
    }
}
#include "sound.h"

VOID playMusic(UINT music) {
    PlaySoundW(MAKEINTRESOURCEW(music), GetModuleHandle(NULL), SND_RESOURCE | SND_LOOP | SND_ASYNC);
}
#ifndef __PLAY_SPEEX_H_
#define __PLAY_SPEEX_H_

#include "base_types.h"

// Open play device, initial ring buffer memory.
void PlaySpeexStart();

// Close play device, release ring buffer memory.
void PlaySpeexStop();

// Decode and play the Speex in the pack file by map ID.
bool PlaySpeexMapID(const BYTE *lpbySpeexPack, int nMapID);

// Decode and play the Speex in the pack file by index.
bool PlaySpeexIndex(const BYTE *lpbySpeexPack, int nIndex);

#endif

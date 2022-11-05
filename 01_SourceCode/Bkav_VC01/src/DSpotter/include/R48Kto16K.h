#ifndef __48K_TO_16K_H__
#define __48K_TO_16K_H__

#include "base_types.h"

#define R48Kto16K_SUCCESS					0
#define R48Kto16K_IllegalParam				-1
#define R48Kto16K_LeaveNoMemory				-2
#define R48Kto16K_NotInit					-3

#ifdef __cplusplus
extern "C" {
#endif

// Initial the 48KHz to 16KHz re-sampler. It will use (nFrameSamples48K + 32)*2 heap size.
// nFrameSamples48K(IN): The samples for each convert frame. It must be multiples of 6.
// Return: R48Kto16K_SUCCESS or error code.
int R48Kto16K_Init(int nFrameSamples48K);

// To re-sample 48K to 16K.
// lpsFrame48K(IN): The 48KHz input audio data.
// nFrameSamples48K(IN): The sample count of lpsFrame48K.
// lpsFrame16K(OUT): The 16KHz output audio data.
// nFrameSamples16K(IN): The sample count of lpsFrame16K, it must be nFrameSamples48K/3.
// Return: R48Kto16K_SUCCESS or error code.
int R48Kto16K_Resample(const short *lpsFrame48K, int nFrameSamples48K, short *lpsFrame16K, int nFrameSamples16K);

// Release heap resource.
// Return: R48Kto16K_SUCCESS or error code.
int R48Kto16K_Release(void);

#ifdef __cplusplus
}
#endif


#endif


#ifndef __DSPOTTERSDTRAIN_API_H
#define __DSPOTTERSDTRAIN_API_H

#include "DSpotterSDKApi.h"

#ifdef __cplusplus
extern "C"{
#endif

// Purpose: Create a trainer
// lpbyCYBase(IN): a base model for trainer
// lpbyGroup(IN): a SI group model for trainer
// lpbyTrimap(IN): a trimap model for trainer
// lpbyMemPool(OUT): memory buffer for trainer
// nMemSize(IN): memory buffer size
// pnErr(OUT): CSPOTTER_SUCCESS indicates success, else error code. It can be NULL.
// Return: a trainer handle or NULL
DSPDLL_API HANDLE DSpotterSD_Init(BYTE *lpbyCYBase, BYTE *lpbyTrimap, BYTE *lpbyMemPool, INT nMemSize, INT *pnErr);

// Purpose: Destroy a trainer (free resources)
// hDSpotter(IN): a handle of the trainer
// Return: Success or error code
DSPDLL_API INT DSpotterSD_Release(HANDLE hDSpotter);

DSPDLL_API INT DSpotterSD_GetMemoryUsage(BYTE *lpbyCYBase, BYTE *lpbyTrimap);

// Purpose: Reset trainer
// hDSpotter(IN): a handle of the trainer
// Return: Success or error code
DSPDLL_API INT DSpotterSD_Reset(HANDLE hDSpotter);

// Purpose: Prepare to add a new utterance for training
// hDSpotter(IN): a handle of the trainer
// lpszDataBuf(IN/OUT): the pointer of data buffer, it must be a DATA FALSH pointer, the buffer is used to store the voice data
// nBufSize(IN): the size of data buffer
// Return: Success or error code
DSPDLL_API INT DSpotterSD_AddUttrStart(HANDLE hDSpotter, SHORT *lpsDataBuf, INT nBufSize);

// Purpose: Transfer voice samples to the trainer for training
// hDSpotter(IN): a handle of the trainer
// lpsSample(IN): the pointer of voice data buffer
// nNumSample(IN): the number of voice data (a unit is a short, we prefer to add 160 samples per call)
// Return: "CSPOTTER_ERR_NeedMoreSample" indicates call this function again, otherwise Success or error code
DSPDLL_API INT DSpotterSD_AddSample(HANDLE hDSpotter, SHORT *lpsSample, INT nNumSample);

// Purpose: Finish the adding process
// hDSpotter(IN): a handle of the trainer
// Return: Success or error code
DSPDLL_API INT DSpotterSD_AddUttrEnd(HANDLE hDSpotter);

// Purpose: Get the utterance boundary
// hDSpotter(IN): a handle of the trainer
// pnStart(OUT): starting point (samples)
// pnEnd(OUT): ending point (samples)
// Return: Success or error code
DSPDLL_API INT DSpotterSD_GetUttrEPD(HANDLE hDSpotter, INT *pnStart, INT *pnEnd);


// Purpose: Train a voice tag
// hDSpotter(IN): a handle of the trainer
// lpszModelAddr(IN/OUT): the pointer of model buffer, it must be a DATA FALSH pointer
// nBufSize(IN): the size of model buffer
// pnUsedSize(OUT): the model size
// Return: Success or error code
DSPDLL_API INT DSpotterSD_TrainWord(HANDLE hDSpotter, char *lpszModelAddr, INT nBufSize, INT *pnUsedSize);

DSPDLL_API INT DSpotterSD_DeleteWord(HANDLE hDSpotter, char *lpszModelAddr, INT nIdx, INT *pnUsedSize);


// The default is 1200. It is equal to the RMS amplitude and the wave amplitude is more than 4 times of that amount.
DSPDLL_API INT DSpotterSD_SetBackgroundEnergyThreshd(HANDLE hCSpotter, INT nThreshold);

// The default is 0. The range is [-100, 100].
DSPDLL_API INT DSpotterSD_SetEpdLevel(HANDLE hCSpotter, INT nEpdLevel);

#ifdef __cplusplus
}
#endif

#endif // __DSPOTTERSDTRAIN_API_H

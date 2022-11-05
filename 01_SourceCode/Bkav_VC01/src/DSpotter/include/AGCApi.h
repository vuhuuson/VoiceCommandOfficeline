#ifndef	__AGC_H__
#define	__AGC_H__

#if defined(_WIN32)
#ifdef AGCDll_EXPORTS
#define AGCDLL_API __declspec(dllexport)
#endif
#endif

#ifndef AGCDLL_API
#define AGCDLL_API
#endif

#include "base_types.h"

/*-----------Error Code--------------*/
#define __AGC_ERROR__                             (0)
#define __AGC_SUCCESS__                           (__AGC_ERROR__-0)
#define __AGC_ERROR_WRONG_PARAMETER__             (__AGC_ERROR__-1)
#define __AGC_ERROR_WRONG_DEFAULT__               (__AGC_ERROR__-2)


/*This is for C++ and does no harm in C*/
#ifdef __cplusplus
extern "C" {
#endif

/*-----------API---------------------*/
INT AGC_GetMemUsage(VOID);
// Purpose               : Get memory usage of AGC;     
// Return                : Size of AGC handle in BYTE;  

HANDLE AGC_Init(BYTE* lpbyMem, INT nMemSize, INT* pnERR);
// Purpose               : Init AGC Handle and set default value; Note that the initial AGC Gain is set to AGC_MaxGain;
// lpbyMem(IN)           : Memory space for create AGC handle, should be get from malloc() with size return by AGC_GetMemUsage();
// nMemSize(IN)          : Memory size of lpbyMem in BYTE, should be get from AGC_GetMemUsage();
// pnERR(OUT)            : __AGC_SUCCESS__ if SUCCESS; Error Code, otherwise;
// Return                : Handle of AGC;

INT AGC_Do(HANDLE hAGC, SHORT* lpsInputSample, INT nNumSample, SHORT* lpsOutputSample);
// Purpose               : Perform AGC on a frame of samples;
// hAGC(IN)              : Handle of AGC;
// lpsInputSample(IN)    : Input sample buffer, the length should be nNumSample;
//			   With sample format 16Bit, MONO, 16000hz;
// nNumSample(IN)        : Number of samples in lpsInputSample;
// lpsOutputSample(Out)  : Output Sample buffer, create and given by outside, the size should be nNumSample*sizeof(SHORT);
// Return                : __AGC_SUCCESS__ if SUCCESS; Error Code, otherwise;

INT AGC_Release(HANDLE hAGC);
// Purpose               : Release AGC handle;
// hAGC(IN)              : Handle of AGC;
// Return                : __AGC_SUCCESS__ if SUCCESS; Error Code, otherwise; 

FLOAT AGC_GetGain(HANDLE hAGC);
// Purpose               : Return current AGC Gain;
// hAGC(IN)              : Handle of AGC;
// Return                : FLOAT Gain > 0 if SUCCESS; Error Code, otherwise; 

INT AGC_GetMaxPeak(HANDLE hAGC);

INT AGC_SetMaxGain(HANDLE hAGC, FLOAT fMaxGain);
// Purpose               : Set the upper bound of AGC Gain,; This will also set the current AGC Gain to fMaxGain;
// hAGC(IN)              : Handle of AGC;
// fMaxGain(IN)          : The upper bound of AGC gain; 
//                         Default is set to 32;
// Return                : __AGC_SUCCESS__ if SUCCESS; Error Code, otherwise; 
// NOTE                  : 1 <= fMaxGain <= 32;

INT AGC_SetIncGainThrd(HANDLE hAGC, SHORT sLowerThrd);
// Purpose               : Set threshholds to trigger AGC Gain increasing.
// hAGC(IN)              : Handle of AGC;
// sLowerThrd(IN)        : AGC Gain will increasing if (current peak of frame)*(AGC Gain) < sLowerThrd;
//                         Default is set to 5000
// Return                : __AGC_SUCCESS__ if SUCCESS; Error Code, otherwise;
// NOTE                  : 0 <= sLowerThrd <= 10000;

#ifdef __cplusplus
}
#endif

#endif	/* __AGC_H__ */
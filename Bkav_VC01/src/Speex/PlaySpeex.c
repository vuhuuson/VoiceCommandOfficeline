#include "hal_data.h"
#include "base_types.h"
#include "RingBuffer.h"
#include "SpeexDecodeApi.h"
#include "R8Kto16K.h"
#include "PlaySpeex.h"
#include "voice_main.h"

#define SPEEX_FRAME_TIME          20
#define SPEEX_FRAME_SAMPLE_16K    (SPEEX_FRAME_TIME*16000/1000)
#define SPEEX_FRAME_SAMPLE_8K     (SPEEX_FRAME_TIME*8000/1000)
#define SPEEX_FRAME_SIZE_16K      (SPEEX_FRAME_SAMPLE_16K*2)
#define SPEEX_FRAME_SIZE_8K       (SPEEX_FRAME_SAMPLE_8K*2)
#define RBUF_PLAY_SIZE            (SPEEX_FRAME_SIZE_16K*3)
#define NCP2809_SHUTDOWN_CTRL     BSP_IO_PORT_00_PIN_15

static HANDLE  g_hRingBufferPlay = NULL;
static BYTE    g_byaRingBufferPlay[RING_BUFFER_GET_MEM_USAGE(RBUF_PLAY_SIZE)];

static const BYTE* UnpackSpeex(const BYTE *lpbyBin, int nIndex, DWORD *pdwUnpackBinSize, int *pnMapID);
static int GetSpeexPackCount(const BYTE *lpbyBin);
static int GetSpeexMapID(const BYTE *lpbyBin, int nIndex);

/* Callback function */
void g_timer_playback_callback(timer_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);

    BYTE byData[2];
    int32_t sound_duty_counts;
    uint16_t sound_duty_counts_tmp;

    //Check ring buffer has sample data
    if (RingBufferGetDataSize(g_hRingBufferPlay) < 2)
        return;

    RingBufferGetData(g_hRingBufferPlay, byData, 2);

    sound_duty_counts = byData[1];
    sound_duty_counts = sound_duty_counts << 8;
    sound_duty_counts = sound_duty_counts + byData[0];
    sound_duty_counts_tmp = (uint16_t)(((sound_duty_counts + 0x0008) >> 4) + 0x0800);

    R_DAC_Write(&g_dac_playback_ctrl, sound_duty_counts_tmp);
}

void PlaySpeexStart()
{
    if (g_hRingBufferPlay == NULL)
    {
        if (RingBufferInit(g_byaRingBufferPlay, sizeof(g_byaRingBufferPlay),
                           RBUF_PLAY_SIZE, &g_hRingBufferPlay) != RING_BUFFER_SUCCESS)
            __asm("BKPT #0\n");
    }

    while(FSP_SUCCESS !=  R_DAC_Open(&g_dac_playback_ctrl, &g_dac_playback_cfg));
    R_BSP_SoftwareDelay(100U, BSP_DELAY_UNITS_MICROSECONDS);
    while(FSP_SUCCESS !=  R_DAC_Start(&g_dac_playback_ctrl));
    R_IOPORT_PinWrite(&g_ioport_ctrl, NCP2809_SHUTDOWN_CTRL, BSP_IO_LEVEL_HIGH);    // release NCP2809 from shutdown mode

    while(FSP_SUCCESS !=  R_GPT_Open(&g_timer_playback_ctrl, &g_timer_playback_cfg));
    while(FSP_SUCCESS !=  R_GPT_Start(&g_timer_playback_ctrl));
}

void PlaySpeexStop()
{
    R_GPT_Stop(&g_timer_playback_ctrl);
    R_GPT_Close(&g_timer_playback_ctrl);

    R_DAC_Stop(&g_dac_playback_ctrl);
    R_DAC_Close(&g_dac_playback_ctrl);
    R_IOPORT_PinWrite(&g_ioport_ctrl, NCP2809_SHUTDOWN_CTRL, BSP_IO_LEVEL_LOW);     // set NCP2809 to shutdown mode

    if (g_hRingBufferPlay != NULL)
    {
        RingBufferRelease(g_hRingBufferPlay);
        g_hRingBufferPlay = NULL;
    }
}

bool PlaySpeexMapID(const BYTE *lpbySpeexPack, int nMapID)
{
    int nSpeexCount;

    nSpeexCount = GetSpeexPackCount(lpbySpeexPack);
    for (int i = 0; i < nSpeexCount; i++)
    {
        if (GetSpeexMapID(lpbySpeexPack, i) == nMapID)
        {
            PlaySpeexIndex(lpbySpeexPack, i);
            return true;
        }
    }

    return false;
}

// This function will use 640B extra stack size
bool PlaySpeexIndex(const BYTE *lpbySpeexPack, int nIndex)
{
    short saSamples[SPEEX_FRAME_SAMPLE_16K];        //lpoSpeexDataHeader->nFrameSamples is 320 for 16 KHz.
    short saSamples2[SPEEX_FRAME_SAMPLE_16K];
    HANDLE hSpeexDecoder = NULL;
    HANDLE hR8Kto16K = NULL;
    int nSpeexCount;
    const BYTE* lpbyUnpack;
    DWORD dwUnpackBinSize;
    SpeexDataHeader *lpoSpeexDataHeader;
    BYTE *lpbyData;
    int nDataSize;
    int nErr;
    int n = 0;
    int nFreeSize = 0;

    nSpeexCount = GetSpeexPackCount(lpbySpeexPack);
    if (nIndex < nSpeexCount)
    {
        lpbyUnpack = UnpackSpeex(lpbySpeexPack, nIndex, &dwUnpackBinSize, NULL);

        lpoSpeexDataHeader = (SpeexDataHeader *)lpbyUnpack;
        lpbyData = (BYTE*)lpbyUnpack + sizeof(SpeexDataHeader);
        nDataSize = (int)(dwUnpackBinSize - sizeof(SpeexDataHeader));

        hSpeexDecoder = SpeexDecoderInit(lpoSpeexDataHeader->nChannelNum, lpoSpeexDataHeader->nSampleRate, lpoSpeexDataHeader->nBitRate, 1, &nErr);
        if (hSpeexDecoder == NULL)
            __asm("BKPT #0\n");

        hR8Kto16K = R8Kto16K_Init(SPEEX_FRAME_SAMPLE_8K, NULL);
        if (hR8Kto16K == NULL)
            __asm("BKPT #0\n");

        n = 0;
        while (n <= nDataSize - lpoSpeexDataHeader->nEncodeSize)
        {
            nErr = SpeexDecoderDo(hSpeexDecoder, lpbyData + n, lpoSpeexDataHeader->nEncodeSize, saSamples, lpoSpeexDataHeader->nFrameSamples);

            // Let Speex to produce interpolation frame if data corrupt.
            if (nErr < 0)
                SpeexDecoderDo(hSpeexDecoder, NULL, 0, saSamples, lpoSpeexDataHeader->nFrameSamples);

            n += lpoSpeexDataHeader->nEncodeSize;

            if (lpoSpeexDataHeader->nSampleRate == 8000)
            {
                //For 8 KHz sample, we up-sampling to 16 KHz to play it.
                R8Kto16K_Resample(hR8Kto16K, saSamples, lpoSpeexDataHeader->nFrameSamples, saSamples2, SPEEX_FRAME_SAMPLE_16K);
                memcpy(saSamples, saSamples2, SPEEX_FRAME_SIZE_16K);
            }

            // Wait ring buffer has enough free size to put saSamples.
            while (1)
            {
                nFreeSize = RingBufferGetFreeSize(g_hRingBufferPlay);
                if (nFreeSize >= (int)sizeof(saSamples))
                    break;
            }
            RingBufferPutData(g_hRingBufferPlay, saSamples, sizeof(saSamples));
        }

        SpeexDecoderRelease(hSpeexDecoder);
        R8Kto16K_Release(hR8Kto16K);
    }
    else
    {
        return false;
    }

    // Wait all ring buffer data played.
    while (1)
    {
        nDataSize = RingBufferGetDataSize(g_hRingBufferPlay);
        if (nDataSize == 0)
            break;
    }

    return true;
}

/*
Packed bin format:

Bin Number ----- 4 bytes, assume its value is N.
Bin 1 size ----- 4 bytes, assume its value is nBin1Size.
...
Bin N size ----- 4 bytes, assume its value is nBinNSize.
Bin 1 ID   ----- 4 bytes
...
Bin N ID   ----- 4 bytes
Bin 1 data ----- nBin1Size bytes
...
Bin N data ----- nBinNSize bytes
*/
const BYTE* UnpackSpeex(const BYTE *lpbyBin, int nIndex, DWORD *pdwUnpackBinSize, int *pnMapID)
{
    DWORD *lpnBin = (DWORD *)lpbyBin;
    DWORD nNumBin = lpnBin[0];
    DWORD *lpnBinSize = lpnBin + 1;
    int *lpnMapID = (int *)lpnBinSize + nNumBin;
    const BYTE *lpbyModel = NULL;
    DWORD i;

    lpbyModel = (const BYTE *)(lpnBinSize + nNumBin*2);
    if (pdwUnpackBinSize != NULL)
        *pdwUnpackBinSize = lpnBinSize[nIndex];
    if (pnMapID != NULL)
        *pnMapID = lpnMapID[nIndex];
    if (nIndex == 0)
        return lpbyModel;

    for (i = 1; i < nNumBin; i++)
    {
        lpbyModel = lpbyModel + lpnBinSize[i - 1];
        if (i == (DWORD)nIndex)
            break;
    }

    return lpbyModel;
}

int GetSpeexPackCount(const BYTE *lpbyBin)
{
    int *lpnBin = (int *)lpbyBin;
    int nNumBin = lpnBin[0];
    return nNumBin;
}

int GetSpeexMapID(const BYTE *lpbyBin, int nIndex)
{
    DWORD *lpnBin = (DWORD *)lpbyBin;
    DWORD nNumBin = lpnBin[0];
    DWORD *lpnBinSize = lpnBin + 1;
    int *lpnMapID = (int *)lpnBinSize + nNumBin;

    if (nIndex < (int)nNumBin)
        return lpnMapID[nIndex];
    else
        return -1;
}

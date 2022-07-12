/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/

/*******************************************************************************
 Include
*******************************************************************************/

#include "voice_main.h"
#include "hal_data.h"

#include "DSpotter/include/base_types.h"
#include "DSpotter/include/Convert2TransferBuffer.h"
#include "DSpotter/include/DSpotterSDKApi_Const.h"
#include "DSpotter/include/DSpotterSDKApi.h"
#include "DSpotter/include/DSpotterSDTrainApi.h"
#include "DSpotter/include/RingBuffer.h"
#include "DSpotter/include/AGCApi.h"
#include "DSpotter/include/CybVADApi.h"

#include "DSpotter/DbgTrace.h"
#include "DSpotter/UartMgr.h"
#include "DSpotter/CybModelInfor.h"
#include "Model/CybModel1.h"
#include "Model/CybModel2.h"
#include "Model/SpeexData1.h"
#include "Model/SpeexData2.h"
#include "Speex/PlaySpeex.h"

/*******************************************************************************
 Macro definitions
*******************************************************************************/

/** LED */
#define LED_G   BSP_IO_PORT_05_PIN_00
#define LED_R   BSP_IO_PORT_01_PIN_01
#define LED_Y   BSP_IO_PORT_01_PIN_02

/** Data flash */
// The following definition is for RA6M1, please change it for different IC.
#define FLASH_HP_DF_BLOCK_SIZE                  (64)
#define FLASH_HP_DF_BLOCK_BASE_ADDRESS          0x40100000U
#define FLASH_HP_DF_BLOCK_ADDRESS(nBlockIndex)  (BYTE *)(FLASH_HP_DF_BLOCK_BASE_ADDRESS + nBlockIndex*FLASH_HP_DF_BLOCK_SIZE)

/** DSpotter */
#ifdef SUPPORT_VOICE_TAG
#define DSPOTTER_MEM_SIZE_1      75000                   // Please modify this number by the return value of DSpotter_GetMemoryUsage_XXX().
#else
#define DSPOTTER_MEM_SIZE_1      50000                   // Please modify this number by the return value of DSpotter_GetMemoryUsage_XXX().
#endif
#define DSPOTTER_MEM_SIZE_2      50000                   // Please modify this number by the return value of DSpotter_GetMemoryUsage_XXX().
#define MAX_COMMAND_TIME         (8000/10)               // Trigger and command must be spoke in 8000 ms (800 frames).
#define COMMAND_RECOGNIZE_TIME   8000                    // At recognize command stage, the max recording time.
#define VAD_FRAME_SIZE           320                     // VAD compute every 10 ms.
#define VAD_CACHE_FRAME_COUNT    40                      // Because of the VAD detect delay, we need to cache some frames for recognition.
#define DSPOTTER_FRAME_SIZE      960                     // DSpotter compute every 30 ms.
#define RECORD_FRAME_SIZE        VAD_FRAME_SIZE          // Must be multiple times of 4, 320 is equal to 10 ms data. Must be 320 for VAD.
#define RECORD_FRAME_SAMPLES     (RECORD_FRAME_SIZE/2)
#define UART_FRAME_TRANSFER_SIZE (RECORD_FRAME_SIZE*5/4) // Every 4-bytes data add one checksum byte
#define RBUF_SIZE_RECORD         (320*9)                 // Record ring buffer size, 90 ms.
#define RBUF_SIZE_CACHE          (320*VAD_CACHE_FRAME_COUNT)  // Cache ring buffer size 10*VAD_CACHE_FRAME_COUNT ms).
#define VOLUME_SCALE_RECONG      3200                    // The AGC volume scale percentage for recognition. It depends on original microphone data.
#define VOLUME_SCALE_VOICE_TAG   800                     // The AGC volume scale percentage for record voice tag (near field).
#define GROUP_INDEX_TRIGGER      0                       // The group index of trigger.
#define GROUP_INDEX_COMMAND      1                       // The group index of command.
#define COMMAND_STAGE_TIME_MIN   6000                    // When no result at command recognition stage, the minimum recording time in ms.
#define COMMAND_STAGE_TIME_MAX   6000                    // When no result at command recognition stage, the maximum recording time in ms.
#define COMMAND_STAGE_TIME_MIN_CMD  20000
#define COMMAND_STAGE_TIME_MAX_CMD  20000
#define VOICE_TAG_VOICE_SIZE     12000                   // According SDK document, voice tag length can be 3000*12/16 ms.
#define VOICE_TAG_MODEL_SIZE     2048                    // Voice tag model, it has 340B header(H) and 400B for each voice tag(T).
#define I2SRBUF_SAMPLES          32

//command uart to control
static const uint8_t cmd_stop[] = {0xF1,
                                   0xF1,
                                   0x2B,
                                   0x00,
                                   0x2B,
                                   0x7E,
                                   0xF1,
                                   0xF1,
                                   0x2B,
                                   0x00,
                                   0x2B,
                                   0x7E,
                                   0xF1,
                                   0xF1,
                                   0x2B,
                                   0x00,
                                   0x2B,
                                   0x7E};//18bytes
static const uint8_t cmd_up[] = {0xF1,
                                 0x1B,
                                 0x02,
                                 0x02,
                                 0x2E,
                                 0x4D,
                                 0x7E,
                                 0xF1,
                                 0xF1,
                                 0x1B,
                                 0x02,
                                 0x02,
                                 0x2E,
                                 0x4D,
                                 0x7E,
                                 0xF1,
                                 0xF1,
                                 0x1B,
                                 0x02,
                                 0x02,
                                 0x2E,
                                 0x4D
                                 ,0x7E};//24 bytes
static const uint8_t cmd_down[] = {0xF1,
                                   0xF1,
                                   0x1B,
                                   0x02,
                                   0x00,
                                   0x00,
                                   0x1D,
                                   0x7E,
                                   0xF1,
                                   0xF1,
                                   0x1B,
                                   0x02,
                                   0x00,
                                   0x00,
                                   0x1D,
                                   0x7E,
                                   0xF1,
                                   0xF1,
                                   0x1B,
                                   0x02,
                                   0x00,
                                   0x00,
                                   0x1D,
                                   0x7E};//24 bytes
/*******************************************************************************
 Private global functions
*******************************************************************************/

static bool     voice_loop(void);
static void     voice_init(void);
static void     voice_release(void);
static bool     ds_decode_init(void);
static int      TestByteOrder();
static void     SetDSpotter(HANDLE hCyModel, int nGroupIndex, BYTE *pbyaDSpotterMem, int nMemSize, HANDLE *phDSpotter);
static void     OnDataReadCompleteCallback(void);
static void     ToggleYellowLED();

/*******************************************************************************
 Private global variables
*******************************************************************************/

static HANDLE   g_hDSpotter1 = NULL;
static BYTE     g_byaDSpotterMem1[DSPOTTER_MEM_SIZE_1];     // The memory for DSpotter engine.
static HANDLE   g_hCybModel1 = NULL;
static BYTE     g_byaCybModelMem1[CYBMODEL_GET_MEM_USAGE()];// The memory for g_hCybModel1 engine.
static HANDLE   g_hRingBuffer = NULL;                       // The I2S one channel data for recording.
static BYTE     g_byaRingBuffer[RING_BUFFER_GET_MEM_USAGE(RBUF_SIZE_RECORD)];
static volatile bool g_bRecording = false;
static volatile bool g_bUartSendRecord = false;
static volatile bool g_bSkipRecordData = false;
static BYTE     g_byaUartTxBuffer[UART_FRAME_TRANSFER_SIZE];
static BYTE     g_byaUartRxBuffer[1];
static int      g_i2s_buffer[I2SRBUF_SAMPLES];
static BYTE     g_i2s_audio_buffer[I2SRBUF_SAMPLES * sizeof(short)];
static volatile int g_nActiveGroupIndex;
static HANDLE   g_hAGC = NULL;
static BYTE     g_byaAGCMem[64];
static int      g_nRecordCount = 0;
static int      g_nRBufLostCount = 0;

#ifdef SUPPORT_RECOG_TWO_MODEL
static HANDLE   g_hDSpotter2 = NULL;
static BYTE     g_byaDSpotterMem2[DSPOTTER_MEM_SIZE_2];     // The memory for DSpotter engine.
static HANDLE   g_hCybModel2 = NULL;
static BYTE     g_byaCybModelMem2[CYBMODEL_GET_MEM_USAGE()];// The memory for g_hCybModel2 engine.
static status_command_t stt_BKAV = UNKNOWN;      // sonvhc-bkav declare
static int COMMAND_STAGE_TIME_MIN_Bkav = 6000;
static int COMMAND_STAGE_TIME_MAX_Bkav = 6000;

#endif

#ifdef SUPPORT_VOICE_TAG
static volatile bool g_bS2Pressed = false;
static BYTE g_byaSDVoice[VOICE_TAG_VOICE_SIZE];       // The memory for store DSpotter SD record data
static BYTE g_byaSDModel[VOICE_TAG_MODEL_SIZE];       // The memory for store DSpotter SD trained model, 300 bytes header, 100~300 bytes for every voice tag.
static void VoiceTagProcess(short *lpsRecordSample);
#endif

#ifdef SUPPORT_VAD
static HANDLE   g_hCacheRingBuffer = NULL;
static BYTE     g_byaCacheRingBuffer[RING_BUFFER_GET_MEM_USAGE(RBUF_SIZE_CACHE)];
static HANDLE   g_hVAD = NULL;
static BYTE     g_byaVADMem[5512];

static void     EnterNormalPowerMode();
static void     EnterLowPowerMode();
#endif

/*******************************************************************************
* Function Name   : voice_main
* Description     : Voice application main process
* Arguments       : none
* Return value    : none
*******************************************************************************/
void voice_main(void)
{
    int nRet = true;
    voice_init();

    //----------------BKAV--------------
    DBG_UART_TRACE("BKAV--------- PLAY SOUND ON BOOT --- Test\r\n");
#ifdef SUPPORT_SPEEX_PLAY
        const BYTE *pbySpeexDataBegin = (const BYTE *)&g_uSpeexData1Begin;
        g_bSkipRecordData = TRUE; // Skip record data to avoid data lost or DSpotter recognize it again.
        PlaySpeexStart();
    #ifdef SUPPORT_RECOG_TWO_MODEL
//      if (nRet2 == DSPOTTER_SUCCESS)
            pbySpeexDataBegin = (const BYTE *)&g_uSpeexData2Begin;
    #endif
        int    nMapID = 109;
        if (!PlaySpeexMapID(pbySpeexDataBegin, nMapID))
            DBG_UART_TRACE("Fail to play Speex by MapID(%d).\r\n", nMapID);
        PlaySpeexStop();
        g_bSkipRecordData = FALSE;
     //----------------BKAV------------------
#endif
    //
    while (true == nRet)
    {
        nRet = voice_loop();
    }

    //Go to here when DSPOTTER_ERR_Expired (over trial limit).
    voice_release();
}
/*******************************************************************************
 End of function voice_main
*******************************************************************************/

/*******************************************************************************
* Function Name: voice_init
* Description  : Initialize
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void voice_init(void)
{
    fsp_err_t   err = FSP_SUCCESS;

    /** LED */
    R_IOPORT_PinWrite(&g_ioport_ctrl, LED_G, BSP_IO_LEVEL_HIGH);     //Green   = OFF
    R_IOPORT_PinWrite(&g_ioport_ctrl, LED_R, BSP_IO_LEVEL_HIGH);     //Red     = OFF
    R_IOPORT_PinWrite(&g_ioport_ctrl, LED_Y, BSP_IO_LEVEL_HIGH);     //Yellow  = OFF

    /** UART for DSpotter */
    err = UartOpen(&g_uart_ds, g_uart_ds.p_cfg->channel, 460800);   //baud rate: 460800, to output DSpotter info
    if (FSP_SUCCESS != err)
        DBGTRACE("Failed to open UART %d! error: %d\r\n", g_uart_ds.p_cfg->channel, err);

    //open uart 4 to send command

    err = R_SCI_UART_Open(&cmd_uart_ctrl, &cmd_uart_cfg);

    if(err != FSP_SUCCESS)
    {
       DBGTRACE("Failed to open BKAV UART command  %d! error: %d\r\n", cmd_uart_ctrl.p_cfg->channel, err); 
    }

    //DBG_UART_TRACE("voice_init() Start\r\n");
    
    /** DSpotter */
    ds_decode_init();

    // Process UART read.
    UartAsyncRead(g_byaUartRxBuffer, 1, OnDataReadCompleteCallback);

#ifdef SUPPORT_VOICE_TAG
    err = R_ICU_ExternalIrqOpen(&g_irq_sw1_ctrl, &g_irq_sw1_cfg);
    if(FSP_SUCCESS != err)
    {
        DBG_UART_TRACE("\r\n[Error] R_ICU_ExternalIrqOpen\r\n");
        __asm("BKPT #0\n");
    }

    err = R_ICU_ExternalIrqEnable(&g_irq_sw1_ctrl);
    if(FSP_SUCCESS != err)
    {
        DBG_UART_TRACE("\r\n[Error] R_ICU_ExternalIrqEnable\r\n");
        __asm("BKPT #0\n");
    }
#endif

    /** SW I2S */
    err = R_GPT_Open(&g_timer_sck_ctrl, &g_timer_sck_cfg);
    if(FSP_SUCCESS != err)
    {
        DBG_UART_TRACE("\r\n[Error] R_GPT_Open: SCK\r\n");
        __asm("BKPT #0\n");
    }
    err = R_GPT_Start(&g_timer_sck_ctrl);
    if(FSP_SUCCESS != err)
    {
        DBG_UART_TRACE("\r\n[Error] R_GPT_Start: SCK\r\n");
        __asm("BKPT #0\n");
    }

    err = R_GPT_Open(&g_timer_ws_ctrl, &g_timer_ws_cfg);
    err = R_GPT_CounterSet(&g_timer_ws_ctrl, g_timer_ws_cfg.period_counts - 2);
    if(FSP_SUCCESS != err)
    {
        DBG_UART_TRACE("\r\n[Error] R_GPT_Open: WS\r\n");
        __asm("BKPT #0\n");
    }
    g_timer_ws_ctrl.p_reg->GTIOR_b.GTIOB = 0b00011;     // Output toggled at GTCCRA/GTCCRB compare match
    err = R_GPT_Start(&g_timer_ws_ctrl);
    if(FSP_SUCCESS != err)
    {
        DBG_UART_TRACE("\r\n[Error] R_GPT_Start: WS\r\n");
        __asm("BKPT #0\n");
    }

    err = R_GPT_Open(&g_timer_SSL_ctrl, &g_timer_SSL_cfg);
    err = R_GPT_CounterSet(&g_timer_SSL_ctrl, g_timer_ws_cfg.period_counts - 3);
    if(FSP_SUCCESS != err)
    {
         DBG_UART_TRACE("\r\n[Error] R_GPT_Open: SSL\r\n");
           __asm("BKPT #0\n");
    }
    g_timer_SSL_ctrl.p_reg->GTIOR_b.GTIOB = 0b00011;     // Output toggled at GTCCRA/GTCCRB compare match
    err = R_GPT_Start(&g_timer_SSL_ctrl);
    if(FSP_SUCCESS != err)
    {
         DBG_UART_TRACE("\r\n[Error] R_GPT_Start: SSL\r\n");
         __asm("BKPT #0\n");
    }

    err = R_POEG_Open(&g_poeg_ws_ctrl, &g_poeg_ws_cfg);
    if(FSP_SUCCESS != err)
    {
        DBG_UART_TRACE("\r\n[Error] R_POEG_Open\r\n");
        __asm("BKPT #0\n");
    }

    err = R_SPI_Open(&g_spi_i2s_ctrl, &g_spi_i2s_cfg);
    if(FSP_SUCCESS != err)
    {
        DBG_UART_TRACE("\r\n[Error] R_SPI_Open: I2S\r\n");
        __asm("BKPT #0\n");
    }

    g_bRecording = true;
    while(FSP_SUCCESS != R_SPI_Read (&g_spi_i2s_ctrl, g_i2s_buffer, I2SRBUF_SAMPLES, SPI_BIT_WIDTH_32_BITS));
    R_IOPORT_PinWrite(&g_ioport_ctrl, LED_R, BSP_IO_LEVEL_LOW);     //Red   = ON

}
/*******************************************************************************
 End of function voice_init
*******************************************************************************/

/*******************************************************************************
* Function Name   : voice_loop
* Description     : Voice application process
* Arguments       : none
* Return value    : none
*******************************************************************************/
static bool voice_loop(void)
{
    short   saRecordSample[RECORD_FRAME_SAMPLES];
    int     nRet1 = DSPOTTER_ERR_NeedMoreSample;
    int     nRet2 = DSPOTTER_ERR_NeedMoreSample;
    static int  s_nRBufLostCount = 0;
#if RECOG_FLOW == RECOG_FLOW_TWO_STAGE
    static  int s_nCommandRecordSample = 0;
    static  int s_nCommandRecognizeLimit = COMMAND_STAGE_TIME_MIN;
#endif
#ifdef SUPPORT_VAD
    static int  s_nVADStatus = CYB_VAD_STATUS_NO_SPEECH;
    static BOOL s_bVADEndDetected = FALSE;
#endif

#ifdef SUPPORT_VOICE_TAG
    if (g_bS2Pressed)
    {
    	VoiceTagProcess(saRecordSample);
        g_bS2Pressed = false;
    	return true;
    }
#endif

    // Process UART read.
    if (!UartIsReading())
        UartAsyncRead(g_byaUartRxBuffer, 1, OnDataReadCompleteCallback);

    if (RingBufferGetDataSize(g_hRingBuffer) < RECORD_FRAME_SIZE)
    {
#ifdef SUPPORT_VAD
        // When use VAD and not in no-speech state, try to recognize the data in g_hCacheRingBuffer.
        if (s_nVADStatus != CYB_VAD_STATUS_INIT && s_nVADStatus != CYB_VAD_STATUS_NO_SPEECH)
            goto L_RCOGNIZE;
        else
        	return true;
#else
        return true;
#endif
    }

    if (s_nRBufLostCount != g_nRBufLostCount)
    {
        s_nRBufLostCount = g_nRBufLostCount;
        DBG_UART_TRACE("Lost=%d, Total=%d.\r\n", g_nRBufLostCount, g_nRecordCount);
    }

    // Get recognize data from recognize ring buffer.
    if (RingBufferGetData(g_hRingBuffer, (void*)saRecordSample, RECORD_FRAME_SIZE) != RING_BUFFER_SUCCESS)
        DBG_UART_TRACE("RingBufferGetData() problem \r\n");

    AGC_Do(g_hAGC, saRecordSample, RECORD_FRAME_SAMPLES, saRecordSample);

    if (g_bUartSendRecord)
    {
        int nTransferSize;
        UartWaitWriteReady();
        nTransferSize = Convert2TransferBuffer((BYTE *)saRecordSample, RECORD_FRAME_SIZE, g_byaUartTxBuffer, sizeof(g_byaUartTxBuffer), eFourByteDataOneChecksum);
        UartAsyncWrite(g_byaUartTxBuffer, (uint32_t)nTransferSize);
    }

#ifdef SUPPORT_VAD
    if (s_nVADStatus != CYB_VAD_STATUS_END)
        s_nVADStatus = CybVAD_CheckEPD(g_hVAD, saRecordSample, RECORD_FRAME_SIZE/sizeof(short));

    switch (s_nVADStatus)
    {
    case CYB_VAD_STATUS_INIT:
    case CYB_VAD_STATUS_NO_SPEECH:
        // If VAD ring buffer is full, drop old data for cache recent data.
        if (RingBufferGetFreeSize(g_hCacheRingBuffer) < RECORD_FRAME_SIZE)
            RingBufferDequeueData(g_hCacheRingBuffer, RECORD_FRAME_SIZE);
        RingBufferPutData(g_hCacheRingBuffer, saRecordSample, RECORD_FRAME_SIZE);
        return true;   //Don't need to recognize cache data.

    case CYB_VAD_STATUS_START:
        DBG_UART_TRACE("VAD voice begin.\r\n");
        EnterNormalPowerMode();
        R_IOPORT_PinWrite(&g_ioport_ctrl, LED_G, BSP_IO_LEVEL_LOW);     //Green   = ON

        // Because cache buffer may be full during no-speech state.
        if (RingBufferGetFreeSize(g_hCacheRingBuffer) < RECORD_FRAME_SIZE)
            RingBufferDequeueData(g_hCacheRingBuffer, RECORD_FRAME_SIZE);

        if (RingBufferGetFreeSize(g_hCacheRingBuffer) >= RECORD_FRAME_SIZE)
            RingBufferPutData(g_hCacheRingBuffer, saRecordSample, RECORD_FRAME_SIZE);
        else
            DBG_UART_TRACE("Cache RingBuffer is full 1! \r\n");
        break;  // Go to recognize cache data.

    case CYB_VAD_STATUS_ACTIVE:
        if (RingBufferGetFreeSize(g_hCacheRingBuffer) >= RECORD_FRAME_SIZE)
            RingBufferPutData(g_hCacheRingBuffer, saRecordSample, RECORD_FRAME_SIZE);
        else
            DBG_UART_TRACE("Cache RingBuffer is full 2! \r\n");
        break;  // Go to recognize cache data.

    case CYB_VAD_STATUS_END:
        if (!s_bVADEndDetected)
        {
            DBG_UART_TRACE("VAD voice end.\r\n");
            s_bVADEndDetected = TRUE;
        }

        // Put record buffer to cache buffer.
        if (RingBufferGetFreeSize(g_hCacheRingBuffer) >= RECORD_FRAME_SIZE)
            RingBufferPutData(g_hCacheRingBuffer, saRecordSample, RECORD_FRAME_SIZE);
        else
            DBG_UART_TRACE("Cache RingBuffer is full 3!\r\n");

    #ifdef SUPPORT_RECOG_TWO_MODEL
        if (!DSpotter_IsKeywordAlive(g_hDSpotter1) && !DSpotter_IsKeywordAlive(g_hDSpotter2))
    #else
        if (!DSpotter_IsKeywordAlive(g_hDSpotter1))
    #endif
        {
        #if RECOG_FLOW == RECOG_FLOW_TWO_STAGE
            // For two stage recognition, don't restart VAD at command stage.
            if (g_nActiveGroupIndex == GROUP_INDEX_COMMAND)
                break;  // Go to recognize cache data.
        #endif

            // Re-start VAD.
            DBG_UART_TRACE("VAD re-start.\r\n");
            DBG_UART_TRACE("\r\n");
            CybVAD_ClearEPD(g_hVAD);
            s_bVADEndDetected = FALSE;
            s_nVADStatus = CYB_VAD_STATUS_NO_SPEECH;

            R_IOPORT_PinWrite(&g_ioport_ctrl, LED_G, BSP_IO_LEVEL_HIGH);     //Green   = OFF
            EnterLowPowerMode();
            return true;  //Don't need to recognize cache data.
        }
        break;
    }

L_RCOGNIZE:
    if (RingBufferGetDataSize(g_hCacheRingBuffer) < RECORD_FRAME_SIZE)
        return true;

    RingBufferGetData(g_hCacheRingBuffer, (void*)saRecordSample, RECORD_FRAME_SIZE);
#endif

    // DSpotter AddSample
    nRet1 = DSpotter_AddSample(g_hDSpotter1, saRecordSample, RECORD_FRAME_SAMPLES);
#ifdef SUPPORT_RECOG_TWO_MODEL
    nRet2 = DSpotter_AddSample(g_hDSpotter2, saRecordSample, RECORD_FRAME_SAMPLES);
#endif
    if (nRet1 == DSPOTTER_SUCCESS || nRet2 == DSPOTTER_SUCCESS)
    {
        int  nCmdIndex, nCmdScore, nCmdSGDiff, nCmdEnergy, nMapID = -1;
        char szCommand[32];

        if (nRet1 == DSPOTTER_SUCCESS)
        {
            DSpotter_GetResultScore(g_hDSpotter1, &nCmdScore, &nCmdSGDiff, NULL);
            nCmdIndex = DSpotter_GetResult(g_hDSpotter1);
            nCmdEnergy = DSpotter_GetCmdEnergy(g_hDSpotter1);
#ifdef SUPPORT_VOICE_TAG
            if (nCmdIndex < DSpotter_GetNumWord((BYTE*)CybModelGetGroup(g_hCybModel1, g_nActiveGroupIndex)))
            {
                CybModelGetCommandInfo(g_hCybModel1, g_nActiveGroupIndex, nCmdIndex, szCommand, sizeof(szCommand), &nMapID, NULL);
                DBG_UART_TRACE("Get command sonvhc1 (%d) : %s, Score=%d, SG_Diff=%d, Energy=%d, MapID=%d\r\n\r\n",
                        nCmdIndex, szCommand, nCmdScore, nCmdSGDiff, nCmdEnergy, nMapID);
            }
            else if (nCmdIndex < DSpotter_GetNumWord((BYTE*)CybModelGetGroup(g_hCybModel1, g_nActiveGroupIndex)) + DSpotter_GetNumWord(g_byaSDModel))
            {
                DBG_UART_TRACE("Get voice tag index %d, Score=%d, SG_Diff=%d, Energy=%d\r\n\r\n",
                        (nCmdIndex - DSpotter_GetNumWord((BYTE*)CybModelGetGroup(g_hCybModel1, g_nActiveGroupIndex)))/3,
                        nCmdScore, nCmdSGDiff, nCmdEnergy);
            }
#else
            CybModelGetCommandInfo(g_hCybModel1, g_nActiveGroupIndex, nCmdIndex, szCommand, sizeof(szCommand), &nMapID, NULL);
            DBG_UART_TRACE("Get command sonvhc2 (%d) : %s, Score=%d, SG_Diff=%d, Energy=%d, MapID=%d\r\n\r\n",
                    nCmdIndex, szCommand, nCmdScore, nCmdSGDiff, nCmdEnergy, nMapID);
#endif
        }
#ifdef SUPPORT_RECOG_TWO_MODEL
        else
        {
            DSpotter_GetResultScore(g_hDSpotter2, &nCmdScore, &nCmdSGDiff, NULL);
            nCmdIndex = DSpotter_GetResult(g_hDSpotter2);
            nCmdEnergy = DSpotter_GetCmdEnergy(g_hDSpotter2);
            CybModelGetCommandInfo(g_hCybModel2, g_nActiveGroupIndex, nCmdIndex, szCommand, sizeof(szCommand), &nMapID, NULL);

            DBG_UART_TRACE("Get command sonvhc3(%d) : %s, Score=%d, SG_Diff=%d, Energy=%d, MapID=%d\r\n\r\n",
                    nCmdIndex, szCommand, nCmdScore, nCmdSGDiff, nCmdEnergy, nMapID);
        }
        DSpotter_Reset(g_hDSpotter2);
#endif
        DSpotter_Reset(g_hDSpotter1);

//RESG vv
        // Flicker red LED to indicate the success of recognition.
        R_IOPORT_PinWrite(&g_ioport_ctrl, LED_R, BSP_IO_LEVEL_HIGH); //Red    = OFF
        R_BSP_SoftwareDelay(50U, BSP_DELAY_UNITS_MILLISECONDS);
        R_IOPORT_PinWrite(&g_ioport_ctrl, LED_R, BSP_IO_LEVEL_LOW);  //Red    = ON
//RESG ^^

#ifdef SUPPORT_SPEEX_PLAY
        const BYTE *pbySpeexDataBegin = (const BYTE *)&g_uSpeexData1Begin;
        g_bSkipRecordData = TRUE; // Skip record data to avoid data lost or DSpotter recognize it again.
        PlaySpeexStart();
    #ifdef SUPPORT_RECOG_TWO_MODEL
        if (nRet2 == DSPOTTER_SUCCESS)
            pbySpeexDataBegin = (const BYTE *)&g_uSpeexData2Begin;
    #endif

#endif

                        /*tren 160: hây méc ken*/
                        if (nMapID<130 || (160<=nMapID && nMapID <= 179) ){
                            if(nCmdSGDiff >= 10 && nCmdEnergy >= 999){
                                COMMAND_STAGE_TIME_MIN_Bkav   = 6000;
                                COMMAND_STAGE_TIME_MAX_Bkav   = 6000;
                                s_nCommandRecordSample = 0;
                                stt_BKAV = WAKEUP;
                            }
                            else {
                                nMapID=-1;
                                if(stt_BKAV==STOP)
                                {
                                s_nCommandRecordSample = 1000000000;          // timeout
                                }
                            }

                       }

                    /*130->139: len*/
                        else if(130<=nMapID && nMapID <= 139 ){
                           if(nCmdSGDiff >= 10 && nCmdEnergy >= 999){
                               if( stt_BKAV != UP && stt_BKAV != DOWN  ){
                                             R_SCI_UART_Write(&cmd_uart_ctrl, cmd_stop, sizeof(cmd_stop));
                                             R_BSP_SoftwareDelay(5U, BSP_DELAY_UNITS_MILLISECONDS);
                                             R_SCI_UART_Write(&cmd_uart_ctrl, cmd_up, sizeof(cmd_up));
                                             stt_BKAV=UP;
                                             DBG_UART_TRACE("-----LEN------ ");
                                             COMMAND_STAGE_TIME_MAX_Bkav   = 17000;
                                             COMMAND_STAGE_TIME_MIN_Bkav   = 17000;
                               }
                               else{
                                      nMapID=-1;               //no answer.
                               }

                           }
                           else  nMapID=-1;

                        }

                       /*140->149: xuong*/
                        else if(140<=nMapID && nMapID <= 149){
                               if(nCmdSGDiff >= 15 && nCmdEnergy >= 999 && nCmdScore >= 30 ){
                                   if(stt_BKAV != UP && stt_BKAV != DOWN){
                                            R_SCI_UART_Write(&cmd_uart_ctrl, cmd_stop, sizeof(cmd_stop));
                                            R_BSP_SoftwareDelay(5U, BSP_DELAY_UNITS_MILLISECONDS);
                                            R_SCI_UART_Write(&cmd_uart_ctrl, cmd_down, sizeof(cmd_down));
                                            stt_BKAV=DOWN;
                                            DBG_UART_TRACE("-----XUONG------ ");
                                            COMMAND_STAGE_TIME_MAX_Bkav   = 12000;
                                            COMMAND_STAGE_TIME_MIN_Bkav   = 12000;
                                   }
                                   else{
                                           nMapID=-1;               // no answer
                                   }
                               }
                               else nMapID=-1;
                           }

                       /*150->159: dung*/

                        else if(150 <=nMapID && nMapID <= 159){
                                     if(nCmdSGDiff >= 10 && nCmdEnergy >= 999)
                                     {
                                         DBG_UART_TRACE("-----DUNG------ ");
                                         R_SCI_UART_Write(&cmd_uart_ctrl, cmd_stop, sizeof(cmd_stop));
                                         s_nCommandRecordSample = 1000000000;             // timeout
                                         stt_BKAV=STOP;
                                     }
                                     else
                                     {
                                       nMapID=-1;               // no answer
                                     }

                               }
                        else  nMapID=-1;

                if (!PlaySpeexMapID(pbySpeexDataBegin, nMapID))                          //Response Voice--SonVHc            / PlaySpeexMAPID
                DBG_UART_TRACE("Fail to play Speex by MapID(%d).\r\n", nMapID);
                PlaySpeexStop();                        //
                g_bSkipRecordData = FALSE;

#if RECOG_FLOW == RECOG_FLOW_TWO_STAGE
        s_nCommandRecognizeLimit = COMMAND_STAGE_TIME_MIN_Bkav;
        if (g_nActiveGroupIndex == GROUP_INDEX_TRIGGER)
        {
        	// After trigger word recognized, switch to command recognition mode
            SetDSpotter(g_hCybModel1, GROUP_INDEX_COMMAND, g_byaDSpotterMem1, sizeof(g_byaDSpotterMem1), &g_hDSpotter1);
        #ifdef SUPPORT_RECOG_TWO_MODEL
            SetDSpotter(g_hCybModel2, GROUP_INDEX_COMMAND, g_byaDSpotterMem2, sizeof(g_byaDSpotterMem2), &g_hDSpotter2);
        #endif
            ToggleYellowLED();  //RESG
        }
//RESG vv
        //----------------------------------------------------------------------------
        // OPTION to have VR return to Trigger Mode after every Command accepted
        //
    #ifdef ALWAYS_TRIGGER_MODE
        else
        {
            // OPTION to have VR return to Trigger Mode after every Command accepted
            // After command word recognized, switch back to trigger recognition mode
            SetDSpotter(g_hCybModel1, GROUP_INDEX_TRIGGER, g_byaDSpotterMem1, sizeof(g_byaDSpotterMem1), &g_hDSpotter1);
        #ifdef SUPPORT_RECOG_TWO_MODEL
            SetDSpotter(g_hCybModel2, GROUP_INDEX_TRIGGER, g_byaDSpotterMem2, sizeof(g_byaDSpotterMem2), &g_hDSpotter2);
        #endif
            ToggleYellowLED();  //RESG
        }
    #endif
       //----------------------------------------------------------------------------
//RESG ^^
#endif

    }
    else if (nRet1 == DSPOTTER_ERR_Expired || nRet2 == DSPOTTER_ERR_Expired)
    {
        DBG_UART_TRACE("The trial version DSpotter reach the max trial usage count, please reset system.\r\n");
        return false;
    }
#if RECOG_FLOW == RECOG_FLOW_TWO_STAGE
    else if (nRet1 == DSPOTTER_ERR_NeedMoreSample && nRet2 == DSPOTTER_ERR_NeedMoreSample && g_nActiveGroupIndex == GROUP_INDEX_COMMAND)
    {
        // Check timeout for command recognition mode
        //s_nCommandRecognizeLimit luon gán với 1 thời gian nào đó.
        s_nCommandRecordSample += (int)(RECORD_FRAME_SIZE/sizeof(short));

        if (s_nCommandRecordSample > 16000 / 1000 * s_nCommandRecognizeLimit) //
        {
        #ifdef SUPPORT_RECOG_TWO_MODEL
            if (DSpotter_IsKeywordAlive(g_hDSpotter1) || DSpotter_IsKeywordAlive(g_hDSpotter2))
        #else
            if (DSpotter_IsKeywordAlive(g_hDSpotter1))
        #endif
            {
                if (s_nCommandRecognizeLimit < COMMAND_STAGE_TIME_MAX_Bkav)
                {
                    s_nCommandRecognizeLimit += 500;
                    return true;
                }
            }
            //return true; //sonvhc
            DBG_UART_TRACE("Timeout for command stage, switch to trigger stage.\r\n");
            //s_nCommandRecognizeLimit = COMMAND_STAGE_TIME_MIN;  // COMMAND_STAGE_TIME_MIN =6000


            SetDSpotter(g_hCybModel1, GROUP_INDEX_TRIGGER, g_byaDSpotterMem1, sizeof(g_byaDSpotterMem1), &g_hDSpotter1);
        #ifdef SUPPORT_RECOG_TWO_MODEL
            SetDSpotter(g_hCybModel2, GROUP_INDEX_TRIGGER, g_byaDSpotterMem2, sizeof(g_byaDSpotterMem2), &g_hDSpotter2);
        #endif
            ToggleYellowLED();  //RESG

             COMMAND_STAGE_TIME_MAX_Bkav   = 6000   ;
             COMMAND_STAGE_TIME_MIN_Bkav   = 6000   ;
             stt_BKAV=STOP;                      //timeout
             s_nCommandRecordSample = 0;         //------
        }
    }
#endif

    return true;
}
/*******************************************************************************
 End of function voice_loop
*******************************************************************************/


/*******************************************************************************
* Function Name: voice_release
* Description  : Release resource
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void voice_release(void)
{
    DSpotter_Release(g_hDSpotter1);
    g_hDSpotter1 = NULL;
    CybModelRelease(g_hCybModel1);
    g_hCybModel1 = NULL;
#ifdef SUPPORT_RECOG_TWO_MODEL
    DSpotter_Release(g_hDSpotter2);
    g_hDSpotter2 = NULL;
    CybModelRelease(g_hCybModel2);
    g_hCybModel2 = NULL;
#endif

    g_bRecording = false;
    R_SPI_Close(&g_spi_i2s_ctrl);
    R_GPT_Stop(&g_timer_sck_ctrl);
    R_GPT_Close(&g_timer_sck_ctrl);
    R_GPT_Stop(&g_timer_ws_ctrl);
    R_GPT_Close(&g_timer_ws_ctrl);
    R_GPT_Stop(&g_timer_SSL_ctrl);
    R_GPT_Close(&g_timer_SSL_ctrl);
    R_POEG_Close(&g_poeg_ws_ctrl);

    RingBufferRelease(g_hRingBuffer);
    g_hRingBuffer = NULL;

#ifdef SUPPORT_VAD
    RingBufferRelease(g_hCacheRingBuffer);
    g_hCacheRingBuffer = NULL;
    CybVAD_Release(g_hVAD);
    g_hVAD = NULL;
#endif

    AGC_Release(g_hAGC);
    g_hAGC = NULL;
    UartClose();
    R_SCI_UART_Close(&cmd_uart_ctrl);   //sonvhc

}
/*******************************************************************************
 End of function voice_release
*******************************************************************************/

static int TestByteOrder()
{
    short nTestWord = 0x0001;      //Little endian memory is 0x01 0x00, big endian memory is 0x00 0x01.
    char *b = (char *)&nTestWord;  //Little endian b[0] is 0x01, big endian memory b[0] is 0x00.
    return (int)b[0];              //Return 1/0 for little/big endian.
}

static void SetDSpotter(HANDLE hCybModel, int nGroupIndex, BYTE *pbyaDSpotterMem, int nMemSize, HANDLE *phDSpotter)
{
    int nRet = DSPOTTER_ERR_IllegalParam;
    BYTE *lppbyModel[2];
    int nModelCount = 1;

    if (phDSpotter != NULL)
    {
        DSpotter_Release(*phDSpotter);
        *phDSpotter = NULL;
    }

#ifdef SUPPORT_VOICE_TAG
    if (hCybModel == g_hCybModel1 && DSpotter_GetNumWord(g_byaSDModel) > 0)
    {
        lppbyModel[1] = g_byaSDModel;
        nModelCount = 2;
    }
#endif

    /*
     * For official version of DSpotter, DSpotter_Init_Multi() will use the EEPROM flash API and
     * its instance to check license. In configuration.xml, please:
     *   1. Add r_flash_hp component.
     *   2. Add Flash Driver stack, named "g_flash0" and set callback as "flash0_bgo_callback".
     * For RA6M1, The EEPROM flash address start at 0x40100000U, total 128 blocks, each block has
     * 64 bytes, total 8K bytes. DSpotter need 256 bytes(4 blocks) to save license information.
     * So, the valid flash address is FLASH_HP_DF_BLOCK_ADDRESS(0) ~ FLASH_HP_DF_BLOCK_ADDRESS(123).
    */
    lppbyModel[0] = (BYTE*)CybModelGetGroup(hCybModel, nGroupIndex);
    *phDSpotter = DSpotter_Init_Multi((BYTE *)CybModelGetBase(hCybModel), lppbyModel, nModelCount, MAX_COMMAND_TIME,
                                      pbyaDSpotterMem, nMemSize, FLASH_HP_DF_BLOCK_ADDRESS(0), FLASH_HP_DF_BLOCK_SIZE, &nRet);

    if (*phDSpotter == NULL)
    {
        DBG_UART_TRACE("DSpotter_Init_XXX() fail, error = %d!\r\n", nRet);
        __asm("BKPT #0\n");
    }
    else
    {
        if (hCybModel == g_hCybModel1)
            DBG_UART_TRACE("Model 0, group index %d active.\r\n", nGroupIndex);
        else
            DBG_UART_TRACE("Model 1, group index %d active.\r\n", nGroupIndex);
    }
    g_nActiveGroupIndex = nGroupIndex;
}

static bool ds_decode_init(void)
{
    const bsp_unique_id_t   *devi_uniq_id;
    BYTE *lppbyModel[1];
    int  nGroupCount;
    char szCommand[32];
    int  nMapID;
    int  nMemSize;
    int  nRecogGroupIndex;

    //DBG_UART_TRACE("ds_decode_init() Begin\r\n");
    /** Memory size check, device authentication and ring buffer initialization */
    // Get unique ID
    devi_uniq_id = R_BSP_UniqueIdGet();

    int nByteOrder = TestByteOrder();
    DBG_UART_TRACE("%s\r\n", nByteOrder == 1 ? "Little Endian" : "Big Endian");
    DBG_UART_TRACE("Compile date: %s. Time: %s\r\n", __DATE__, __TIME__);
    DBG_UART_TRACE("Unique ID: 0x%X-%X-%X-%X\r\n", devi_uniq_id->unique_id_words[0],
            devi_uniq_id->unique_id_words[1], devi_uniq_id->unique_id_words[2], devi_uniq_id->unique_id_words[3]);

    if (nByteOrder == 0)
    {
        DBG_UART_TRACE("The DSpotter use Renesas RA default compile setting: CM4/HardFPU/little endian, not big endian!\r\n");
        return FSP_ERR_ASSERTION;
    }

    g_hCybModel1 = CybModelInit((const BYTE*)&uCYModel1Begin, g_byaCybModelMem1, sizeof(g_byaCybModelMem1), NULL);
#ifdef SUPPORT_RECOG_TWO_MODEL
    g_hCybModel2 = CybModelInit((const BYTE*)&uCYModel2Begin, g_byaCybModelMem2, sizeof(g_byaCybModelMem2), NULL);
#endif

    // Check memory for every group and show their commands.
    for (int nModel = 0; nModel < 2; nModel++)
    {
        HANDLE hCybModel;
        int nDSpotterMem;

        if (nModel == 0)
        {
            hCybModel = g_hCybModel1;
            nDSpotterMem = (int)sizeof(g_byaDSpotterMem1);
        }
        else
        {
#ifdef SUPPORT_RECOG_TWO_MODEL
            hCybModel = g_hCybModel2;
            nDSpotterMem = (int)sizeof(g_byaDSpotterMem2);
#else
            break;
#endif
        }

        DBG_UART_TRACE("The DSpotter model %d declare memory buffer size = %d\r\n", nModel, nDSpotterMem);
#ifdef SUPPORT_VOICE_TAG
        if (nModel == 0)
        {
            if (CybModelGetTriMap(g_hCybModel1) == NULL)
            {
                DBG_UART_TRACE("Please use XXX_pack_withTxtAndTri.bin!\r\n");
                __asm("BKPT #0\n");
            }

            nMemSize = DSpotterSD_GetMemoryUsage((BYTE *)CybModelGetBase(g_hCybModel1), (BYTE *)CybModelGetTriMap(g_hCybModel1));
            DBG_UART_TRACE("The voice tag needed working memory size = %d\r\n", nMemSize);
            if (nDSpotterMem < nMemSize)
            {
                DBG_UART_TRACE("The  DSpotter model %d declare memory buffer size is tool small!\r\n", nModel);
                __asm("BKPT #0\n");
            }
        }
#endif
        nGroupCount = CybModelGetGroupCount(hCybModel);
        for (int nGroup = 0; nGroup < nGroupCount; nGroup++)
        {
#if RECOG_FLOW == RECOG_FLOW_ONE_STAGE
            if (nGroup != ONE_STAGE_GROUP_INDEX)
                continue;
#endif

            lppbyModel[0] = (BYTE*)CybModelGetGroup(hCybModel, nGroup);
            nMemSize = DSpotter_GetMemoryUsage_Multi((BYTE *)CybModelGetBase(hCybModel), lppbyModel, 1, MAX_COMMAND_TIME);
            DBG_UART_TRACE("The DSpotter model %d, group %d needed working memory size = %d\r\n", nModel, nGroup, nMemSize);
            if (nDSpotterMem < nMemSize)
            {
                DBG_UART_TRACE("The  DSpotter model %d declare memory buffer size is tool small!\r\n", nModel);
                __asm("BKPT #0\n");
            }

            DBG_UART_TRACE("The model %d, command list of group index %d: \r\n", nModel, nGroup);
            for (int i = 0; i < CybModelGetCommandCount(hCybModel, nGroup); i++)
            {
                CybModelGetCommandInfo(hCybModel, nGroup, i, szCommand, sizeof(szCommand), &nMapID, NULL);
                if (strlen(szCommand) > 0)
                    DBG_UART_TRACE("    %s, Index = %d, Map ID = %d\r\n", szCommand, i, nMapID);
            }
            DBG_UART_TRACE("\r\n");
        }
        DBG_UART_TRACE("\r\n");
    }

    if (RingBufferInit(g_byaRingBuffer, sizeof(g_byaRingBuffer),
                       RBUF_SIZE_RECORD, &g_hRingBuffer) != RING_BUFFER_SUCCESS)
    {
        DBG_UART_TRACE("Record RingBufferInit() fail!\r\n");
        __asm("BKPT #0\n");
    }

    g_hAGC = AGC_Init(g_byaAGCMem, sizeof(g_byaAGCMem), NULL);
    if (g_hAGC == NULL)
    {
        DBG_UART_TRACE("AGC init fail!\r\n");
        __asm("BKPT #0\n");
    }
    AGC_SetMaxGain(g_hAGC, VOLUME_SCALE_RECONG/100.0F);

#if RECOG_FLOW == RECOG_FLOW_TWO_STAGE
    nRecogGroupIndex = GROUP_INDEX_TRIGGER;
#else
    nRecogGroupIndex = ONE_STAGE_GROUP_INDEX;
#endif

    SetDSpotter(g_hCybModel1, nRecogGroupIndex, g_byaDSpotterMem1, sizeof(g_byaDSpotterMem1), &g_hDSpotter1);
#ifdef SUPPORT_RECOG_TWO_MODEL
    SetDSpotter(g_hCybModel2, nRecogGroupIndex, g_byaDSpotterMem2, sizeof(g_byaDSpotterMem2), &g_hDSpotter2);
#endif

#ifdef SUPPORT_VAD
    INT nErr;

    if (RingBufferInit(g_byaCacheRingBuffer, sizeof(g_byaCacheRingBuffer),
                       RBUF_SIZE_CACHE, &g_hCacheRingBuffer) != RING_BUFFER_SUCCESS)
    {
        DBG_UART_TRACE("VAD RingBufferInit() fail!\r\n");
        __asm("BKPT #0\n");
    }

    nMemSize = CybVAD_GetMemoryUsage(16000);
    DBG_UART_TRACE("The VAD needed working memory size = %d\r\n", nMemSize);
    if ((int)sizeof(g_byaVADMem) < nMemSize)
    {
        DBG_UART_TRACE("The VAD memory buffer size is tool small!\r\n");
        __asm("BKPT #0\n");
    }
    g_hVAD = CybVAD_Init(16000, g_byaVADMem, sizeof(g_byaVADMem), FLASH_HP_DF_BLOCK_ADDRESS(0), &nErr);
    CybVAD_SetEndSilenceLength(g_hVAD, 160);
#endif

    //DBG_UART_TRACE("ds_decode_init() End\r\n");

    return true;
}

void OnDataReadCompleteCallback(void)
{
    DBGTRACE("R:%c\r\n", g_byaUartRxBuffer[0]);

    if (g_byaUartRxBuffer[0] == 0x31)
    {
        if (!g_bUartSendRecord)
        {
            g_bUartSendRecord = true;
            DBGTRACE("Start send recording data.\r\n");
        }
    }
    else if (g_byaUartRxBuffer[0] == 0x32)
    {
        if (g_bUartSendRecord)
        {
            g_bUartSendRecord = false;
            DBGTRACE("Stop send recording data.\r\n");
        }
    }

    //Continue to read next byte.
    UartAsyncRead(g_byaUartRxBuffer, 1, OnDataReadCompleteCallback);
}

/* Callback function */
void g_spi_cb(spi_callback_args_t *p_args)
{
	int nDataSize, nFreeSize;

    if( NULL != p_args)
    {
        /* capture callback event for validating the i2s transfer event*/
        if (p_args->event == SPI_EVENT_TRANSFER_COMPLETE)
        {
            for (int i = 0; i < I2SRBUF_SAMPLES; i++)
            {
                g_i2s_audio_buffer[i*2]     = (uint8_t)(g_i2s_buffer[i]>>16);
                g_i2s_audio_buffer[i*2 + 1] = (uint8_t)(g_i2s_buffer[i]>>24);
            }

            nDataSize = sizeof(g_i2s_audio_buffer);
            nFreeSize = RingBufferGetFreeSize(g_hRingBuffer);
            if (nFreeSize < nDataSize)
            {
                DBGTRACE("ERROR : Data lost!\r\n");
                g_nRBufLostCount++;
//RESG                ToggleYellowLED();
                nDataSize = nFreeSize;
            }

			if (!g_bSkipRecordData)
            	RingBufferPutData(g_hRingBuffer, g_i2s_audio_buffer, I2SRBUF_SAMPLES*sizeof(short));

            if (g_bRecording)
                while(FSP_SUCCESS != R_SPI_Read (&g_spi_i2s_ctrl, g_i2s_buffer, I2SRBUF_SAMPLES, SPI_BIT_WIDTH_32_BITS));
        }
    }
    return;
}

static void ToggleYellowLED()
{
    static int nLevel = BSP_IO_LEVEL_LOW;

    R_IOPORT_PinWrite(&g_ioport_ctrl, LED_Y, nLevel);
    if (nLevel == BSP_IO_LEVEL_LOW)
        nLevel = BSP_IO_LEVEL_HIGH;
    else
        nLevel = BSP_IO_LEVEL_LOW;
}

#ifdef SUPPORT_VAD
static void EnterNormalPowerMode()
{
    // Power control: Set to normal frequency mode
    R_IOPORT_PinWrite(&g_ioport_ctrl, LED_G, BSP_IO_LEVEL_LOW);     //Green   = ON
}

static void EnterLowPowerMode()
{
    // Power control: Set to low frequency mode
    R_IOPORT_PinWrite(&g_ioport_ctrl, LED_G, BSP_IO_LEVEL_HIGH);     //Green   = OFF
}
#endif


#ifdef SUPPORT_VOICE_TAG

void g_irq_sw1_cb(external_irq_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    g_bS2Pressed = true;
}

static void VoiceTagProcess(short *lpsRecordSample)
{
    HANDLE hDSpotterSD = NULL;
    int nUsedSize;
    int nRet;

#ifdef SUPPORT_VAD
    EnterNormalPowerMode();
#endif

    // For re-use g_byaDSpotterMem1, we must release g_hDSpotter1.
    DSpotter_Release(g_hDSpotter1);
    g_hDSpotter1 = NULL;

    hDSpotterSD = DSpotterSD_Init((BYTE *)CybModelGetBase(g_hCybModel1), (BYTE *)CybModelGetTriMap(g_hCybModel1),
                                    g_byaDSpotterMem1, sizeof(g_byaDSpotterMem1), &nRet);
    if (hDSpotterSD == NULL)
    {
        DBG_UART_TRACE("DSpotterSD_Init() fail, nRet = %d!\r\n", nRet);
        SetDSpotter(g_hCybModel1, g_nActiveGroupIndex, g_byaDSpotterMem1, sizeof(g_byaDSpotterMem1), &g_hDSpotter1);
    #ifdef SUPPORT_RECOG_TWO_MODEL
        SetDSpotter(g_hCybModel2, g_nActiveGroupIndex, g_byaDSpotterMem2, sizeof(g_byaDSpotterMem2), &g_hDSpotter2);
    #endif
        return;
    }

    DBG_UART_TRACE("\r\nRecord voice tag \r\n");
    R_IOPORT_PinWrite(&g_ioport_ctrl, LED_R, BSP_IO_LEVEL_HIGH); //Red    = OFF

    nRet = DSpotterSD_AddUttrStart(hDSpotterSD, (SHORT*)g_byaSDVoice, VOICE_TAG_VOICE_SIZE);
    if (nRet != DSPOTTER_SUCCESS)
    {
        DBG_UART_TRACE("DSpotterSD_AddUttrStart() fail, nRet = %d!\r\n", nRet);
    }
    else
    {
        DBG_UART_TRACE("Please say voice tag in 5 seconds.\r\n");
        int nCount = 0;

        // Clear recording data.
        RingBufferDequeueData(g_hRingBuffer, RingBufferGetDataSize(g_hRingBuffer));
        // Set gain for record voice tag.
        AGC_SetMaxGain(g_hAGC, VOLUME_SCALE_VOICE_TAG/100.0F);

        while (nCount < 5000/(RECORD_FRAME_SIZE/32))
        {
            if (RingBufferGetDataSize(g_hRingBuffer) < RECORD_FRAME_SIZE)
                continue;

            nCount++;
            RingBufferGetData(g_hRingBuffer, lpsRecordSample, RECORD_FRAME_SIZE);

            AGC_Do(g_hAGC, lpsRecordSample, RECORD_FRAME_SAMPLES, lpsRecordSample);
            nRet = DSpotterSD_AddSample(hDSpotterSD, lpsRecordSample, RECORD_FRAME_SAMPLES);
            if (nRet != DSPOTTER_ERR_NeedMoreSample)
                break;
        }

        if (nRet != DSPOTTER_SUCCESS)
        {
            DBG_UART_TRACE("No clear voice detected in 3 seconds.\r\n");
        }
        else
        {
            g_bSkipRecordData = TRUE;
            nRet = DSpotterSD_AddUttrEnd(hDSpotterSD);
            if (nRet != DSPOTTER_SUCCESS)
            {
                DBG_UART_TRACE("DSpotterSD_AddUttrEnd() fail, nRet = %d!\r\n", nRet);
            }
            else
            {
                // Training stage, and then add voice tag to the model for recognition
                DBG_UART_TRACE("Training start!\r\n");
                nRet = DSpotterSD_TrainWord(hDSpotterSD, (char *)g_byaSDModel, sizeof(g_byaSDModel), &nUsedSize);
                if (nRet != DSPOTTER_SUCCESS)
                    DBG_UART_TRACE("Training fail! nRet = %d \r\n", nRet);
                else
                {
                    // TODO: We may write voice tag model from memory(g_byaSDModel) to flash.

                    DBG_UART_TRACE("Training OK, total %d voice tags! nUsedSize = %d\r\n", DSpotter_GetNumWord(g_byaSDModel)/3, nUsedSize);
                }
            }
            g_bSkipRecordData = FALSE;
        }
    }

    SetDSpotter(g_hCybModel1, g_nActiveGroupIndex, g_byaDSpotterMem1, sizeof(g_byaDSpotterMem1), &g_hDSpotter1);
#ifdef SUPPORT_RECOG_TWO_MODEL
    SetDSpotter(g_hCybModel2, g_nActiveGroupIndex, g_byaDSpotterMem2, sizeof(g_byaDSpotterMem2), &g_hDSpotter2);
#endif

    AGC_SetMaxGain(g_hAGC, VOLUME_SCALE_RECONG/100.0F);

    // Clear recording data.
    RingBufferDequeueData(g_hRingBuffer, RingBufferGetDataSize(g_hRingBuffer));

    R_IOPORT_PinWrite(&g_ioport_ctrl, LED_R, BSP_IO_LEVEL_LOW);  //Red    = ON
}

#endif


/*******************************************************************************
 End Of File
*******************************************************************************/

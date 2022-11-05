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

#ifndef VOICE_MAIN_H
#define VOICE_MAIN_H

/*******************************************************************************
 Include
*******************************************************************************/

/*******************************************************************************
 Macro definitions
*******************************************************************************/
#define RECOG_FLOW_ONE_STAGE     1                       // Only recognize one group, please define ONE_STAGE_GROUP_INDEX.
#define RECOG_FLOW_TWO_STAGE     2                       // To recognize trigger word first, then recognize command word.

#define RECOG_FLOW               RECOG_FLOW_TWO_STAGE
#define SUPPORT_SPEEX_PLAY
#define SUPPORT_RECOG_TWO_MODEL                          // Recognize two model by two DSpotter instance at the same time, the CPU/RAM/Model_ROM usage will double.
//#define SUPPORT_VOICE_TAG
//#define SUPPORT_VAD
//#define ALWAYS_TRIGGER_MODE        //RESG  (Define if selecting Always Return to Trigger Mode after every Command)

#if RECOG_FLOW == RECOG_FLOW_ONE_STAGE
//RESG    #define ONE_STAGE_GROUP_INDEX       GROUP_INDEX_COMMAND   // The index of recognition group(A packed model file may have many groups).
    #define ONE_STAGE_GROUP_INDEX       GROUP_INDEX_TRIGGER   //RESG The index of recognition group(A packed model file may have many groups).
#endif


/*******************************************************************************
 Exported global variables
*******************************************************************************/

/*******************************************************************************
 Exported global functions
*******************************************************************************/

typedef enum status_command
{   UNKNOWN=0,
    WAKEUP=1,
    STOP=2,
    UP=3,
    DOWN=4,        
}status_command_t;

void voice_main(void);

#endif /* VOICE_MAIN_H */

/*******************************************************************************
 End Of File
*******************************************************************************/

/*******************************************************************************
* File Name: QuadDec_Motor_1_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "QuadDec_Motor_1.h"

static QuadDec_Motor_1_BACKUP_STRUCT QuadDec_Motor_1_backup = {0u};


/*******************************************************************************
* Function Name: QuadDec_Motor_1_SaveConfig
********************************************************************************
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void QuadDec_Motor_1_SaveConfig(void) 
{
    #if (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_8_BIT)
        QuadDec_Motor_1_Cnt8_SaveConfig();
    #else 
        /* (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_16_BIT) || 
         * (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_32_BIT)
         */
        QuadDec_Motor_1_Cnt16_SaveConfig();
    #endif /* (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: QuadDec_Motor_1_RestoreConfig
********************************************************************************
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void QuadDec_Motor_1_RestoreConfig(void) 
{
    #if (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_8_BIT)
        QuadDec_Motor_1_Cnt8_RestoreConfig();
    #else 
        /* (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_16_BIT) || 
         * (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_32_BIT) 
         */
        QuadDec_Motor_1_Cnt16_RestoreConfig();
    #endif /* (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: QuadDec_Motor_1_Sleep
********************************************************************************
* 
* Summary:
*  Prepare Quadrature Decoder Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  QuadDec_Motor_1_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void QuadDec_Motor_1_Sleep(void) 
{
    if (0u != (QuadDec_Motor_1_SR_AUX_CONTROL & QuadDec_Motor_1_INTERRUPTS_ENABLE))
    {
        QuadDec_Motor_1_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        QuadDec_Motor_1_backup.enableState = 0u;
    }

    QuadDec_Motor_1_Stop();
    QuadDec_Motor_1_SaveConfig();
}


/*******************************************************************************
* Function Name: QuadDec_Motor_1_Wakeup
********************************************************************************
*
* Summary:
*  Prepare Quadrature Decoder Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  QuadDec_Motor_1_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void QuadDec_Motor_1_Wakeup(void) 
{
    QuadDec_Motor_1_RestoreConfig();

    if (QuadDec_Motor_1_backup.enableState != 0u)
    {
        #if (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_8_BIT)
            QuadDec_Motor_1_Cnt8_Enable();
        #else 
            /* (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_16_BIT) || 
            *  (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_32_BIT) 
            */
            QuadDec_Motor_1_Cnt16_Enable();
        #endif /* (QuadDec_Motor_1_COUNTER_SIZE == QuadDec_Motor_1_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        QuadDec_Motor_1_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */


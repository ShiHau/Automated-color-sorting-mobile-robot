/*******************************************************************************
* File Name: Measure_Timer_PM.c
* Version 2.80
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "Measure_Timer.h"

static Measure_Timer_backupStruct Measure_Timer_backup;


/*******************************************************************************
* Function Name: Measure_Timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Measure_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Measure_Timer_SaveConfig(void) 
{
    #if (!Measure_Timer_UsingFixedFunction)
        Measure_Timer_backup.TimerUdb = Measure_Timer_ReadCounter();
        Measure_Timer_backup.InterruptMaskValue = Measure_Timer_STATUS_MASK;
        #if (Measure_Timer_UsingHWCaptureCounter)
            Measure_Timer_backup.TimerCaptureCounter = Measure_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Measure_Timer_UDB_CONTROL_REG_REMOVED)
            Measure_Timer_backup.TimerControlRegister = Measure_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Measure_Timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Measure_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Measure_Timer_RestoreConfig(void) 
{   
    #if (!Measure_Timer_UsingFixedFunction)

        Measure_Timer_WriteCounter(Measure_Timer_backup.TimerUdb);
        Measure_Timer_STATUS_MASK =Measure_Timer_backup.InterruptMaskValue;
        #if (Measure_Timer_UsingHWCaptureCounter)
            Measure_Timer_SetCaptureCount(Measure_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Measure_Timer_UDB_CONTROL_REG_REMOVED)
            Measure_Timer_WriteControlRegister(Measure_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Measure_Timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Measure_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Measure_Timer_Sleep(void) 
{
    #if(!Measure_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Measure_Timer_CTRL_ENABLE == (Measure_Timer_CONTROL & Measure_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Measure_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Measure_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Measure_Timer_Stop();
    Measure_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Measure_Timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  Measure_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Measure_Timer_Wakeup(void) 
{
    Measure_Timer_RestoreConfig();
    #if(!Measure_Timer_UDB_CONTROL_REG_REMOVED)
        if(Measure_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Measure_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

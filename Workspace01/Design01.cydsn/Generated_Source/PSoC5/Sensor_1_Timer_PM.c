/*******************************************************************************
* File Name: Sensor_1_Timer_PM.c
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

#include "Sensor_1_Timer.h"

static Sensor_1_Timer_backupStruct Sensor_1_Timer_backup;


/*******************************************************************************
* Function Name: Sensor_1_Timer_SaveConfig
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
*  Sensor_1_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Sensor_1_Timer_SaveConfig(void) 
{
    #if (!Sensor_1_Timer_UsingFixedFunction)
        Sensor_1_Timer_backup.TimerUdb = Sensor_1_Timer_ReadCounter();
        Sensor_1_Timer_backup.InterruptMaskValue = Sensor_1_Timer_STATUS_MASK;
        #if (Sensor_1_Timer_UsingHWCaptureCounter)
            Sensor_1_Timer_backup.TimerCaptureCounter = Sensor_1_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!Sensor_1_Timer_UDB_CONTROL_REG_REMOVED)
            Sensor_1_Timer_backup.TimerControlRegister = Sensor_1_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: Sensor_1_Timer_RestoreConfig
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
*  Sensor_1_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Sensor_1_Timer_RestoreConfig(void) 
{   
    #if (!Sensor_1_Timer_UsingFixedFunction)

        Sensor_1_Timer_WriteCounter(Sensor_1_Timer_backup.TimerUdb);
        Sensor_1_Timer_STATUS_MASK =Sensor_1_Timer_backup.InterruptMaskValue;
        #if (Sensor_1_Timer_UsingHWCaptureCounter)
            Sensor_1_Timer_SetCaptureCount(Sensor_1_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!Sensor_1_Timer_UDB_CONTROL_REG_REMOVED)
            Sensor_1_Timer_WriteControlRegister(Sensor_1_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: Sensor_1_Timer_Sleep
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
*  Sensor_1_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void Sensor_1_Timer_Sleep(void) 
{
    #if(!Sensor_1_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(Sensor_1_Timer_CTRL_ENABLE == (Sensor_1_Timer_CONTROL & Sensor_1_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            Sensor_1_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            Sensor_1_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    Sensor_1_Timer_Stop();
    Sensor_1_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: Sensor_1_Timer_Wakeup
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
*  Sensor_1_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Sensor_1_Timer_Wakeup(void) 
{
    Sensor_1_Timer_RestoreConfig();
    #if(!Sensor_1_Timer_UDB_CONTROL_REG_REMOVED)
        if(Sensor_1_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                Sensor_1_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */

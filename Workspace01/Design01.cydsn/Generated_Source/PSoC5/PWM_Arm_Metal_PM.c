/*******************************************************************************
* File Name: PWM_Arm_Metal_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_Arm_Metal.h"

static PWM_Arm_Metal_backupStruct PWM_Arm_Metal_backup;


/*******************************************************************************
* Function Name: PWM_Arm_Metal_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Arm_Metal_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_Arm_Metal_SaveConfig(void) 
{

    #if(!PWM_Arm_Metal_UsingFixedFunction)
        #if(!PWM_Arm_Metal_PWMModeIsCenterAligned)
            PWM_Arm_Metal_backup.PWMPeriod = PWM_Arm_Metal_ReadPeriod();
        #endif /* (!PWM_Arm_Metal_PWMModeIsCenterAligned) */
        PWM_Arm_Metal_backup.PWMUdb = PWM_Arm_Metal_ReadCounter();
        #if (PWM_Arm_Metal_UseStatus)
            PWM_Arm_Metal_backup.InterruptMaskValue = PWM_Arm_Metal_STATUS_MASK;
        #endif /* (PWM_Arm_Metal_UseStatus) */

        #if(PWM_Arm_Metal_DeadBandMode == PWM_Arm_Metal__B_PWM__DBM_256_CLOCKS || \
            PWM_Arm_Metal_DeadBandMode == PWM_Arm_Metal__B_PWM__DBM_2_4_CLOCKS)
            PWM_Arm_Metal_backup.PWMdeadBandValue = PWM_Arm_Metal_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_Arm_Metal_KillModeMinTime)
             PWM_Arm_Metal_backup.PWMKillCounterPeriod = PWM_Arm_Metal_ReadKillTime();
        #endif /* (PWM_Arm_Metal_KillModeMinTime) */

        #if(PWM_Arm_Metal_UseControl)
            PWM_Arm_Metal_backup.PWMControlRegister = PWM_Arm_Metal_ReadControlRegister();
        #endif /* (PWM_Arm_Metal_UseControl) */
    #endif  /* (!PWM_Arm_Metal_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_Arm_Metal_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Arm_Metal_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Arm_Metal_RestoreConfig(void) 
{
        #if(!PWM_Arm_Metal_UsingFixedFunction)
            #if(!PWM_Arm_Metal_PWMModeIsCenterAligned)
                PWM_Arm_Metal_WritePeriod(PWM_Arm_Metal_backup.PWMPeriod);
            #endif /* (!PWM_Arm_Metal_PWMModeIsCenterAligned) */

            PWM_Arm_Metal_WriteCounter(PWM_Arm_Metal_backup.PWMUdb);

            #if (PWM_Arm_Metal_UseStatus)
                PWM_Arm_Metal_STATUS_MASK = PWM_Arm_Metal_backup.InterruptMaskValue;
            #endif /* (PWM_Arm_Metal_UseStatus) */

            #if(PWM_Arm_Metal_DeadBandMode == PWM_Arm_Metal__B_PWM__DBM_256_CLOCKS || \
                PWM_Arm_Metal_DeadBandMode == PWM_Arm_Metal__B_PWM__DBM_2_4_CLOCKS)
                PWM_Arm_Metal_WriteDeadTime(PWM_Arm_Metal_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_Arm_Metal_KillModeMinTime)
                PWM_Arm_Metal_WriteKillTime(PWM_Arm_Metal_backup.PWMKillCounterPeriod);
            #endif /* (PWM_Arm_Metal_KillModeMinTime) */

            #if(PWM_Arm_Metal_UseControl)
                PWM_Arm_Metal_WriteControlRegister(PWM_Arm_Metal_backup.PWMControlRegister);
            #endif /* (PWM_Arm_Metal_UseControl) */
        #endif  /* (!PWM_Arm_Metal_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_Arm_Metal_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Arm_Metal_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_Arm_Metal_Sleep(void) 
{
    #if(PWM_Arm_Metal_UseControl)
        if(PWM_Arm_Metal_CTRL_ENABLE == (PWM_Arm_Metal_CONTROL & PWM_Arm_Metal_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_Arm_Metal_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_Arm_Metal_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_Arm_Metal_UseControl) */

    /* Stop component */
    PWM_Arm_Metal_Stop();

    /* Save registers configuration */
    PWM_Arm_Metal_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_Arm_Metal_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWM_Arm_Metal_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_Arm_Metal_Wakeup(void) 
{
     /* Restore registers values */
    PWM_Arm_Metal_RestoreConfig();

    if(PWM_Arm_Metal_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_Arm_Metal_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

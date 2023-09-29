/*******************************************************************************
* File Name: Counter_RGB_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Counter_RGB.h"

static Counter_RGB_backupStruct Counter_RGB_backup;


/*******************************************************************************
* Function Name: Counter_RGB_SaveConfig
********************************************************************************
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
*  Counter_RGB_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Counter_RGB_SaveConfig(void) 
{
    #if (!Counter_RGB_UsingFixedFunction)

        Counter_RGB_backup.CounterUdb = Counter_RGB_ReadCounter();

        #if(!Counter_RGB_ControlRegRemoved)
            Counter_RGB_backup.CounterControlRegister = Counter_RGB_ReadControlRegister();
        #endif /* (!Counter_RGB_ControlRegRemoved) */

    #endif /* (!Counter_RGB_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_RGB_RestoreConfig
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
*  Counter_RGB_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_RGB_RestoreConfig(void) 
{      
    #if (!Counter_RGB_UsingFixedFunction)

       Counter_RGB_WriteCounter(Counter_RGB_backup.CounterUdb);

        #if(!Counter_RGB_ControlRegRemoved)
            Counter_RGB_WriteControlRegister(Counter_RGB_backup.CounterControlRegister);
        #endif /* (!Counter_RGB_ControlRegRemoved) */

    #endif /* (!Counter_RGB_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Counter_RGB_Sleep
********************************************************************************
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
*  Counter_RGB_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Counter_RGB_Sleep(void) 
{
    #if(!Counter_RGB_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Counter_RGB_CTRL_ENABLE == (Counter_RGB_CONTROL & Counter_RGB_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Counter_RGB_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Counter_RGB_backup.CounterEnableState = 0u;
        }
    #else
        Counter_RGB_backup.CounterEnableState = 1u;
        if(Counter_RGB_backup.CounterEnableState != 0u)
        {
            Counter_RGB_backup.CounterEnableState = 0u;
        }
    #endif /* (!Counter_RGB_ControlRegRemoved) */
    
    Counter_RGB_Stop();
    Counter_RGB_SaveConfig();
}


/*******************************************************************************
* Function Name: Counter_RGB_Wakeup
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
*  Counter_RGB_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Counter_RGB_Wakeup(void) 
{
    Counter_RGB_RestoreConfig();
    #if(!Counter_RGB_ControlRegRemoved)
        if(Counter_RGB_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Counter_RGB_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Counter_RGB_ControlRegRemoved) */
    
}


/* [] END OF FILE */

/*******************************************************************************
* File Name: Color_LED_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Color_LED.h"

/* Check for removal by optimization */
#if !defined(Color_LED_Sync_ctrl_reg__REMOVED)

static Color_LED_BACKUP_STRUCT  Color_LED_backup = {0u};

    
/*******************************************************************************
* Function Name: Color_LED_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Color_LED_SaveConfig(void) 
{
    Color_LED_backup.controlState = Color_LED_Control;
}


/*******************************************************************************
* Function Name: Color_LED_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void Color_LED_RestoreConfig(void) 
{
     Color_LED_Control = Color_LED_backup.controlState;
}


/*******************************************************************************
* Function Name: Color_LED_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Color_LED_Sleep(void) 
{
    Color_LED_SaveConfig();
}


/*******************************************************************************
* Function Name: Color_LED_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void Color_LED_Wakeup(void)  
{
    Color_LED_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */

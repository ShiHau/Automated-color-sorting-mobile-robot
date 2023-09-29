/*******************************************************************************
* File Name: Color_LED_Reg_PM.c
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

#include "Color_LED_Reg.h"

/* Check for removal by optimization */
#if !defined(Color_LED_Reg_Sync_ctrl_reg__REMOVED)

static Color_LED_Reg_BACKUP_STRUCT  Color_LED_Reg_backup = {0u};

    
/*******************************************************************************
* Function Name: Color_LED_Reg_SaveConfig
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
void Color_LED_Reg_SaveConfig(void) 
{
    Color_LED_Reg_backup.controlState = Color_LED_Reg_Control;
}


/*******************************************************************************
* Function Name: Color_LED_Reg_RestoreConfig
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
void Color_LED_Reg_RestoreConfig(void) 
{
     Color_LED_Reg_Control = Color_LED_Reg_backup.controlState;
}


/*******************************************************************************
* Function Name: Color_LED_Reg_Sleep
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
void Color_LED_Reg_Sleep(void) 
{
    Color_LED_Reg_SaveConfig();
}


/*******************************************************************************
* Function Name: Color_LED_Reg_Wakeup
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
void Color_LED_Reg_Wakeup(void)  
{
    Color_LED_Reg_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */

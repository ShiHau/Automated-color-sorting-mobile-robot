/*******************************************************************************
* File Name: Zones_75_Reg_PM.c
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

#include "Zones_75_Reg.h"

/* Check for removal by optimization */
#if !defined(Zones_75_Reg_Sync_ctrl_reg__REMOVED)

static Zones_75_Reg_BACKUP_STRUCT  Zones_75_Reg_backup = {0u};

    
/*******************************************************************************
* Function Name: Zones_75_Reg_SaveConfig
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
void Zones_75_Reg_SaveConfig(void) 
{
    Zones_75_Reg_backup.controlState = Zones_75_Reg_Control;
}


/*******************************************************************************
* Function Name: Zones_75_Reg_RestoreConfig
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
void Zones_75_Reg_RestoreConfig(void) 
{
     Zones_75_Reg_Control = Zones_75_Reg_backup.controlState;
}


/*******************************************************************************
* Function Name: Zones_75_Reg_Sleep
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
void Zones_75_Reg_Sleep(void) 
{
    Zones_75_Reg_SaveConfig();
}


/*******************************************************************************
* Function Name: Zones_75_Reg_Wakeup
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
void Zones_75_Reg_Wakeup(void)  
{
    Zones_75_Reg_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */

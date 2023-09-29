/*******************************************************************************
* File Name: Zones_68_Reg.c  
* Version 1.80
*
* Description:
*  This file contains API to enable firmware control of a Control Register.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Zones_68_Reg.h"

/* Check for removal by optimization */
#if !defined(Zones_68_Reg_Sync_ctrl_reg__REMOVED)

    
/*******************************************************************************
* Function Name: Zones_68_Reg_Write
********************************************************************************
*
* Summary:
*  Write a byte to the Control Register.
*
* Parameters:
*  control:  The value to be assigned to the Control Register.
*
* Return:
*  None.
*
*******************************************************************************/
void Zones_68_Reg_Write(uint8 control) 
{
    Zones_68_Reg_Control = control;
}


/*******************************************************************************
* Function Name: Zones_68_Reg_Read
********************************************************************************
*
* Summary:
*  Reads the current value assigned to the Control Register.
*
* Parameters:
*  None.
*
* Return:
*  Returns the current value in the Control Register.
*
*******************************************************************************/
uint8 Zones_68_Reg_Read(void) 
{
    return Zones_68_Reg_Control;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */

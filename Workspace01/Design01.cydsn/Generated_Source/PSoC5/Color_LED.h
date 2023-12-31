/*******************************************************************************
* File Name: Color_LED.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_Color_LED_H) /* CY_CONTROL_REG_Color_LED_H */
#define CY_CONTROL_REG_Color_LED_H

#include "cyfitter.h"

#if ((CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4) || \
     (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5))
    #include "cytypes.h"
#else
    #include "syslib/cy_syslib.h"
#endif

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} Color_LED_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    Color_LED_Write(uint8 control) ;
uint8   Color_LED_Read(void) ;

void Color_LED_SaveConfig(void) ;
void Color_LED_RestoreConfig(void) ;
void Color_LED_Sleep(void) ; 
void Color_LED_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define Color_LED_Control        (* (reg8 *) Color_LED_Sync_ctrl_reg__CONTROL_REG )
#define Color_LED_Control_PTR    (  (reg8 *) Color_LED_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_Color_LED_H */


/* [] END OF FILE */

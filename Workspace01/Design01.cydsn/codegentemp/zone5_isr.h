/*******************************************************************************
* File Name: zone5_isr.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_zone5_isr_H)
#define CY_ISR_zone5_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void zone5_isr_Start(void);
void zone5_isr_StartEx(cyisraddress address);
void zone5_isr_Stop(void);

CY_ISR_PROTO(zone5_isr_Interrupt);

void zone5_isr_SetVector(cyisraddress address);
cyisraddress zone5_isr_GetVector(void);

void zone5_isr_SetPriority(uint8 priority);
uint8 zone5_isr_GetPriority(void);

void zone5_isr_Enable(void);
uint8 zone5_isr_GetState(void);
void zone5_isr_Disable(void);

void zone5_isr_SetPending(void);
void zone5_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the zone5_isr ISR. */
#define zone5_isr_INTC_VECTOR            ((reg32 *) zone5_isr__INTC_VECT)

/* Address of the zone5_isr ISR priority. */
#define zone5_isr_INTC_PRIOR             ((reg8 *) zone5_isr__INTC_PRIOR_REG)

/* Priority of the zone5_isr interrupt. */
#define zone5_isr_INTC_PRIOR_NUMBER      zone5_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable zone5_isr interrupt. */
#define zone5_isr_INTC_SET_EN            ((reg32 *) zone5_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the zone5_isr interrupt. */
#define zone5_isr_INTC_CLR_EN            ((reg32 *) zone5_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the zone5_isr interrupt state to pending. */
#define zone5_isr_INTC_SET_PD            ((reg32 *) zone5_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the zone5_isr interrupt. */
#define zone5_isr_INTC_CLR_PD            ((reg32 *) zone5_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_zone5_isr_H */


/* [] END OF FILE */

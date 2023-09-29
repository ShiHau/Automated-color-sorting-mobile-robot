/*******************************************************************************
* File Name: zone8_isr.h
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
#if !defined(CY_ISR_zone8_isr_H)
#define CY_ISR_zone8_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void zone8_isr_Start(void);
void zone8_isr_StartEx(cyisraddress address);
void zone8_isr_Stop(void);

CY_ISR_PROTO(zone8_isr_Interrupt);

void zone8_isr_SetVector(cyisraddress address);
cyisraddress zone8_isr_GetVector(void);

void zone8_isr_SetPriority(uint8 priority);
uint8 zone8_isr_GetPriority(void);

void zone8_isr_Enable(void);
uint8 zone8_isr_GetState(void);
void zone8_isr_Disable(void);

void zone8_isr_SetPending(void);
void zone8_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the zone8_isr ISR. */
#define zone8_isr_INTC_VECTOR            ((reg32 *) zone8_isr__INTC_VECT)

/* Address of the zone8_isr ISR priority. */
#define zone8_isr_INTC_PRIOR             ((reg8 *) zone8_isr__INTC_PRIOR_REG)

/* Priority of the zone8_isr interrupt. */
#define zone8_isr_INTC_PRIOR_NUMBER      zone8_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable zone8_isr interrupt. */
#define zone8_isr_INTC_SET_EN            ((reg32 *) zone8_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the zone8_isr interrupt. */
#define zone8_isr_INTC_CLR_EN            ((reg32 *) zone8_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the zone8_isr interrupt state to pending. */
#define zone8_isr_INTC_SET_PD            ((reg32 *) zone8_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the zone8_isr interrupt. */
#define zone8_isr_INTC_CLR_PD            ((reg32 *) zone8_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_zone8_isr_H */


/* [] END OF FILE */

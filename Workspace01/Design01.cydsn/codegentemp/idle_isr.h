/*******************************************************************************
* File Name: idle_isr.h
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
#if !defined(CY_ISR_idle_isr_H)
#define CY_ISR_idle_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void idle_isr_Start(void);
void idle_isr_StartEx(cyisraddress address);
void idle_isr_Stop(void);

CY_ISR_PROTO(idle_isr_Interrupt);

void idle_isr_SetVector(cyisraddress address);
cyisraddress idle_isr_GetVector(void);

void idle_isr_SetPriority(uint8 priority);
uint8 idle_isr_GetPriority(void);

void idle_isr_Enable(void);
uint8 idle_isr_GetState(void);
void idle_isr_Disable(void);

void idle_isr_SetPending(void);
void idle_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the idle_isr ISR. */
#define idle_isr_INTC_VECTOR            ((reg32 *) idle_isr__INTC_VECT)

/* Address of the idle_isr ISR priority. */
#define idle_isr_INTC_PRIOR             ((reg8 *) idle_isr__INTC_PRIOR_REG)

/* Priority of the idle_isr interrupt. */
#define idle_isr_INTC_PRIOR_NUMBER      idle_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable idle_isr interrupt. */
#define idle_isr_INTC_SET_EN            ((reg32 *) idle_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the idle_isr interrupt. */
#define idle_isr_INTC_CLR_EN            ((reg32 *) idle_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the idle_isr interrupt state to pending. */
#define idle_isr_INTC_SET_PD            ((reg32 *) idle_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the idle_isr interrupt. */
#define idle_isr_INTC_CLR_PD            ((reg32 *) idle_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_idle_isr_H */


/* [] END OF FILE */

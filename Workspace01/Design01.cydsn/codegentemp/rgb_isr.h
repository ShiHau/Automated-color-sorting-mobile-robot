/*******************************************************************************
* File Name: rgb_isr.h
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
#if !defined(CY_ISR_rgb_isr_H)
#define CY_ISR_rgb_isr_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void rgb_isr_Start(void);
void rgb_isr_StartEx(cyisraddress address);
void rgb_isr_Stop(void);

CY_ISR_PROTO(rgb_isr_Interrupt);

void rgb_isr_SetVector(cyisraddress address);
cyisraddress rgb_isr_GetVector(void);

void rgb_isr_SetPriority(uint8 priority);
uint8 rgb_isr_GetPriority(void);

void rgb_isr_Enable(void);
uint8 rgb_isr_GetState(void);
void rgb_isr_Disable(void);

void rgb_isr_SetPending(void);
void rgb_isr_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the rgb_isr ISR. */
#define rgb_isr_INTC_VECTOR            ((reg32 *) rgb_isr__INTC_VECT)

/* Address of the rgb_isr ISR priority. */
#define rgb_isr_INTC_PRIOR             ((reg8 *) rgb_isr__INTC_PRIOR_REG)

/* Priority of the rgb_isr interrupt. */
#define rgb_isr_INTC_PRIOR_NUMBER      rgb_isr__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable rgb_isr interrupt. */
#define rgb_isr_INTC_SET_EN            ((reg32 *) rgb_isr__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the rgb_isr interrupt. */
#define rgb_isr_INTC_CLR_EN            ((reg32 *) rgb_isr__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the rgb_isr interrupt state to pending. */
#define rgb_isr_INTC_SET_PD            ((reg32 *) rgb_isr__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the rgb_isr interrupt. */
#define rgb_isr_INTC_CLR_PD            ((reg32 *) rgb_isr__INTC_CLR_PD_REG)


#endif /* CY_ISR_rgb_isr_H */


/* [] END OF FILE */

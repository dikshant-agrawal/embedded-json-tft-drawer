/*******************************************************************************
* File Name: p_JSON.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_p_JSON_H) /* Pins p_JSON_H */
#define CY_PINS_p_JSON_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "p_JSON_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 p_JSON__PORT == 15 && ((p_JSON__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    p_JSON_Write(uint8 value);
void    p_JSON_SetDriveMode(uint8 mode);
uint8   p_JSON_ReadDataReg(void);
uint8   p_JSON_Read(void);
void    p_JSON_SetInterruptMode(uint16 position, uint16 mode);
uint8   p_JSON_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the p_JSON_SetDriveMode() function.
     *  @{
     */
        #define p_JSON_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define p_JSON_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define p_JSON_DM_RES_UP          PIN_DM_RES_UP
        #define p_JSON_DM_RES_DWN         PIN_DM_RES_DWN
        #define p_JSON_DM_OD_LO           PIN_DM_OD_LO
        #define p_JSON_DM_OD_HI           PIN_DM_OD_HI
        #define p_JSON_DM_STRONG          PIN_DM_STRONG
        #define p_JSON_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define p_JSON_MASK               p_JSON__MASK
#define p_JSON_SHIFT              p_JSON__SHIFT
#define p_JSON_WIDTH              1u

/* Interrupt constants */
#if defined(p_JSON__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in p_JSON_SetInterruptMode() function.
     *  @{
     */
        #define p_JSON_INTR_NONE      (uint16)(0x0000u)
        #define p_JSON_INTR_RISING    (uint16)(0x0001u)
        #define p_JSON_INTR_FALLING   (uint16)(0x0002u)
        #define p_JSON_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define p_JSON_INTR_MASK      (0x01u) 
#endif /* (p_JSON__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define p_JSON_PS                     (* (reg8 *) p_JSON__PS)
/* Data Register */
#define p_JSON_DR                     (* (reg8 *) p_JSON__DR)
/* Port Number */
#define p_JSON_PRT_NUM                (* (reg8 *) p_JSON__PRT) 
/* Connect to Analog Globals */                                                  
#define p_JSON_AG                     (* (reg8 *) p_JSON__AG)                       
/* Analog MUX bux enable */
#define p_JSON_AMUX                   (* (reg8 *) p_JSON__AMUX) 
/* Bidirectional Enable */                                                        
#define p_JSON_BIE                    (* (reg8 *) p_JSON__BIE)
/* Bit-mask for Aliased Register Access */
#define p_JSON_BIT_MASK               (* (reg8 *) p_JSON__BIT_MASK)
/* Bypass Enable */
#define p_JSON_BYP                    (* (reg8 *) p_JSON__BYP)
/* Port wide control signals */                                                   
#define p_JSON_CTL                    (* (reg8 *) p_JSON__CTL)
/* Drive Modes */
#define p_JSON_DM0                    (* (reg8 *) p_JSON__DM0) 
#define p_JSON_DM1                    (* (reg8 *) p_JSON__DM1)
#define p_JSON_DM2                    (* (reg8 *) p_JSON__DM2) 
/* Input Buffer Disable Override */
#define p_JSON_INP_DIS                (* (reg8 *) p_JSON__INP_DIS)
/* LCD Common or Segment Drive */
#define p_JSON_LCD_COM_SEG            (* (reg8 *) p_JSON__LCD_COM_SEG)
/* Enable Segment LCD */
#define p_JSON_LCD_EN                 (* (reg8 *) p_JSON__LCD_EN)
/* Slew Rate Control */
#define p_JSON_SLW                    (* (reg8 *) p_JSON__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define p_JSON_PRTDSI__CAPS_SEL       (* (reg8 *) p_JSON__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define p_JSON_PRTDSI__DBL_SYNC_IN    (* (reg8 *) p_JSON__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define p_JSON_PRTDSI__OE_SEL0        (* (reg8 *) p_JSON__PRTDSI__OE_SEL0) 
#define p_JSON_PRTDSI__OE_SEL1        (* (reg8 *) p_JSON__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define p_JSON_PRTDSI__OUT_SEL0       (* (reg8 *) p_JSON__PRTDSI__OUT_SEL0) 
#define p_JSON_PRTDSI__OUT_SEL1       (* (reg8 *) p_JSON__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define p_JSON_PRTDSI__SYNC_OUT       (* (reg8 *) p_JSON__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(p_JSON__SIO_CFG)
    #define p_JSON_SIO_HYST_EN        (* (reg8 *) p_JSON__SIO_HYST_EN)
    #define p_JSON_SIO_REG_HIFREQ     (* (reg8 *) p_JSON__SIO_REG_HIFREQ)
    #define p_JSON_SIO_CFG            (* (reg8 *) p_JSON__SIO_CFG)
    #define p_JSON_SIO_DIFF           (* (reg8 *) p_JSON__SIO_DIFF)
#endif /* (p_JSON__SIO_CFG) */

/* Interrupt Registers */
#if defined(p_JSON__INTSTAT)
    #define p_JSON_INTSTAT            (* (reg8 *) p_JSON__INTSTAT)
    #define p_JSON_SNAP               (* (reg8 *) p_JSON__SNAP)
    
	#define p_JSON_0_INTTYPE_REG 		(* (reg8 *) p_JSON__0__INTTYPE)
#endif /* (p_JSON__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_p_JSON_H */


/* [] END OF FILE */

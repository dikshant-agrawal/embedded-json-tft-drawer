/*******************************************************************************
* File Name: JSON.h  
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

#if !defined(CY_PINS_JSON_H) /* Pins JSON_H */
#define CY_PINS_JSON_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "JSON_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 JSON__PORT == 15 && ((JSON__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    JSON_Write(uint8 value);
void    JSON_SetDriveMode(uint8 mode);
uint8   JSON_ReadDataReg(void);
uint8   JSON_Read(void);
void    JSON_SetInterruptMode(uint16 position, uint16 mode);
uint8   JSON_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the JSON_SetDriveMode() function.
     *  @{
     */
        #define JSON_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define JSON_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define JSON_DM_RES_UP          PIN_DM_RES_UP
        #define JSON_DM_RES_DWN         PIN_DM_RES_DWN
        #define JSON_DM_OD_LO           PIN_DM_OD_LO
        #define JSON_DM_OD_HI           PIN_DM_OD_HI
        #define JSON_DM_STRONG          PIN_DM_STRONG
        #define JSON_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define JSON_MASK               JSON__MASK
#define JSON_SHIFT              JSON__SHIFT
#define JSON_WIDTH              1u

/* Interrupt constants */
#if defined(JSON__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in JSON_SetInterruptMode() function.
     *  @{
     */
        #define JSON_INTR_NONE      (uint16)(0x0000u)
        #define JSON_INTR_RISING    (uint16)(0x0001u)
        #define JSON_INTR_FALLING   (uint16)(0x0002u)
        #define JSON_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define JSON_INTR_MASK      (0x01u) 
#endif /* (JSON__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define JSON_PS                     (* (reg8 *) JSON__PS)
/* Data Register */
#define JSON_DR                     (* (reg8 *) JSON__DR)
/* Port Number */
#define JSON_PRT_NUM                (* (reg8 *) JSON__PRT) 
/* Connect to Analog Globals */                                                  
#define JSON_AG                     (* (reg8 *) JSON__AG)                       
/* Analog MUX bux enable */
#define JSON_AMUX                   (* (reg8 *) JSON__AMUX) 
/* Bidirectional Enable */                                                        
#define JSON_BIE                    (* (reg8 *) JSON__BIE)
/* Bit-mask for Aliased Register Access */
#define JSON_BIT_MASK               (* (reg8 *) JSON__BIT_MASK)
/* Bypass Enable */
#define JSON_BYP                    (* (reg8 *) JSON__BYP)
/* Port wide control signals */                                                   
#define JSON_CTL                    (* (reg8 *) JSON__CTL)
/* Drive Modes */
#define JSON_DM0                    (* (reg8 *) JSON__DM0) 
#define JSON_DM1                    (* (reg8 *) JSON__DM1)
#define JSON_DM2                    (* (reg8 *) JSON__DM2) 
/* Input Buffer Disable Override */
#define JSON_INP_DIS                (* (reg8 *) JSON__INP_DIS)
/* LCD Common or Segment Drive */
#define JSON_LCD_COM_SEG            (* (reg8 *) JSON__LCD_COM_SEG)
/* Enable Segment LCD */
#define JSON_LCD_EN                 (* (reg8 *) JSON__LCD_EN)
/* Slew Rate Control */
#define JSON_SLW                    (* (reg8 *) JSON__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define JSON_PRTDSI__CAPS_SEL       (* (reg8 *) JSON__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define JSON_PRTDSI__DBL_SYNC_IN    (* (reg8 *) JSON__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define JSON_PRTDSI__OE_SEL0        (* (reg8 *) JSON__PRTDSI__OE_SEL0) 
#define JSON_PRTDSI__OE_SEL1        (* (reg8 *) JSON__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define JSON_PRTDSI__OUT_SEL0       (* (reg8 *) JSON__PRTDSI__OUT_SEL0) 
#define JSON_PRTDSI__OUT_SEL1       (* (reg8 *) JSON__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define JSON_PRTDSI__SYNC_OUT       (* (reg8 *) JSON__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(JSON__SIO_CFG)
    #define JSON_SIO_HYST_EN        (* (reg8 *) JSON__SIO_HYST_EN)
    #define JSON_SIO_REG_HIFREQ     (* (reg8 *) JSON__SIO_REG_HIFREQ)
    #define JSON_SIO_CFG            (* (reg8 *) JSON__SIO_CFG)
    #define JSON_SIO_DIFF           (* (reg8 *) JSON__SIO_DIFF)
#endif /* (JSON__SIO_CFG) */

/* Interrupt Registers */
#if defined(JSON__INTSTAT)
    #define JSON_INTSTAT            (* (reg8 *) JSON__INTSTAT)
    #define JSON_SNAP               (* (reg8 *) JSON__SNAP)
    
	#define JSON_0_INTTYPE_REG 		(* (reg8 *) JSON__0__INTTYPE)
#endif /* (JSON__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_JSON_H */


/* [] END OF FILE */

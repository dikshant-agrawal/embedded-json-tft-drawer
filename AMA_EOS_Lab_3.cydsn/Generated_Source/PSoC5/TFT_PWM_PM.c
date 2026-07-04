/*******************************************************************************
* File Name: TFT_PWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "TFT_PWM.h"

static TFT_PWM_backupStruct TFT_PWM_backup;


/*******************************************************************************
* Function Name: TFT_PWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  TFT_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void TFT_PWM_SaveConfig(void) 
{

    #if(!TFT_PWM_UsingFixedFunction)
        #if(!TFT_PWM_PWMModeIsCenterAligned)
            TFT_PWM_backup.PWMPeriod = TFT_PWM_ReadPeriod();
        #endif /* (!TFT_PWM_PWMModeIsCenterAligned) */
        TFT_PWM_backup.PWMUdb = TFT_PWM_ReadCounter();
        #if (TFT_PWM_UseStatus)
            TFT_PWM_backup.InterruptMaskValue = TFT_PWM_STATUS_MASK;
        #endif /* (TFT_PWM_UseStatus) */

        #if(TFT_PWM_DeadBandMode == TFT_PWM__B_PWM__DBM_256_CLOCKS || \
            TFT_PWM_DeadBandMode == TFT_PWM__B_PWM__DBM_2_4_CLOCKS)
            TFT_PWM_backup.PWMdeadBandValue = TFT_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(TFT_PWM_KillModeMinTime)
             TFT_PWM_backup.PWMKillCounterPeriod = TFT_PWM_ReadKillTime();
        #endif /* (TFT_PWM_KillModeMinTime) */

        #if(TFT_PWM_UseControl)
            TFT_PWM_backup.PWMControlRegister = TFT_PWM_ReadControlRegister();
        #endif /* (TFT_PWM_UseControl) */
    #endif  /* (!TFT_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: TFT_PWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  TFT_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void TFT_PWM_RestoreConfig(void) 
{
        #if(!TFT_PWM_UsingFixedFunction)
            #if(!TFT_PWM_PWMModeIsCenterAligned)
                TFT_PWM_WritePeriod(TFT_PWM_backup.PWMPeriod);
            #endif /* (!TFT_PWM_PWMModeIsCenterAligned) */

            TFT_PWM_WriteCounter(TFT_PWM_backup.PWMUdb);

            #if (TFT_PWM_UseStatus)
                TFT_PWM_STATUS_MASK = TFT_PWM_backup.InterruptMaskValue;
            #endif /* (TFT_PWM_UseStatus) */

            #if(TFT_PWM_DeadBandMode == TFT_PWM__B_PWM__DBM_256_CLOCKS || \
                TFT_PWM_DeadBandMode == TFT_PWM__B_PWM__DBM_2_4_CLOCKS)
                TFT_PWM_WriteDeadTime(TFT_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(TFT_PWM_KillModeMinTime)
                TFT_PWM_WriteKillTime(TFT_PWM_backup.PWMKillCounterPeriod);
            #endif /* (TFT_PWM_KillModeMinTime) */

            #if(TFT_PWM_UseControl)
                TFT_PWM_WriteControlRegister(TFT_PWM_backup.PWMControlRegister);
            #endif /* (TFT_PWM_UseControl) */
        #endif  /* (!TFT_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: TFT_PWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  TFT_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void TFT_PWM_Sleep(void) 
{
    #if(TFT_PWM_UseControl)
        if(TFT_PWM_CTRL_ENABLE == (TFT_PWM_CONTROL & TFT_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            TFT_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            TFT_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (TFT_PWM_UseControl) */

    /* Stop component */
    TFT_PWM_Stop();

    /* Save registers configuration */
    TFT_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: TFT_PWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  TFT_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void TFT_PWM_Wakeup(void) 
{
     /* Restore registers values */
    TFT_PWM_RestoreConfig();

    if(TFT_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        TFT_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */

/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "global.h"
#include "tft.h"
#include "ringbuffer.h"
#include "parser.h"
#include "drawer.h"
#include "ringbuffer_cfg.h" 
#include "message.h"
#include "stdio.h"

#define UART_ERROR_STATES (UART_LOG_RX_STS_BREAK | UART_LOG_RX_STS_PAR_ERROR | UART_LOG_RX_STS_STOP_ERROR | UART_LOG_RX_STS_OVERRUN)

ringbuffer_hdl_t bufferObject;      // Ring buffer object to store the msg from rx isr
char buffer[PARSERSTRINGSIZE];      //Actual global defination of the buffer to be used for ring buffer

Parser_t jsonParser;                //Json parser object that needs to be used for parsing json string (Can be implemented in the local stack)
MSG_messagebox_t message;           //Messsage box object to hold the msgs for inter task communication


/*
void UART_Error_Handler(uint8_t UART_Error_Code){
    //UART_LOG_PutChar('H');
    //UART_LOG_PutChar(UART_Error_Code);
}
*/


int main(void)
{   
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    EE_systick_set_period(MILLISECONDS_TO_TICKS(1, BCLK__BUS_CLK__HZ));
	EE_systick_enable_int();

    for(;;)
    StartOS(OSDEFAULTAPPMODE); 

}

void unhandledException(){
    __asm("bkpt");
}

TASK(Task_Init){
    
    //Initialization
    UART_LOG_Start();
    TFT_init();
    TFT_setBacklight(100);
    TFT_fillScreen(WHITE);
    
    //Initialize the ring buffer object with NO ERIKA RES
    Ringbuffer_Init(&bufferObject, RINGBUFFER_NO_ERIKA_RES, buffer, PARSERSTRINGSIZE);
    
    //Initialize the messaging service object with res_msgBox erika resource
    if(MSG_init(&message, 10, ev_TFT, Task_TFT, res_msgBox)!= RC_SUCCESS){
        UART_LOG_PutString("INIT FAIL\n");
    }

    //Initialize the parser object
    if(PARSER_init(&jsonParser) != RC_SUCCESS){
        UART_LOG_PutString("INIT FAIL\n");
    }
    
    //Reconfigure ISRs with OS parameters.
    //This line MUST be called after the hardware driver initialisation!
    EE_system_init();
	
    //Start SysTick
	//Must be done here, because otherwise the isr vector is not overwritten yet
    EE_systick_start();
    
    //Activate the extended Task JSON and TFT
    ActivateTask(Task_JSON);
    ActivateTask(Task_TFT);

    //Terminate the INIT Task
    TerminateTask();
}


//TASK JSON : Handles the JSON Msg from the RX buffer and Parse it using parser.c and checks for drawing command and triggers messaging service.
TASK(Task_JSON){
    UART_LOG_PutString("JSON TASK\n");
    uint32_t length;            //Lenght of data in the buffer.
    Drawer_t jsonDraw;          //Draer object in which send msg writes the drawing command
    MSG_id_t msgId;             //Message id that determines wheather its color cmd or draw cmd
    char tempPayload[PARSERSTRINGSIZE]; //Temp paylod to store data from ringbuffer to transmit it to parser buffer

    while(1){
        WaitEvent(ev_json);
        ClearEvent(ev_json);
        
        UART_LOG_PutChar('J');      // To check if the task is entering the JSON Event
        length = bufferObject.filled;
        
        //Read from ring buffer and store in tempPayload
        if(Ringbuffer_Read(&bufferObject, tempPayload, length) == RC_SUCCESS){
            
            /*
            * Calling JSON Parser API to read the data from ring buffer and store it in tempBuffer 
            */
            for(uint32_t i = 0; i < length; i++){
                PARSER_addChar(&jsonParser, tempPayload[i]);
            }
            //Add a terminating character
            PARSER_addEndl(&jsonParser);
            
            //Start parsing and storing the parsed token in parser object
            if(RC_SUCCESS != PARSER_parse(&jsonParser)){
                UART_LOG_PutString("ERROR IN PARSE:\n");
            }
        }
        
        //Ring buffer read failed
        else{
            UART_LOG_PutString("READ FAILED:\n");
        }
        
        // Iterate over all the tokens and check for drawing command for success it triggers the messaging service to tft task
        while (1){
            RC_t r = DRAWER_getNextDrawCommand(&jsonDraw, &jsonParser, &msgId);
            if (r == RC_SUCCESS) {
                RC_t res = MSG_sendMessage(&message, &jsonDraw, sizeof(Drawer_t), msgId);
                if (res != RC_SUCCESS) {
                    UART_LOG_PutString("SEND FAIL\n");
                }
                if (r == RC_SUCCESS) {
                    UART_LOG_PutChar(
                        msgId.m_event == EVE_COLOR ? 'C' : 'D'
                    );
                }

            }
            
            // Skip Token
            else if (r == RC_ERROR_NOT_MATCH) {
                continue;  
            }
            
            // Either the parser is broken or its the end of tokens
            else {
                break;     
            }
        }
    }
}

// TFT TASK to fetch the messaging command and draw it to the TFT Display
TASK(Task_TFT){
    UART_LOG_PutString("TFT TASK\n");
    MSG_id_t id;        //Fetch the msg ID and check for color or draw command
    Drawer_t tftDraw;   //Draw object used for the drawing on tft
    TFT_colors_t color; //color variable to hold the color command
    color = RED;        //Decalred red as default if no color command the system will not crash.

    while(1){
        if (MSG_getFillLevel(&message) == 0){
            MSG_waitNextMessage(&message);
        }        
        //UART_LOG_PutChar('A'); //To see if the TFT event has been called or not
        
        
        //Iterate over all the recived message untill its empty
        while(MSG_getFillLevel(&message) > 0){
            
            //If message is fethced succesfully
            if(RC_SUCCESS == MSG_receiveMessage(&message, &tftDraw, sizeof(Drawer_t), &id)){
                
                //If the event id is Color
                if(id.m_event == EVE_COLOR){
                    color = tftDraw.data.color;
                    UART_LOG_PutChar('C');
                }
                
                //If the event id is Draw
                else if(id.m_event == EVE_DRAW){
                    UART_LOG_PutChar('D');
                    TFT_writeLine(
                        tftDraw.data.coordinate[0],
                        tftDraw.data.coordinate[1],
                        tftDraw.data.coordinate[2],
                        tftDraw.data.coordinate[3],
                        color);
                }
            }
            
            // If message fetched is failed
            else{
                UART_LOG_PutString("FAILED TO DRAW");
            }
        }
    }
}


//Cyclic task called by systick with no op
TASK(Task_Background){
    __asm("nop");
    TerminateTask();
}

/********************************************************************************
 * ISR Definitions
 ********************************************************************************/

//ISR which will increment the systick counter every ms 
ISR(systick_handler) {
    CounterTick(cnt_systick);   
}
//ISR to handle the rx bytes and store it in a global ringbuffer object
ISR2(isr_uartRx){
   
    uint8_t readData;
    uint8_t readStatus;
    uint8_t UART_LOG_errorStatus = 0u;
    
    do{ 
        readStatus = UART_LOG_RXSTATUS_REG;
        readData = readStatus;
        
        if((readStatus & UART_ERROR_STATES) != 0u){
            UART_LOG_errorStatus |= (readStatus & UART_ERROR_STATES);
            //UART_Error_Handler(UART_LOG_errorStatus);     //Error handler (not clear in the assignment of what to do if error bits are there)
        }
        
        //Fetch all the bytes until the buffer is empty
        if((readStatus & UART_LOG_RX_STS_FIFO_NOTEMPTY) != 0){
            readData = UART_LOG_RXDATA_REG;
            if((Ringbuffer_Write(&bufferObject, &readData, 1u)) == RC_SUCCESS){
                if(readData == 0x00){
                    PARSER_clear(&jsonParser);
                    SetEvent(Task_JSON, ev_json);
                } 
            }
        }
    }
    
    // While FIFO is empty continue to read it
    while((readStatus & UART_LOG_RX_STS_FIFO_NOTEMPTY)!= 0u);
}


/* [] END OF FILE */



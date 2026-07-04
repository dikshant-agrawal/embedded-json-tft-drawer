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

#include "drawer.h"
#include "stdlib.h"
#include <string.h>


// Convert color string from JSON into TFT color enum
static TFT_color_t DRAWER__colorFromToken(char* buf)
{
    
    if(strcmp(buf, "red") == 0) return RED;
    if(strcmp(buf, "white") == 0) return WHITE;
    if(strcmp(buf, "blue") == 0) return BLUE;
    if(strcmp(buf, "black") == 0) return BLACK;
    if(strcmp(buf, "green") == 0) return GREEN;
    if(strcmp(buf, "cyan") == 0) return CYAN;
    if(strcmp(buf, "yellow") == 0) return YELLOW;
    if(strcmp(buf, "brown") == 0) return BROWN;
    if(strcmp(buf, "gray") == 0) return GRAY;
    
    return RED;
}


/**
 * Translates a JSON tag into a command type for the TFT
 * \param Drawer_t *const me            - [OUT] the next command
 * \param Parser_t *const parser        - [IN/OUT] Parser object pointer
 * \param MSG_id_t *const msgId         - [IN/OUT] Msg id for the messaging service
 * \return RC_SUCCESS if a command was found, RC_ERROR_BUFFER_EMTPY if parsing has come to an end, other error code as required
 */

RC_t DRAWER_getNextDrawCommand(Drawer_t *const me, Parser_t *const parser, MSG_id_t *const msgId){
    
    // Check for invalid pointers
    if(!parser || !me || !msgId){
        return RC_ERROR_BAD_PARAM;
    }
    
    jsmntok_t token;
    uint16_t savedIndex = parser->nextToken; // Save parser state for rollback

    // Get next JSON token
   if (PARSER_getNextToken(parser, &token) != RC_SUCCESS) {
        return RC_ERROR_OVERRUN;
    }
    
    // Expect a string command
    if(token.type != JSMN_STRING){
        return RC_ERROR_NOT_MATCH;
    }
    
    // Length of color string
    uint16_t length = token.end - token.start;
    
    // color string must be exactly one character
    if(length != 1){
        parser->nextToken = savedIndex;
        return RC_ERROR_NOT_MATCH;
    }
    
    // Read color character
    char str = parser->content[token.start];
    
    // COLOR COMMAND FOUND
    if( str == 'c'){

         // Read color string token
        if (PARSER_getNextToken(parser, &token) != RC_SUCCESS) {
            return RC_ERROR_OVERRUN;
        }

        // Color must be a string
        if(token.type != JSMN_STRING){
            return RC_ERROR_BAD_DATA;
        }
        
        uint8_t strLenght = token.end - token.start;    // Color string length
        
        char tempBuf[16];
        memcpy(tempBuf, &parser->content[token.start], strLenght);  // Copy color text
        tempBuf[strLenght] = '\0';                                  // Null-terminate string
        
        msgId->m_signal = 0;                            // MSG SIGNAL 0
        msgId->m_event = EVE_COLOR;                     // Color event
        
        me->command = DRAWER_CMD_COLOR;                 // Set color command
        me->data.color = DRAWER__colorFromToken(tempBuf);// Convert color
        
        return RC_SUCCESS;
    }
    
    // DRAW COMMAND FOUND
    if(str == 'd'){

        // Read array of coordinates
        if (PARSER_getNextToken(parser, &token) != RC_SUCCESS) {
            return RC_ERROR_OVERRUN;
        }
        
        // Expect array with 4 values
        if(token.type != JSMN_ARRAY || token.size != 4){
            return RC_ERROR_BAD_DATA;
        }
        
        // Parse x1, y1, x2, y2
        for(uint8_t i = 0; i < 4; i++){
            if (PARSER_getNextToken(parser, &token) != RC_SUCCESS) {
                return RC_ERROR_OVERRUN;
            }
            
            // Coordinates must be numbers
            if (token.type != JSMN_PRIMITIVE) {
                return RC_ERROR_BAD_DATA;
            }

            uint16_t arrLength = token.end - token.start;
            
            char tempBuf[16];
            memcpy(tempBuf, &parser->content[token.start], arrLength);
            tempBuf[arrLength] = '\0';
            

            me->data.coordinate[i] = (uint16_t)atoi(tempBuf); // Convert to integer
        } 
        
        msgId->m_signal = 0;       
        msgId->m_event = EVE_DRAW;      // Draw event
            
        me->command = DRAWER_CMD_DRAW;  // Set draw command

        return RC_SUCCESS;
    }
    
     // Roll back parser if command not recognized
    parser->nextToken = savedIndex;
    return RC_ERROR_NOT_MATCH;
}

/* [] END OF FILE */

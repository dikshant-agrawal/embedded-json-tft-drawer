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

#include "parser.h"
#include "project.h"
#include "stdio.h"
#include "stdlib.h"



/**
 * Create a parser object
 * \param Parser_t *const me - [OUT] Parser bject pointer
 * \return RC_SUCCESS
 */
RC_t PARSER_init(Parser_t *const me){
    
    jsmn_init(&me->parser);
    me->nextToken = 0;
    me->nextFreePos = 0;
    me->tokenCount = 0;
    
    return RC_SUCCESS;

}

/**
 * Clear a parser object
 * \param Parser_t *const me - [OUT] Parser object pointer
 * \return RC_SUCCESS
 */
RC_t PARSER_clear(Parser_t *const me){
    
    PARSER_init(me);
    
    return RC_SUCCESS;
}

/**
 * Add a char to the buffer
 * \param Parser_t *const me - [IN/OUT] Parser bject pointer
 * \param char data - [IN] next char of the json string
 * \return RC_SUCCESS
 */
RC_t PARSER_addChar(Parser_t *const me, char data){

    me->content[me->nextFreePos] = data;
    me->nextFreePos++;

    return RC_SUCCESS;
    
}

/**
 * Add an endl to the buffer
 * \param Parser_t *const me - [IN/OUT] Parser bject pointer
 * \return RC_SUCCESS
 */
RC_t PARSER_addEndl(Parser_t *const me){
    
    me->content[me->nextFreePos] = '\0';
    return RC_SUCCESS;
}

/**
 * Perform the parsing and create the token list
 * \param Parser_t *const me - [IN/OUT] Parser bject pointer
 * \param char data - [IN] next char of the json string
 * \return RC_SUCCESS
 */
RC_t PARSER_parse(Parser_t *const me){
    me-> nextToken = 0;
    
    int tokenCount = jsmn_parse(&me->parser, me->content, me->nextFreePos, me->token, PARSERMAXTOKEN);
    
    if( tokenCount < 0){
        return RC_ERROR_NOT_IMPLEMENTED;
    }
    
    me->tokenCount = tokenCount;

    return RC_SUCCESS;
    
}
/**
 * Get the next token of the list
 * \param Parser_t *const me - [IN/OUT] Parser object pointer
 * \param jsmntok_t const* token - [OUT] Next token of the list
 * \return RC_SUCCESS
 */

RC_t PARSER_getNextToken(Parser_t *const me, jsmntok_t *const token)
{
    if (me->nextToken < me->tokenCount) {
        *token = me->token[me->nextToken];
        me->nextToken++;
        return RC_SUCCESS;
    }
    else {
        return RC_ERROR_OVERRUN;
    }
}


/**
 * Print a parser object content
 * \param Parser_t *const me - [OUT] Parser bject pointer
 * \return RC_SUCCESS
 */
RC_t PARSER_dbg_printContent(Parser_t *const me){
    
    PARSER_addEndl(me);
    UART_LOG_PutString(me->content);
    
    return RC_SUCCESS;
}

/**
 * Print a parser object content
 * \param Parser_t *const me - [OUT] Parser bject pointer
 * \return RC_SUCCESS
 */

RC_t PARSER_dbg_printToken(Parser_t *const me){

    if(me->nextToken < me->tokenCount){

        jsmntok_t *tok = &me->token[me->nextToken];

        uint16_t len = tok->end - tok->start;
        if (len <= 0) {
            return RC_ERROR_NOT_IMPLEMENTED;
        }

        //TODO Size checking is to be implemted 
        char temp[64];  
        if (len >= sizeof(temp)) {
            len = sizeof(temp) - 1;
        }
  
        //Copy the token content to temp buf for priting to UART
        memcpy(temp, &me->content[tok->start], len);
        temp[len] = '\0';

        UART_LOG_PutString(temp);
        UART_LOG_PutString("\n");
 
    }
    
    else{
        UART_LOG_PutString("Token_Over\n");
    }

    return RC_SUCCESS;
}


/* [] END OF FILE */

/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Console utils
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "console_utils.h"

// Macros to select the OS to be used
#define WINDOWS
//#define UNIX

//Constants
#define STDIN_BUF_SIZE          4096

void cleanScreen(void)
{
#if defined(WINDOWS)
    system("cls");
#elif defined(UNIX) 
    system("clear");
#endif
}

static void truncateStringWithControlChars(char *str, uint32_t size)
{
    int32_t i = 0;

    while (i < size)
    {
        //if the character is a control character
        if (iscntrl(str[i]))
        {
            //replace this character with a null terminator
            str[i] = '\0';

            //end loop
            break;
        }

        i++;
    }
}

int32_t getUInt32FromConsole(const char *consoleStr)
{
    uint32_t ret = 0;

    if (consoleStr != NULL)
    {
        char stdinBuf[STDIN_BUF_SIZE];
        unsigned long result = 0L;

        memset(&stdinBuf, 0, sizeof(char)*STDIN_BUF_SIZE);
        
        printf(consoleStr);

        //prevent buffer overflows with the variable stdinBuf
        fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
        truncateStringWithControlChars(stdinBuf, STDIN_BUF_SIZE);

        result = strtoul(stdinBuf, NULL, 0);

        ret = (uint32_t)result;
    }

    return ret; 
}

int32_t getInt32FromConsole(const char *consoleStr)
{
    int32_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (int32_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

uint16_t getUInt16FromConsole(const char *consoleStr)
{
    uint16_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (uint16_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

int16_t getInt16FromConsole(const char *consoleStr)
{
    int16_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (int16_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

uint8_t getUint8FromConsole(const char *consoleStr)
{
    uint8_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (uint8_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

int8_t getInt8FromConsole(const char *consoleStr)
{
    int8_t ret = 0;

    if (consoleStr != NULL)
    {
        ret = (int8_t)getUInt32FromConsole(consoleStr); 
    }

    return ret;
}

void getStringFromConsole(const char *consoleStr, char *outputStr, uint32_t maxSize)
{
    if (consoleStr != NULL
        && outputStr != NULL)
    {
        char stdinBuf[STDIN_BUF_SIZE];

        memset(&stdinBuf, 0, sizeof(char)*STDIN_BUF_SIZE);

        printf(consoleStr);

        //prevent buffer overflows with the variable stdinBuf
        fgets(stdinBuf, STDIN_BUF_SIZE, stdin);
        truncateStringWithControlChars(stdinBuf, STDIN_BUF_SIZE);

        memcpy(outputStr, stdinBuf, sizeof(char)*maxSize); 
    }
}

char getFirstCharFromConsole(const char *consoleStr)
{
    char ret = 0;

    if (consoleStr != NULL)
    {
        getStringFromConsole(consoleStr, &ret, 1);
    }

    return ret; 
}

bool validateIntInput(int32_t value, int32_t lowerLimit, int32_t upperLimit)
{
    bool isValid = true;

    if (value < lowerLimit || value > upperLimit)
    {
        printf("\nEntrada invalida\n");
        isValid = false;

        //waiting for a key input
        getchar();
    }

    return isValid;
}

bool getYesOrNotFromConsole(const char *consoleStr)
{
    bool ret = false;
    bool loop = false;
    char inputChar = 0;
    char inputStr[8];

    do
    {
        loop = false;

        memset(inputStr, 0, sizeof(char)*8);
        getStringFromConsole(consoleStr, inputStr, 8); 
        inputChar = inputStr[0];

        if (strlen(inputStr) == 1)   //1 character
        {
            if ((inputChar == 's' || inputChar == 'S')
                || (inputChar == 'n' || inputChar == 'N'))
            {
                if (inputChar == 's' || inputChar == 'S') 
                {
                    ret = true;
                }
            }
            else
            {
                printf("\nEntrada invalida\n");
                //waiting for a key input
                getchar();
                loop = true;
            }
        }
        else
        {
            printf("\nEntrada invalida\n");
            //waiting for a key input
            getchar();
            loop = true;
        }
    }
    while (loop);

    return ret;
}

uint8_t createMenuWithMultipleOptions(const char * title,
                                      const char * header,
                                      const char * options,
                                      const char * footer,
                                      bool needValidateInput,
                                      int32_t lowerLimit, 
                                      int32_t upperLimit, 
                                      bool needCleanScreen)
{
    uint8_t optionSelected = 0;
    bool    isValidOption = true;

    do 
    {
        if (needCleanScreen)
        {
            cleanScreen();
        }

        if (title != NULL) 
        {
            printf(title); 
        }
        if (header != NULL)
        {
            printf(header); 
        }
        if (options != NULL)
        {
            printf(options); 
        }

        if (footer != NULL)
        {
            optionSelected = getUint8FromConsole(footer); 
        }

        if (needValidateInput)
        {
            isValidOption = validateIntInput(optionSelected, lowerLimit, upperLimit); 
        }
    }while (!isValidOption);

    return optionSelected;
}

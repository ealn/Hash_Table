/*
 * Copyright (c) 2016 by Adrian Luna and Ricardo Gonzales
 * All Rights Reserved
 *
 * Authors: - Adrian Luna
 *          - Ricardo Gonzales
 *
 * Porpuse: Implementation of trace module
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "trace.h"

//Files:
#define LOG_FILE     "log.txt"

//Constants
#define MONTH_OFFSET    1
#define YEAR_OFFSET     1900
#define TIME_BUF_SIZE   20
#define MESS_BUF_SIZE   12
#define PARM_BUF_SIZE   256
#define BUF_SIZE        TIME_BUF_SIZE + MESS_BUF_SIZE + PARM_BUF_SIZE


//Trace file
FILE * traceFile = NULL;

//Definition of static functions
static void writeTrace(char * str);

void initTrace(void)
{
    if (traceFile == NULL)
    {
        traceFile = fopen(LOG_FILE, "a");
    }
}

void termTrace(void)
{
    if (traceFile != NULL)
    {
        fclose(traceFile);
        traceFile = NULL;
    }
}

static void writeTrace(char * str)
{
    if (traceFile != NULL)
    {
        fprintf(traceFile, str);
        fflush(traceFile);
    }
}

void getTimeStamp(char * pOutputTime)
{
    if (pOutputTime != NULL)
    {
        struct tm *pLocalTime = NULL; 
        time_t rawtime;
        
        memset(pOutputTime, 0, sizeof(char)*TIME_BUF_SIZE);
        time(&rawtime);

        //get local time
        pLocalTime = localtime(&rawtime);

        if (pLocalTime != NULL)
        {
           //We need to add the offset to the variables tm_mon and tm_year
            pLocalTime->tm_mon += MONTH_OFFSET;
            pLocalTime->tm_year += YEAR_OFFSET;

            sprintf(pOutputTime,
                    "%i/%i/%i %i:%i:%i",
                    pLocalTime->tm_mday,
                    pLocalTime->tm_mon,
                    pLocalTime->tm_year,
                    pLocalTime->tm_hour,
                    pLocalTime->tm_min,
                    pLocalTime->tm_sec);

        }
    }
}

void traceDataDebug(const char *pParm, ...)
{
    va_list argList;
    char parBuf[PARM_BUF_SIZE];
    char timeStamp[TIME_BUF_SIZE];
    char MessageType[MESS_BUF_SIZE] = "DEBUG -";
    char buff[BUF_SIZE];
    
    memset(parBuf, 0, sizeof(char)*PARM_BUF_SIZE);
    memset(buff, 0, sizeof(char)*BUF_SIZE);

    va_start(argList, pParm);
    vsnprintf(parBuf, sizeof(parBuf), pParm, argList);
    va_end(argList);

    getTimeStamp(timeStamp);

    sprintf(buff,
            "%s %s %s",
            timeStamp,
            MessageType,
            parBuf);

    writeTrace(buff);
}

void traceDataWarning(const char *pParm, ...)
{
    va_list argList;
    char parBuf[PARM_BUF_SIZE];
    char timeStamp[TIME_BUF_SIZE];
    char MessageType[MESS_BUF_SIZE] = "WARNING -";
    char buff[BUF_SIZE];
    
    memset(parBuf, 0, sizeof(char)*PARM_BUF_SIZE);
    memset(buff, 0, sizeof(char)*BUF_SIZE);

    va_start(argList, pParm);
    vsnprintf(parBuf, sizeof(parBuf), pParm, argList);
    va_end(argList);

    getTimeStamp(timeStamp);

    sprintf(buff,
            "%s %s %s",
            timeStamp,
            MessageType,
            parBuf);

    writeTrace(buff);
}

void traceDataError(const char *pParm, ...)
{
    va_list argList;
    char parBuf[PARM_BUF_SIZE];
    char timeStamp[TIME_BUF_SIZE];
    char MessageType[MESS_BUF_SIZE] = "ERROR -";
    char buff[BUF_SIZE];
    
    memset(parBuf, 0, sizeof(char)*PARM_BUF_SIZE);
    memset(buff, 0, sizeof(char)*BUF_SIZE);

    va_start(argList, pParm);
    vsnprintf(parBuf, sizeof(parBuf), pParm, argList);
    va_end(argList);

    getTimeStamp(timeStamp);

    sprintf(buff,
            "%s %s %s",
            timeStamp,
            MessageType,
            parBuf);

    writeTrace(buff);
}

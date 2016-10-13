/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Implementation of trace module
 */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "trace.h"

//Files:
#define LOG_FILE     "log.txt"

//Constants
#define MONTH_OFFSET        1
#define YEAR_OFFSET         1900
#define TIME_BUF_SIZE       20
#define MESS_BUF_SIZE       12
#define CALLER_BUF_SIZE     20
#define PARM_BUF_SIZE       256
#define BUF_SIZE        TIME_BUF_SIZE + MESS_BUF_SIZE + CALLER_BUF_SIZE + PARM_BUF_SIZE


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

static void getTimeStamp(char * pOutputTime)
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
           //Add the offset to the variables tm_mon and tm_year
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

static void traceData(const char *pCaller, const char *pMessType, const char * pParm)
{
    char timeStamp[TIME_BUF_SIZE];
    char messageType[MESS_BUF_SIZE];
    char buff[BUF_SIZE];
    char callerBuf[CALLER_BUF_SIZE];
    
    memset(timeStamp, 0, sizeof(char)*TIME_BUF_SIZE);
    memset(messageType, 0, sizeof(char)*MESS_BUF_SIZE);
    memset(buff, 0, sizeof(char)*BUF_SIZE);
    memset(callerBuf, 0, sizeof(char)*CALLER_BUF_SIZE);

    memcpy(messageType, pMessType, sizeof(char)*MESS_BUF_SIZE);
    memcpy(callerBuf, pCaller, sizeof(char)*(CALLER_BUF_SIZE - 2)); //the size of parenthesis is 2
    getTimeStamp(timeStamp);
    
    sprintf(buff,
            "%s %s %s() %s",
            timeStamp,
            messageType,
            callerBuf,
            pParm);

    writeTrace(buff);
}

void traceDataDebug(const char *pCaller, const char *pParm, ...)
{
    char parBuf[PARM_BUF_SIZE];
    va_list argList;

    memset(parBuf, 0, sizeof(char)*PARM_BUF_SIZE);

    va_start(argList, pParm);
    vsnprintf(parBuf, sizeof(char)*PARM_BUF_SIZE, pParm, argList);
    va_end(argList);

    traceData(pCaller, "DEBUG -", parBuf);
}

void traceDataWarning(const char *pCaller, const char *pParm, ...)
{
    char parBuf[PARM_BUF_SIZE];
    va_list argList;

    memset(parBuf, 0, sizeof(char)*PARM_BUF_SIZE);

    va_start(argList, pParm);
    vsnprintf(parBuf, sizeof(char)*PARM_BUF_SIZE, pParm, argList);
    va_end(argList);

    traceData(pCaller, "WARNING -", parBuf);
}

void traceDataError(const char *pCaller, const char *pParm, ...)
{
    char parBuf[PARM_BUF_SIZE];
    va_list argList;

    memset(parBuf, 0, sizeof(char)*PARM_BUF_SIZE);

    va_start(argList, pParm);
    vsnprintf(parBuf, sizeof(char)*PARM_BUF_SIZE, pParm, argList);
    va_end(argList);

    traceData(pCaller, "ERROR -", parBuf);
}

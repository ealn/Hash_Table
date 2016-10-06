/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
 *
 * Porpuse: Implementation of trace module
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "trace.h"

//Files:
#define LOG_FILE     "log.txt"

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
    if (traceFile == NULL)
    {
        initTrace();
    }
    
    if (traceFile != NULL)
    {
        fprintf(traceFile, str);
        fflush(traceFile);
    }
}

void traceData(const char *pStr, ...)
{
    va_list argList;
    char buf[256];
    
    memset(buf, 0, sizeof(char)*256);

    va_start(argList, pStr);
    vsnprintf(buf, sizeof(buf), pStr, argList);
    va_end(argList);

    writeTrace(buf);
}

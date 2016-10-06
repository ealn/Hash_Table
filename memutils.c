/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
 *
 * Porpuse: Implementation of memory utils
 */
 
#include <malloc.h>
#include <string.h>

#include "memutils.h"

void * memAlloc(size_t size,
                const char * file,
                int line,
                const char * func)
{
    void * ptr = NULL;
    
    if (size > 0)
    {
       ptr = malloc(size);
    
       if (ptr != NULL)
       {
           memset(ptr, 0, size);
       }
    }
    
    return ptr;
}

void memFree(void * ptr,
             const char * file,
             int line,
             const char * func)
{
    if (ptr != NULL)
    {
       free(ptr);
    }
}

void * memRealloc(void *ptr, 
                  size_t origSize, 
                  size_t newSize,
                  const char * file,
                  int line,
                  const char * func)
{
    void * newPtr = NULL;
    
    if (ptr != NULL)
    {
        newPtr = memAlloc(newSize, file, line, func);
        
        if (newPtr != NULL)
        {
            if (origSize < newSize)
            {
               memcpy(newPtr, ptr, origSize);
            }
            else
            {
               memcpy(newPtr, ptr, newSize); 
            }
            
            memFree(ptr, file, line, func);
        }       
    }
    
    return newPtr;
}


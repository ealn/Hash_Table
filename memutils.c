/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Implementation of memory utils
 */
 
#include <malloc.h>
#include <string.h>

#include "memutils.h"
#include "trace.h"

void * memAlloc(size_t size,
                const char * func,
                const char * file,
                int32_t line)
{
    void * ptr = NULL;
    
    if (size > 0)
    {
       ptr = malloc(size);
       
       MEM_DEBUG("ptr=%08lx size=%i caller:%s() file:%s line:%d\n", 
                 ptr, 
                 size, 
                 func, 
                 file,
                 line);

       if (ptr != NULL)
       {
           memset(ptr, 0, size);
       }
       else
       {
           MEM_ERROR("Memory could not be allocated\n");
       }
    }
    else
    {
        MEM_ERROR("size=0\n");
    }
    
    return ptr;
}

void memFree(void * ptr,
             const char * func,
             const char * file,
             int32_t line)
{
    if (ptr != NULL)
    {
       MEM_DEBUG("ptr=%08lx caller:%s() file:%s line:%d\n", 
                 ptr, 
                 func, 
                 file,
                 line);

       free(ptr);
    }
    else
    {
        MEM_WARNING("Pointer is null\n");
    }
}

void * memRealloc(void *ptr, 
                  size_t origSize, 
                  size_t newSize,
                  const char * func,
                  const char * file,
                  int32_t line)
{
    void * newPtr = NULL;

    if (ptr != NULL)
    {
        newPtr = memAlloc(newSize, func, file, line);

        MEM_DEBUG("ptr=%08lx origSize=%i newSize=%i newPtr=%08lx caller:%s() file:%s line:%d\n", 
                  ptr,
                  origSize,
                  newSize,
                  newPtr,
                  func, 
                  file,
                  line);
        
        if (newPtr != NULL)
        {
            if (origSize < newSize)
            {
               memcpy(newPtr, ptr, origSize);
            }
            else
            {
               MEM_WARNING("New size is minor than origin size\n");
               memcpy(newPtr, ptr, newSize); 
            }
            
            memFree(ptr, func, file, line);
        }
        else
        {
            MEM_ERROR("Memory could not be allocated\n");
        }       
    }
    else
    {
        MEM_WARNING("Pointer is null\n");
    }
    
    return newPtr;
}


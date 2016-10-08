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
#include "trace.h"

void * memAlloc(size_t size,
                const char * func,
                const char * file,
                int line)
{
    void * ptr = NULL;
    
    if (size > 0)
    {
       ptr = malloc(size);
       
       MEM_DEBUG("memAlloc() ptr=%08lx size=%i caller:%s() file:%s line:%d\n", 
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
           MEM_ERROR("memAlloc() memory could not be allocated\n");
       }
    }
    else
    {
        MEM_ERROR("memAlloc() size=0\n");
    }
    
    return ptr;
}

void memFree(void * ptr,
             const char * func,
             const char * file,
             int line)
{
    if (ptr != NULL)
    {
       MEM_DEBUG("memFree() ptr=%08lx caller:%s() file:%s line:%d\n", 
                 ptr, 
                 func, 
                 file,
                 line);

       free(ptr);
    }
    else
    {
        MEM_WARNING("memFree() pointer is null\n");
    }
}

void * memRealloc(void *ptr, 
                  size_t origSize, 
                  size_t newSize,
                  const char * func,
                  const char * file,
                  int line)
{
    void * newPtr = NULL;

    if (ptr != NULL)
    {
        newPtr = memAlloc(newSize, func, file, line);

        MEM_DEBUG("memRealloc() ptr=%08lx origSize=%i newSize=%i newPtr=%08lx caller:%s() file:%s line:%d\n", 
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
               MEM_WARNING("memRealloc() new size is minor than origin size\n");
               memcpy(newPtr, ptr, newSize); 
            }
            
            memFree(ptr, func, file, line);
        }
        else
        {
            MEM_ERROR("memRealloc() memory could not be allocated\n");
        }       
    }
    else
    {
        MEM_WARNING("memRealloc() pointer is null\n");
    }
    
    return newPtr;
}


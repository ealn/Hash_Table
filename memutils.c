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
                const char * file,
                int line,
                const char * func)
{
    void * ptr = NULL;
    
    MEM_DEBUG("memAlloc DEBUG - called from: %s line: %d func: %s size_to_be_allocate= %i\n", 
              file, 
              line, 
              func, 
              size);
    
    if (size > 0)
    {
       ptr = malloc(size);
    
       if (ptr != NULL)
       {
           memset(ptr, 0, size);
       }
       else
       {
           MEM_ERROR("memAlloc ERROR - memory could not be allocated\n");
       }
    }
    
    return ptr;
}

void memFree(void * ptr,
             const char * file,
             int line,
             const char * func)
{
    MEM_DEBUG("memFree DEBUG - called from: %s line: %d func: %s pointer_to_be_freed= %08lx\n", 
              file, 
              line, 
              func, 
              ptr);
    
    if (ptr != NULL)
    {
       free(ptr);
    }
    else
    {
        MEM_WARNING("memFree WARNING pointer is null\n");
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
    
    MEM_DEBUG("memRealloc DEBUG - called from: %s line: %d func: %s pointer_to_be_realocate= %08lx origSize= %i newSize= %i\n", 
              file, 
              line, 
              func, 
              ptr);
    
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
               MEM_WARNING("memRealloc WARNING - new size is minor than origin size\n");
               memcpy(newPtr, ptr, newSize); 
            }
            
            memFree(ptr, file, line, func);
        }
        else
        {
            MEM_ERROR("memRealloc ERROR - memory could not be allocated\n");
        }       
    }
    
    return newPtr;
}


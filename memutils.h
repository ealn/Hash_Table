/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
 *
 * Porpuse: Header that contains all functions to allocate and free memory
 */

#include <stddef.h>
#include <stdio.h>

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

// Functions:
void * memAlloc(size_t size,
                const char * file,
                int line,
                const char * func);

void memFree(void * ptr,
             const char * file,
             int line,
             const char * func);

void * memRealloc(void *ptr, 
                  size_t origSize, 
                  size_t newSize,
                  const char * file,
                  int line,
                  const char * func);                             
// Macros:
#define MEMALLOC(size)                       memAlloc((size), __FILE__, __LINE__, __func__)
#define MEMFREE(ptr)                         memFree((ptr), __FILE__, __LINE__, __func__)
#define MEMREALLOC(ptr, origSize, newSize)   memRealloc((ptr), (origSize), (newSize), __FILE__, __LINE__, __func__)

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

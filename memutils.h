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
                const char * func,
                const char * file,
                int line);

void memFree(void * ptr,
             const char * func,
             const char * file,
             int line);

void * memRealloc(void *ptr, 
                  size_t origSize, 
                  size_t newSize,
                  const char * func,
                  const char * file,
                  int line);

// Macros:
#define MEMALLOC(size)                       memAlloc((size), __func__, __FILE__, __LINE__)
#define MEMFREE(ptr)                         memFree((ptr), __func__, __FILE__, __LINE__)
#define MEMREALLOC(ptr, origSize, newSize)   memRealloc((ptr), (origSize), (newSize), __func__, __FILE__, __LINE__)

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

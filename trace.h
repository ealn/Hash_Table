/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
 *
 * Porpuse: Header that contains the functions to add trace in the code
 */

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

void traceData(const char *pStr, ...);
void initTrace(void);
void termTrace(void);

//Macros:
#define MEM_ERROR            traceData
#define MEM_WARNING          traceData
#define MEM_DEBUG            traceData

#define HASHTAB_ERROR        traceData
#define HASHTAB_WARNING      traceData
#define HASHTAB_DEBUG        traceData

#define TREE_ERROR           traceData
#define TREE_WARNING         traceData
#define TREE_DEBUG           traceData

#define MAIN_ERROR           traceData
#define MAIN_WARNING         traceData
#define MAIN_DEBUG           traceData

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif 

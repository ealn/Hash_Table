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

void traceDataError(const char *pParm, ...);
void traceDataWarning(const char *pParm, ...);
void traceDataDebug(const char *pParm, ...);
void initTrace(void);
void termTrace(void);

//Macros:
#define MEM_ERROR            traceDataError
#define MEM_WARNING          traceDataWarning
#define MEM_DEBUG            traceDataDebug

#define HASHTAB_ERROR        traceDataError
#define HASHTAB_WARNING      traceDataWarning
#define HASHTAB_DEBUG        traceDataDebug

#define TREE_ERROR           traceDataError
#define TREE_WARNING         traceDataWarning
#define TREE_DEBUG           traceDataDebug

#define MAIN_ERROR           traceDataError
#define MAIN_WARNING         traceDataWarning
#define MAIN_DEBUG           traceDataDebug

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif 

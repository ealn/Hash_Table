/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Header that contains the functions to add trace in the code
 */

#include <stdarg.h>

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

void traceDataError(const char *pCaller, const char *pParm, ...);
void traceDataWarning(const char *pCaller, const char *pParm, ...);
void traceDataDebug(const char *pCaller, const char *pParm, ...);
void initTrace(void);
void termTrace(void);

//Macros:
#define MEM_ERROR(...)            traceDataError(__func__, __VA_ARGS__)
#define MEM_WARNING(...)          traceDataWarning(__func__, __VA_ARGS__)
#define MEM_DEBUG(...)            traceDataDebug(__func__, __VA_ARGS__)

#define HASHTAB_ERROR(...)        traceDataError(__func__, __VA_ARGS__)
#define HASHTAB_WARNING(...)      traceDataWarning(__func__, __VA_ARGS__)
#define HASHTAB_DEBUG(...)        traceDataDebug(__func__, __VA_ARGS__)

#define TREE_ERROR(...)           traceDataError(__func__, __VA_ARGS__)
#define TREE_WARNING(...)         traceDataWarning(__func__, __VA_ARGS__)
#define TREE_DEBUG(...)           traceDataDebug(__func__, __VA_ARGS__)

#define MAIN_ERROR(...)           traceDataError(__func__, __VA_ARGS__)
#define MAIN_WARNING(...)         traceDataWarning(__func__, __VA_ARGS__)
#define MAIN_DEBUG(...)           traceDataDebug(__func__, __VA_ARGS__)

#define UI_ERROR(...)             traceDataError(__func__, __VA_ARGS__)
#define UI_WARNING(...)           traceDataWarning(__func__, __VA_ARGS__)
#define UI_DEBUG(...)             traceDataDebug(__func__, __VA_ARGS__)

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif 

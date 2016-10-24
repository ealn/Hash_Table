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

#define TIME_BUF_SIZE       20

#define MEM_COMP        "MEMORY  -"
#define HASH_COMP       "HASHTAB -"
#define TREE_COMP       "TREE    -"
#define MAIN_COMP       "MAIN    -"
#define UI_COMP         "UI      -"

void traceDataError(const char *pComponent, const char *pCaller, const char *pParm, ...);
void traceDataWarning(const char *pComponent, const char *pCaller, const char *pParm, ...);
void traceDataDebug(const char *pComponent, const char *pCaller, const char *pParm, ...);
void initTrace(void);
void termTrace(void);
void getTimeStamp(char * pOutputTime);

//Macros:
#define MEM_ERROR(...)            traceDataError(MEM_COMP, __func__, __VA_ARGS__)
#define MEM_WARNING(...)          traceDataWarning(MEM_COMP, __func__, __VA_ARGS__)
#define MEM_DEBUG(...)            traceDataDebug(MEM_COMP, __func__, __VA_ARGS__)

#define HASHTAB_ERROR(...)        traceDataError(HASH_COMP, __func__, __VA_ARGS__)
#define HASHTAB_WARNING(...)      traceDataWarning(HASH_COMP, __func__, __VA_ARGS__)
#define HASHTAB_DEBUG(...)        traceDataDebug(HASH_COMP, __func__, __VA_ARGS__)

#define TREE_ERROR(...)           traceDataError(TREE_COMP, __func__, __VA_ARGS__)
#define TREE_WARNING(...)         traceDataWarning(TREE_COMP, __func__, __VA_ARGS__)
#define TREE_DEBUG(...)           traceDataDebug(TREE_COMP, __func__, __VA_ARGS__)

#define MAIN_ERROR(...)           traceDataError(MAIN_COMP, __func__, __VA_ARGS__)
#define MAIN_WARNING(...)         traceDataWarning(MAIN_COMP, __func__, __VA_ARGS__)
#define MAIN_DEBUG(...)           traceDataDebug(MAIN_COMP, __func__, __VA_ARGS__)

#define UI_ERROR(...)             traceDataError(UI_COMP, __func__, __VA_ARGS__)
#define UI_WARNING(...)           traceDataWarning(UI_COMP, __func__, __VA_ARGS__)
#define UI_DEBUG(...)             traceDataDebug(UI_COMP, __func__, __VA_ARGS__)

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif 

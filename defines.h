/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains the constants to detect errors
 */

#include <stdint.h>
#include <stdbool.h>

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

// Type definitions
typedef struct _Register Register;
typedef struct _Table Table;
typedef struct _Node Node;

// Macros
#define SUCCESS                0
#define FAIL                  -1
#define REG_NOT_FOUND         -2
#define REG_DUPLICATED        -3

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

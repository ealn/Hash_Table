/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Header that contains the functions of User Interface
 */
#include <stdint.h>

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

int32_t showUI(void);
int32_t showRegInfo(uint32_t ID, char *name, char *tel, char *address, char *city);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

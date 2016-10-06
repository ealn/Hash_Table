/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
 *
 * Porpuse: Header that contains all functions to handle trees
 */

#include "defines.h"

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

// Functions:
int createTree(Register *baseReg, Register *newReg);
void destroyTree(Node * tree);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

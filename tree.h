/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: Adrian Luna
 *
 * Porpuse: Header that contains all functions to handle trees
 */

#include "defines.h"

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

// Functions:
int insertRegIntoTree(Register *baseReg, Register *newReg);
int searchIDIntoTree(Register *baseReg, Register **ppOutputRegister, long ID, unsigned int *numberOfSteps);
int removeRegIntoTree(Register *reg);
int removeIDIntoTree(Register *baseReg, int ID, int *numberOfSteps);
void destroyTree(Node * tree);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

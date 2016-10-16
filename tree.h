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
int32_t insertRegIntoTree(Register *baseReg, Register *newReg, uint32_t * numberOfSteps);
int32_t searchIDIntoTree(Register *baseReg, Register **ppOutputRegister, uint32_t ID, uint32_t *numberOfSteps);
int32_t removeRegIntoTree(Register *reg, uint32_t * numberOfSteps);
int32_t removeIDIntoTree(Register *baseReg, uint32_t ID, uint32_t *numberOfSteps);
void destroyTree(Node * tree);
int32_t showTree(Node * tree);
uint32_t getNodeLevel(Node * pNode);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
 *
 * Porpuse: Header that contains all functions needed to handle hash tables
 */
 
#include "defines.h"

#if (defined(_cplusplus) || defined(__cplusplus))
extern "C" {
#endif

// Constants
#define NAME_SIZE           40
#define TEL_SIZE            20
#define ADD_SIZE            80
#define CITY_SIZE           30

// Functions:
int createTable(void);
void destroyTable(void);
int32_t displayTable(void);
int32_t insertReg(uint32_t ID, char *name, char *tel, char *address, char *city, uint32_t * numberOfSteps);
int32_t searchReg(uint32_t ID, uint32_t *numberOfSteps);
int32_t removeReg(uint32_t ID, uint32_t *numberOfSteps);
int32_t changeReg(uint32_t ID, uint32_t *numberOfSteps);
int destroyReg(Register *reg);
int cleanReg(Register *reg);
Node * getTreeFromReg(Register *reg);
int32_t getIDFromReg(Register *reg);
void setTreeToReg(Register *reg, Node * newTree);
int32_t printRegInfo(Register * reg);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

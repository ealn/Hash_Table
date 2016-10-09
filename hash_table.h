/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
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
void displayTable(void);
int insertReg(long ID, char *name, char *tel, char *address, char *city);
int searchReg(long ID, unsigned int *numberOfSteps);
int removeReg(long ID);
Node * getTreeFromReg(Register *reg);
long getIDFromReg(Register *reg);
void setTreeToReg(Register *reg, Node * newTree);

#if (defined(_cplusplus) || defined(__cplusplus))
} // extern "C"
#endif

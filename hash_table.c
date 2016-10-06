/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
 *
 * Porpuse: Implementation of algorithms to handle Hash tables
 */

#include <stdio.h>
 
#include "hash_table.h"
#include "memutils.h"
#include "tree.h"
#include "trace.h"

//Constants
#define NUMBER_OF_REG       30
#define REGISTER_SIZE       sizeof(Register)
#define TABLE_SIZE          sizeof(Table) 

//Structs
struct _Register
{
    long   ID;
    char   name[NAME_SIZE];
    char   tel[TEL_SIZE];
    char   address[ADD_SIZE];
    char   city[CITY_SIZE];
    Node * tree;    
};

struct _Table
{
    int        size;
    Register * registers;
};

//Global variables
Table * g_hashTable = NULL;         //Hash table instance

//Definition of static functions
static Register * allocRegisters(int numberOfReg);
static void allocTable(int numberOfReg);
static Register * reallocRegisters(Register * ptrOrigReg, int origNumberOfReg, int newNumberOfReg);
static void freeRegister(Register *pRegister);
static void freeTable(void);
static Register * getRegisterAtIndex(int index);
static int changeTableSize(int newNumberOfReg);

//Implementation
static Register * allocRegisters(int numberOfReg)
{
    Register *reg = NULL;
    
    if (numberOfReg > 0)
    {
       reg = (Register *)MEMALLOC(REGISTER_SIZE * numberOfReg);
    
       if (reg == NULL)
       {
          HASHTAB_ERROR("allocRegisters ERROR - allocating %i Registers\n", numberOfReg);
       }
    }
    
    return reg;
}

static void allocTable(int numberOfReg)
{
    g_hashTable = (Table *)MEMALLOC(TABLE_SIZE);
    
    if (g_hashTable != NULL)
    {
        g_hashTable->size = numberOfReg;
        g_hashTable->registers = allocRegisters(numberOfReg);
    }
    else
    {
        HASHTAB_ERROR("allocTable ERROR - allocating hash table\n");
    }
}

static Register * reallocRegisters(Register * ptrOrigReg, int origNumberOfReg, int newNumberOfReg)
{
    Register *reg = NULL;
    
    if (newNumberOfReg > 0)
    {
       reg = (Register *)MEMREALLOC(ptrOrigReg,
                                    REGISTER_SIZE * origNumberOfReg,
                                    REGISTER_SIZE * newNumberOfReg);
    
       if (reg == NULL)
       {
          HASHTAB_ERROR("reallocRegisters ERROR - allocating %i Registers\n", newNumberOfReg);
       }
    }
    
    return reg;
}

static void freeRegister(Register *pRegister)
{
    if (pRegister != NULL)
    {
        // TODO: free registers in the tree
        //if (pRegister->tree != NULL)
        //{
        //    destroyTree(pRegister->tree);
        //}
        
        MEMFREE((void *)pRegister);
    }
    else
    {
        HASHTAB_WARNING("freeRegister WARNING - register is null\n");
    }
}

static void freeTable(void)
{
    if (g_hashTable != NULL)
    {
        int index = 0;
        
        // TODO: free registers in the tree
        //while (index < g_hashTable->size)
        //{
        //    freeRegister(getRegisterAtIndex(index));
        //    index++;
        //}
        
        MEMFREE((void *)g_hashTable->registers);
        MEMFREE((void *)g_hashTable);
        g_hashTable = NULL; 
    }
    else
    {
        HASHTAB_WARNING("freeTable WARNING - hash table is null\n");
    }
}

static Register * getRegisterAtIndex(int index)
{
    Register *reg = NULL;
    
    if (g_hashTable != NULL)
    {
        reg = g_hashTable->registers + index;
    }
    
    return reg;
}

static int changeTableSize(int newNumberOfReg)
{
    int ret = SUCCESS;
    
    if (g_hashTable != NULL
        && g_hashTable->registers != NULL)
    {
        g_hashTable->registers = reallocRegisters(g_hashTable->registers, 
                                                  g_hashTable->size, 
                                                  newNumberOfReg);
        g_hashTable->size = newNumberOfReg;                                          
    }
    
    return ret;
}

// Entry points
int createTable(void)
{
    int ret = SUCCESS;
    
    allocTable(NUMBER_OF_REG);
    
    //handle errors
    if (g_hashTable == NULL)
    {
        ret = FAIL;
    }
    else if (g_hashTable != NULL
             && g_hashTable->registers == NULL)
    {
        destroyTable();
        ret = FAIL;
    }
    
    return ret;
}

void destroyTable(void)
{
    if (g_hashTable != NULL)
    {
        freeTable();
    }
}

void displayTable(void)
{
    //TODO: Search register into hash table
}

int insertReg(long ID, char *name, char *tel, char *address, char *city)
{
    int ret = SUCCESS;
    
    //TODO: Insert register into hash table
    
    return ret;
}

int searchReg(long ID, int *numberOfSteps)
{
    int ret = SUCCESS;
    
    //TODO
    
    return ret;
}

Node * getTreeFromReg(Register *reg)
{
    Node * ret = NULL;
     
    if (reg != NULL)
    {
        ret = reg->tree;
    }
    
    return ret;
}

void setTreeToReg(Register *reg, Node * newTree)
{
    if (reg != NULL)
    {
        reg->tree = newTree;
    }
}

long getIDFromReg(Register *reg)
{
    long ret = 0L;
     
    if (reg != NULL)
    {
        ret = reg->ID;
    }
    
    return ret;
}


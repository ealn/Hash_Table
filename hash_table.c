/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
 *
 * Porpuse: Implementation of algorithms to handle Hash tables
 */

#include <stdio.h>
#include <string.h>
 
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
    unsigned int size;
    Register   * registers;
};

//Global variables
Table * g_hashTable = NULL;         //Hash table instance

//Definition of static functions
static Register * allocRegisters(unsigned int numberOfReg);
static void allocTable(unsigned int numberOfReg);
static void freeRegister(Register *pRegister);
static void freeTable(void);
static Register * getRegisterAtIndex(unsigned int index);

//Implementation
static Register * allocRegisters(unsigned int numberOfReg)
{
    Register *reg = NULL;
    
    if (numberOfReg > 0)
    {
       reg = (Register *)MEMALLOC(REGISTER_SIZE * numberOfReg);
    
       if (reg == NULL)
       {
          HASHTAB_ERROR("allocRegisters() could not be allocated %i Registers\n", numberOfReg);
       }
    }
    
    return reg;
}

static void allocTable(unsigned int numberOfReg)
{
    g_hashTable = (Table *)MEMALLOC(TABLE_SIZE);
    
    if (g_hashTable != NULL)
    {
        g_hashTable->size = numberOfReg;
        g_hashTable->registers = allocRegisters(numberOfReg);

        HASHTAB_DEBUG("allocTable() g_hashTable=%08lx size = %d\n", g_hashTable, g_hashTable->size);
    }
    else
    {
        HASHTAB_ERROR("allocTable() could not be allocated the hash table\n");
    }
}

static void freeRegister(Register *pRegister)
{
    if (pRegister != NULL)
    {
        HASHTAB_DEBUG("freeRegister() pRegister->ID=%d\n", pRegister->ID);

        MEMFREE((void *)pRegister);
    }
    else
    {
        HASHTAB_WARNING("freeRegister() register is null\n");
    }
}

static void freeTable(void)
{
    if (g_hashTable != NULL)
    {
        HASHTAB_DEBUG("freeTable() g_hashTable=%08lx\n", g_hashTable);

        freeRegister(g_hashTable->registers);
        MEMFREE((void *)g_hashTable);
        g_hashTable = NULL;
    }
    else
    {
        HASHTAB_WARNING("freeTable() g_hashTable is null\n");
    }
}

static Register * getRegisterAtIndex(unsigned int index)
{
    Register *reg = NULL;
    
    if (g_hashTable != NULL
        && index < g_hashTable->size)
    {
        reg = g_hashTable->registers + index;

        HASHTAB_DEBUG("getRegisterAtIndex() reg->ID\n", reg->ID); 
    }
    
    return reg;
}

static unsigned int getHashValue(int ID)
{
    unsigned int hashValue = 0;

    hashValue = ID % g_hashTable->size;

    return hashValue;
}

static void printRegInfo(Register * reg)
{
    if (reg != NULL)
    {
        printf("\n\n         **** Informacion del Registro ****\n\n");
        printf("ID: %d\n",      reg->ID);
        printf("Name: %s\n",    reg->name);
        printf("Tel: %s\n",     reg->tel);
        printf("Address: %s\n", reg->address);
        printf("City: %s\n\n",  reg->city);
    }
    else
    {
        HASHTAB_ERROR("printRegInfo() Register is null\n");
    }
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
        unsigned int index = 0;
        Register * reg = NULL;
        
        // Free registers in the tree
        while (index < g_hashTable->size)
        {
            reg = getRegisterAtIndex(index);

            if (reg != NULL
                && reg->tree != NULL)
            {
                destroyTree(reg->tree);
            }

            index++; 
        }
        
        freeTable();
    }
    else
    {
        HASHTAB_WARNING("destroyTable() hash table is null\n");
    }
}

void displayTable(void)
{
    //TODO: Search register into hash table
}

int insertReg(long ID, char *name, char *tel, char *address, char *city)
{
    int           ret = SUCCESS;
    unsigned int  hashValue = 0;
    Register     *reg = NULL;

    hashValue = getHashValue(ID);
    reg = getRegisterAtIndex(hashValue);

    if (reg != NULL)
    {
        HASHTAB_DEBUG("insertReg() reg->ID=%d reg->tree=%08lx\n", reg->ID, reg->tree);

        //if the register is empty
        if (reg->ID != 0)
        {
            //copy the information
            reg->ID = ID;
            memcpy(reg->name, name, sizeof(char) * NAME_SIZE);
            memcpy(reg->tel, tel, sizeof(char) * TEL_SIZE);
            memcpy(reg->address, address, sizeof(char) * ADD_SIZE);
            memcpy(reg->city, city, sizeof(char) * CITY_SIZE);
        }
        else   //there is a collision
        {
            //create a new register
            Register *newReg = NULL;

            newReg = allocRegisters(1);

            //copy the information
            newReg->ID = ID;
            memcpy(newReg->name, name, sizeof(char) * NAME_SIZE);
            memcpy(newReg->tel, tel, sizeof(char) * TEL_SIZE);
            memcpy(newReg->address, address, sizeof(char) * ADD_SIZE);
            memcpy(newReg->city, city, sizeof(char) * CITY_SIZE);

            //Insert the new register into tree
            insertRegIntoTree(reg, newReg);
        }
    }
    else
    {
        HASHTAB_ERROR("insertReg() register was not found\n");
    }
    
    return ret;
}

int searchReg(long ID, unsigned int *numberOfSteps)
{
    int ret = SUCCESS;
    
    if (numberOfSteps != NULL)
    {
        unsigned int  hashValue = 0;
        Register     *reg = NULL;

        hashValue = getHashValue(ID);
        reg = getRegisterAtIndex(hashValue);

        if (reg != NULL)
        {
            *numberOfSteps = 1;

            if (reg->ID == ID)   //if the IDs are equals
            {
                //register found, print information
                printRegInfo(reg);
            }
            else if (reg->tree != NULL)  //if this register has a tree
            {
                Register * outputReg = NULL;

                //search register into tree
                ret = searchIDIntoTree(reg, &outputReg, ID, numberOfSteps);

                //if the register was found
                if (ret == SUCCESS
                    && outputReg != NULL)
                {
                    //print information
                    printRegInfo(outputReg);
                }
            }
            else
            {
                //register was not found
                HASHTAB_WARNING("searchReg() register with ID=%d was not found\n", ID);
                ret = FAIL;
            }
        }
        else
        {
            HASHTAB_ERROR("searchReg() register was not found\n");
        }
    }
    else
    {
        HASHTAB_ERROR("searchReg() number of steps is null\n");
        ret = FAIL;
    }

    return ret;
}

int removeReg(long ID)
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


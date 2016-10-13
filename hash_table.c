/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
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
          HASHTAB_ERROR("Could not be allocated %i Registers\n", numberOfReg);
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

        HASHTAB_DEBUG("g_hashTable=%08lx size = %d\n", g_hashTable, g_hashTable->size);
    }
    else
    {
        HASHTAB_ERROR("Could not be allocated the hash table\n");
    }
}

static void freeRegister(Register *pRegister)
{
    if (pRegister != NULL)
    {
        HASHTAB_DEBUG("pRegister->ID=%d\n", pRegister->ID);

        MEMFREE((void *)pRegister);
    }
    else
    {
        HASHTAB_WARNING("Register is null\n");
    }
}

static void freeTable(void)
{
    if (g_hashTable != NULL)
    {
        HASHTAB_DEBUG("g_hashTable=%08lx\n", g_hashTable);

        freeRegister(g_hashTable->registers);
        MEMFREE((void *)g_hashTable);
        g_hashTable = NULL;
    }
    else
    {
        HASHTAB_WARNING("g_hashTable is null\n");
    }
}

static Register * getRegisterAtIndex(unsigned int index)
{
    Register *reg = NULL;
    
    if (g_hashTable != NULL
        && index < g_hashTable->size)
    {
        reg = g_hashTable->registers + index;

        HASHTAB_DEBUG("reg->ID=%d\n", reg->ID); 
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
        printf("Name: %s",    reg->name);
        printf("Tel: %s",     reg->tel);
        printf("Address: %s", reg->address);
        printf("City: %s\n",  reg->city);
    }
    else
    {
        HASHTAB_ERROR("Register is null\n");
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
        HASHTAB_WARNING("Hash table is null\n");
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
        HASHTAB_DEBUG("hashValue=%d reg->ID=%d reg->tree=%08lx\n", 
                      hashValue,
                      reg->ID, 
                      reg->tree);

        //if the register is empty
        if (reg->ID == 0)
        {
            HASHTAB_DEBUG("Empty slot found\n");

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

            HASHTAB_DEBUG("Collision detected\n");

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
        HASHTAB_ERROR("Register was not found\n");
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

            HASHTAB_DEBUG("hashValue=%d reg->ID=%d ID=%d reg->tree=%08lx\n", 
                          hashValue, 
                          reg->ID, 
                          ID, 
                          reg->tree);

            if (reg->ID == ID)   //if the IDs are equals
            {
                HASHTAB_DEBUG("Register was found with the hash value\n");

                //register found, print information
                printRegInfo(reg);
            }
            else if (reg->tree != NULL)  //if this register has a tree
            {
                Register * outputReg = NULL;

                HASHTAB_DEBUG("Search register in the tree\n");

                //search register into tree
                ret = searchIDIntoTree(reg, &outputReg, ID, numberOfSteps);

                //if the register was found
                if (ret == SUCCESS
                    && outputReg != NULL)
                {
                    //print information
                    printRegInfo(outputReg);
                }
                else
                {
                    //register was not found
                    HASHTAB_WARNING("Register with ID=%d was not found\n", ID);
                    ret = FAIL;
                }
            }
            else
            {
                //register was not found
                HASHTAB_WARNING("Register with ID=%d was not found\n", ID);
                ret = FAIL;
            }
        }
        else
        {
            HASHTAB_ERROR("Register was not found\n");
        }
    }
    else
    {
        HASHTAB_ERROR("Number of steps is null\n");
        ret = FAIL;
    }

    return ret;
}

int removeReg(long ID)
{
    int ret = SUCCESS;
    unsigned int  hashValue = 0;
    Register     *reg = NULL;
    int           numberOfSteps = 0;

    hashValue = getHashValue(ID);
    reg = getRegisterAtIndex(hashValue);

    if (reg != NULL)
    {
        HASHTAB_DEBUG("hashValue=%d reg->ID=%d ID=%d reg->tree=%08lx\n", 
                      hashValue, 
                      reg->ID, 
                      ID, 
                      reg->tree);

        if (reg->ID == ID)   //if the IDs are equals
        {
            HASHTAB_DEBUG("Register was found with the hash value\n");

            //register found and the register does not have a tree
            if (reg->tree == NULL)
            {
                // clean information
                ret = cleanReg(reg);
            }
            else
            {
                //remove top node
                ret = removeRegIntoTree(reg);
            }
        }
        else if (reg->tree != NULL)  //if this register has a tree
        {
            //search register into tree
            ret = removeIDIntoTree(reg, ID, &numberOfSteps);
        }
    }
    else
    {
        HASHTAB_ERROR("Register was not found\n");
    }

    return ret;
}

int destroyReg(Register *reg)
{
    int ret = SUCCESS;
    
    if (reg != NULL)
    {
        freeRegister(reg);
    }
    else
    {
        HASHTAB_WARNING("Register is null\n");
    }

    return ret;
}

int cleanReg(Register *reg)
{
    int ret = SUCCESS;
    
    if (reg != NULL)
    {
        memset(reg, 0, sizeof(Register));
    }
    else
    {
        HASHTAB_WARNING("Register is null\n");
    }

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


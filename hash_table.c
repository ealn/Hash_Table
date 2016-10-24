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
#include "UI.h"

//Constants
#define NUMBER_OF_REG       20
#define REGISTER_SIZE       sizeof(Register)
#define TABLE_SIZE          sizeof(Table) 

//Structs
struct _Register
{
    uint32_t  hashValue;
    uint32_t  ID;
    char      first_name[FIRST_NAME_SIZE];
    char      last_name[LAST_NAME_SIZE];
    char      address[ADD_SIZE];
    char      city[CITY_SIZE];
    char      tel1[TEL_SIZE];
    char      tel2[TEL_SIZE];
    Node    * tree;
};

struct _Table
{
    uint32_t     size;
    Register   * registers;
};

//Global variables
Table * g_hashTable = NULL;         //Hash table instance

//Definition of static functions
static Register * allocRegisters(uint32_t numberOfReg);
static void allocTable(uint32_t numberOfReg);
static void freeRegister(Register *pRegister);
static void freeTable(void);
static Register * getRegisterAtIndex(uint32_t index);

//Implementation
static Register * allocRegisters(uint32_t numberOfReg)
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

static void allocTable(uint32_t numberOfReg)
{
    if (g_hashTable == NULL)
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
    else
    {
        HASHTAB_ERROR("Hash table is not null\n");
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

static Register * getRegisterAtIndex(uint32_t index)
{
    Register *reg = NULL;
    
    if (g_hashTable != NULL
        && index < g_hashTable->size)
    {
        reg = g_hashTable->registers + index;
    }
    
    return reg;
}

static uint32_t getHashValue(uint32_t ID)
{
    uint32_t hashValue = 0;

    hashValue = ID % g_hashTable->size;

    return hashValue;
}

static void insertInfoIntoReg(Register * reg,
                              uint32_t hashValue,
                              uint32_t ID,
                              char *pFirstName,
                              char *pLastName,
                              char *pAddress, 
                              char *pCity,
                              char *pTel1,
                              char *pTel2)
{
    if (reg != NULL)
    {
        reg->hashValue = hashValue;
        reg->ID = ID;
        memcpy(reg->first_name, pFirstName, sizeof(char) * (FIRST_NAME_SIZE - 1)); //-1 to keep the '\0' at the end
        memcpy(reg->last_name,  pLastName,  sizeof(char) * (LAST_NAME_SIZE - 1));  //-1 to keep the '\0' at the end
        memcpy(reg->address,    pAddress,   sizeof(char) * (ADD_SIZE - 1));        //-1 to keep the '\0' at the end
        memcpy(reg->city,       pCity,      sizeof(char) * (CITY_SIZE - 1));       //-1 to keep the '\0' at the end
        memcpy(reg->tel1,       pTel1,      sizeof(char) * (TEL_SIZE - 1));        //-1 to keep the '\0' at the end
        memcpy(reg->tel2,       pTel2,      sizeof(char) * (TEL_SIZE - 1));        //-1 to keep the '\0' at the end
    }
}

void copyRegister(Register *destination, Register *source)
{
    if (destination != NULL
        && source != NULL)
    {
        HASHTAB_DEBUG("Destination hashValue=%d reg->ID=%d reg->tree=%08lx\n", 
                      destination->hashValue,
                      destination->ID, 
                      destination->tree);

        HASHTAB_DEBUG("Source hashValue=%d reg->ID=%d reg->tree=%08lx\n", 
                      source->hashValue,
                      source->ID, 
                      source->tree);

        memcpy(destination, source, sizeof(Register));
    }
}

void swapRegister(Register *reg1, Register *reg2)
{
    if (reg1 != NULL
        && reg2 != NULL)
    {
        Register  tempReg;

        HASHTAB_DEBUG("reg1 hashValue=%d reg->ID=%d reg->tree=%08lx\n", 
                      reg1->hashValue,
                      reg1->ID, 
                      reg1->tree);

        HASHTAB_DEBUG("reg2 hashValue=%d reg->ID=%d reg->tree=%08lx\n", 
                      reg2->hashValue,
                      reg2->ID, 
                      reg2->tree);

        //swap registers
        copyRegister(&tempReg, reg1);
        copyRegister(reg1, reg2);
        copyRegister(reg2, &tempReg);
    }
}

int32_t printRegInfo(Register * reg)
{
    int32_t ret = SUCCESS;

    if (reg != NULL)
    {
        //ignore the empty registers
        if (reg->ID != 0)
        {
            ret = showRegInfo(reg->hashValue,
                              getNodeLevel(reg->tree),
                              reg->ID,
                              getParentID(reg->tree),
                              getNodeSide(reg->tree),
                              reg->first_name,
                              reg->last_name, 
                              reg->address, 
                              reg->city,
                              reg->tel1,
                              reg->tel2); 
        }
    }
    else
    {
        HASHTAB_ERROR("Register is null\n");
        ret = FAIL;
    }

    return ret;
}

// Entry points
int32_t createTable(void)
{
    int32_t ret = SUCCESS;
    
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
        uint32_t index = 0;
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
        HASHTAB_ERROR("Hash table is null\n");
    }
}

int32_t displayTable(void)
{
    int32_t ret = SUCCESS;

    if (g_hashTable != NULL)
    {
        uint32_t index = 0;
        Register * reg = NULL;
        
        // Free registers in the tree
        while (index < g_hashTable->size)
        {
            reg = getRegisterAtIndex(index);

            if (reg != NULL
                && reg->tree != NULL)
            {
                ret = showTree(reg->tree);
            }
            else
            {
                ret = printRegInfo(reg);
            }

            index++; 
        }
    }
    else
    {
        HASHTAB_ERROR("Hash table is null\n");
        ret = FAIL;
    }

    return ret;
}

int32_t insertReg(uint32_t ID,
                  char * pFirstName,
                  char * pLastName,
                  char * pAddress,
                  char * pCity,
                  char * pTel1,
                  char * pTel2,
                  uint32_t * numberOfSteps)
{
    int32_t       ret = SUCCESS;
    uint32_t      hashValue = 0;
    Register     *reg = NULL;

    hashValue = getHashValue(ID);
    reg = getRegisterAtIndex(hashValue);

    if (reg != NULL)
    {
        HASHTAB_DEBUG("hashValue=%d reg->ID=%d reg->tree=%08lx\n", 
                      hashValue,
                      reg->ID, 
                      reg->tree);
        if (numberOfSteps != NULL)
        {
            *numberOfSteps = 0;
        }

        //if the register is empty
        if (reg->ID == 0)
        {
            HASHTAB_DEBUG("Empty slot found\n");

            if (numberOfSteps != NULL)
            {
                *numberOfSteps = 1;
            }

            //copy the information
            insertInfoIntoReg(reg, 
                              hashValue, 
                              ID, 
                              pFirstName, 
                              pLastName, 
                              pAddress, 
                              pCity, 
                              pTel1, 
                              pTel2);
        }
        else   //there is a collision
        {
            HASHTAB_DEBUG("Collision detected\n");

            if (reg->ID == ID)
            {
                ret = REG_DUPLICATED;
            }
            else
            {
                //create a new register
                Register *newReg = NULL;

                newReg = allocRegisters(1);

                //copy the information
                insertInfoIntoReg(newReg, 
                                  hashValue, 
                                  ID, 
                                  pFirstName, 
                                  pLastName, 
                                  pAddress, 
                                  pCity, 
                                  pTel1, 
                                  pTel2);

                //Insert the new register into tree
                ret = insertRegIntoTree(reg, newReg, numberOfSteps);
            }
        }
    }
    else
    {
        HASHTAB_ERROR("Register is null\n");
        ret = FAIL;
    }
    
    return ret;
}

int32_t searchReg(uint32_t ID, uint32_t *numberOfSteps)
{
    int32_t ret = SUCCESS;
    uint32_t      hashValue = 0;
    Register     *reg = NULL;

    hashValue = getHashValue(ID);
    reg = getRegisterAtIndex(hashValue);

    if (reg != NULL)
    {
        HASHTAB_DEBUG("hashValue=%d reg->ID=%d ID=%d reg->tree=%08lx\n", 
                      hashValue, 
                      reg->ID, 
                      ID, 
                      reg->tree);

        if (numberOfSteps != NULL)
        {
            *numberOfSteps = 0;
        }

        if (reg->ID == ID)   //if the IDs are equals
        {
            HASHTAB_DEBUG("Register was found with the hash value\n");

            if (numberOfSteps != NULL)
            {
                *numberOfSteps = 1;
            }

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
        }
        else
        {
            //register was not found
            HASHTAB_WARNING("Register with ID=%d was not found\n", ID);
            ret = REG_NOT_FOUND;
        }
    }
    else
    {
        HASHTAB_ERROR("Register is null\n");
        ret = FAIL;
    }

    return ret;
}

int32_t removeReg(uint32_t ID, uint32_t *numberOfSteps)
{
    int32_t       ret = SUCCESS;
    uint32_t      hashValue = 0;
    Register     *reg = NULL;

    hashValue = getHashValue(ID);
    reg = getRegisterAtIndex(hashValue);

    if (reg != NULL)
    {
        HASHTAB_DEBUG("hashValue=%d reg->ID=%d ID=%d reg->tree=%08lx\n", 
                      hashValue, 
                      reg->ID, 
                      ID, 
                      reg->tree);

        if (numberOfSteps != NULL)
        {
            *numberOfSteps = 0;
        }

        if (reg->ID == ID)   //if the IDs are equals
        {
            HASHTAB_DEBUG("Register was found with the hash value\n");

            //register found and the register does not have a tree
            if (reg->tree == NULL)
            {
                if (numberOfSteps != NULL)
                {
                    *numberOfSteps = 1;
                }

                // clean information
                ret = cleanReg(reg);
            }
            else
            {
                //remove top node
                ret = removeRegIntoTree(reg, numberOfSteps);
            }
        }
        else if (reg->tree != NULL)  //if this register has a tree
        {
            //search register into tree
            ret = removeIDIntoTree(reg, ID, numberOfSteps);
        }
        else
        {
            //register was not found
            HASHTAB_WARNING("Register with ID=%d was not found\n", ID);
            ret = REG_NOT_FOUND;
        }
    }
    else
    {
        HASHTAB_ERROR("Register is null\n");
        ret = FAIL;
    }

    return ret;
}

int32_t changeReg(uint32_t ID, uint32_t *numberOfSteps)
{
    int32_t ret = SUCCESS;
    uint32_t      hashValue = 0;
    Register     *reg = NULL;

    hashValue = getHashValue(ID);
    reg = getRegisterAtIndex(hashValue);

    if (reg != NULL)
    {
        HASHTAB_DEBUG("hashValue=%d reg->ID=%d ID=%d reg->tree=%08lx\n", 
                      hashValue, 
                      reg->ID, 
                      ID, 
                      reg->tree);

        if (numberOfSteps != NULL)
        {
            *numberOfSteps = 0;
        }

        if (reg->ID == ID)   //if the IDs are equals
        {
            HASHTAB_DEBUG("Register was found with the hash value\n");

            if (numberOfSteps != NULL)
            {
                *numberOfSteps = 1;
            }

            //register found, print information
            printRegInfo(reg);
            ret = changeFieldsOfReg(reg->first_name, 
                                    reg->last_name, 
                                    reg->address, 
                                    reg->city, 
                                    reg->tel1, 
                                    reg->tel2);
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
                ret = changeFieldsOfReg(outputReg->first_name, 
                                        outputReg->last_name, 
                                        outputReg->address, 
                                        outputReg->city, 
                                        outputReg->tel1, 
                                        outputReg->tel2);
                printRegInfo(outputReg);
            }
        }
        else
        {
            //register was not found
            HASHTAB_WARNING("Register with ID=%d was not found\n", ID);
            ret = REG_NOT_FOUND;
        }
    }
    else
    {
        HASHTAB_ERROR("Register is null\n");
        ret = FAIL;
    }

    return ret;
}

int32_t destroyReg(Register *reg)
{
    int32_t ret = SUCCESS;
    
    if (reg != NULL)
    {
        freeRegister(reg);
    }
    else
    {
        HASHTAB_WARNING("Register is null\n");
        ret = FAIL;
    }

    return ret;
}

int32_t cleanReg(Register *reg)
{
    int32_t ret = SUCCESS;
    
    if (reg != NULL)
    {
        memset(reg, 0, sizeof(Register));
    }
    else
    {
        HASHTAB_WARNING("Register is null\n");
        ret = FAIL;
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

int32_t getIDFromReg(Register *reg)
{
    int32_t ret = 0L;
     
    if (reg != NULL)
    {
        ret = reg->ID;
    }
    
    return ret;
}


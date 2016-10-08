/*
 * Copyright (c) 2016 by Adrian Luna and 
 * All Rights Reserved
 *
 * Authors: Adrian Luna and 
 *
 * Porpuse: Implementation of algorithms to handle trees
 */

#include <stdio.h>
 
#include "tree.h"
#include "hash_table.h"
#include "memutils.h"
#include "trace.h"

//Defines
#define NODE_SIZE sizeof(Node)

//Structs
struct _Node
{
    long       ID;
    Node     * leftSide;
    Node     * rightSide;
    Register * reg;
};

//Definition of static functions
static Node * allocNode(void);
static void freeNode(Node *pNode);

//Implementation
static Node * allocNode(void)
{
    Node * pNode = NULL;
    
    pNode = (Node *)MEMALLOC(NODE_SIZE);
    
    if (pNode == NULL)
    {
        TREE_ERROR("allocNode() Node could not be allocated\n");
    }
    
    return pNode;
}

static void freeNode(Node *pNode)
{
    if (pNode != NULL)
    {
        MEMFREE((void *)pNode);
    }
    else
    {
        TREE_WARNING("freeNode() Node is null\n");
    }
}

static Node * createNode(Register *reg)
{
    Node * newNode = NULL;
    
    if (reg != NULL)
    {
        newNode = allocNode();
        
        if (newNode != NULL)
        {
            newNode->ID = getIDFromReg(reg);
            newNode->reg = reg;
        }
    }
    
    return newNode;
}

static void insertNode(Node *topNode, Node *newNode)
{
    if (newNode != NULL)
    {
        if(topNode == NULL)
        {
            //end of recursivity
            topNode = newNode;
        }
        else if(newNode->ID < topNode->ID)
        {
            insertNode(topNode->leftSide, newNode);
        }
        else if(newNode->ID > topNode->ID)
        {
            insertNode(topNode->rightSide, newNode);
        }
    }
}

static void balanceTree(Node *topNode)
{
    
}

int createTree(Register *baseReg, Register *newReg)
{
    int ret = SUCCESS;
    
    if (baseReg != NULL
       && newReg != NULL)
    {
        Node *topTree = NULL;
        Node *newNode = NULL;
        
        topTree = getTreeFromReg(baseReg);
        
        if (topTree == NULL)
        {
            //create top node of tree
            topTree = createNode(baseReg);
            setTreeToReg(baseReg, topTree);
        }
        
        if (topTree != NULL)
        {
            //create new node
            newNode = createNode(newReg);
            setTreeToReg(newReg, newNode);
            
            if (newNode != NULL)
            {
                insertNode(topTree, newNode);
                balanceTree(topTree);
            }
            else
            {
                ret = FAIL;
                TREE_ERROR("createTree() the new node could not be created\n");
            }
        }
        else
        {
            ret = FAIL;
            TREE_ERROR("createTree() the top node cannot be created\n");
        }
    }
    
    return ret;
}

void destroyTree(Node * tree)
{
    
}


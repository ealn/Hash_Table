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
    Node     * parent;
    Node     * leftSide;
    Node     * rightSide;
    Register * reg;
};

//Definition of static functions
static Node * allocNode(void);
static void freeNode(Node *pNode);
static Node * createNode(Register *reg);
static int insertNode(Node *topNode, Node *newNode);
static void balanceTree(Node *topNode);
static int createTree(Register *baseReg, Register *newReg);
static int searchID(Node *topNode, long ID, unsigned int *numberOfSteps, Node **ppOutputNode);

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

            TREE_DEBUG("createNode() newNode->ID=%d newNode->reg=%08lx", newNode->ID, newNode->reg);
        }
    }
    
    return newNode;
}

static int insertNode(Node *topNode, Node *newNode)
{
    int ret = SUCCESS;

    if (topNode != NULL
        && newNode != NULL)
    {
        Node *pNode = topNode;

        while (pNode != NULL)
        {
            TREE_DEBUG("insertNode() - newNode->ID=%d pNode->ID=%d\n", newNode->ID, pNode->ID);

            if (newNode->ID < pNode->ID)
            {
                if (pNode->leftSide != NULL)
                {
                    pNode = pNode->leftSide; 
                }
                else
                {
                    //position found
                    pNode->leftSide = newNode;
                    newNode->parent = pNode;
                    break;
                }
            }
            else if (newNode->ID > pNode->ID)
            {
                if (pNode->rightSide != NULL)
                {
                    pNode = pNode->rightSide; 
                }
                else
                {
                    //position found
                    pNode->rightSide = newNode;
                    newNode->parent = pNode;
                    break;
                }
            }
            else  //IDs are equal
            {
                ret = FAIL;
                TREE_ERROR("insertNode() IDs repeated\n");
            }
        }
    }
    else
    {
        TREE_ERROR("insertNode() top node is null or new node is null\n");
        ret = FAIL;
    }

    return ret;
}

static void balanceTree(Node *topNode)
{
    
}

static int searchID(Node *topNode, long ID, unsigned int *numberOfSteps, Node **ppOutputNode)
{
    int ret = SUCCESS;

    if (topNode != NULL
        && numberOfSteps != NULL
        && ppOutputNode != NULL)
    {
        Node *pNode = topNode;

        while (pNode != NULL)
        {
            TREE_DEBUG("searchID() - pNode->ID=%d ID=%d\n", pNode->ID, ID);

            if (ID == pNode->ID)  //IDs are equal
            {
                //Register found
                *ppOutputNode = pNode;
                break;
            }
            if (ID < pNode->ID) 
            {
                if (pNode->leftSide != NULL)
                {
                    pNode = pNode->leftSide; 
                }
                else
                {
                    //ID was not found
                    TREE_ERROR("searchID() ID was not found\n");
                    ret = FAIL;
                    break;
                }
            }
            else if (ID > pNode->ID)
            {
                if (pNode->rightSide != NULL)
                {
                    pNode = pNode->rightSide; 
                }
                else
                {
                    //ID was not found
                    TREE_ERROR("searchID() ID was not found\n");
                    ret = FAIL;
                    break;
                }
            }

            (*numberOfSteps)++;
        }
    }
    else
    {
        TREE_ERROR("searchID() at least one parameter is null\n");
    }

    return ret; 
}

static int createTree(Register *baseReg, Register *newReg)
{
    int ret = SUCCESS;
    
    if (baseReg != NULL
       && newReg != NULL)
    {
        Node *topTree = NULL;
        Node *newNode = NULL;
        
        topTree = getTreeFromReg(baseReg);
        
        //if the baseReg does not have a tree
        if (topTree == NULL)
        {
            //create top node of tree
            topTree = createNode(baseReg);
            setTreeToReg(baseReg, topTree);
            TREE_DEBUG("createTree() creating top node baseReg->tree=%08lx\n", getTreeFromReg(baseReg));
        }
        
        if (topTree != NULL)
        {
            //create new node
            newNode = createNode(newReg);
            setTreeToReg(newReg, newNode);
            TREE_DEBUG("createTree() creating new node newReg->tree=%08lx\n", getTreeFromReg(newReg));
            
            if (newNode != NULL)
            {
                ret = insertNode(topTree, newNode);

                if (ret == SUCCESS)
                {
                    balanceTree(topTree); 
                }
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

int insertRegIntoTree(Register *baseReg, Register *newReg)
{
    int ret = SUCCESS;

    if (baseReg != NULL
       && newReg != NULL)
    {
        TREE_DEBUG("insertRegIntoTree() baseReg=%08lx baseReg->ID=%d newReg=%08lx newReg->ID=%d\n",
                   baseReg,
                   getIDFromReg(baseReg),
                   newReg,
                   getIDFromReg(baseReg));

        ret = createTree(baseReg, newReg);
    }

    return ret;
}

int searchIDIntoTree(Register *baseReg, Register **ppOutputRegister, long ID, unsigned int *numberOfSteps)
{
    int ret = SUCCESS;

    if (baseReg != NULL
        && numberOfSteps != NULL
        && ppOutputRegister != NULL)
    {
        Node *topNode = NULL;
        Node *outputNode = NULL;

        TREE_DEBUG("searchIDIntoTree() baseReg=%08lx baseReg->ID=%d ID=%d *numberOfSteps=%d\n",
                   baseReg,
                   getIDFromReg(baseReg),
                   ID,
                   *numberOfSteps);

        topNode = getTreeFromReg(baseReg);

        if (topNode != NULL)
        {
            ret = searchID(topNode, ID, numberOfSteps, &outputNode);

            if (ret == SUCCESS
                && outputNode != NULL)
            {
                *ppOutputRegister = outputNode->reg;

                TREE_DEBUG("searchIDIntoTree() *ppOutputRegister=%08lx *ppOutputRegister->ID=%d *numberOfSteps=%d\n",
                            *ppOutputRegister,
                            getIDFromReg(*ppOutputRegister),
                            *numberOfSteps);
            }
        }
        else
        {
            TREE_ERROR("searchIDIntoTree() top node is null\n");
            ret = FAIL;
        }
    }
    else
    {
        TREE_ERROR("searchIDIntoTree() at least one parameter is null\n");
    }

    return ret;
}

void destroyTree(Node * tree)
{
    
}


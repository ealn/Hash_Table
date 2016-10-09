/*
 * Copyright (c) 2016 by Adrian Luna and Ricardo Gonzales
 * All Rights Reserved
 *
 * Authors: - Adrian Luna
 *          - Ricardo Gonzales
 *
 * Porpuse: Implementation of algorithms to handle trees
 */

#include <stdio.h>
#include <string.h>
 
#include "tree.h"
#include "hash_table.h"
#include "memutils.h"
#include "trace.h"

//Defines
#define NODE_SIZE sizeof(Node)
#define NAV_INORDER       1
#define NAV_PREORDER      2
#define NAV_POSTORDER     3

//Structs
struct _Node
{
    long       ID;
    Node     * parent;
    Node     * leftSide;
    Node     * rightSide;
    Register * reg;
};

//Function pointers
typedef int (*NavFunction)(Node *pNode);

//Definition of static functions
static Node * allocNode(void);
static void freeNode(Node *pNode);
static Node * createNode(Register *reg);
static int isTopNode(Node *pNode);
static int navigateTree(Node *topNode, int algorithm, NavFunction pNavFunction);
static int inOrden(Node *pNode, NavFunction pNavFunction);
static int preOrden(Node *pNode, NavFunction pNavFunction);
static int postOrden(Node *pNode, NavFunction pNavFunction);
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

            TREE_DEBUG("createNode() newNode->ID=%d newNode->reg=%08lx\n", newNode->ID, newNode->reg);
        }
    }
    
    return newNode;
}

static void destroyNode(Node * pNode)
{
    if (pNode != NULL)
    {
        Register * reg = pNode->reg;

        TREE_DEBUG("destroyNode() pNode->ID=%d pNode->parent=%08lx\n", pNode->ID, pNode->parent);

        //if this node was the top node
        if (isTopNode(pNode))
        {
            //Just clean the register
            cleanReg(reg);
        }
        else
        {
            //release the register
            destroyReg(reg);
        }

        freeNode(pNode);
    }
    else
    {
        TREE_WARNING("destroyNode() Node is null\n");
    }
}

static int isTopNode(Node *pNode)
{
    int ret = 0;

    if (pNode != NULL
        && pNode->parent == NULL)
    {
        ret = 1;
    }

    return ret; 
}

static Node * getTopNode(Node *pNode)
{
    Node *topNode = NULL;

    if (pNode != NULL)
    {
        topNode = pNode;

        while (topNode->parent != NULL)
        {
            topNode = topNode->parent;
        }
    }
    else
    {
        TREE_WARNING("getTopNode() Node is null\n");
    }

    return topNode;
}

static void replaceNode(Node *pNode, Node *pNewNode)
{
    if (pNode != NULL
        && pNewNode != NULL)
    {
        Node *parent = pNode->parent;

        pNewNode->parent = parent;

        if (parent != NULL)
        {
            if (parent->leftSide == pNode)
            {
                parent->leftSide = pNewNode; 
            }
            else if (parent->rightSide == pNode)
            {
                parent->rightSide = pNewNode;
            }
        }

        // Set pointers to NULL except the pointer to
        // the parent node because if this node is released
        // we need to know if this node was the top node
        pNode->leftSide = NULL;
        pNode->rightSide = NULL;
    }
    else
    {
        TREE_WARNING("replaceNode() at least one node is null\n");
    }
}

static int removeLeaf(Node *pNode)
{
    int ret = SUCCESS;

    if (pNode != NULL)
    {
        Node * parent;

        TREE_DEBUG("removeLeaf() pNode->ID=%d pNode->leftSide=%08lx pNode->rigthSide=%08lx\n", 
                   pNode->ID,
                   pNode->leftSide,
                   pNode->rightSide);

        parent = pNode->parent;

        if (pNode->leftSide == NULL
            && pNode->rightSide == NULL)
        {
            //Detach Node
            if (parent != NULL)
            {
                if (parent->leftSide == pNode)
                {
                    parent->leftSide = NULL;
                }
                else if (parent->rightSide == pNode)
                {
                    parent->rightSide = NULL;
                }
            }

            destroyNode(pNode); 
        }
        else
        {
            TREE_WARNING("removeLeaf() Node is not a leaf\n");
        }
    }
    else
    {
        TREE_WARNING("removeLeaf() Node is null\n");
    }

    return ret;
}

static int removeTreeWithOneChild(Node *pNode)
{
    int ret = SUCCESS;

    if (pNode != NULL)
    {
        TREE_DEBUG("removeTreeWithOneChild() pNode->ID=%d pNode->leftSide=%08lx pNode->rigthSide=%08lx\n", 
                   pNode->ID,
                   pNode->leftSide,
                   pNode->rightSide);

        if ((pNode->leftSide == NULL
             && pNode->rightSide != NULL)
            || (pNode->leftSide != NULL
                && pNode->rightSide == NULL))
        {
            Node * child = NULL;

            //if left child is not null
            if (pNode->leftSide != NULL)
            {
                child = pNode->leftSide;
            }
            else //else right child is not null
            {
                child = pNode->rightSide;
            }

            replaceNode(pNode, child);

            TREE_DEBUG("removeTreeWithOneChild() child->ID=%d child->leftSide=%08lx child->rigthSide=%08lx child->parent=%08lx\n", 
                       child->ID,
                       child->leftSide,
                       child->rightSide,
                       child->parent);

            destroyNode(pNode); 
        }
        else
        {
            ret = FAIL;
            TREE_WARNING("removeTreeWithOneChild() Node does not have one child\n");
        }
    }
    else
    {
        ret = FAIL;
        TREE_WARNING("removeTreeWithOneChild() Node is null\n");
    }

    return ret;
}

static int removeTreeWithTwoChilds(Node *pNode)
{
    int ret = SUCCESS;

    if (pNode != NULL)
    {
        TREE_DEBUG("removeTreeWithOneChild() pNode->ID=%d pNode->leftSide=%08lx pNode->rigthSide=%08lx\n", 
                   pNode->ID,
                   pNode->leftSide,
                   pNode->rightSide);

        if (pNode->leftSide != NULL
            && pNode->rightSide != NULL)
        {
            //TODO
            destroyNode(pNode);
        }
        else
        {
            ret = FAIL;
            TREE_WARNING("removeTreeWithOneChild() Node does not have one child\n");
        }
    }
    else
    {
        ret = FAIL;
        TREE_WARNING("removeTreeWithOneChild() Node is null\n");
    }

    return ret;
}

static int removeNodeFromTree(Node *pNode)
{
    int ret = SUCCESS;

    if (pNode != NULL)
    {
        int needBalance = 0;
        Node *topNode = NULL;

        TREE_DEBUG("removeNodeFromTree() pNode->ID=%d pNode->leftSide=%08lx pNode->rigthSide=%08lx\n", 
                   pNode->ID,
                   pNode->leftSide,
                   pNode->rightSide);

        topNode = getTopNode(pNode);

        if (pNode->leftSide == NULL
            && pNode->rightSide == NULL)
        {
            ret = removeLeaf(pNode);
        }
        else if ((pNode->leftSide == NULL
                  && pNode->rightSide != NULL)
                 || (pNode->leftSide != NULL
                     && pNode->rightSide == NULL))
        {
            ret = removeTreeWithOneChild(pNode);
            needBalance = 1;
        }
        else if (pNode->leftSide != NULL
                 && pNode->rightSide != NULL)
        {
            ret = removeTreeWithTwoChilds(pNode);
            needBalance = 1;
        }

        if (needBalance == 1)
        {
            balanceTree(topNode);
        }
    }
    else
    {
        TREE_ERROR("removeNodeFromTree() Node is null\n");
    }

    return ret;
}

/*************************************************
     Navigate Tree algorithms                     
                                                  
               15                                 
              /  \                                
             /    \                               
            /      \                              
           9        20                            
          / \       / \                           
         /   \     /   \                          
        6     14  17   64                         
              /        / \                        
             /        /   \                       
            13       26   72                      
                                                  
Inorden = [6, 9, 13, 14, 15, 17, 20, 26, 64, 72]  
Preorden = [15, 9, 6, 14, 13, 20, 17, 64, 26, 72] 
Postorden =[6, 13, 14, 9, 17, 26, 72, 64, 20, 15] 
 
**************************************************/

static int navigateTree(Node *topNode, int algorithm, NavFunction pNavFunction)
{
    int ret = SUCCESS;

    if (topNode != NULL
        && pNavFunction != NULL)
    {
        switch (algorithm)
        {
            case NAV_INORDER: ret = inOrden(topNode, pNavFunction);
                break;
            case NAV_PREORDER: ret = preOrden(topNode, pNavFunction);
                break;
            case NAV_POSTORDER: ret = postOrden(topNode, pNavFunction);
                break;
            default:
                TREE_WARNING("navigateTree() invalid algorithm\n");
                break;
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("navigateTree() top node is null or navigation function is null\n");
    }

    return ret;
}

static int inOrden(Node *pNode, NavFunction pNavFunction)
{
    int ret = SUCCESS;

    if (pNavFunction != NULL)
    {
        if (pNode != NULL) 
        {
            Node *leftNode = pNode->leftSide;
            Node *rightNode = pNode->rightSide;

            ret = inOrden(leftNode, pNavFunction);

            TREE_DEBUG("inOrden() pNode->ID=%d\n",pNode->ID);

            ret = pNavFunction(pNode);

            ret = inOrden(rightNode, pNavFunction);
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("inOrden() navigation function is null\n");
    }

    return ret;
}

static int preOrden(Node *pNode, NavFunction pNavFunction)
{
    int ret = SUCCESS;

    if (pNavFunction != NULL)
    {
        if (pNode != NULL) 
        {
            Node *leftNode = pNode->leftSide;
            Node *rightNode = pNode->rightSide;

            TREE_DEBUG("preOrden() pNode->ID=%d\n",pNode->ID);

            ret = pNavFunction(pNode);

            ret = preOrden(leftNode, pNavFunction);
            ret = preOrden(rightNode, pNavFunction);
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("preOrden() navigation function is null\n");
    }

    return ret;
}

static int postOrden(Node *pNode, NavFunction pNavFunction)
{
    int ret = SUCCESS;

    if (pNavFunction != NULL)
    {
        if (pNode != NULL) 
        {
            Node *leftNode = pNode->leftSide;
            Node *rightNode = pNode->rightSide;

            ret = postOrden(leftNode, pNavFunction);
            ret = postOrden(rightNode, pNavFunction);

            TREE_DEBUG("postOrden() pNode->ID=%d\n",pNode->ID);

            ret = pNavFunction(pNode);
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("postOrden() navigation function is null\n");
    }

    return ret;
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

            if (numberOfSteps != NULL)
            {
                (*numberOfSteps)++; 
            }
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
        && ppOutputRegister != NULL)
    {
        Node *topNode = NULL;
        Node *outputNode = NULL;

        TREE_DEBUG("searchIDIntoTree() baseReg=%08lx baseReg->ID=%d ID=%d\n",
                   baseReg,
                   getIDFromReg(baseReg),
                   ID);

        topNode = getTreeFromReg(baseReg);

        if (topNode != NULL)
        {
            ret = searchID(topNode, ID, numberOfSteps, &outputNode);

            if (ret == SUCCESS
                && outputNode != NULL)
            {
                *ppOutputRegister = outputNode->reg;

                TREE_DEBUG("searchIDIntoTree() *ppOutputRegister=%08lx *ppOutputRegister->ID=%d\n",
                            *ppOutputRegister,
                            getIDFromReg(*ppOutputRegister));
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
    if (tree != NULL)
    {
        NavFunction pNavFunction = NULL;
        int ret;

        //set the function removeNodeFromTree
        pNavFunction = removeNodeFromTree;

        ret = navigateTree(tree, NAV_POSTORDER, pNavFunction);
        
        if (ret != SUCCESS)
        {
            TREE_ERROR("destroyTree() ret=%d\n", ret);
        }
    }
    else
    {
        TREE_ERROR("destroyTree() tree is null\n");
    }
}


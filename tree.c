/*
 * Copyright (c) 2016 by Adrian Luna
 * All Rights Reserved
 *
 * Author: - Adrian Luna
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
#define NODE_SIZE         sizeof(Node)
#define NAV_INORDER       1
#define NAV_PREORDER      2
#define NAV_POSTORDER     3

#define LEFT_TOO_HEAVY   -2
#define LEFT_HEAVY       -1
#define BALANCED          0
#define RIGHT_HEAVY       1
#define RIGHT_TOO_HEAVY   2

#define LEFT_SIDE         1
#define RIGHT_SIDE        2

//Structs
struct _Node
{
    uint32_t   ID;
    uint32_t   level;
    int32_t    FE;
    bool       needFreeReg;
    Node     * parent;
    Node     * leftSide;
    Node     * rightSide;
    Register * reg;
};

//Type definitions
typedef int32_t (*NavFunction)(Node *pNode,  uint32_t * numberOfSteps);

typedef struct _NavParmBlock
{
    NavFunction   pNavFunction;
    uint32_t    * numberOfSteps;
}NavParmBlock;

//Definition of static functions
static Node * allocNode(void);
static void freeNode(Node *pNode);
static Node * createNode(Register *reg);
static int32_t isTopNode(Node *pNode);
static int32_t navigateTree(Node *topNode, uint32_t algorithm, NavParmBlock * pNavParmBlock);
static int32_t inOrden(Node *pNode, NavParmBlock * pNavParmBlock);
static int32_t preOrden(Node *pNode, NavParmBlock * pNavParmBlock);
static int32_t postOrden(Node *pNode, NavParmBlock * pNavParmBlock);
static int32_t insertNode(Node **ppTopNode, Node *newNode, uint32_t * numberOfSteps);
static int32_t balanceTree(Node **ppTopNode, Node *pNode, uint8_t side, bool isNewNode, uint32_t * numberOfSteps);
static int32_t SimpleRotationLeft(Node **ppTopNode, Node *pNode);
static int32_t SimpleRotationRight(Node **ppTopNode, Node *pNode); 
static int32_t DoubleRotationLeft(Node **ppTopNode, Node *pNode);
static int32_t DoubleRotationRight(Node **ppTopNode, Node *pNode);
static int32_t createTree(Register *baseReg, Register *newReg, uint32_t * numberOfSteps);
static int32_t searchID(Node *topNode, uint32_t ID, uint32_t *numberOfSteps, Node **ppOutputNode);

//Implementation
static Node * allocNode(void)
{
    Node * pNode = NULL;
    
    pNode = (Node *)MEMALLOC(NODE_SIZE);
    
    if (pNode == NULL)
    {
        TREE_ERROR("Node could not be allocated\n");
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
        TREE_WARNING("Node is null\n");
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

            TREE_DEBUG("newNode->ID=%d newNode->reg=%08lx\n", newNode->ID, newNode->reg);
        }
    }
    
    return newNode;
}

static void destroyNode(Node * pNode)
{
    if (pNode != NULL)
    {
        Register * reg = NULL;

        reg = pNode->reg;

        TREE_DEBUG("pNode->ID=%d pNode->parent=%08lx pNode->reg=%08lx\n", 
                   pNode->ID, 
                   pNode->parent,
                   pNode->reg);

        if (reg != NULL)
        {
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
        }

        freeNode(pNode);
    }
    else
    {
        TREE_WARNING("Node is null\n");
    }
}

static int32_t isTopNode(Node *pNode)
{
    int32_t ret = 0;

    if (pNode != NULL
        && pNode->parent == NULL)
    {
        ret = 1;
    }

    return ret; 
}

static Node * getTopNode(Node *pNode, uint32_t * numberOfSteps)
{
    Node *topNode = NULL;

    if (pNode != NULL)
    {
        topNode = pNode;

        while (topNode->parent != NULL)
        {
            topNode = topNode->parent;

            if (numberOfSteps != NULL)
            {
                (*numberOfSteps)++;
            }
        }
    }

    return topNode;
}

static Node * getLastLeftChild(Node *pNode, uint32_t * numberOfSteps)
{
    Node * lastLeftChild = NULL;

    if (pNode != NULL)
    {
        lastLeftChild = pNode;

        while (lastLeftChild != NULL
               && lastLeftChild->leftSide != NULL)
        {
            lastLeftChild = lastLeftChild->leftSide;

            if (numberOfSteps != NULL)
            {
                (*numberOfSteps)++;
            }
        }
    }

    return lastLeftChild;
}

static void changeTopNode(Node *pTopNode, Node *pNewNode)
{
    if (pTopNode != NULL
        && pNewNode != NULL
        && isTopNode(pTopNode))
    {
        Register *pTopReg;
        Register *pNewReg;

        pTopReg = pTopNode->reg;
        pNewReg = pNewNode->reg;

        //deteach registers
        setTreeToReg(pTopReg, NULL);
        setTreeToReg(pNewReg, NULL);
        pTopNode->reg = NULL;
        pNewNode->reg = NULL;

        //swap registers
        swapRegister(pTopReg, pNewReg);

        //swap nodes
        setTreeToReg(pTopReg, pNewNode);
        setTreeToReg(pNewReg, pTopNode);
        pTopNode->reg = pNewReg;
        pNewNode->reg = pTopReg;
    }
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

        pNewNode->level = pNode->level;

        if (isTopNode(pNode))
        {
            changeTopNode(pNode, pNewNode);
        }

        // Set pointers to NULL except the pointer to
        // the parent node because if this node is released
        // we need to know if this node was the top node
        //pNode->leftSide = NULL;
        //pNode->rightSide = NULL;
    }
    else
    {
        TREE_WARNING("At least one node is null\n");
    }
}

static int32_t removeLeaf(Node *pNode)
{
    int32_t ret = SUCCESS;

    if (pNode != NULL)
    {
        Node * parent;

        TREE_DEBUG("pNode->ID=%d pNode->leftSide=%08lx pNode->rigthSide=%08lx\n", 
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
            TREE_WARNING("Node is not a leaf\n");
        }
    }
    else
    {
        TREE_WARNING("Node is null\n");
    }

    return ret;
}

static int32_t removeTreeWithOneChild(Node *pNode)
{
    int32_t ret = SUCCESS;

    if (pNode != NULL)
    {
        TREE_DEBUG("pNode->ID=%d pNode->leftSide=%08lx pNode->rigthSide=%08lx\n", 
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

            TREE_DEBUG("child->ID=%d child->leftSide=%08lx child->rigthSide=%08lx child->parent=%08lx\n", 
                       child->ID,
                       child->leftSide,
                       child->rightSide,
                       child->parent);

            destroyNode(pNode); 
        }
        else
        {
            ret = FAIL;
            TREE_WARNING("Node does not have one child\n");
        }
    }
    else
    {
        ret = FAIL;
        TREE_WARNING("Node is null\n");
    }

    return ret;
}

static int32_t removeTreeWithTwoChilds(Node *pNode, uint32_t * numberOfSteps)
{
    int32_t ret = SUCCESS;

    if (pNode != NULL)
    {
        TREE_DEBUG("pNode->ID=%d pNode->leftSide=%08lx pNode->rigthSide=%08lx\n", 
                   pNode->ID,
                   pNode->leftSide,
                   pNode->rightSide);

        if (pNode->leftSide != NULL
            && pNode->rightSide != NULL)
        {
            Node *rightChild = NULL;
            Node *leftChild = NULL;
            Node *lastLeftChild = NULL;

            rightChild = pNode->rightSide;
            leftChild = pNode->leftSide;

            //find the last left child
            lastLeftChild = getLastLeftChild(rightChild, numberOfSteps);

            // if there is not more left childs
            if (lastLeftChild == rightChild)
            {
                replaceNode(pNode, lastLeftChild);
                lastLeftChild->leftSide = leftChild;
                leftChild->parent = lastLeftChild;
            }
            else
            {
                //if the last left child has a right child
                if (lastLeftChild->rightSide != NULL)
                {
                    //replace the last left child with the right node
                    replaceNode(lastLeftChild, lastLeftChild->rightSide);
                }

                // replace pNode with the last left child
                replaceNode(pNode, lastLeftChild);
                lastLeftChild->leftSide = leftChild;
                lastLeftChild->rightSide = rightChild;
                leftChild->parent = lastLeftChild;
                rightChild->parent = lastLeftChild;
            }

            destroyNode(pNode); 
        }
        else
        {
            ret = FAIL;
            TREE_WARNING("Node does not have one child\n");
        }
    }
    else
    {
        ret = FAIL;
        TREE_WARNING("Node is null\n");
    }

    return ret;
}

static int32_t removeNodeFromTree(Node *pNode, uint32_t * numberOfSteps)
{
    int32_t ret = SUCCESS;

    if (pNode != NULL)
    {
        bool needBalance = false;
        Node *topNode = NULL;

        TREE_DEBUG("pNode->ID=%d pNode->leftSide=%08lx pNode->rigthSide=%08lx\n", 
                   pNode->ID,
                   pNode->leftSide,
                   pNode->rightSide);

        if (isTopNode(pNode))
        {
            //TODO
        }
        else
        {
            topNode = getTopNode(pNode, numberOfSteps); 
        }

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
            ret = removeTreeWithTwoChilds(pNode, numberOfSteps);
            needBalance = 1;
        }
        else
        {
            ret = FAIL;
        }
    }
    else
    {
        TREE_ERROR("Node is null\n");
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

static int32_t navigateTree(Node *topNode, uint32_t algorithm, NavParmBlock * pNavParmBlock)
{
    int32_t ret = SUCCESS;

    if (topNode != NULL
        && pNavParmBlock != NULL
        && pNavParmBlock->pNavFunction != NULL)
    {
        switch (algorithm)
        {
            case NAV_INORDER: ret = inOrden(topNode, pNavParmBlock);
                break;
            case NAV_PREORDER: ret = preOrden(topNode, pNavParmBlock);
                break;
            case NAV_POSTORDER: ret = postOrden(topNode, pNavParmBlock);
                break;
            default:
                TREE_WARNING("Invalid algorithm\n");
                break;
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("Top node is null or navigation function is null\n");
    }

    return ret;
}

static int32_t inOrden(Node *pNode, NavParmBlock * pNavParmBlock)
{
    int32_t ret = SUCCESS;

    if (pNavParmBlock != NULL
        && pNavParmBlock->pNavFunction != NULL)
    {
        if (pNode != NULL) 
        {
            Node *leftNode = pNode->leftSide;
            Node *rightNode = pNode->rightSide;

            ret = inOrden(leftNode, pNavParmBlock);

            //TODO: break recursivity when a fails occurs

            TREE_DEBUG("pNode->ID=%d\n",pNode->ID);

            ret = pNavParmBlock->pNavFunction(pNode, 
                                              pNavParmBlock->numberOfSteps);

            ret = inOrden(rightNode, pNavParmBlock);
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("Navigation function is null\n");
    }

    return ret;
}

static int32_t preOrden(Node *pNode, NavParmBlock * pNavParmBlock)
{
    int32_t ret = SUCCESS;

    if (pNavParmBlock != NULL
        && pNavParmBlock->pNavFunction != NULL)
    {
        if (pNode != NULL) 
        {
            Node *leftNode = pNode->leftSide;
            Node *rightNode = pNode->rightSide;

            TREE_DEBUG("pNode->ID=%d\n",pNode->ID);

            ret = pNavParmBlock->pNavFunction(pNode, 
                                              pNavParmBlock->numberOfSteps);

            //TODO: break recursivity when a fails occurs
            ret = preOrden(leftNode, pNavParmBlock);
            ret = preOrden(rightNode, pNavParmBlock);
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("Navigation function is null\n");
    }

    return ret;
}

static int32_t postOrden(Node *pNode, NavParmBlock * pNavParmBlock)
{
    int32_t ret = SUCCESS;

    if (pNavParmBlock != NULL
        && pNavParmBlock->pNavFunction != NULL)
    {
        if (pNode != NULL) 
        {
            Node *leftNode = pNode->leftSide;
            Node *rightNode = pNode->rightSide;

            //TODO: break recursivity when a fails occurs
            ret = postOrden(leftNode, pNavParmBlock);
            ret = postOrden(rightNode, pNavParmBlock);

            TREE_DEBUG("pNode->ID=%d\n",pNode->ID);

            ret = pNavParmBlock->pNavFunction(pNode, 
                                              pNavParmBlock->numberOfSteps);
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("Navigation function is null\n");
    }

    return ret;
}

static int32_t insertNode(Node **ppTopNode, Node *newNode, uint32_t * numberOfSteps)
{
    int32_t ret = SUCCESS;

    if (ppTopNode != NULL
        && *ppTopNode != NULL
        && newNode != NULL)
    {
        Node *pNode = *ppTopNode;

        while (pNode != NULL)
        {
            TREE_DEBUG("newNode->ID=%d pNode->ID=%d\n", newNode->ID, pNode->ID);

            if (numberOfSteps != NULL)
            {
                (*numberOfSteps)++;
            }

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
                    newNode->level = pNode->level + 1;
                    //balance tree
                    //balanceTree(ppTopNode, pNode, LEFT_SIDE, true, numberOfSteps);
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
                    newNode->level = pNode->level + 1;
                    //balance tree
                    //balanceTree(ppTopNode, pNode, RIGHT_SIDE, true, numberOfSteps);
                    break;
                }
            }
            else  //IDs are equal
            {
                ret = REG_DUPLICATED;
                TREE_ERROR("IDs repeated\n");
                break;
            }
        }
    }
    else
    {
        TREE_ERROR("Top node is null or new node is null\n");
        ret = FAIL;
    }

    return ret;
}

static int32_t balanceTree(Node **ppTopNode, Node *pNode, uint8_t side, bool isNewNode, uint32_t * numberOfSteps) 
{ 
    int32_t ret = SUCCESS;

    if (ppTopNode != NULL
        && *ppTopNode != NULL
        && pNode != NULL)
    {
        bool exitLoop = false;
        Node *leftSide = NULL;
        Node *rightSide = NULL;
        Node *parent = NULL;

        TREE_DEBUG("topnode->ID=%d pNode->ID=%d side=%d isNewNode=%d\n", 
                   (*ppTopNode)->ID,
                   pNode->ID,
                   side,
                   isNewNode);

        // Browse using a reverse path and update values of FE
        while(pNode != NULL 
              && !exitLoop) 
        { 
            leftSide = pNode->leftSide;
            rightSide = pNode->rightSide;

            TREE_DEBUG("pNode->ID=%d pNode->FE=%d\n", pNode->ID, pNode->FE);

            // if this is a new node
            if(isNewNode) 
            {
                if(side == LEFT_SIDE)
                {
                    pNode->FE--;
                }
                else //RIGHT_SIDE
                {
                    pNode->FE++;
                }
            }            
            else // else the node was removed
            {
                if(side == LEFT_SIDE) 
                {
                    pNode->FE++;
                }
                else //RIGHT_SIDE
                {
                    pNode->FE--;
                }
            }
            
            //if tree is balanced
            if(pNode->FE == BALANCED)
            {
                //exit
                exitLoop = true;
            }
            else if(pNode->FE == LEFT_TOO_HEAVY) 
            { 
                // rotate to the right and exit
                if(leftSide->FE == RIGHT_HEAVY)
                {
                    //make double rotation to the right
                    ret = DoubleRotationRight(ppTopNode, pNode);
                }
                else // LEFT_HEAVY
                {
                    //make a simple rotation to the right
                    ret = SimpleRotationRight(ppTopNode, pNode);
                }

                exitLoop = true; 
            } 
            else if(pNode->FE == RIGHT_TOO_HEAVY) 
            { 
                /* rotate to the left and exit*/ 
                if(rightSide->FE == LEFT_HEAVY)
                {
                    //make double rotation to the left
                    ret = DoubleRotationLeft(ppTopNode, pNode);
                }
                else // RIGHT_HEAVY
                {
                    //make a simple rotation to the left
                    ret = SimpleRotationLeft(ppTopNode, pNode);
                }

                exitLoop = true; 
            } 
            
            parent = pNode->parent;

            if(parent != NULL) 
            {
                if(parent->rightSide == pNode)
                {
                    side = RIGHT_SIDE;
                }
                else if (parent->leftSide == pNode)
                {
                    side = LEFT_SIDE;
                }
                else
                {
                    TREE_ERROR("Node parent is not pointing to the node\n");
                    ret = FAIL;
                    exitLoop = true;
                }

                //calculate the next node
                pNode = pNode->parent;
            } 

            if (numberOfSteps != NULL)
            {
                (*numberOfSteps)++;
            }
        } 
    }
    else
    {
        TREE_ERROR("Top node is null or node is null\n");
        ret = FAIL;
    }

    return ret;
}

static int32_t SimpleRotationLeft(Node **ppTopNode, Node *pNode)
{ 
    int32_t ret = SUCCESS;

    if (ppTopNode != NULL
        && *ppTopNode != NULL
        && pNode != NULL
        && pNode->rightSide != NULL)
    {
        Node * parent = pNode->parent; 
        Node * rightSide = pNode->rightSide; 
        Node * leftChildOfRightSide = rightSide->leftSide;

        TREE_DEBUG("pNode->ID=%d pNode->rightSide->ID=%d\n",
                   pNode->ID,
                   pNode->rightSide->ID);
        
        if(parent != NULL) 
        {
            TREE_DEBUG("parent->ID=%d\n", parent->ID);

            if(parent->rightSide == pNode) 
            {
                parent->rightSide = rightSide;
            }
            else
            {
                parent->leftSide = rightSide;
            }
        }
        else
        {
            *ppTopNode = rightSide;
        }
        
        // Rebuild tree
        pNode->rightSide = leftChildOfRightSide; 
        rightSide->leftSide = pNode; 
        
        // Reassign parents
        pNode->parent = rightSide; 
        
        if(leftChildOfRightSide != NULL)
        {
            TREE_DEBUG("pNode->rightSide->leftSide->ID=%d\n",
                       leftChildOfRightSide->ID);

            leftChildOfRightSide->parent = pNode;
        }

        rightSide->parent = parent; 
        
        // set to 0 the FE
        pNode->FE = BALANCED; 
        rightSide->FE = BALANCED; 
    }
    else
    {
        TREE_ERROR("At least one parameter is null\n");
        ret = FAIL;
    }

    return ret;
} 

static int32_t SimpleRotationRight(Node **ppTopNode, Node *pNode) 
{ 
    int32_t ret = SUCCESS;

    if (ppTopNode != NULL
        && *ppTopNode != NULL
        && pNode != NULL
        && pNode->leftSide != NULL)
    {
        Node *parent = pNode->parent; 
        Node *leftSide = pNode->leftSide; 
        Node *rightChildOfLeftSide = leftSide->rightSide;  //pNode->leftSide->rightSide
        
        TREE_DEBUG("pNode->ID=%d pNode->leftSide->ID=%d\n",
                   pNode->ID,
                   pNode->leftSide->ID);

        if(parent != NULL) 
        {
            replaceNode(pNode, leftSide);
            /*
            if(parent->rightSide == pNode) 
            {
                parent->rightSide = leftSide;
            }
            else
            {
                parent->leftSide = leftSide;
            }*/
        }
        else
        {
            replaceNode(*ppTopNode, leftSide);
            //*ppTopNode = leftSide;
        }
        
        // Rebuild tree 
        pNode->leftSide = rightChildOfLeftSide; 
        leftSide->rightSide = pNode;
        
        // Reassign parents
        pNode->parent = leftSide; 
        
        if(rightChildOfLeftSide != NULL)
        {
            TREE_DEBUG("pNode->leftSide->rightSide->ID=%d\n",
                       rightChildOfLeftSide->ID);

            rightChildOfLeftSide->parent = pNode;
        }

        //leftSide->parent = parent; 
        
        // set to 0 the FE
        pNode->FE = BALANCED; 
        leftSide->FE = BALANCED; 
    }
    else
    {
        TREE_ERROR("At least one parameter is null\n");
        ret = FAIL;
    }

    return ret;
} 

static int32_t DoubleRotationLeft(Node **ppTopNode, Node *pNode) 
{ 
    int32_t ret = SUCCESS;

    if (ppTopNode != NULL
        && *ppTopNode != NULL
        && pNode != NULL
        && pNode->rightSide != NULL
        && pNode->rightSide->leftSide != NULL)
    {
        Node * parent = pNode->parent; 
        Node * rightSide = pNode->rightSide; 
        Node * leftChildOfRightSide = rightSide->leftSide;                  //pNode->rightSide->leftSide
        Node * leftLeftChildOfRightSide = leftChildOfRightSide->leftSide;   //pNode->rightSide->leftSide->leftSide
        Node * leftRightChildOfRightSide = leftChildOfRightSide->rightSide; //pNode->rightSide->leftSide->rightSide
        
        TREE_DEBUG("pNode->ID=%d pNode->rightSide->ID=%d pNode->rightSide->leftSide->ID=%d\n",
                   pNode->ID,
                   rightSide->ID,
                   leftChildOfRightSide->leftSide->ID);

        if(parent != NULL) 
        {
            TREE_DEBUG("parent->ID=%d\n", parent->ID);

            if(parent->rightSide == pNode) 
            {
                parent->rightSide = leftChildOfRightSide;
            }
            else 
            {
                parent->leftSide = leftChildOfRightSide;
            }
        }
        else
        {
            *ppTopNode = leftChildOfRightSide;
        }
        
        // Rebuild tree 
        pNode->rightSide = leftLeftChildOfRightSide; 
        rightSide->leftSide = leftRightChildOfRightSide; 
        leftChildOfRightSide->leftSide = pNode; 
        leftChildOfRightSide->rightSide = rightSide; 
        
        // Reassign parents
        leftChildOfRightSide->parent = parent;
        pNode->parent = rightSide->parent = leftChildOfRightSide; 
        
        if(leftLeftChildOfRightSide != NULL) 
        {
            TREE_DEBUG("pNode->rightSide->leftSide->leftSide->ID=%d\n",
                       leftLeftChildOfRightSide->ID);

            leftLeftChildOfRightSide->parent = pNode;
        }
        if(leftRightChildOfRightSide != NULL) 
        {
            TREE_DEBUG("pNode->rightSide->leftSide->rightSide->ID=%d\n",
                       leftRightChildOfRightSide->ID);

            leftRightChildOfRightSide->parent = rightSide;
        }
        
        // adjust FE
        switch(leftChildOfRightSide->FE) 
        {
            case LEFT_HEAVY:  pNode->FE = BALANCED; 
                              rightSide->FE = RIGHT_HEAVY; 
                          break; 
            case BALANCED:    pNode->FE = BALANCED; 
                              rightSide->FE = BALANCED; 
                          break; 
            case RIGHT_HEAVY: pNode->FE = LEFT_HEAVY; 
                              rightSide->FE = BALANCED; 
                          break; 
        } 
        
        leftChildOfRightSide->FE = BALANCED; 
    }
    else
    {
        TREE_ERROR("At least one parameter is null\n");
        ret = FAIL;
    }

    return ret;
} 

static int32_t DoubleRotationRight(Node **ppTopNode, Node *pNode) 
{ 
    int32_t ret = SUCCESS;

    if (ppTopNode != NULL
        && *ppTopNode != NULL
        && pNode != NULL
        && pNode->leftSide != NULL
        && pNode->leftSide->rightSide != NULL)
    {
        Node * parent = pNode->parent; 
        Node * leftSide = pNode->leftSide;
        Node * rightChildOfLeftSide = leftSide->rightSide;                   //pNode->leftSide->rightSide
        Node * rightLeftChildOfLeftSide = rightChildOfLeftSide->leftSide;    //pNode->leftSide->rightSide->leftSide  
        Node * rightRightChildOfLeftSide = rightChildOfLeftSide->rightSide;  //pNode->leftSide->rightSide->rightSide 
        
        TREE_DEBUG("pNode->ID=%d pNode->leftSide->ID=%d pNode->leftSide->rightSide->ID=%d\n",
                   pNode->ID,
                   leftSide->ID,
                   rightChildOfLeftSide->ID);

        if(parent != NULL) 
        {
            TREE_DEBUG("parent->ID=%d\n", parent->ID);

            if(parent->rightSide == pNode)
            {
                parent->rightSide = rightChildOfLeftSide;
            }
            else 
            {
                parent->leftSide = rightChildOfLeftSide;
            }
        }
        else
        {
            *ppTopNode = rightChildOfLeftSide;
        }
        
        // Rebuild tree 
        leftSide->rightSide = rightLeftChildOfLeftSide; 
        pNode->leftSide = rightRightChildOfLeftSide; 
        rightChildOfLeftSide->leftSide = leftSide; 
        rightChildOfLeftSide->rightSide = pNode; 
        
        // Reassign parents
        rightChildOfLeftSide->parent = parent; 
        pNode->parent = leftSide->parent = rightChildOfLeftSide; 
        
        if(rightLeftChildOfLeftSide != NULL)
        {
            TREE_DEBUG("pNode->leftSide->rightSide->leftSide->ID=%d\n",
                       rightLeftChildOfLeftSide->ID);

            rightLeftChildOfLeftSide->parent = leftSide;
        }
        if(rightRightChildOfLeftSide != NULL)
        {
            TREE_DEBUG("pNode->leftSide->rightSide->rightSide->ID=%d\n",
                       rightRightChildOfLeftSide->ID);

            rightRightChildOfLeftSide->parent = pNode;
        }
        
        // adjust FE
        switch(rightChildOfLeftSide->FE) 
        { 
            case LEFT_HEAVY:  leftSide->FE = BALANCED; 
                              pNode->FE = RIGHT_HEAVY; 
                           break;
            case BALANCED:    leftSide->FE = BALANCED; 
                              pNode->FE = BALANCED; 
                           break; 
            case RIGHT_HEAVY: leftSide->FE = LEFT_HEAVY; 
                              pNode->FE = BALANCED; 
                           break; 
        } 
        
        rightChildOfLeftSide->FE = 0; 
    }
    else
    {
        TREE_ERROR("At least one parameter is null\n");
        ret = FAIL;
    }

    return ret;
}

static int32_t searchID(Node *topNode, uint32_t ID, uint32_t *numberOfSteps, Node **ppOutputNode)
{
    int32_t ret = SUCCESS;

    if (topNode != NULL
        && ppOutputNode != NULL)
    {
        Node *pNode = topNode;

        while (pNode != NULL)
        {
            TREE_DEBUG("pNode->ID=%d ID=%d\n", pNode->ID, ID);

            if (numberOfSteps != NULL)
            {
                (*numberOfSteps)++; 
            }

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
                    TREE_ERROR("ID was not found\n");
                    ret = REG_NOT_FOUND;
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
                    TREE_ERROR("ID was not found\n");
                    ret = REG_NOT_FOUND;
                    break;
                }
            }
        }
    }
    else
    {
        TREE_ERROR("At least one parameter is null\n");
    }

    return ret; 
}

static int32_t createTree(Register *baseReg, Register *newReg, uint32_t * numberOfSteps)
{
    int32_t ret = SUCCESS;
    
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
            TREE_DEBUG("Creating top node baseReg->tree=%08lx\n", getTreeFromReg(baseReg));
        }
        
        if (topTree != NULL)
        {
            //create new node
            newNode = createNode(newReg);
            setTreeToReg(newReg, newNode);
            TREE_DEBUG("Creating new node newReg->tree=%08lx\n", getTreeFromReg(newReg));
            
            if (newNode != NULL)
            {
                ret = insertNode(&topTree, newNode, numberOfSteps);
            }
            else
            {
                ret = FAIL;
                TREE_ERROR("New node could not be created\n");
            }
        }
        else
        {
            ret = FAIL;
            TREE_ERROR("Top node cannot be created\n");
        }
    }
    
    return ret;
}

int32_t insertRegIntoTree(Register *baseReg, Register *newReg, uint32_t * numberOfSteps)
{
    int32_t ret = SUCCESS;

    if (baseReg != NULL
       && newReg != NULL)
    {
        TREE_DEBUG("baseReg=%08lx baseReg->ID=%d newReg=%08lx newReg->ID=%d\n",
                   baseReg,
                   getIDFromReg(baseReg),
                   newReg,
                   getIDFromReg(baseReg));

        ret = createTree(baseReg, newReg, numberOfSteps);
    }

    return ret;
}

int32_t searchIDIntoTree(Register *baseReg, Register **ppOutputRegister, uint32_t ID, uint32_t *numberOfSteps)
{
    int32_t ret = SUCCESS;

    if (baseReg != NULL
        && ppOutputRegister != NULL)
    {
        Node *topNode = NULL;
        Node *outputNode = NULL;

        TREE_DEBUG("baseReg=%08lx baseReg->ID=%d ID=%d\n",
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

                TREE_DEBUG("*ppOutputRegister=%08lx *ppOutputRegister->ID=%d\n",
                            *ppOutputRegister,
                            getIDFromReg(*ppOutputRegister));
            }
        }
        else
        {
            TREE_ERROR("Top node is null\n");
            ret = FAIL;
        }
    }
    else
    {
        TREE_ERROR("At least one parameter is null\n");
    }

    return ret;
}

void destroyTree(Node * tree)
{
    if (tree != NULL)
    {
        NavParmBlock navParmBlock;
        int32_t ret;

        memset(&navParmBlock, 0, sizeof(NavParmBlock));

        //set the function removeNodeFromTree
        navParmBlock.pNavFunction = removeNodeFromTree;

        ret = navigateTree(tree, NAV_POSTORDER, &navParmBlock);
        
        if (ret != SUCCESS)
        {
            TREE_ERROR("ret=%d\n", ret);
        }
    }
    else
    {
        TREE_ERROR("Tree is null\n");
    }
}

int32_t removeRegIntoTree(Register *reg, uint32_t * numberOfSteps)
{
    int32_t ret = SUCCESS;

    if (reg != NULL)
    {
        Node * pNode = NULL;

        pNode = getTreeFromReg(reg);

        if (pNode != NULL)
        {
            ret = removeNodeFromTree(pNode, numberOfSteps);
        }
        else
        {
            ret = FAIL;
            TREE_ERROR("Node is null\n");
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("Register is null\n");
    }

    return ret;
}

int32_t removeIDIntoTree(Register *baseReg, uint32_t ID, uint32_t *numberOfSteps)
{
    int32_t ret = SUCCESS;

    if (baseReg != NULL)
    {
        Node * pNode = NULL;
        Register *outputReg = NULL;

        pNode = getTreeFromReg(baseReg);

        if (pNode != NULL)
        {
            ret = searchIDIntoTree(baseReg, &outputReg, ID, numberOfSteps);

            if (ret == SUCCESS
                && outputReg != NULL)
            {
                ret = removeRegIntoTree(outputReg, numberOfSteps);
            }
        }
        else
        {
            ret = FAIL;
            TREE_ERROR("Node is null\n");
        }
    }
    else
    {
        ret = FAIL;
        TREE_ERROR("Register is null\n");
    }

    return ret;
}

static int32_t showNode(Node *pNode, uint32_t * numberOfSteps)
{
    int32_t ret = SUCCESS;

    if (pNode != NULL
        && pNode->reg != NULL)
    {
        ret = printRegInfo(pNode->reg);
    }
    else
    {
        TREE_ERROR("Register is null\n");
        ret = FAIL;
    }

    return ret;
}

int32_t showTree(Node * tree)
{
    int32_t ret = SUCCESS;

    if (tree != NULL)
    {
        NavParmBlock navParmBlock;

        memset(&navParmBlock, 0, sizeof(NavParmBlock));

        //set the function showNode
        navParmBlock.pNavFunction = showNode;

        ret = navigateTree(tree, NAV_PREORDER, &navParmBlock);
        
        if (ret != SUCCESS)
        {
            TREE_ERROR("ret=%d\n", ret);
        }
    }
    else
    {
        TREE_ERROR("Tree is null\n");
        ret = FAIL;
    }

    return ret;
}

uint32_t getNodeLevel(Node * pNode)
{
    uint32_t level = 0;

    if (pNode != NULL)
    {
        level = pNode->level;
    }

    return level;
}
